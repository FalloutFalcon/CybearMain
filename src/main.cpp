#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	lcd::set_text(1, "Hello PROS User!");

	lcd::register_btn1_cb(on_center_button);

	Motor front_left_wheel_initializer (FRONT_LEFT_WHEEL_PORT, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
    Motor back_left_wheel_initializer (BACK_LEFT_WHEEL_PORT, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
    Motor front_right_wheel_initializer (FRONT_RIGHT_WHEEL_PORT, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
    Motor back_right_wheel_initializer (BACK_RIGHT_WHEEL_PORT, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {}

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
	Controller controller_1(E_CONTROLLER_MASTER);
	Motor front_left_wheel(FRONT_LEFT_WHEEL_PORT);
	Motor front_right_wheel(FRONT_RIGHT_WHEEL_PORT);
	Motor back_left_wheel(BACK_LEFT_WHEEL_PORT);
	Motor back_right_wheel(BACK_RIGHT_WHEEL_PORT);

	while (true) {
		lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		
		// calculate the drivetrain motor velocities from the controller joystick axies
        // left = Axis3 + Axis1
        // right = Axis3 - Axis1
        int left = controller_1.get_analog(ANALOG_LEFT_Y) + controller_1.get_analog(ANALOG_RIGHT_X);
        int right = controller_1.get_analog(ANALOG_LEFT_Y) - controller_1.get_analog(ANALOG_RIGHT_X);

	    char print_left = left;
	    char print_right = right;

		//lcd::print(4, print_left );
		//lcd::print(5, print_right );

		front_left_wheel.move(left);
		front_right_wheel.move(right);
		back_left_wheel.move(left);
		back_right_wheel.move(right);
		

		delay(20);
	}
}
