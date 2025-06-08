/**
 * @file drive.cpp
 * @brief Dispatches driver control logic for different driving styles.
 *
 * Uses a custom `drive_type` enum to switch between tank, standard arcade,
 * or flipped control modes during operator control.
 */

#include "main.h"
#include "subsystems.hpp"


/**
 * @brief Selects and runs the appropriate control scheme.
 *
 * This function is called once per opcontrol loop and allows for
 * dynamically changing drive input styles.
 *
 * @param driveMode Control mode selection from drive_type enum.
 */
void ChassisController(drive_type driveMode){
    switch (driveMode) {
        
        // Tank control
        case drive_type::TANK:
            chassis.opcontrol_tank();
            break;

        // Standard split arcade
        case drive_type::ARCADE_SPLIT:
            chassis.opcontrol_arcade_standard(ez::SPLIT);
            break;

        // Standard single arcade
        case drive_type::ARCADE_SINGLE:
            chassis.opcontrol_arcade_standard(ez::SINGLE);
            break;

        // Flipped split arcade
        case drive_type::ARCADE_FLIPPED_SPLIT:
            chassis.opcontrol_arcade_flipped(ez::SPLIT);
            break;
        
        // Flipped single arcade
        case drive_type::ARCADE_FLIPPED_SINGLE:
            chassis.opcontrol_arcade_flipped(ez::SINGLE);
            break;
    }
}