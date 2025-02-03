#include "main.h"
#include "subsystems.hpp"


void DoinkerDown(){ doinkerPiston.set_value(true); }
void DoinkerUp(){ doinkerPiston.set_value(false); }

void DoinkerController(){
    // Doinker Controller -- holding 
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        DoinkerDown();
    else
        DoinkerUp();
}