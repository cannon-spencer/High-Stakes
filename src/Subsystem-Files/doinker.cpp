/**
 * @file doinker.cpp
 * @brief Arm ("doinker") actuator control.
 *
 * Provides toggles for manually deploying or retracting the back piston,
 * which may be used for pushing goals or rings out of the corner.
 */

#include "main.h"
#include "subsystems.hpp"


/**
 * @brief Deploys the rear doinker piston.
 */
void DoinkerDown(){ doinkerPiston.set_value(true); }


/**
 * @brief Retracts the rear doinker piston.
 */
void DoinkerUp(){ doinkerPiston.set_value(false); }


/**
 * @brief Maps the rear doinker piston to the DOWN controller button.
 *
 * If the button is pressed, deploys the piston. Otherwise, retracts it.
 */
void DoinkerController(){
    // Doinker Controller -- holding 
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
        DoinkerDown();
    else
        DoinkerUp();
}