#include "main.h"
#include "subsystems.hpp"

const int GOAL_HUE_MIN = 62;
const int GOAL_HUE_MAX = 94;

void OpenClamp(){ clampPiston.set_value(false); }
void CloseClamp(){ clampPiston.set_value(true); }

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

void ClampController(){
    pros::lcd::print(5, "Goal In Clamp: %d \n", IsGoalClamped());
    // Clamp Controller -- open only when holding 
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        OpenClamp();
    else
        CloseClamp();
}