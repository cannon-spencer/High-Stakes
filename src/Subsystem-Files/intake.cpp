#include "main.h"
#include "subsystems.hpp"


int INTAKE_SPEED = 127;
int F_INTAKE_SPEED  = 127;

char allianceColor = 'B';


void SetIntake(int f_intake, int m_intake){
    frontIntake.move(f_intake);
    mainIntake.move(m_intake);
}

void RunIntake(){ SetIntake(F_INTAKE_SPEED, INTAKE_SPEED); }

void StopIntake(){ SetIntake(0, 0); }

void ReverseIntake(){ SetIntake(-F_INTAKE_SPEED, -INTAKE_SPEED); }

void UnhookIntake(){ mainIntake.move_relative(-100, INTAKE_SPEED); }

void IntakeUp(){ intakePiston.set_value(true); }

void IntakeDown(){ intakePiston.set_value(false); }

bool IsIntakeRunning(){
    double intakeVelocity = mainIntake.get_actual_velocity();
    double velocityThreshold = 150.00;
    return (std::abs(intakeVelocity) > velocityThreshold);
}


bool RingColorCheck(char aColor, double hue) {
    // Define hue ranges for each color
    double blue_min = 165, blue_max = 250; // Blue range
    double red_min = 1, red_max = 20;      // Red range

    // Check the hue against the range based on the color input
    switch (aColor) {
    case 'R': // Red alliance -- Filter out blue
        return (hue >= blue_min && hue <= blue_max);
    case 'B': // Blue alliance -- Filter out red
        return (hue >= red_min && hue <= red_max);
    }

    return false;
}

void IntakeController(){
    while(1){
        
        // Driver Control Task
        if(!pros::competition::is_autonomous()){

            // run main intake
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
                RunIntake();
            } else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
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
            if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
                IntakeUp();
            else
                IntakeDown();

            // Emergency Swap Color -- display to controller 

        }
        
        // Always run jam detection & color sorting
        int hue = clampOptical.get_hue();
        pros::lcd::print(6, "RED: %d, BLUE: %d", RingColorCheck('R', hue), RingColorCheck('B', hue));
        pros::lcd::print(7, "Intake Running: %d", IsIntakeRunning());

        // reverse intake when a ring is detected
        if (RingColorCheck(allianceColor, intakeOptical.get_hue())){
            pros::delay(230);
            StopIntake();
            pros::delay(150);
            RunIntake();
        }

        // if scoremode and ring detected, send lift pid down, eject, wait, and fix the position back

        pros::delay(ez::util::DELAY_TIME);
    }
}
pros::Task IntakeTask(IntakeController);