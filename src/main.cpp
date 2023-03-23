#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Initialize!");

	pros::Motor front_left_wheel_initializer (FRONT_LEFT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor back_left_wheel_initializer (BACK_LEFT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor front_right_wheel_initializer (FRONT_RIGHT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor back_right_wheel_initializer (BACK_RIGHT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor launcher_initializer (LAUNCHER_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor intake_initializer (INTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor expansion_initializer (EXPANSION_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	pros::lcd::set_text(1, "Disabled!");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	pros::lcd::set_text(1, "Competition Initalize!");
	// uses the 3 buttons to select a auton script
	//lcdselect();
	//autonsetup();

}

/**
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
	pros::lcd::set_text(1, "Autonomous!");
	//autonstart();
}

/**
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
	pros::lcd::set_text(1, "Opcontrol!");
	pros::Controller controller_1(pros::E_CONTROLLER_MASTER);
	pros::Motor front_left_wheel(FRONT_LEFT_WHEEL_PORT);
	pros::Motor front_right_wheel(FRONT_RIGHT_WHEEL_PORT);
	pros::Motor back_left_wheel(BACK_LEFT_WHEEL_PORT);
	pros::Motor back_right_wheel(BACK_RIGHT_WHEEL_PORT);

	pros::Motor launcher(LAUNCHER_PORT);
	pros::Motor intake(INTAKE_PORT);
	pros::Motor expansion(EXPANSION_PORT);
	lcdselect();

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		
		// calculate the drivetrain motor velocities from the controller joystick axies
        // left = Axis3 + Axis1
        // right = Axis3 - Axis1
        int left = controller_1.get_analog(ANALOG_LEFT_Y) + controller_1.get_analog(ANALOG_RIGHT_X);
        int right = controller_1.get_analog(ANALOG_LEFT_Y) - controller_1.get_analog(ANALOG_RIGHT_X);

		pros::lcd::print(4, "left = %d right = %d", left, right);

		front_left_wheel.move(left);
		front_right_wheel.move(right);
		back_left_wheel.move(left);
		back_right_wheel.move(right);

		if (controller_1.get_digital(DIGITAL_R1)) {
          intake.move(127);
        }
		else if(controller_1.get_digital(DIGITAL_R2)) {
		  intake.move(-127);
		}
        else {
          intake.move(0);
        }

		if (controller_1.get_digital(DIGITAL_L1)) {
          launcher.move(127);
        }
		else if(controller_1.get_digital(DIGITAL_L2)) {
		  launcher.move(-127);
		}
        else {
          launcher.move(0);
        }

		pros::delay(20);
	}
}
