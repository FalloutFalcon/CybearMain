/*
https://github.com/FalloutFalcon/CybearMain
*/
#include "main.h"
using namespace okapi;
Controller controller;
// way to many lines for this tbh
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
ControllerButton left_button(ControllerDigital::left);
ControllerButton right_button(ControllerDigital::right);

// Launcher related objects
Motor launcher(LAUNCHER_PORT, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::counts);
int launcher_efficency;
int motor_voltage = 12000;
int current_laucher_voltage;
bool launcher_on = false;

// Intake related objects
Motor top_intake(TOP_INTAKE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::counts);
Motor bottom_intake(BOTTOM_INTAKE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::counts);
MotorGroup intake_group({top_intake, bottom_intake});
int top_efficency;
int bottom_efficency;

Motor expansion(EXPANSION_PORT);

// vision related objects
ADIUltrasonic ultrasonic(ULTRASONIC_PING_PORT, ULTRASONIC_ECHO_PORT);
pros::Vision vision_sensor(VISION_PORT);
int objects;
double disk_x_mid;
double disk_y_mid;
double disk_dir;
bool disk_ready;

double forward_move;
double yaw_move;

std::shared_ptr<ChassisController> drive;

int auton_mode = 0;

Timer timer;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Initialize!");
	driveinitialize();
    visionintit();
    // sets the string to hold the motor steady and prevents it from unspooling during match
    expansion.setBrakeMode(AbstractMotor::brakeMode::hold);
    pros::lcd::set_text(1, "Initialize Done!");

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	pros::lcd::set_text(1, "Disabled!");
    // should already do this but its a backup just in case
    drive->getModel()->arcade(0, 0); 
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
	//lcdselect();
    pros::lcd::set_text(1, "Competition Initalize Done!");

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
	//lcd select and auton start belong here but they still remain pretty buggy and dont work when not in comp
    skillsauton();
    pros::lcd::set_text(1, "Autonomous Done!");
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
    //skillsauton();
	okapiopcontrol();
    pros::lcd::set_text(1, "Opcontrol Done!");
}

void driveinitialize() {
    pros::lcd::set_text(2, "Drive initalize!");
    std::shared_ptr<ChassisController> drivebuild =
        ChassisControllerBuilder()
            .withMotors(
                {FRONT_LEFT_WHEEL_PORT, BACK_LEFT_WHEEL_PORT}, 
                {-FRONT_RIGHT_WHEEL_PORT, -BACK_RIGHT_WHEEL_PORT}
            )
            // Green gearset, 4 in wheel diam, 12 in wheel track
            .withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
            .build();
    pros::lcd::set_text(2, "Drive initalize Done!");
    drive = drivebuild;
}

void okapiopcontrol() {
    pros::lcd::set_text(2, "Okapi opcontrol!");

    while (true) {
        //graphMotor();
        //Arcade drive with the left stick controller.getAnalog(ControllerAnalog::leftY) > 10 && controller.getAnalog(ControllerAnalog::rightX) > 10
        forward_move = controller.getAnalog(ControllerAnalog::leftY);
        yaw_move = controller.getAnalog(ControllerAnalog::rightX);
        if (abs (forward_move) > .2 || abs (yaw_move) > .2) {
            drive->getModel()->arcade(forward_move, yaw_move);     
        } else {
            drive->getModel()->arcade(0, 0); 
        }         

        // Allows for rapidly changing the voltage on the launcher it also prevents
        // It from going above recommended voltage or putting it in the negatives which reverses the spins on the motors
        if (up_button.changedToPressed() && motor_voltage <= 11000) {
            motor_voltage = motor_voltage + 1000;
            pros::lcd::print(5, "VOLTAGE:Launcher:%d/12000", motor_voltage); 
        }
        if (down_button.changedToPressed() && motor_voltage >= 1000) {
            motor_voltage = motor_voltage - 1000;
            pros::lcd::print(5, "VOLTAGE:Launcher:%d/12000", motor_voltage); 
        }
        
        current_laucher_voltage = launcher.getVoltage();
        if(launcher_on == true) {
            if (right_1_button.changedToPressed() || right_2_button.changedToPressed()) {
                launcher.moveVoltage(0);
                launcher_on = false;
                pros::lcd::set_text(5, "Disk Launch = False!");
            }
        }
        else if (launcher_on == false) {
            if (right_1_button.changedToPressed()) {
                launcher.moveVoltage(motor_voltage);
                launcher_on = true;
                pros::lcd::set_text(6, "Disk Launch = True!");
            }
            if (right_2_button.changedToPressed()) {
                launcher.moveVoltage(-motor_voltage);
                launcher_on = true;
                pros::lcd::set_text(6, "Disk Launch = True!");
            }
        }

        if (left_1_button.isPressed()) {
            intake_group.moveVoltage(motor_voltage);
        } else if (left_2_button.isPressed()) {
            intake_group.moveVoltage(-motor_voltage);
        } else {
            top_intake.moveVoltage(0);
            bottom_intake.moveVelocity(forward_move);
        }

        if (a_button.isPressed()) {
            expansion.moveVoltage(motor_voltage);
        } else if (b_button.isPressed()) {
            expansion.moveVoltage(-motor_voltage);
        } else {
            expansion.moveVoltage(0);
        }

        launcher_efficency = launcher.getEfficiency();
        top_efficency = top_intake.getEfficiency();
        bottom_efficency = bottom_intake.getEfficiency();
        pros::lcd::print(4, "EFF:out:%d Mid:%d In:%d", launcher_efficency, top_efficency, bottom_efficency);
        
        // Run the test autonomous routine if we press the button
        if (x_button.changedToPressed()) {
            launcher.moveVoltage(0);
        }
            
        // Wait and give up the time we don't need to other tasks.
        // Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
        pros::delay(10); 
    }
    pros::lcd::set_text(2, "Okapi opcontrol Done!");
}

