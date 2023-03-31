#ifndef _motorcontrol_HPP_
#define _motorcontrol_HPP_

/*
// Launcher related objects
extern okapi::Motor launcher(LAUNCHER_PORT);
extern int launcher_voltage;

// Intake related objects
extern okapi::Motor top_intake(TOP_INTAKE_PORT);
extern okapi::Motor bottom_intake(BOTTOM_INTAKE_PORT);
extern okapi::MotorGroup intake_group({top_intake, bottom_intake});


extern okapi::Motor expansion(EXPANSION_PORT);
*/

extern double forward_move;
extern double yaw_move;

namespace cybear {
void okapiinitialize ();
void okapiopcontrol ();
void disklaunch (int i);
void leftsideauton ();
void skillsauton ();
void rightsideauton ();
}
#endif //_DRIVE_HPP_