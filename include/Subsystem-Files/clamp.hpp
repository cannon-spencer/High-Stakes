/**
 * @file clamp.hpp
 * @brief Control interface for the pneumatic clamp subsystem.
 *
 * Provides functions to open/close the clamp and detect if a goal is securely clamped
 * using optical sensor proximity and hue thresholds.
 */

#pragma once

/// Opens the clamp to release the held object.
void OpenClamp();

/// Closes the clamp to grip a goal.
void CloseClamp();

/// Handles driver input (R2 button) to control the clamp during opcontrol.
void ClampController();

/// Returns true if a goal is detected and secured in the clamp.
bool IsGoalClamped();
