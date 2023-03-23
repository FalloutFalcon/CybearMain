#include "main.h"

namespace cybear {
void autonsetup () {
    if (auton_mode == 0)
    {
        pros::lcd::set_text(7, "No auton selected");
    }
    else if (auton_mode == 1)
    {
        pros::lcd::set_text(7, "Left auton selected");
    }
    else if (auton_mode == 2)
    {
        pros::lcd::set_text(7, "Skills auton selected");
    }
    else if (auton_mode == 3)
    {
        pros::lcd::set_text(7, "Right auton selected");
    }
    else {
        pros::lcd::set_text(7, "Error in auton selection");
    }
    
        
}


void leftsideauton () {
    pros::lcd::set_text(7, "Left auton started");
}

void rightsideauton () {
    pros::lcd::set_text(7, "Skills auton started");
}

void skillsauton () {
    pros::lcd::set_text(7, "Right auton selected");
}

void autonstart () {
    pros::lcd::set_text(7, "Auton Started");
    
}
}