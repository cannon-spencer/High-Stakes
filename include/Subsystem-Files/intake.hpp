#pragma once

#include "main.h"

// enum defs
enum class AllianceMode { BLUE, RED, OFF };
enum class IntakeSpeed {FAST, MED, SLOW, STOP, REVERSE, UNHOOK, PULSE};
enum class IntakeExit {TIMEOUT, RING_DETECTED};
enum class EjectMode {FRONT, TOP, AUTO};

// Core intake control functions
void SetRejectMode(EjectMode eMode);
void SetIntake(int frontIntake, int mainIntake);
void RunIntake(IntakeSpeed speed, int pulseTime = 0);
void IntakeUp();
void IntakeDown();

// Auton Functions / Util Functions
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
