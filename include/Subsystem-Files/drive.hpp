#pragma once

// Scoped enum for drive modes
enum class drive_type {
    TANK,
    ARCADE_SPLIT,
    ARCADE_SINGLE,
    ARCADE_FLIPPED_SPLIT,
    ARCADE_FLIPPED_SINGLE
};

// Declare the function that uses it
void ChassisController(drive_type driveMode);