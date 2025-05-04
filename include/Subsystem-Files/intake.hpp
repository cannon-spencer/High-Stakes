#pragma once

enum class AllianceMode { BLUE, RED, OFF };

void SetIntake(int frontIntake, int mainIntake);
void RunIntake();
void oneIntake();
void StopIntake();
void ReverseIntake();
void UnhookIntake();
void IntakeUp();
void IntakeDown();
bool RingColorCheck(AllianceMode aMode, double hue);
void IntakeWait(AllianceMode aMode, int maxWaitTimeMs);
void IntakeController();
bool IsIntakeRunning();
void CycleAllianceMode();
void SetAllianceMode(AllianceMode aMode);

extern bool exited;

extern pros::Task IntakeTask;