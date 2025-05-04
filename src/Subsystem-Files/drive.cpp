#include "main.h"
#include "subsystems.hpp"

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