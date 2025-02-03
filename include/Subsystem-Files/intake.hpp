#pragma once

void SetIntake(int frontIntake, int mainIntake);
void RunIntake();
void StopIntake();
void ReverseIntake();
void UnhookIntake();
void IntakeUp();
void IntakeDown();
bool RingColorCheck(char color, double hue);
void IntakeController();
extern pros::Task IntakeTask;