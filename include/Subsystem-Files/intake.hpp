/**
 * @file intake.hpp
 * @brief Intake and color-sorting logic with ring ejection handling.
 *
 * Handles all intake control including fast/slow speeds, color sorting,
 * alliance detection, jam recovery, and subsystem task control.
 */

#pragma once

#include "main.h"

/// Alliance colors and control logic.
enum class AllianceMode { BLUE, RED, OFF };

/// Intake speed presets.
enum class IntakeSpeed { FAST, MED, SLOW, STOP, REVERSE, UNHOOK, PULSE };

/// Result codes for color-detection wait functions.
enum class IntakeExit { TIMEOUT, RING_DETECTED };

/// Ejection strategy modes.
enum class EjectMode { FRONT, TOP, AUTO };

// Core intake control
void SetRejectMode(EjectMode eMode);
void SetIntake(int frontIntake, int mainIntake);
void RunIntake(IntakeSpeed speed, int pulseTime = 0);
void IntakeUp();
void IntakeDown();

// Autonomous helpers
IntakeExit IntakeWait(AllianceMode aMode, int maxWaitTimeMs);
bool RingColorCheck(AllianceMode aMode, double hue);
void SetAllianceMode(AllianceMode aMode);
AllianceMode GetAllianceMode();
bool IsIntakeRunning();
void CycleAllianceMode();
void PulseIntakeBlocking(int ms);

// Task-level control
void IntakeController();
extern pros::Task IntakeTask;
