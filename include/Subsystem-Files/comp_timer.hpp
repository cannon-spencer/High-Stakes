/**
 * @file comp_timer.hpp
 * @brief Provides timed controller rumble warnings during match play.
 *
 * Alerts the driver at specified match intervals (e.g., 30 seconds remaining)
 * using the controller rumble API. Useful for endgame strategy awareness.
 */

#pragma once

/// Global start time for driver control mode (ms).
extern int matchStartTime;

/// Sends periodic rumble alerts during endgame warning window.
void CompTimerController();
