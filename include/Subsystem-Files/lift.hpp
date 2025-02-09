#pragma once

void LiftController();
void WaitLadyBrown(int position);
void AsyncLadyBrown(int position);
extern bool scoreMode;
extern pros::Task LiftTask;
extern ez::PID liftPID;

extern const int BASE_POSITION;
extern const int PRIMED_POSITION;
extern const int WALLSTAKE_POSITION;