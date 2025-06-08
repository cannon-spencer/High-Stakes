/**
 * @file lift.hpp
 * @brief Control functions for vertical lift mechanism ("Lady Brown").
 *
 * Includes scoring modes, PID control, and async/auton-compatible commands.
 * Uses sensor feedback from rotation sensor to maintain target lift positions.
 */

#pragma once

/// Starts the lift control task and manages scoring logic.
void LiftController();

/// Blocks until the lift reaches the target position.
void WaitLadyBrown(int position);

/// Sets the lift to move to a new target position asynchronously.
void AsyncLadyBrown(int position);

// Shared lift state
extern bool scoreMode;
extern pros::Task LiftTask;
extern ez::PID liftPID;

// Position presets
extern const int BASE_POSITION;
extern const int PRIMED_POSITION;
extern const int WALLSTAKE_POSITION;
