#include "main.h"
void lcdselect () {
    pros::lcd::set_text(1, "LCDSelect!");
    int auton_mode;
    auton_mode = -1;
	while (auton_mode == -1) {
		unsigned int button = pros::lcd::read_buttons();
        if (button == 1) {
            auton_mode = 0;
        }
        if (button == 2) {
            auton_mode = 1;
        }
        if (button == 4) {
            auton_mode = 2;
        }
        pros::delay(20);
	}
    pros::lcd::print(7, "auton mode selected = %d", auton_mode); 
}
	