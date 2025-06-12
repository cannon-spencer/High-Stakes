/**
 * @file autons.hpp
 * @brief Autonomous routines, motion tests, and chassis tuning functions.
 *
 * This file includes all autonomous modes used in competition, as well as motion
 * profiling demos and odometry experiments using EZ-Template. Each routine is meant
 * to be modular, readable, and easily selectable via LCD or dev macros.
 */

#pragma once

/**
 * @brief Sets default PID, slew, and odometry constants.
 *
 * Configures drive/turn/swing constants, exit conditions, lookahead values,
 * and prioritization for odometry-based path tracking.
 */
void default_constants();

/**
 * @brief Full autonomous routine for Skills Challenge.
 *
 * Includes wall scoring, ring collection, and color-sorting logic.
 */
void skills();

/**
 * @brief Autonomous routine for Red Alliance match start.
 */
void RedMatchAuton();

/**
 * @brief Autonomous routine for Blue Alliance match start.
 */
void BlueMatchAuton();

/**
 * @brief Shared logic used by both red and blue match autonomous routines.
 */
void MatchAuton();

/// @name Motion Examples and Demos
/// @{

/// Basic drive forward/backward using PID.
void drive_example();

/// Turns the robot to multiple angles using PID.
void turn_example();

/// Combines drive and turn into a sequence.
void drive_and_turn();

/// Demonstrates max speed change using `pid_wait_until`.
void wait_until_change_speed();

/// Demonstrates swing turns (one side drive).
void swing_example();

/// Demonstrates motion chaining with `pid_wait_quick_chain`.
void motion_chaining();

/// Combines drive, turn, and swing in one autonomous.
void combining_movements();

/// Retries backwards tug motion if robot is interfered with.
void tug(int attempts);

/// Placeholder for testing interference logic.
void interfered_example();

/// @}

/// @name Odometry-Based Examples
/// @{

/// Uses odometry to drive straight with feedback correction.
void odom_drive_example();

/// Uses odometry and pure pursuit to reach a series of waypoints.
void odom_pure_pursuit_example();

/// Starts intake when robot passes a certain pure pursuit waypoint.
void odom_pure_pursuit_wait_until_example();

/// Demonstrates boomerang-style motion with curvature targeting.
void odom_boomerang_example();

/// Combines boomerang and pure pursuit for complex paths.
void odom_boomerang_injected_pure_pursuit_example();

/// Runs turn calibration to compute odometry tracker wheel offsets.
void measure_offsets();

/// @}
