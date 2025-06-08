/**
 * @file doinker.hpp
 * @brief Control logic for rear ejection piston ("doinker").
 *
 * The doinker is a rear-facing actuator used to push goals or rings.
 * This file maps basic driver controls to the piston state.
 */

#pragma once

/// Extends the doinker piston.
void DoinkerDown();

/// Retracts the doinker piston.
void DoinkerUp();

/// Reads controller input (DOWN button) and actuates the doinker accordingly.
void DoinkerController();
