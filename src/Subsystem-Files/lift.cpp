#include "main.h"
#include "subsystems.hpp"

const int BASE_POSITION = 9000;
const int PRIMED_POSITION = 10800;
const int WALLSTAKE_POSITION = 22440;

bool scoreMode = false;

ez::PID liftPID{0.05, 0, 0.1, 0, "Lift"};  

void LiftController(){

    // pre-set base target position
    liftPID.target_set(BASE_POSITION);

    while(1){

        // Op-Control Task
        if(!pros::competition::is_autonomous()){
            // Toggle Scoring Mode Button 
            if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
                if(scoreMode)
                    master.rumble(".");
                else
                    master.rumble("..");
                
                scoreMode = !scoreMode;
            }
            
            // Send lift to score while holding, cancel target when released
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

        // always set motors to the PID Target
        ladyBrown.move(liftPID.compute(liftRotation.get_position())); 

        pros::delay(ez::util::DELAY_TIME);

    }
}
pros::Task LiftTask(LiftController);