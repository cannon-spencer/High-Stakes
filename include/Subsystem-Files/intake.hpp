#pragma once

enum class AllianceMode { BLUE, RED, OFF };
enum class IntakeSpeed {FAST, SLOW, STOP, REVERSE, UNHOOK};
enum class IntakeExit {TIMEOUT, RING_DETECTED};
enum class EjectMode {FRONT, TOP, AUTO};

IntakeExit IntakeWait(AllianceMode aMode, int maxWaitTimeMs);
void SetRejectMode(EjectMode eMode);
void SetIntake(int frontIntake, int mainIntake);
void RunIntake(IntakeSpeed speed);
void IntakeUp();
void IntakeDown();
bool RingColorCheck(AllianceMode aMode, double hue);
void SetAllianceMode(AllianceMode aMode);
void IntakeController();
bool IsIntakeRunning();
void CycleAllianceMode();
extern pros::Task IntakeTask;