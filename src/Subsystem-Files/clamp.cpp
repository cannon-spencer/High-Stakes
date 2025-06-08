/**
 * @file clamp.cpp
 * @brief Clamp subsystem functions and goal detection.
 *
 * Contains logic for opening/closing the ring clamp, and using optical sensors
 * to detect whether a green goal is currently clamped securely.
 */

#include "main.h"
#include "subsystems.hpp"

const int GOAL_HUE_MIN = 62;  // hue lower bound
const int GOAL_HUE_MAX = 94;  // hue upper bound

/**
 * @brief Opens the ring clamp.
 *
 * Sets the pneumatic clamp piston to false, allowing goal to be released.
 */
 void OpenClamp(){ clampPiston.set_value(false); }

 
/**
 * @brief Closes the ring clamp.
 *
 * Sets the pneumatic clamp piston to true, securing goal inside the claw.
 */
 void CloseClamp(){ clampPiston.set_value(true); }


/**
 * @brief Checks if a valid ring is detected in the clamp.
 *
 * Uses proximity and hue thresholds to determine if a green goal object
 * is clamped close enough to be considered secured.
 *
 * @return true if a green ring is present in close proximity.
 */
bool IsGoalClamped(){
    // Proximity 0-255, check for close proximity and correct color hue
    if(clampOptical.get_proximity() > 180){
        int hue = clampOptical.get_hue();
        if(hue > GOAL_HUE_MIN && hue < GOAL_HUE_MAX){
            return true;
        }
    }
    return false;
}


/**
 * @brief Handles manual clamp input using the R2 button.
 *
 * Continuously checks controller input and opens the clamp
 * while the R2 button is held. Otherwise, keeps it closed.
 */
 void ClampController(){
    pros::lcd::print(5, "Goal In Clamp: %d \n", IsGoalClamped());
    // Clamp Controller -- open only when holding 
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        OpenClamp();
    else
        CloseClamp();
}