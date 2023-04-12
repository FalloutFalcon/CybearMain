#include "main.h"
using namespace okapi;
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

std::shared_ptr<ChassisController> drive;

int auton_mode = -1;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Initialize!");
	okapiinitialize();

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
	rightsideauton();
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
	okapiopcontrol();
}

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



void okapiinitialize () {
    pros::lcd::set_text(2, "Okapi initalize!");
    //sets the string to hold the motor steady and prevents it from unspooling during match
    expansion.setBrakeMode(AbstractMotor::brakeMode::hold);

    std::shared_ptr<ChassisController> drivebuild =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
    drive = drivebuild;
}

void okapiopcontrol () {
    pros::lcd::set_text(2, "Okapi opcontrol!");

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
            autonstart();
			lcdselect();
            
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
    drive->setMaxVelocity(100);
    drive->moveDistance(6_in);
    pros::delay(i * 1000);
    drive->moveDistance(-3_in);
    top_intake.moveVoltage(0);
}


void leftsideauton () {
    pros::lcd::set_text(7, "Left auton started");
    roller(1);
}

void rightsideauton () {
    pros::lcd::set_text(7, "Right auton started");
    drive->moveDistance(6_in);
    drive->turnAngle(90_deg);   
    roller(1);
    drive->turnAngle (-90_deg);
    drive->moveDistance(3_ft);
    //intake_group.moveVoltage(-12000);
    //launcher.moveVoltage(-12000);
}

void skillsauton () {
    Motor expansion(EXPANSION_PORT);
    pros::lcd::set_text(7, "Skills auton selected");
    roller(2);
    drive->setMaxVelocity(50);
    drive->turnAngle(90_deg);
    drive->waitUntilSettled();
    //expansion.moveVoltage(-6000);
    drive->moveDistance(-24_ft);               
    
}

void lcdselect () {
    pros::lcd::set_text(2, "LCDSelect!");
	while (auton_mode == -1) {
		unsigned int button = pros::lcd::read_buttons();
        if (button == 1) {
            auton_mode = 1;
        } else if (button == 2) {
            auton_mode = 2;
        } else if (button == 4) {
            auton_mode = 3;
        }
        pros::delay(10);
	}
    pros::lcd::print(7, "auton mode selected = %d", auton_mode); 
}

void autonstart () {
    pros::lcd::set_text(2, "Autonstart!");
    if (auton_mode == 0) {
        pros::lcd::set_text(7, "No auton started");
    } else if (auton_mode == 1) {
        pros::lcd::set_text(7, "Left auton started");
        leftsideauton();
    } else if (auton_mode == 2) {
        pros::lcd::set_text(7, "Skills auton started");
        skillsauton();
    } else if (auton_mode == 3)  {
        pros::lcd::set_text(7, "Right auton started");
        rightsideauton();
    } else {
        pros::lcd::set_text(7, "Error in auton start");
    }    
}
