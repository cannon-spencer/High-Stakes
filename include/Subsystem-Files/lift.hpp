#pragma once

extern ez::PID liftPID;

extern const int BASE_POSITION;
extern const int PRIMED_POSITION;
extern const int WALLSTAKE_POSITION;

void LiftController();

void autoLadyBrown(const int LBstate);

extern bool scoreMode;
extern pros::Task LiftTask;