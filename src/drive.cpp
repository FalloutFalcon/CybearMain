#include "main.h"
using namespace okapi;

std::shared_ptr<ChassisController> drive;

namespace cybear {
void okapiinitialize () {
    pros::lcd::set_text(2, "Okapi initalize!");
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(-FRONT_LEFT_WHEEL_PORT, FRONT_RIGHT_WHEEL_PORT, -BACK_LEFT_WHEEL_PORT, BACK_RIGHT_WHEEL_PORT)
            // Green gearset, 4 in wheel diam, 11.5 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
            .build();
}

void prosinitialize () {
    pros::lcd::set_text(2, "Pros initalize!");
    pros::Motor front_left_wheel_initializer (FRONT_LEFT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor back_left_wheel_initializer (BACK_LEFT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor front_right_wheel_initializer (FRONT_RIGHT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor back_right_wheel_initializer (BACK_RIGHT_WHEEL_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor launcher_initializer (LAUNCHER_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor top_intake_initializer (TOP_INTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor bottom_intake_initializer (BOTTOM_INTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor expansion_initializer (EXPANSION_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
}

void okapiopcontrol () {
    pros::lcd::set_text(2, "Okapi opcontrol!");

    pros::lcd::set_text(2, "Okapi initalize!");
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {-FRONT_LEFT_WHEEL_PORT, -BACK_LEFT_WHEEL_PORT}, 
                {FRONT_RIGHT_WHEEL_PORT, BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 11.5 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 9.5_in}, imev5GreenTPR})
            .build();
    // Joystick to read analog values for tank or arcade control
    // Master controller by default
    Controller controller;

    // launcher related objects
    ControllerButton launcher_forward_button(ControllerDigital::R1);
    ControllerButton launcher_reverse_button(ControllerDigital::R2);
    Motor launcher_motor(LAUNCHER_PORT);

    ControllerButton intake_forward_button(ControllerDigital::L1);
    ControllerButton intake_reverse_button(ControllerDigital::L2);
	Motor top_intake(TOP_INTAKE_PORT);
    Motor bottom_intake(BOTTOM_INTAKE_PORT);
    MotorGroup intake_group({top_intake, bottom_intake});

    // Button to run our sample autonomous routine
    ControllerButton run_auto_button(ControllerDigital::X);

    while (true) {
        // Arcade drive with the left stick
        drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
                                  controller.getAnalog(ControllerAnalog::leftX));

        // else, the launcher isn't all the way down
        if (launcher_forward_button.isPressed()) {
            launcher_motor.moveVoltage(12000);
        } else if (launcher_reverse_button.isPressed()) {
            launcher_motor.moveVoltage(-12000);
        } else {
            launcher_motor.moveVoltage(0);
        }

        if (intake_forward_button.isPressed()) {
            intake_group.moveVoltage(12000);
        } else if (intake_reverse_button.isPressed()) {
            intake_group.moveVoltage(-12000);
        } else {
            intake_group.moveVoltage(0);
        }
        }

        // Run the test autonomous routine if we press the button
        if (run_auto_button.changedToPressed()) {
            // Drive the robot in a square pattern using closed-loop control
            for (int i = 0; i < 4; i++) {
                drive->moveDistance(12_in); // Drive forward 12 inches
                drive->turnAngle(90_deg);   // Turn in place 90 degrees
            }
        }

        // Wait and give up the time we don't need to other tasks.
        // Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
        pros::delay(10);
    }

void prosopcontrol () {
    pros::lcd::set_text(2, "Pros opcontrol!");
    pros::Controller controller_1(pros::E_CONTROLLER_MASTER);
	pros::Motor front_left_wheel(FRONT_LEFT_WHEEL_PORT);
	pros::Motor front_right_wheel(FRONT_RIGHT_WHEEL_PORT);
	pros::Motor back_left_wheel(BACK_LEFT_WHEEL_PORT);
	pros::Motor back_right_wheel(BACK_RIGHT_WHEEL_PORT);

	pros::Motor launcher(LAUNCHER_PORT);
	pros::Motor top_intake(TOP_INTAKE_PORT);
    pros::Motor bottom_intake(BOTTOM_INTAKE_PORT);
    pros::Motor_Group intake({top_intake, bottom_intake});
	pros::Motor expansion(EXPANSION_PORT);

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
}

