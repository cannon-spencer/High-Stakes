#include "main.h"
#include "subsystems.hpp"

// Enum for Alliance Modes
AllianceMode intakeMode = AllianceMode::OFF;

// manual flag for ejecting out of the front
bool ejectFront = false; 

// Intake Constants
const int INTAKE_SPEED = 100;
const int F_INTAKE_SPEED  = 127;

// Timestamp tracking when intake was last commanded to run
const int INTAKE_SPINUP_TIME = 700;
const int JAM_DETECTION_DELAY = 500; 
int intakeStartTime = 0;
int lastRunningTime = 0;
bool wasIntakeStopped = true;


void SetIntake(int f_intake, int m_intake){
    frontIntake.move(f_intake);
    mainIntake.move(m_intake);

    // If intake was previously stopped and is now starting, update the timestamp
    if (wasIntakeStopped && (f_intake != 0 || m_intake != 0)) {
        intakeStartTime = pros::millis();
        wasIntakeStopped = false; 
    } 
    // If intake is stopped, set flag so next run can update timestamp
    else if (f_intake <= 0 && m_intake <= 0) {
        wasIntakeStopped = true;
    }
    
}

void RunIntake(IntakeSpeed speed) {
    switch (speed) {
        case IntakeSpeed::FAST:
            SetIntake(F_INTAKE_SPEED, INTAKE_SPEED);
            break;
        case IntakeSpeed::SLOW:
            SetIntake(F_INTAKE_SPEED / 2, INTAKE_SPEED / 2);
            break;
        case IntakeSpeed::STOP:
            SetIntake(0, 0);
            break;
        case IntakeSpeed::REVERSE:
            SetIntake(-F_INTAKE_SPEED, -INTAKE_SPEED);
            break;
        case IntakeSpeed::UNHOOK:
            mainIntake.move_relative(-100, INTAKE_SPEED);
            break;
    }
}

void IntakeUp(){ intakePiston.set_value(false); }

void IntakeDown(){ intakePiston.set_value(true); }

bool IsIntakeRunning() {
    double mainIntakeVelocity = mainIntake.get_actual_velocity();
    double frontIntakeVelocity = frontIntake.get_actual_velocity();
    double velocityThreshold = 100.0;

    // Return true only if the main intake is running
    return (std::abs(mainIntakeVelocity) > velocityThreshold);
}


bool RingColorCheck(AllianceMode aMode, double hue) {
    double blue_min = 165, blue_max = 250;
    double red_min = 1, red_max = 20;

    switch (aMode) {
        case AllianceMode::RED: return (hue >= blue_min && hue <= blue_max);
        case AllianceMode::BLUE: return (hue >= red_min && hue <= red_max);
        case AllianceMode::OFF: return false;
    }
    return false;
}


void SetRejectMode(EjectMode eMode){
    if(eMode == EjectMode::FRONT)
        ejectFront = true;
    else
        ejectFront = false;
}


void DisplayAllianceMode(){
    switch (intakeMode) {
        case AllianceMode::BLUE: master.print(0, 0, "ALLIANCE: BLUE"); break;
        case AllianceMode::RED: master.print(0, 0, "ALLIANCE: RED "); break;
        case AllianceMode::OFF: master.print(0, 0, "ALLIANCE: OFF "); break;
    }
}


void CycleAllianceMode() {
    // Cycle to next mode
    switch (intakeMode) {
        case AllianceMode::BLUE: intakeMode = AllianceMode::RED; break;
        case AllianceMode::RED: intakeMode = AllianceMode::OFF; break;
        case AllianceMode::OFF: intakeMode = AllianceMode::BLUE; break;
    }

    // Display the selected mode on the controller
    DisplayAllianceMode();
}

void SetAllianceMode(AllianceMode aMode) {
    intakeMode = aMode;
    DisplayAllianceMode();
}


IntakeExit IntakeWait(AllianceMode aMode, int maxWaitTimeMs) {
    int startTime = pros::millis(); // Record the start time

    // Flip the alliance mode (because we're waiting for as specific color)
    switch (aMode) {
        case AllianceMode::BLUE:
            aMode = AllianceMode::RED;
            break;
        case AllianceMode::RED:
            aMode = AllianceMode::BLUE;
            break;
    }

    while (!RingColorCheck(aMode, intakeOptical.get_hue())) {
        if (pros::millis() - startTime >= maxWaitTimeMs) {
            return IntakeExit::TIMEOUT;
        }
        pros::delay(10);
    }

    RunIntake(IntakeSpeed::STOP);

    return IntakeExit::RING_DETECTED;
}




void IntakeController(){

    // Display mode on Startup
    DisplayAllianceMode();

    while(1){
        
        // Driver Control Task
        if(!pros::competition::is_autonomous()){
            
            // ensure rejecting reverts to auto mode after autonomous
            SetRejectMode(EjectMode::AUTO);

            // run main intake
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
                RunIntake(IntakeSpeed::FAST);
            } else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                RunIntake(IntakeSpeed::REVERSE);
            } else {
                RunIntake(IntakeSpeed::STOP);
            }

            // Reverse intake slightly when scoring with lady brown
            if(scoreMode && master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
                RunIntake(IntakeSpeed::REVERSE);
                pros::delay(30);
            } 

            // Intake Piston Control -- almost never needed
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
                IntakeUp();
            else
                IntakeDown();

            // Manual Allianace Color Swap -- almost never needed
            if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
                CycleAllianceMode();
            }

        }
        
        // Always run jam detection & color sorting
        int hue = intakeOptical.get_hue();
        pros::lcd::print(6, "BLUE: %d, RED: %d", RingColorCheck(AllianceMode::RED, hue), RingColorCheck(AllianceMode::BLUE, hue));
        pros::lcd::print(7, "Intake Running: %d", IsIntakeRunning());

        // Only color sort if the intake is running!
        if(IsIntakeRunning()){
            lastRunningTime = pros::millis();

            // reverse intake when a ring is detected
            if (RingColorCheck(intakeMode, intakeOptical.get_hue())){
                master.rumble(".");
                
                // reverse out of the front
                // check for lady brown staging and manual set flag
                if(scoreMode || ejectFront){
                    pros::delay(40);
                    RunIntake(IntakeSpeed::REVERSE);
                    pros::delay(425);
                    RunIntake(IntakeSpeed::FAST);
                } 
                
                // throw ring off the top
                else {
                    pros::delay(230);
                    RunIntake(IntakeSpeed::STOP);
                    pros::delay(150);
                    RunIntake(IntakeSpeed::FAST);
                }

            }
        } else {
            // Check if intake should be running but hasn't moved for the delay time
            if (std::abs(mainIntake.get_target_velocity()) > 0 && 
                (pros::millis() - lastRunningTime) > JAM_DETECTION_DELAY && 
                (pros::millis() - intakeStartTime) > INTAKE_SPINUP_TIME) {
                
                // make sure we're not scoring lady brown rings
                if(!scoreMode){
                    RunIntake(IntakeSpeed::REVERSE);
                    pros::delay(150);
                    RunIntake(IntakeSpeed::FAST);
                }

            }
        }

        pros::delay(ez::util::DELAY_TIME);
    }
}
pros::Task IntakeTask(IntakeController);