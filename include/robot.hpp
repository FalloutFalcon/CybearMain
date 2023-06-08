#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

/**
 * Motors
 */
#define FRONT_LEFT_WHEEL_PORT 20
#define BACK_LEFT_WHEEL_PORT 19
#define FRONT_RIGHT_WHEEL_PORT 11
#define BACK_RIGHT_WHEEL_PORT 6

#define LAUNCHER_PORT 1
#define TOP_INTAKE_PORT 2
#define BOTTOM_INTAKE_PORT 4
#define EXPANSION_PORT 5
#define ULTRASONIC_PING_PORT 'A'
#define ULTRASONIC_ECHO_PORT 'B'
#define VISION_PORT 18
#define DISK_SIG 1
#define GOAL_SIG 2
#define EXAMPLE_SIG 3


void okapiopcontrol();
void driveinitialize();
void disklaunch();
void roller(int i);
void visionintit();
void vision();
void rightsideauton();
void skillsauton();
void leftsideatuon();
void lcdselect();
void autonstart();
void diskfinder();
void graphMotor();
#endif //_ROBOT_HPP_