#include "main.h"

pros::Motor frontIntake(-6, pros::MotorGearset::blue);
pros::MotorGroup mainIntake({-10, 21}, pros::MotorGearset::blue);

pros::MotorGroup ladyBrown({8, -9}, pros::MotorGearset::green);
pros::Rotation liftRotation(7);

pros::ADIDigitalOut intakePiston('H');
pros::ADIDigitalOut doinkerPiston('F');
pros::ADIDigitalOut clampPiston('E');

pros::Optical clampOptical(3);
pros::Optical intakeOptical(19);

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {5, -13, 2, -4},  // Left Chassis Ports (negative port will reverse it!)
    {11, -1, -14, 12},  // Right Chassis Ports (negative port will reverse it!)

    20,      // IMU Port
    3.25,   // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    600);  // Wheel RPM = cartridge * (motor gear / wheel gear)

//  - you should get positive values on the encoders going FORWARD and RIGHT
ez::tracking_wheel horiz_tracker(17, 2, -4.18);  // This tracking wheel is perpendicular to the drive wheels
ez::tracking_wheel vert_tracker(15, 2, -0.54);   // This tracking wheel is parallel to the drive wheels