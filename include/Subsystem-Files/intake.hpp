#pragma once

enum class AllianceMode { BLUE, RED, OFF };
enum class IntakeSpeed {FAST, SLOW, STOP, REVERSE, UNHOOK};

void SetIntake(int frontIntake, int mainIntake);
void RunIntake(IntakeSpeed speed);
void IntakeUp();
void IntakeDown();
bool RingColorCheck(AllianceMode aMode, double hue);
void SetAllianceMode(AllianceMode aMode);
void IntakeWait(AllianceMode aMode, int maxWaitTimeMs);
void IntakeController();
bool IsIntakeRunning();
void CycleAllianceMode();
extern pros::Task IntakeTask;