void disklaunch() {
    pros::lcd::set_text(2, "Disk launch!");
    launcher.moveVoltage(12000);
    pros::delay(5000);
    intake_group.moveVoltage(12000);
    pros::delay(3000);
    launcher.moveVoltage(0);
    intake_group.moveVoltage(0);
    pros::lcd::set_text(2, "Disk launch done!");
}

void roller (double i) {
    pros::lcd::set_text(2, "Roller!");
    top_intake.moveVoltage(-12000);
    drive->getModel()->arcade(0.5, 0); 
    pros::delay(i * 1000);
    top_intake.moveVoltage(0);
    drive->getModel()->arcade(0, 0); 
    pros::lcd::set_text(2, "Roller done!");
}

/**
 * sets up the vision sensor to track disks
 */
void visionintit() {
    pros::lcd::set_text(2, "Vision intit!");
    pros::Vision vision_sensor (VISION_PORT);
    vision_sensor.clear_led();
    pros::vision_signature_s_t YELLOW_SIG =
    pros::Vision::signature_from_utility(EXAMPLE_SIG, 1235, 1575, 1404, -4241, -3935, -4088, 3.000, 0);
    vision_sensor.set_signature(EXAMPLE_SIG, &YELLOW_SIG);
    pros::lcd::set_text(2, "Vision intit done!");
}

/**
 * tracks disks and locates there center cordinates
 * also checks if the disk is close enough to the intake
 */
void vision () {
    pros::lcd::set_text(2, "Vision!");
    pros::lcd::set_text(5, "getting object count");
    objects = vision_sensor.get_object_count();
    if (objects != 0) {
        // Gets the largest object
        pros::lcd::print(5, "object count = %d", objects);      
        pros::vision_object_s_t rtn = vision_sensor.get_by_size(0);
        disk_x_mid = rtn.x_middle_coord;
        disk_y_mid = rtn.y_middle_coord;
        pros::lcd::print(6, "disk cord x=%d y=%d", disk_x_mid, disk_y_mid);
        std::cout << "sig: " << disk_x_mid;
        vision_sensor.set_led(COLOR_YELLOW);

        if (disk_x_mid > 158.0) {
            disk_dir = disk_x_mid - 158;
        }
        else if (disk_x_mid < 158.0) {
            disk_dir = 158 - disk_x_mid;
            disk_dir = disk_dir * -1;
        }
        else {
            disk_dir = 0;
        }
        pros::lcd::print(4, "disk direction = %d", disk_dir);
        }
    else {
        pros::lcd::set_text(5, "no object found");
        vision_sensor.set_led(COLOR_RED);
    }
    // checks to see if the disk is infront of the intake
    if (disk_x_mid > 148 && disk_x_mid < 168) {
        if (disk_y_mid > 100) {
            disk_ready = true;
        }
    }
    
    pros::lcd::set_text(2, "Vision done!");
}

