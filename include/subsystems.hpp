#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

#include "Subsystem-Files/clamp.hpp"
#include "Subsystem-Files/doinker.hpp"
#include "Subsystem-Files/drive.hpp"
#include "Subsystem-Files/intake.hpp"
#include "Subsystem-Files/lift.hpp"

// EZ Constructors
extern Drive chassis;

extern ez::tracking_wheel horiz_tracker;
extern ez::tracking_wheel vert_tracker;

// Motors
extern pros::Motor frontIntake;
extern pros::Motor mainIntake;
extern pros::MotorGroup ladyBrown;

// Pistons
extern pros::adi::DigitalOut intakePiston;
extern pros::adi::DigitalOut doinkerPiston;
extern pros::adi::DigitalOut clampPiston;

// Sensors
extern pros::Optical clampOptical;
extern pros::Optical intakeOptical;
