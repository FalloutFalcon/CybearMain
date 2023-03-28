#include "main.h"
namespace cybear {

int auton_mode = -1;
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
        pros::lcd::set_text(6, "No auton started");
    } else if (auton_mode == 1) {
        pros::lcd::set_text(6, "Left auton started");
        cybear::leftsideauton();
    } else if (auton_mode == 2) {
        pros::lcd::set_text(6, "Skills auton started");
        cybear::skillsauton();
    } else if (auton_mode == 3)  {
        pros::lcd::set_text(6, "Right auton started");
        cybear::rightsideauton();
    } else {
        pros::lcd::set_text(6, "Error in auton start");
    }    
}
}
