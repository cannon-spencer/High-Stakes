/**
 * @file drive.hpp
 * @brief Enumerated drive mode control for opcontrol.
 *
 * Defines a scoped enum `drive_type` for various arcade and tank configurations,
 * and allows dynamic switching during runtime via `ChassisController`.
 */

#pragma once

/// Control schemes supported by the chassis.
enum class drive_type {
    TANK,
    ARCADE_SPLIT,
    ARCADE_SINGLE,
    ARCADE_FLIPPED_SPLIT,
    ARCADE_FLIPPED_SINGLE
};

/// Dispatches control based on selected drive mode.
void ChassisController(drive_type driveMode);
