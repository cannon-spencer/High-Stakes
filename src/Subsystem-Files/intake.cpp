/**
 * @file intake.cpp
 * @brief Intake subsystem logic for autonomous and driver control.
 *
 * This file defines the intake control system including color sorting,
 * piston deployment, jam detection, and alliance-based behavior. It also manages
 * dynamic runtime switching between alliance colors and handles intake behavior
 * during scoring scenarios.
 */

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


/**
 * @brief Directly sets motor speeds for front and main intakes.
 *
 * Tracks start time and flags for jam detection.
 *
 * @param f_intake Speed for front intake motor
 * @param m_intake Speed for main intake group
 */
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


/**
 * @brief Runs the intake in various modes based on enum.
 *
 * Supports FAST, SLOW, MED, STOP, REVERSE, UNHOOK, and PULSE modes.
 *
 * @param speed IntakeSpeed enum
 * @param pulseTime Optional pulse time in ms (used only if PULSE is selected)
 */
void RunIntake(IntakeSpeed speed, int pulseTime) {
    switch (speed) {
        case IntakeSpeed::FAST:
            SetIntake(F_INTAKE_SPEED, INTAKE_SPEED);
            break;
        case IntakeSpeed::SLOW:
            SetIntake(F_INTAKE_SPEED / 2, INTAKE_SPEED / 2);
            break;
        case IntakeSpeed::MED:
            SetIntake(F_INTAKE_SPEED / 1.5, INTAKE_SPEED / 1.5);
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
        case IntakeSpeed::PULSE:
            PulseIntakeBlocking(pulseTime);
            break;
    }
}


/** @brief Retracts intake piston. */
void IntakeUp(){ intakePiston.set_value(false); }


/** @brief Deploys intake piston. */
void IntakeDown(){ intakePiston.set_value(true); }


/**
 * @brief Determines if intake is currently running.
 *
 * Uses velocity thresholds to infer intake motor activity.
 *
 * @return true if intake is moving; false otherwise.
 */
bool IsIntakeRunning() {
    double mainIntakeVelocity = mainIntake.get_actual_velocity();
    double frontIntakeVelocity = frontIntake.get_actual_velocity();
    double velocityThreshold = 100.0;

    // Return true only if the main intake is running
    return (std::abs(mainIntakeVelocity) > velocityThreshold);
}


/**
 * @brief Checks if a hue matches the rejection condition for the selected alliance.
 *
 * This ensures rings of the *opposite* color are ejected.
 *
 * @param aMode AllianceMode enum
 * @param hue Current hue value from optical sensor
 * @return true if a ring of the opposite color is detected
 */
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


/**
 * @brief Sets the ejection mode (front or automatic).
 *
 * @param eMode EjectMode enum
 */
void SetRejectMode(EjectMode eMode){
    if(eMode == EjectMode::FRONT)
        ejectFront = true;
    else
        ejectFront = false;
}


/**
 * @brief Displays current alliance mode on controller LCD.
 */
void DisplayAllianceMode(){
    switch (intakeMode) {
        case AllianceMode::BLUE: master.print(0, 0, "ALLIANCE: BLUE"); break;
        case AllianceMode::RED: master.print(0, 0, "ALLIANCE: RED "); break;
        case AllianceMode::OFF: master.print(0, 0, "ALLIANCE: OFF "); break;
    }
}


/**
 * @brief Cycles through BLUE -> RED -> OFF -> BLUE, etc.
 */
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


/**
 * @brief Displays current alliance mode on controller LCD.
 */
void SetAllianceMode(AllianceMode aMode) {
    intakeMode = aMode;
    DisplayAllianceMode();
}


/**
 * @brief Returns the current alliance mode.
 *
 * @return AllianceMode
 */
AllianceMode GetAllianceMode(){
    return intakeMode;
}


/**
 * @brief Blocks until an opposite-color ring is detected or timeout occurs.
 *
 * @param aMode Current alliance mode
 * @param maxWaitTimeMs Maximum time to wait in milliseconds
 * @return IntakeExit enum indicating result (TIMEOUT or RING_DETECTED)
 */
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


/**
 * @brief Pulses the intake motor on and off repeatedly for a given duration.
 *
 * @param ms Total pulse duration in milliseconds
 */
void PulseIntakeBlocking(int ms) {
    const int pulseTime = 80;
    int elapsed = 0;

    while (elapsed < ms) {
        RunIntake(IntakeSpeed::FAST);
        pros::delay(pulseTime);
        elapsed += pulseTime;

        if (elapsed >= ms) break;

        RunIntake(IntakeSpeed::STOP);
        pros::delay(pulseTime);
        elapsed += pulseTime ;
    }

    RunIntake(IntakeSpeed::STOP);
}


/**
 * @brief Intake task loop for both driver control and autonomous.
 *
 * Handles jam recovery, color-based ejection, piston control, alliance detection,
 * and interaction with the scoring subsystem. Should be launched as a task.
 */
void IntakeController(){

    // Display mode on Startup
    DisplayAllianceMode();

    while(1){
        
        // Driver Control Task - main block requires autonomous mode off and disable mode off
        if(!pros::competition::is_autonomous() && !pros::competition::is_disabled()){
            
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
                    pros::delay(60);
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
                    pros::delay(180);
                    RunIntake(IntakeSpeed::FAST);
                }

            }
        }

        pros::delay(ez::util::DELAY_TIME);
    }
}
pros::Task IntakeTask(IntakeController);

