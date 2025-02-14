#include "main.h"
#include "subsystems.hpp"

const int BASE_POSITION = 0;
const int PRIMED_POSITION = -225;
const int WALLSTAKE_POSITION = -1050;

const int MAX_SPEED_AUTON = 65;

pros::Motor ladyBrown1(19, pros::MotorGearset::green);
pros::Motor ladyBrown2(-13, pros::MotorGearset::green);

void set_lift(int input) {
    ladyBrown1.move(input);
    ladyBrown2.move(input);
}

bool scoreMode = false;

ez::PID liftPID{0.4, 0, 0, 0, "Lift"};

void LiftController(){

    // pre-set base target position
    liftPID.target_set(BASE_POSITION);

    while(1){

        // Op-Control Task
        if(!pros::competition::is_autonomous()){
            // Toggle Scoring Mode Button 
            if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
                scoreMode = !scoreMode;
            
            // Send lift to score while holding, cancel target when released
            int ladyBrownError;
            if(scoreMode && master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
                liftPID.target_set(WALLSTAKE_POSITION);
            } else {
                // Base Case: Target Base pos or Primed pos
                if(scoreMode)
                    liftPID.target_set(PRIMED_POSITION);
                else
                    liftPID.target_set(BASE_POSITION);

            }
        }

        double error = liftPID.compute(ladyBrown.get_position());

        if(pros::competition::is_autonomous()){
            error = ((error > MAX_SPEED_AUTON) ? MAX_SPEED_AUTON : error);
        }

        // always set motors to the PID Target
        ladyBrown.move(liftPID.compute(ladyBrown.get_position()));

        pros::delay(ez::util::DELAY_TIME);

    }
}

void autoLadyBrown(const int LBstate) {

    liftPID.target_set(LBstate);
    while (liftPID.exit_condition({ladyBrown1, ladyBrown2}, true) == ez::RUNNING) {
        pros::delay(ez::util::DELAY_TIME);
    }

}

pros::Task LiftTask(LiftController);