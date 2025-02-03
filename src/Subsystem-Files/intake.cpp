#include "main.h"
#include "subsystems.hpp"


int INTAKE_SPEED = 127;
int F_INTAKE_SPEED  = 127;


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


bool RingColorCheck(char color, double hue) {
    // Define hue ranges for each color
    double blue_min = 110, blue_max = 250; // Blue range
    double red_min = 0, red_max = 40;      // Red range

    // Check the hue against the range based on the color input
    switch (color) {
    case 'B': // Blue
        return (hue >= blue_min && hue <= blue_max);
    case 'R': // Red
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

        }
        
        // Always run jam detection & color sorting

        int hue = clampOptical.get_hue();
        pros::lcd::print(6, "RED: %d, BLUE: %d", RingColorCheck('R', hue), RingColorCheck('B', hue));

        // reverse intake when a ring is detected
        /*if(RingColorCheck('R', hue)){
            ReverseIntake();
            pros::delay(30);
        }*/

        // if scoremode and ring detected, send lift pid down, eject, wait, and fix the position back

        pros::delay(ez::util::DELAY_TIME);
    }
}
pros::Task IntakeTask(IntakeController);