/**
 * a simple pathfindng system to home in on disks infront of it
 * also collects them into the intake
 */
void diskfinder () {
    pros::lcd::set_text(2, "Disk finder!"); 
    disk_ready = false;
    forward_move = -0.2;
    while(disk_ready == false) {
        vision();
        pros::delay(10);
        // gets disk_dir to a range of -0.1 to 0.1
        yaw_move = disk_dir/158;
        yaw_move = yaw_move/2;
        drive->getModel()->arcade(forward_move, yaw_move); 
        pros::delay(10);
    }
    forward_move = -0.3;
    yaw_move = 0;
    drive->getModel()->arcade(forward_move, yaw_move);
    intake_group.moveVoltage(12000);
    pros::delay(2000);
    drive->getModel()->arcade(0, 0);
    pros::lcd::set_text(2, "Disk finder done!"); 
}

void leftsideauton () {
    pros::lcd::set_text(7, "Left auton started");
    diskfinder();
}

void rightsideauton () {
    Motor expansion(EXPANSION_PORT);
    pros::lcd::set_text(7, "Skills auton selected");
    drive->setMaxVelocity(50);
    // launcher preloads into the goal
    drive->moveDistance(6_in);
    drive->turnAngle(15_deg);
    drive->waitUntilSettled();
    disklaunch();
    drive->turnAngle(-15_deg);
    drive->moveDistance(-6_in);
    // navigate to roller
    drive->moveDistance(-60_in);
    drive->waitUntilSettled();
    drive->turnAngle(-90_deg);
    drive->waitUntilSettled();
    drive->moveDistance(12_in);
    drive->waitUntilSettled();
    roller(.2);
}

/**
 * I hate having this all hard coded but the vision sensor has not been tested enough to trust it in match
 */
void skillsauton () {
    pros::lcd::set_text(7, "Skills auton selected");
    drive->setMaxVelocity(50);
    // Bold move
    intake_group.moveVoltage(1000);
    pros::delay(1000);
    intake_group.moveVoltage(0);
    drive->moveDistance(6_in);
    drive->turnAngle(15_deg);
    drive->waitUntilSettled();
    disklaunch();
    drive->turnAngle(-15_deg);
    drive->moveDistance(-6_in);
    intake_group.moveVoltage(12000);
    pros::delay(1000);
    intake_group.moveVoltage(0);
    // launcher preloads into the goal
    drive->moveDistance(6_in);
    drive->turnAngle(15_deg);
    drive->waitUntilSettled();
    disklaunch();
    drive->turnAngle(-15_deg);
    drive->moveDistance(-6_in);
    // navigate to roller
    drive->moveDistance(-60_in);
    drive->waitUntilSettled();
    drive->turnAngle(-90_deg);
    drive->waitUntilSettled();
    drive->moveDistance(12_in);
    drive->waitUntilSettled();
    roller(.2);
    // try and grab the disc inbetween
    // other roller
    drive->moveDistance(-28_in);
    drive->turnAngle(-90_deg);
    drive->waitUntilSettled();
    drive->moveDistance(16_in);
    drive->waitUntilSettled();
    roller(.2);
    // collect disks
    // shoot discs into opposite goal while navigating towards oposite rollers 
    // collect and shoot discs till timer is out
    // need to make a loop that disk finds, naviagtes to the goal, and launchs
    // last 10 seconds deploy expainsion
    drive->moveDistance(-12_in);
    drive->turnAngle(45_deg);
    expansion.moveVoltage(-motor_voltage);
    drive->moveDistance(-12_ft);

             
    
}

/**
 * allows you to choose which auton is used
 */
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

/**
 * selects a auton based on what you choose in lcd select
 */
void autonstart () {
    pros::lcd::set_text(2, "Autonstart!");
    if (auton_mode == 0) {
        pros::lcd::set_text(7, "Default auton started");
        leftsideauton();
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

void graphMotor () {
    pros::lcd::set_text(3, "Graph Motor!");
    QTime time = timer.millis(); 
    std::cout << time.getValue() << " , " 
  << launcher.getActualVelocity() << " , " <<  launcher.getTargetVelocity() << " , " 
  << launcher.getTorque() << " , " << launcher.getTemperature()
  << std::endl;
}
