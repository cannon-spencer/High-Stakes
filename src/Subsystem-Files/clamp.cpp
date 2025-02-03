#include "main.h"
#include "subsystems.hpp"

void OpenClamp(){ clampPiston.set_value(false); }
void CloseClamp(){ clampPiston.set_value(true); }

void ClampController(){
    // Clamp Controller -- open only when holding 
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
        OpenClamp();
    else
        CloseClamp();
}