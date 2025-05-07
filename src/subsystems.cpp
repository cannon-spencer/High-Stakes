#include "main.h"

pros::Motor frontIntake(20, pros::MotorGearset::blue);
pros::Motor mainIntake(-2, pros::MotorGearset::blue);

pros::MotorGroup ladyBrown({19, -13}, pros::MotorGearset::green);

pros::adi::DigitalOut intakePiston('C');
pros::adi::DigitalOut doinkerPiston('B');
pros::adi::DigitalOut clampPiston('A');

pros::Optical clampOptical(1);
pros::Optical intakeOptical(17);

//pros::Controller master(pros::E_CONTROLLER_MASTER);

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {-21, -8, -6},     // Left Chassis Ports (negative port will reverse it!)
    {5, 3, 10},  // Right Chassis Ports (negative port will reverse it!)

    14,      // IMU Port
    3.25,   // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    458.8235292);  // Wheel RPM = cartridge * (motor gear / wheel gear)

//  - you should get positive values on the encoders going FORWARD and RIGHT
//ez::tracking_wheel horiz_tracker(12, 2, -3.8125);  // This tracking wheel is perpendicular to the drive wheels
//ez::tracking_wheel vert_tracker(11, 2, 0);   // This tracking wheel is parallel to the drive wheels