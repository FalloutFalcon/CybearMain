#include "main.h"
using namespace okapi;
/*
std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
*/

Controller controller;
ControllerButton right_1_button(ControllerDigital::R1);
ControllerButton right_2_button(ControllerDigital::R2);
ControllerButton left_1_button(ControllerDigital::L1);
ControllerButton left_2_button(ControllerDigital::L2);
ControllerButton a_button(ControllerDigital::A);
ControllerButton b_button(ControllerDigital::B);
ControllerButton x_button(ControllerDigital::X);
ControllerButton y_button(ControllerDigital::Y);
ControllerButton up_button(ControllerDigital::up);
ControllerButton down_button(ControllerDigital::down);

// Launcher related objects
Motor launcher(LAUNCHER_PORT, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::counts);
int launcher_efficency;
int launcher_voltage = 12000;

// Intake related objects
Motor top_intake(TOP_INTAKE_PORT);
Motor bottom_intake(BOTTOM_INTAKE_PORT);
MotorGroup intake_group({top_intake, bottom_intake});
int top_efficency;
int bottom_efficency;

Motor expansion(EXPANSION_PORT);

double forward_move;
double yaw_move;

void controllerbuild () {
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
}

namespace cybear {
void okapiinitialize () {
    pros::lcd::set_text(2, "Okapi initalize!");
    //controllerbuild();
    //sets the string to hold the motor steady and prevents it from unspooling during match
    expansion.setBrakeMode(AbstractMotor::brakeMode::hold);
}

void okapiopcontrol () {
    pros::lcd::set_text(2, "Okapi opcontrol!");
    // Master controller by default
    
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();

    while (true) {
        // Arcade drive with the left stick controller.getAnalog(ControllerAnalog::leftY) > 10 && controller.getAnalog(ControllerAnalog::rightX) > 10
        forward_move = controller.getAnalog(ControllerAnalog::leftY);
        yaw_move = controller.getAnalog(ControllerAnalog::rightX);
        if (abs (forward_move) > .2 || abs (yaw_move) > .2) {
            drive->getModel()->arcade(forward_move, yaw_move);     
        } else {
            drive->getModel()->arcade(0, 0); 
        } 

        if (y_button.changedToPressed()) {
            disklaunch(3);
        }
        

        //Allows for rapidly changing the voltage on the launcher it also prevents
        //It from going above recommended voltage or putting it in the negatives which reverses the spins on the motors
        if (up_button.changedToPressed() && launcher_voltage <= 11000) {
            launcher_voltage = launcher_voltage + 1000;
            pros::lcd::print(5, "VOLTAGE:Launcher:%d/12000", launcher_voltage); 
        }
        if (down_button.changedToPressed() && launcher_voltage >= 1000) {
            launcher_voltage = launcher_voltage - 1000;
            pros::lcd::print(5, "VOLTAGE:Launcher:%d/12000", launcher_voltage); 
        }

        if (right_1_button.isPressed())  {
            launcher.moveVoltage(launcher_voltage);
        } else if (right_2_button.isPressed()) {
            launcher.moveVoltage(-launcher_voltage);
        } else {
            launcher.moveVoltage(0);
        }

        if (left_1_button.isPressed()) {
            intake_group.moveVoltage(launcher_voltage);
        } else if (left_2_button.isPressed()) {
            intake_group.moveVoltage(-launcher_voltage);
        } else {
            top_intake.moveVoltage(0);
            bottom_intake.moveVelocity(forward_move);
        }

        if (a_button.isPressed()) {
            expansion.moveVoltage(launcher_voltage);
        } else if (b_button.isPressed()) {
            expansion.moveVoltage(-launcher_voltage);
        } else {
            expansion.moveVoltage(0);
        }
        launcher_efficency = launcher.getEfficiency();
        top_efficency = top_intake.getEfficiency();
        bottom_efficency = bottom_intake.getEfficiency();
        pros::lcd::print(4, "EFF:out:%d Mid:%d In:%d", launcher_efficency, top_efficency, bottom_efficency);
        // Run the test autonomous routine if we press the button
        
        
        if (x_button.changedToPressed()) {
            pros::lcd::set_text(2, "Auton Test!");
            cybear::lcdselect();
            cybear::autonstart();
        }
        

        // Wait and give up the time we don't need to other tasks.
        // Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
        pros::delay(10); 
    }


}

void disklaunch (int i) {
    launcher.moveVoltage(12000);
    pros::delay(1000);
    intake_group.moveVoltage(12000);
    pros::delay(i * 1000);
    launcher.moveVoltage(0);
    intake_group.moveVoltage(0);
}

void roller (int i) {
    top_intake.moveVoltage(6000);
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
    drive->moveDistance(3_in);
    pros::delay(i * 1000);
    drive->moveDistance(-3_in);
    top_intake.moveVoltage(0);
}

void leftsideauton () {
    pros::lcd::set_text(7, "Left auton started");
    //controllerbuild();
    roller(2);
}

void rightsideauton () {
    pros::lcd::set_text(7, "Right auton started");
    //controllerbuild();
    roller(2);
}

void skillsauton () {
    pros::lcd::set_text(7, "Skills auton selected");
    std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
    roller(2);
    drive->setMaxVelocity(200);
    drive->turnAngle(90_deg);
    drive->waitUntilSettled();
    drive->moveDistance(12_ft);
    expansion.moveVoltage(12000);
}
}
