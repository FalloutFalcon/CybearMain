#include "main.h"

namespace cybear {

void leftsideauton () {
    pros::lcd::set_text(7, "Left auton started");
    disklaunch(5);
    
}

void rightsideauton () {
    pros::lcd::set_text(7, "Right auton started");
}

void skillsauton () {
    pros::lcd::set_text(7, "Skills auton selected");
}
}