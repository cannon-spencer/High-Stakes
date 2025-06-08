/**
 * @file lift.cpp
 * @brief Lift control logic using PID for scoring.
 *
 * Manages scoring behavior, toggling between "primed" and "wallstake" positions
 * depending on operator input. Uses a PID loop to control lift height based on
 * rotation sensor feedback.
 */

#include "main.h"
#include "subsystems.hpp"
#include <cstdint>

const int BASE_POSITION = 9000;          // Default resting height
const int PRIMED_POSITION = 10800;       // Slightly lifted "ready to score" height
const int WALLSTAKE_POSITION = 23500;    // Full extension height for scoring

bool scoreMode = false;                  // Global scoring mode toggle

/// PID controller instance for lift motor group
/// @param kP Proportional gain
/// @param kI Integral gain
/// @param kD Derivative gain
/// @param kF Feedforward (unused)
/// @param name PID name for debugging
ez::PID liftPID{0.044, 0, 0.16, 0, "Lift"};


/**
 * @brief Lift controller task loop.
 *
 * Responds to operator input to toggle scoring mode and set lift targets accordingly.
 * Uses sensor feedback and PID control to precisely position the lift.
 */
void LiftController(){

    // pre-set base target position
    liftPID.target_set(BASE_POSITION);

    // block this task until signalled to begin
    while(pros::Task::notify_take(true, TIMEOUT_MAX));

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


/**
 * @brief Waits until the lift reaches a specified target position.
 *
 * This is a blocking function that halts progress until the PID controller exits.
 *
 * @param position Target lift position in encoder ticks
 */
void WaitLadyBrown(int position){
    liftPID.target_set(position);

    // wait on the lift to exit
    while (liftPID.exit_condition(true) == ez::RUNNING) {
        pros::delay(ez::util::DELAY_TIME);
    }

}

/**
 * @brief Sends lift to a specified target without blocking.
 *
 * @param position Target lift position in encoder ticks
 */
void AsyncLadyBrown(int position){ liftPID.target_set(position); }
