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

void okapiopcontrol();
void okapiinitialize();
void disklaunch(int i);
void roller(int i);
void rightsideauton();
void skillsauton();
void leftsideatuon();
void lcdselect();
void autonstart();
#endif //_ROBOT_HPP_