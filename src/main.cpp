/**
 * @file main.cpp
 * @brief Core control entry point for VEX competition lifecycle.
 *
 * This file defines all lifecycle functions required by the PROS competition template:
 * - `initialize()` for pre-match hardware setup
 * - `autonomous()` for executing selected routines
 * - `opcontrol()` for handling tele-op logic
 *
 * Subsystems like intake, lift, clamp, and drive are modularized and called from here.
 * The file also integrates EZ-Template for odometry-based movement and autonomous selection.
 *
 * @note
 *   This is the central coordination point for the robot's runtime behavior.
 */

#include "main.h"
#include "subsystems.hpp"

/**
 * @brief Initializes all robot hardware, chassis, and sensors.
 *
 * Sets up odometry, optical sensor sampling rates, autonomous routines, and controller curves.
 * Configures tools like autonomous selector and drive curve settings.
 */
void initialize() {

  ez::ez_template_print();
  pros::delay(500);

  chassis.odom_tracker_back_set(&horiz_tracker);
  chassis.odom_tracker_left_set(&vert_tracker);

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(false);    // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.0);     // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0.25, 2.25);  // Defaults for curve. If using tank, only the first parameter is used.

  // Set the drive to constants from autons.cpp
  default_constants();

  // Use a limit switch to select autons
  ez::as::limit_switch_lcd_initialize(&selectButton);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      {"Blue Match Auton\n\nNegative Setup", BlueMatchAuton},
      {"Red Match Auton\n\nPositive Setup", RedMatchAuton},
      {"Skills\n\nRight side setup", skills},
      {"Tounge Test", tounge_test},
      {"Drive\n\nDrive forward and come back", drive_example},
      {"Turn\n\nTurn 3 times.", turn_example},
      {"Drive and Turn\n\nDrive forward, turn, come back", drive_and_turn},
      {"Drive and Turn\n\nSlow down during drive", wait_until_change_speed},
      {"Swing Turn\n\nSwing in an 'S' curve", swing_example},
      {"Motion Chaining\n\nDrive forward, turn, and come back, but blend everything together :D", motion_chaining},
      {"Combine all 3 movements", combining_movements},
      {"Simple Odom\n\nThis is the same as the drive example, but it uses odom instead!", odom_drive_example},
      {"Pure Pursuit\n\nGo to (0, 30) and pass through (6, 10) on the way.  Come back to (0, 0)", odom_pure_pursuit_example},
      {"Pure Pursuit Wait Until\n\nGo to (24, 24) but start running an intake once the robot passes (12, 24)", odom_pure_pursuit_wait_until_example},
      {"Boomerang\n\nGo to (0, 24, 45) then come back to (0, 0, 0)", odom_boomerang_example},
      {"Boomerang Pure Pursuit\n\nGo to (0, 24, 45) on the way to (24, 24) then come back to (0, 0, 0)", odom_boomerang_injected_pure_pursuit_example},
      {"Measure Offsets\n\nThis will turn the robot a bunch of times and calculate your offsets for your tracking wheels.", measure_offsets},
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");

  // Update optical sensor every 15ms instead of every 100ms
  intakeOptical.set_integration_time(15);

  // Update rotation sensor a little faster 
  liftRotation.set_data_rate(5);

  liftPID.exit_condition_set(80, 50, 300, 150, 500, 500);

}


/**
 * @brief Handles logic for when the robot enters the disabled state.
 * 
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled(){}

/**
 *
 * @brief Competition-safe setup function for pre-autonomous preparation.
 *
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  IntakeUp(); // default intake up
}

/**
 * @brief Executes the autonomous routine selected from the LCD menu.
 * 
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();                   // Resets PID targets to 0
  chassis.drive_imu_reset();                     // Reset gyro position to 0
  chassis.drive_sensor_reset();                  // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);       // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);     // Set motors to hold.  This helps autonomous consistency
  IntakeTask.resume();
  LiftTask.resume();
  LiftTask.notify();
  ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}


/**
 * @brief Helper function to print tracker sensor data to the Brain screen.
 *
 * Includes tracker reading and its calibrated offset from the chassis center.
 *
 * @param tracker Pointer to a tracking wheel object.
 * @param name    Tracker label ("l", "r", "b", "f").
 * @param line    Brain screen line number to print to.
 */
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Check if the tracker exists
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}


/**
 *
 * @brief Runs an odometry debug page on the screen.
 *
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
  while (true) {
    // Only run this when not connected to a competition switch
    if (!pros::competition::is_connected()) {
      // Blank page for odom debugging
      if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
        // If we're on the first blank page...
        if (ez::as::page_blank_is_on(0)) {
          // Display X, Y, and Theta
          ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
                               "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
                               "\na: " + util::to_string_with_precision(chassis.odom_theta_get()),
                           1);  // Don't override the top Page line

          // Display all trackers that are being used
          screen_print_tracker(chassis.odom_tracker_left, "l", 4);
          screen_print_tracker(chassis.odom_tracker_right, "r", 5);
          screen_print_tracker(chassis.odom_tracker_back, "b", 6);
          screen_print_tracker(chassis.odom_tracker_front, "f", 7);
        }
      }
    }

    // Remove all blank pages when connected to a comp switch
    else {
      if (ez::as::page_blank_amount() > 0)
        ez::as::page_blank_remove_all();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task ezScreenTask(ez_screen_task);


/**
 * @brief Main driver control loop.
 *
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  // ensure tasks are resumed before match
  IntakeTask.resume();
  LiftTask.resume();
  LiftTask.notify();

  // make sure intake is down
  IntakeDown(); 

  // set lady brown score mode off
  scoreMode = false;

  // Store the start time for timer calculations
  matchStartTime = pros::millis();

  while (true) {
    // Run the drive mode
    ChassisController(drive_type::ARCADE_SPLIT);

    // Run doinker - driver control
    DoinkerController();

    // Run clamp - driver control
    ClampController();

    // Run timer that vibrates leading up to 30 seconds
    CompTimerController();

    // NOTE: intake and lift run on their own tasks

    pros::delay(ez::util::DELAY_TIME);
  }
}
