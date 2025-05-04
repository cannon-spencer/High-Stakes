#include "main.h"
#include "subsystems.hpp"

const int DRIVER_CONTROL_TIME = 90000;                      // 90 seconds
const int WARNING_START_TIME = DRIVER_CONTROL_TIME - 35000; // 35 seconds
const int WARNING_END_TIME = DRIVER_CONTROL_TIME - 30000;   // 30 seconds
const int RUMBLE_INTERVAL = 1000;                           // 1 second interval
int matchStartTime = 0;                                     // Track the time opcontrol begins
int lastRumbleTime = 0;                                     // Track last rumble event


void CompTimerController(){
    // Rumble Controller in Competition
    if(pros::competition::is_connected()){
        // Track elapsed match time
        int elapsedTime = pros::millis() - matchStartTime;
    
        // If we're in the 5-second warning window (35s - 30s remaining)
        if (elapsedTime >= WARNING_START_TIME && elapsedTime <= WARNING_END_TIME) {
            // If 1 second has passed since the last rumble, trigger another
            if (pros::millis() - lastRumbleTime >= RUMBLE_INTERVAL) {
                master.rumble("-");
                lastRumbleTime = pros::millis();
            }
        }
    }
}