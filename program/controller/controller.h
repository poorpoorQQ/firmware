#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "pwm.h"
#include "radio_control.h"
#include "attitude_stabilizer.h"
#include "vertical_stabilizer.h"
#include "led.h"
#include "global.h"

#define CONTROLLER_DISABLE 0
#define CONTROLLER_ENABLE 1

#include "navigation.h"
void PID_init(attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,vertical_pid_t* ,vertical_pid_t* ,nav_pid_t*);
void PID_output(radio_controller_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,vertical_pid_t* );
void PID_rc_pass_command(attitude_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,vertical_pid_t* ,vertical_pid_t* ,nav_pid_t* ,radio_controller_t* );
void PID_control_parameter_update(attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw_rate,attitude_stablizer_pid_t* PID_heading,vertical_pid_t* PID_Z,vertical_pid_t* PID_Zd,nav_pid_t* PID_nav);

#endif