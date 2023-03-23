#include "main.h"
void lcdselect () {
    int auton_mode = -1;
    pros::lcd::set_text(1, "LCDSelect!");
	while (auton_mode == -1) {
		unsigned int button = pros::lcd::read_buttons();
        if (button == 1) {
            auton_mode = 1;
        }
        if (button == 2) {
            auton_mode = 2;
        }
        if (button == 4) {
            auton_mode = 3;
        }
        else {
            auton_mode = 0;
        }
        pros::delay(20);
	}
    pros::lcd::print(7, "auton mode selected = %d", auton_mode); 
}
	