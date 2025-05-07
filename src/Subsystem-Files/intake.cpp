#include "main.h"
#include "subsystems.hpp"

// Enum for Alliance Modes
AllianceMode intakeMode = AllianceMode::BLUE;

// Intake Constants
const int INTAKE_SPEED = 127;
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

void RunIntake(){ SetIntake(F_INTAKE_SPEED, INTAKE_SPEED); }

void oneIntake(){ SetIntake(-F_INTAKE_SPEED, INTAKE_SPEED); }

void StopIntake(){ SetIntake(0, 0); }

void ReverseIntake(){ SetIntake(-F_INTAKE_SPEED, -INTAKE_SPEED); }

void UnhookIntake(){ mainIntake.move_relative(-100, INTAKE_SPEED); }

void IntakeUp(){ intakePiston.set_value(true); }

void IntakeDown(){ intakePiston.set_value(false); }

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

bool exited = false;

void IntakeWait(AllianceMode aMode, int maxWaitTimeMs) {
    int startTime = pros::millis(); // Record the start time
    exited = false;

    // Flip the alliance mode (because we're waiting for as specific color)
    switch (aMode) {
        case AllianceMode::BLUE:
            aMode = AllianceMode::RED;
            break;
        case AllianceMode::RED:
            aMode = AllianceMode::BLUE;
            break;
    }

    while (1) {
        if (pros::millis() - startTime >= maxWaitTimeMs) {
            break; // Exit the loop if max wait time is reached
        }
        pros::delay(10);

        if(RingColorCheck(aMode, intakeOptical.get_hue())){
            exited = true;
            break;
        }
    }
}


void IntakeController(){

    // Display mode on Startup
    DisplayAllianceMode();

    while(1){
        
        // Driver Control Task
        if(!pros::competition::is_autonomous()){

            // run main intake
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
                RunIntake();
            } else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
                ReverseIntake();
            } else {
                StopIntake();
            }

            // Reverse intake slightly when scoring with lady brown
            if(scoreMode && master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
                ReverseIntake();
                pros::delay(30);
            } 

            // Intake Piston Control -- almost never needed
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
                IntakeUp();
            else
                IntakeDown();

            // Manual Allianace Color Swap -- almost never needed
            if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
                CycleAllianceMode();
            }

        }
        
        // Always run jam detection & color sorting
        int hue = intakeOptical.get_hue();
        //pros::lcd::print(6, "BLUE: %d, RED: %d", RingColorCheck(AllianceMode::RED, hue), RingColorCheck(AllianceMode::BLUE, hue));
        //pros::lcd::print(7, "Intake Running: %d", IsIntakeRunning());

        // Only color sort if the intake is running!
        if(IsIntakeRunning()){
            lastRunningTime = pros::millis();

            // reverse intake when a ring is detected
            if (!scoreMode && RingColorCheck(intakeMode, intakeOptical.get_hue())){
                //master.rumble(".");
                pros::delay(205); // short hooks 220, long hooks 205
                StopIntake();
                pros::delay(150);
                RunIntake();
            }
        } else {
            // Check if intake should be running but hasn't moved for the delay time
            if (std::abs(mainIntake.get_target_velocity()) > 0 && 
                (pros::millis() - lastRunningTime) > JAM_DETECTION_DELAY && 
                (pros::millis() - intakeStartTime) > INTAKE_SPINUP_TIME) {
                
                if (!scoreMode){
                    ReverseIntake();
                    pros::delay(150);
                    RunIntake();
                }
            }
        }

        pros::delay(ez::util::DELAY_TIME);
    }
}
pros::Task IntakeTask(IntakeController);