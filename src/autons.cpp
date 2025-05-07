#include "main.h"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int MAX_SPEED = 127;
const int SLOW_DRIVE_SPEED = 50;
const int TURN_SPEED = 100;
const int SLOW_TURN_SPEED = 55;
const int SWING_SPEED = 110;

///
// Constants
///
void default_constants() {
  // P, I, D, and Start I
  chassis.pid_drive_constants_set(19.4, 0.0, 109.00);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(10.7, 0.0, 21.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(3.3, 0, 28.75, 0);     // Turn in place constants
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // Exit conditions
  chassis.pid_turn_exit_condition_set(200_ms, 1.5_deg, 350_ms, 5_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 5_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 5_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 0.5_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}


void skills(){

  /*
  // used for color sort!
  SetAllianceMode(AllianceMode::RED);

  // face goal
  chassis.pid_turn_set(27_deg, TURN_SPEED);
  chassis.pid_wait();

  // drive towards goal and pick it up
  chassis.pid_drive_set(-38_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(-18_in);
  chassis.pid_speed_max_set(30);
  chassis.pid_wait_until(-29_in);
  CloseClamp();
  chassis.pid_wait();
  chassis.pid_drive_set(2_in, SLOW_DRIVE_SPEED, true);
  chassis.pid_wait();
  
  // collect first ring
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  // Turn to angle towards 2nd ring
  chassis.pid_turn_set(225_deg, TURN_SPEED);
  chassis.pid_wait();
  
  // Intake second and third rings
  chassis.pid_drive_set(33.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(16_in);
  chassis.pid_speed_max_set(35);
  chassis.pid_wait();
  chassis.pid_turn_set(268_deg, 50);
  chassis.pid_wait();

  // Score wallstake
  AsyncLadyBrown(PRIMED_POSITION);
  chassis.pid_drive_set(12_in, 30, true);
  chassis.pid_wait();
  pros::delay(400);
  chassis.pid_drive_set(-4_in, 30, true);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::STOP);
  AsyncLadyBrown(WALLSTAKE_POSITION);
  chassis.pid_drive_set(5_in, 30, true);
  chassis.pid_wait();
  pros::delay(400);

  //Drive back and intake next ring
  chassis.pid_drive_set(-9_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(30_in);
  chassis.pid_speed_max_set(35);
  chassis.pid_wait();
  chassis.pid_drive_set(-3_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  //Intake corner and score goal
  chassis.pid_turn_set(315_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-12_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(135_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-14_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  OpenClamp();
  chassis.pid_drive_set(18_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  //Intake next stack
  RunIntake(IntakeSpeed::STOP);
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-21_in, 40, true);
  chassis.pid_wait();
  chassis.pid_drive_set(18_in, 50, true);
  chassis.pid_wait();
  chassis.pid_turn_set(180_deg, TURN_SPEED);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::SLOW);
  chassis.pid_drive_set(-21_in, 40, true);
  chassis.pid_wait();
  chassis.pid_drive_set(90_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(40_in);
  chassis.pid_speed_max_set(SLOW_DRIVE_SPEED);
  IntakeWait(AllianceMode::RED, 1500);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::STOP);

  // Turn and clamp goal
  chassis.pid_turn_set(270_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-27_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(-10_in);
  chassis.pid_speed_max_set(30);
  chassis.pid_wait_until(-20_in);
  CloseClamp();
  pros::delay(300);
  RunIntake(IntakeSpeed::FAST);

  // Face ring stack and collect ring
  chassis.pid_turn_set(180_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  pros::delay(200);
  chassis.pid_drive_set(-2_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  // Grab the second ring stack
  chassis.pid_turn_set(270_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-6_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  pros::delay(200);
  
  // Clear corner of rings and score goal
  chassis.pid_turn_set(235_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(17_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  DoinkerDown();
  pros::delay(500);
  chassis.pid_drive_set(-4_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(320_deg, TURN_SPEED);
  chassis.pid_wait();
  DoinkerUp();
  pros::delay(400);
  chassis.pid_turn_set(215_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-15_in, DRIVE_SPEED, true);
  chassis.pid_wait(); */

  /* face corner, release goal
  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-6_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  OpenClamp();

  chassis.pid_drive_set(10_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  */


   // used for color sort!
   SetAllianceMode(AllianceMode::RED);
   IntakeDown();

   // face goal
   chassis.pid_turn_set(27_deg, TURN_SPEED);
   chassis.pid_wait();
 
   // drive towards goal and pick it up
   chassis.pid_drive_set(-38_in, DRIVE_SPEED, true);
   chassis.pid_wait_until(-14_in);
   chassis.pid_speed_max_set(30);
   chassis.pid_wait_until(-29_in);
   CloseClamp();
   chassis.pid_wait();
   chassis.pid_drive_set(2_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   
   // collect first ring
   RunIntake(IntakeSpeed::FAST);
   chassis.pid_turn_set(90_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
   chassis.pid_wait();
 
   // Turn to angle towards 2nd ring
   chassis.pid_turn_set(225_deg, TURN_SPEED);
   chassis.pid_wait();
   
   // Intake second and third rings
   chassis.pid_drive_set(33.5_in, DRIVE_SPEED, true);
   chassis.pid_wait_until(16_in);
   chassis.pid_speed_max_set(35);
   chassis.pid_wait();
   chassis.pid_turn_set(268_deg, TURN_SPEED);
   chassis.pid_wait();
 
   // Score wallstake
   scoreMode = true;
   AsyncLadyBrown(PRIMED_POSITION);
   chassis.pid_drive_set(11_in, 35, true);
   chassis.pid_wait();
   pros::delay(600);
   chassis.pid_drive_set(5_in, 35, true);
   chassis.pid_wait();
   pros::delay(1500);
   RunIntake(IntakeSpeed::STOP);
   AsyncLadyBrown(WALLSTAKE_POSITION);
   scoreMode = false;
   pros::delay(400);
 
   //Drive back and intake next ring
   chassis.pid_drive_set(-10_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   //AsyncLadyBrown(BASE_POSITION);
   chassis.pid_turn_set(0_deg, TURN_SPEED);
   chassis.pid_wait();
   RunIntake(IntakeSpeed::FAST);
   chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
   chassis.pid_wait_until(30_in);
   chassis.pid_speed_max_set(35);
   chassis.pid_wait();
   chassis.pid_drive_set(-3_in, DRIVE_SPEED, true);
   chassis.pid_wait();
 
   //Intake corner and score goal
   chassis.pid_turn_set(315_deg, SLOW_TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   chassis.pid_drive_set(-12_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   chassis.pid_turn_set(135_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(-14_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   OpenClamp();
   chassis.pid_drive_set(18_in, DRIVE_SPEED, true);
   chassis.pid_wait();
 
   //Intake next stack
   chassis.pid_turn_set(180_deg, TURN_SPEED);
   chassis.pid_wait();
   RunIntake(IntakeSpeed::SLOW);
   chassis.pid_drive_set(69_in, SLOW_DRIVE_SPEED, true);
   IntakeWait(AllianceMode::RED, 3000);
   chassis.pid_wait();
   RunIntake(IntakeSpeed::STOP);
 
   // Turn and clamp goal
   chassis.pid_turn_set(270_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(-26_in, 40, true);
   chassis.pid_wait_until(-23_in);
   CloseClamp();
   pros::delay(300);
   RunIntake(IntakeSpeed::FAST);
 
   // Face ring stack and collect ring
   chassis.pid_turn_set(180_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   pros::delay(200);
   //chassis.pid_drive_set(-2_in, DRIVE_SPEED, true);
   //chassis.pid_wait();
 
   // Grab the second ring stack
   chassis.pid_turn_set(270_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   pros::delay(1500);
 
   // face corner, release goal
   RunIntake(IntakeSpeed::STOP);
   chassis.pid_turn_set(45_deg, TURN_SPEED);
   chassis.pid_wait();
   chassis.pid_drive_set(-13_in, DRIVE_SPEED, true);
   chassis.pid_wait_until(-10_in);
   OpenClamp();
   chassis.pid_wait();
   pros::delay(500);

   chassis.pid_drive_set(10_in, DRIVE_SPEED, true);
   chassis.pid_wait();

   // ram back into corner for good measure
   chassis.pid_drive_set(-10_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   chassis.pid_drive_set(10_in, DRIVE_SPEED, true);
   chassis.pid_wait();
   RunIntake(IntakeSpeed::STOP); 


   // attempt hang
   /*chassis.pid_turn_set(225_deg, TURN_SPEED);
   chassis.pid_wait();
   AsyncLadyBrown(WALLSTAKE_POSITION);
   chassis.pid_drive_set(-70_in, SLOW_DRIVE_SPEED, true);
   chassis.pid_wait_until(-40_in);
   chassis.pid_speed_max_set(25);
   chassis.pid_wait();*/

}


void RedMatchAuton() {
  IntakeDown();
  SetAllianceMode(AllianceMode::RED);
  MatchAuton();
}

void BlueMatchAuton() {
  IntakeDown();
  SetAllianceMode(AllianceMode::BLUE);
  MatchAuton();
}


void MatchAuton(){

  // Flip angles dynamically
  //int baseAngleOffset = chassis.odom_theta_direction_get() ? -19 : 360 - (-19);
  //chassis.drive_angle_set(-19);

  // turn to face goal
  chassis.pid_turn_set(-23_deg, 120);
  chassis.pid_wait();

  DoinkerDown();

  // set rings to eject out of the front
  SetRejectMode(EjectMode::FRONT);

  // drive forward and doinker goal
  OpenClamp();
  chassis.pid_drive_set(45_in, DRIVE_SPEED);
  chassis.pid_wait();

  // drive back with the goal
  chassis.pid_drive_set(-41_in, DRIVE_SPEED);
  chassis.pid_wait_until(-20_in);
  
  if (chassis.interfered) {
      // while(true)
      // drive backwards full speed
  }

  DoinkerUp();
  chassis.pid_wait();

  // swing goal into corner
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  // drive back and clamp second goal
  chassis.pid_drive_set(-26_in, SLOW_DRIVE_SPEED);
  chassis.pid_wait_until(-23_in); //21
  CloseClamp();
  chassis.pid_wait();

  // drive toward first stack
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_drive_set(47_in, DRIVE_SPEED);
  chassis.pid_wait();
  //pros::delay(650);
  
  // face second stack and intake
  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_drive_set(30.5_in, 30); 
  //chassis.pid_wait_until(10_in);
  //chassis.pid_speed_max_set(30);
  chassis.pid_wait();
  pros::delay(850);

  // turn to face ring stack 3 and intake
  chassis.pid_turn_set(55_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(6_in, 70);
  chassis.pid_wait();
  
  // swing towards ring on the line
  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100);
  chassis.pid_wait();
  chassis.pid_drive_set(3_in, 40);
  chassis.pid_wait();
  pros::delay(500); //800
  
  //Turn around and drive towards corner
  chassis.pid_drive_set(-20_in, MAX_SPEED);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::STOP);
  chassis.pid_turn_set(20_deg, TURN_SPEED,ez::cw);
  chassis.pid_wait();
  chassis.pid_drive_set(-30_in, MAX_SPEED);
  chassis.pid_wait();

  // turn to face last ring
  chassis.pid_turn_set(-95_deg, TURN_SPEED);
  chassis.pid_wait();

  // if we're red we drop the goal off in the corner
  if(GetAllianceMode() == AllianceMode::RED){ 
    chassis.pid_drive_set(-8_in, DRIVE_SPEED);
    OpenClamp();
    chassis.pid_wait();
    chassis.pid_drive_set(8_in, DRIVE_SPEED);
    chassis.pid_wait();
  } 
  
  // if we're blue we just leave the goal right infront of the negative zone
  else {
    OpenClamp();
  }


  // drive towards last ring
  chassis.pid_drive_set(50_in, DRIVE_SPEED);
  RunIntake(IntakeSpeed::MED);
  //chassis.pid_wait_until(40_in);
  //chassis.pid_speed_max_set(50);
  //IntakeWait(GetAllianceMode(), 5000);
  chassis.pid_wait();
  pros::delay(350);
  RunIntake(IntakeSpeed::STOP);


  // clamp last goal
  chassis.pid_turn_set(201_deg, TURN_SPEED);
  chassis.pid_wait();
  AsyncLadyBrown(WALLSTAKE_POSITION + 2000);
  chassis.pid_drive_set(-32.5_in, DRIVE_SPEED);
  chassis.pid_wait_until(-15_in);
  chassis.pid_speed_max_set(30);
  chassis.pid_wait_until(-27_in);
  CloseClamp();
  chassis.pid_wait();

  pros::delay(300);
  RunIntake(IntakeSpeed::FAST);

  // turn at the end to touch the bar
  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();



  /*// Then drive into corner reversing
  RunIntake(IntakeSpeed::REVERSE);
  chassis.pid_drive_set(20_in, 50);
  chassis.pid_wait();
  pros::delay(400);
  
  // ram into wall while intaking 
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_drive_set(2_in, MAX_SPEED);
  chassis.pid_wait();
  OpenClamp();
  chassis.pid_drive_set(-5_in, 50);
  chassis.pid_wait();

  // turn to face last ring
  chassis.pid_turn_set(-92_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(50_in, MAX_SPEED);
  // if(GetAllianceMode() == AllianceMode::RED){
  //   OpenClamp();
  // } else {
  //   chassis.pid_wait_until(20_in);
  //   OpenClamp();
  // }
  IntakeWait(GetAllianceMode(), 1500);
  chassis.pid_wait();

  // turn to clamp goal at end
  chassis.pid_turn_set(-110_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-25_in, MAX_SPEED);
  chassis.pid_wait();*/
  



  /*
  // turn around to score goal
  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  // if we're red we drop the goal off in the corner
  if(GetAllianceMode() == AllianceMode::RED){ 
    chassis.pid_drive_set(-8_in, DRIVE_SPEED);
    OpenClamp();
    chassis.pid_wait();
    chassis.pid_drive_set(8_in, DRIVE_SPEED);
    chassis.pid_wait();
  } 
  
  // if we're blue we just leave the goal right infront of the negative zone
  else {
    OpenClamp();
  }


  // drive forward a little bit, zig zag to get to the goal
  chassis.pid_drive_set(10_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(119_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-31_in, DRIVE_SPEED);
  chassis.pid_wait_until(-25_in);
  chassis.pid_speed_max_set(30);
  chassis.pid_wait_until(-28_in);
  CloseClamp();
  chassis.pid_wait();


  chassis.pid_turn_set(-135_deg, TURN_SPEED);
  chassis.pid_wait();
  RunIntake(IntakeSpeed::FAST);
  chassis.pid_drive_set(10_in, DRIVE_SPEED);
  chassis.pid_wait(); */

}


void tounge_test(){
  IntakeDown();

  // Intake reverse
  RunIntake(IntakeSpeed::REVERSE);

  // drive to corner rings
  chassis.pid_drive_set(15_in, 90);
  chassis.pid_wait();

  // pulse to prime ring
  chassis.pid_drive_set(4_in, MAX_SPEED);
  RunIntake(IntakeSpeed::PULSE, 1600); // blocking
  chassis.pid_wait();

  // slowly reverse out
  chassis.pid_drive_set(-15_in, 40);
  RunIntake(IntakeSpeed::PULSE, 1500); // blocking
  chassis.pid_wait();

  RunIntake(IntakeSpeed::FAST);
  pros::delay(1000);

  chassis.pid_drive_set(9_in, 50);
  chassis.pid_wait();

}


///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(180_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

///
// Odom Drive PID
///
void odom_drive_example() {
  // This works the same as pid_drive_set, but it uses odom instead!
  // You can replace pid_drive_set with pid_odom_set and your robot will
  // have better error correction.

  chassis.pid_odom_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit
///
void odom_pure_pursuit_example() {
  // Drive to 0, 30 and pass through 6, 10 and 0, 20 on the way, with slew
  chassis.pid_odom_set({{{6_in, 10_in}, fwd, DRIVE_SPEED},
                        {{0_in, 20_in}, fwd, DRIVE_SPEED},
                        {{0_in, 30_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  // Drive to 0, 0 backwards
  chassis.pid_odom_set({{0_in, 0_in}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit Wait Until
///
void odom_pure_pursuit_wait_until_example() {
  chassis.pid_odom_set({{{0_in, 24_in}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait_until_index(1);  // Waits until the robot passes 12, 24
  // Intake.move(127);  // Set your intake to start moving once it passes through the second point in the index
  chassis.pid_wait();
  // Intake.move(0);  // Turn the intake off
}

///
// Odom Boomerang
///
void odom_boomerang_example() {
  chassis.pid_odom_set({{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Boomerang Injected Pure Pursuit
///
void odom_boomerang_injected_pure_pursuit_example() {
  chassis.pid_odom_set({{{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Calculate the offsets of your tracking wheels
///
void measure_offsets() {
  // Number of times to test
  int iterations = 10;

  // Our final offsets
  double l_offset = 0.0, r_offset = 0.0, b_offset = 0.0, f_offset = 0.0;

  // Reset all trackers if they exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->reset();
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->reset();
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->reset();
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->reset();
  
  for (int i = 0; i < iterations; i++) {
    // Reset pid targets and get ready for running an auton
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    chassis.odom_xyt_set(0_in, 0_in, 0_deg);
    double imu_start = chassis.odom_theta_get();
    double target = i % 2 == 0 ? 90 : 270;  // Switch the turn target every run from 270 to 90

    // Turn to target at half power
    chassis.pid_turn_set(target, 63, ez::raw);
    chassis.pid_wait();
    pros::delay(250);

    // Calculate delta in angle
    double t_delta = util::to_rad(fabs(util::wrap_angle(chassis.odom_theta_get() - imu_start)));

    // Calculate delta in sensor values that exist
    double l_delta = chassis.odom_tracker_left != nullptr ? chassis.odom_tracker_left->get() : 0.0;
    double r_delta = chassis.odom_tracker_right != nullptr ? chassis.odom_tracker_right->get() : 0.0;
    double b_delta = chassis.odom_tracker_back != nullptr ? chassis.odom_tracker_back->get() : 0.0;
    double f_delta = chassis.odom_tracker_front != nullptr ? chassis.odom_tracker_front->get() : 0.0;

    // Calculate the radius that the robot traveled
    l_offset += l_delta / t_delta;
    r_offset += r_delta / t_delta;
    b_offset += b_delta / t_delta;
    f_offset += f_delta / t_delta;
  }

  // Average all offsets
  l_offset /= iterations;
  r_offset /= iterations;
  b_offset /= iterations;
  f_offset /= iterations;

  // Set new offsets to trackers that exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->distance_to_center_set(l_offset);
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->distance_to_center_set(r_offset);
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->distance_to_center_set(b_offset);
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->distance_to_center_set(f_offset);
}
