#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

#include "Subsystem-Files/clamp.hpp"
#include "Subsystem-Files/doinker.hpp"
#include "Subsystem-Files/drive.hpp"
#include "Subsystem-Files/intake.hpp"
#include "Subsystem-Files/lift.hpp"
#include "Subsystem-Files/comp_timer.hpp"

// EZ Constructors
extern Drive chassis;

extern ez::tracking_wheel horiz_tracker;
extern ez::tracking_wheel vert_tracker;

// Motors
extern pros::Motor frontIntake;
extern pros::MotorGroup mainIntake;
extern pros::MotorGroup ladyBrown;

// Pistons
extern pros::ADIDigitalOut intakePiston;
extern pros::ADIDigitalOut doinkerPiston;
extern pros::ADIDigitalOut clampPiston;

// Button
extern pros::adi::DigitalIn selectButton;

// Sensors
extern pros::Rotation liftRotation;
extern pros::Optical clampOptical;
extern pros::Optical intakeOptical;

