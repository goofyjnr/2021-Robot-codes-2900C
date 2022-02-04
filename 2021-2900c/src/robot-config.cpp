#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftMotorA = motor(PORT15, ratio18_1, false);
motor leftMotorB = motor(PORT14, ratio18_1, true);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT11, ratio18_1, false);
motor rightMotorB = motor(PORT12, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);
motor goalm = motor(PORT17,ratio36_1,false);
motor spinb = motor(PORT10, ratio18_1, true);
motor spiner = motor(PORT9,ratio18_1,true);
motor hook = motor(PORT1,ratio18_1,true);
controller Controller1 = controller(primary);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1RightShoulderControlMotorsStopped = true;
bool Controller1LeftShoulderControlMotorsStopped = true;
bool DrivetrainNeedsToBeStopped_Controller1 = true;
bool Controller1XBButtonsControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis4
      // right = Axis3 - Axis4
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis4.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis4.position();
      
      // check if the values are inside of the deadband range
      if (abs(drivetrainLeftSideSpeed) < 5 && abs(drivetrainRightSideSpeed) < 5) {
        // check if the motors have already been stopped
        if (DrivetrainNeedsToBeStopped_Controller1) {
          // stop the drive motors
          LeftDriveSmart.stop();
          RightDriveSmart.stop();
          // tell the code that the motors have been stopped
          DrivetrainNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the motors next time the input is in the deadband range
        DrivetrainNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonR1/ButtonR2 status to control Left_4bar
      if (Controller1.ButtonR1.pressing()) {
        goalm.spin(directionType::rev,50,velocityUnits::pct);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        goalm.spin(directionType::fwd,80,velocityUnits::pct);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        goalm.stop(brakeType::coast);
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }

      if(Controller1.ButtonA.pressing()){
        goalm.spinTo(15, degrees);
      }
      else if (Controller1.ButtonY.pressing()) {
        goalm.spinTo(245, degrees);
      }
      if (Controller1.ButtonL1.pressing()) {
        spinb.spin(directionType::fwd,20,velocityUnits::pct);
        spiner.spin(directionType::fwd,30,velocityUnits::pct);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        spinb.spin(directionType::rev,40,velocityUnits::pct);
        spiner.spin(directionType::rev,40,velocityUnits::pct);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        spinb.stop(brakeType::coast);
        spiner.stop(brakeType::coast);
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      }
    if (Controller1.ButtonB.pressing()) {
        hook.spin(forward);
        Controller1XBButtonsControlMotorsStopped = false;
      } else if (Controller1.ButtonX.pressing()) {
        hook.spin(reverse);
        Controller1XBButtonsControlMotorsStopped = false;
      } else if (!Controller1XBButtonsControlMotorsStopped) {
        hook.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1XBButtonsControlMotorsStopped = true;
      }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}