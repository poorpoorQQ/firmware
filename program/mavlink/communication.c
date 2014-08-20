#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "_math.h"

#include "usart.h"
#include "radio_control.h"

#include "FreeRTOS.h"
#include "task.h"

#include "mavlink.h"

#include "global.h"

#include "communication.h"
#include "command_parser.h"
#include "FreeRTOS.h"
#include "system_time.h"
#include "io.h"

mavlink_message_t received_msg;
mavlink_status_t received_status;

void send_package(mavlink_message_t *msg)
{
	uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		usart3_send(buf[i]);
}

void clear_message_id(mavlink_message_t *message)
{
	message->msgid = 0;
}

static void send_heartbeat_info(void)
{
	mavlink_message_t msg;
	uint8_t safty_button, mode_button; 
	uint8_t mav_mode, mav_state;
	
	read_global_data_value(SAFTY_BUTTON, DATA_POINTER_CAST(&safty_button));
	read_global_data_value(MODE_BUTTON, DATA_POINTER_CAST(&mode_button));

	/* Check the safty button status */
	if(safty_button == ENGINE_ON) {
		/* Check the flight mode */
		if(mode_button == MODE_1 || mode_button == MODE_2)
			mav_mode = MAV_MODE_STABILIZE_ARMED;
		else
			mav_mode = MAV_MODE_GUIDED_ARMED;

		mav_state = MAV_STATE_ACTIVE;
	} else {
		/* Check the flight mode */
		if(mode_button == MODE_1 || mode_button == MODE_2)
			mav_mode = MAV_MODE_STABILIZE_DISARMED;
		else
			mav_mode = MAV_MODE_GUIDED_DISARMED;

		mav_state = MAV_STATE_STANDBY;
	}

	mavlink_msg_heartbeat_pack(1, 200, &msg,
		MAV_TYPE_QUADROTOR, 
		MAV_AUTOPILOT_GENERIC, 
		mav_mode, 
		0,
		mav_state
	);

	send_package(&msg);
}

static void send_gps_info(void)
{
	int32_t latitude, longitude, altitude;
	int16_t gps_vx, gps_vy, gps_vz;

	/* Prepare the GPS data */
	read_global_data_value(GPS_LAT, DATA_POINTER_CAST(&latitude));
	read_global_data_value(GPS_LON, DATA_POINTER_CAST(&longitude));
	read_global_data_value(GPS_ALT, DATA_POINTER_CAST(&altitude));
	read_global_data_value(GPS_VX, DATA_POINTER_CAST(&gps_vx));
	read_global_data_value(GPS_VY, DATA_POINTER_CAST(&gps_vy));
	read_global_data_value(GPS_VZ, DATA_POINTER_CAST(&gps_vz));

	mavlink_message_t msg;

	mavlink_msg_global_position_int_pack(1, 220, &msg, 
		get_system_time_ms(),   		       //time 
		latitude ,  //Latitude
		longitude ,  //Longitude
		altitude, //Altitude
		0,
		gps_vx * 100,   //Speed-Vx
		gps_vy * 100,   //Speed-Vy
		gps_vz * 100,   //Speed-Vz
		45
	);

	send_package(&msg);
}

static void send_attitude_info(void)
{
	mavlink_message_t msg;
	float attitude_roll, attitude_pitch, attitude_yaw;

	/* Prepare the attitude data */
	read_global_data_value(TRUE_ROLL, DATA_POINTER_CAST(&attitude_roll));
	read_global_data_value(TRUE_PITCH, DATA_POINTER_CAST(&attitude_pitch));
	read_global_data_value(TRUE_YAW, DATA_POINTER_CAST(&attitude_yaw));

	mavlink_msg_attitude_pack(1, 200, &msg,
		get_system_time_ms(),
		toRad(attitude_roll), 
		toRad(attitude_pitch), 
		toRad(attitude_yaw), 
		0.0, 0.0, 0.0
	);

	send_package(&msg);
}

#if 0
static void send_system_info(void)
{
	mavlink_message_t msg;

	mavlink_msg_sys_status_pack(1, 0, &msg,
		0,
		0,
		0,
		0,
		12.5 * 1000, //Battery voltage
		-1,
		100,         //Battery remaining
		0,
		0,
		0,
		0,
		0,
		0
	);

	send_package(&msg);
}
#endif

void ground_station_task(void)
{
	uint32_t delay_t =(uint32_t) 50.0/(1000.0 / configTICK_RATE_HZ);
	uint32_t cnt = 0;
	
	while(1) {
		if(cnt == 15) {
			send_heartbeat_info();
			send_gps_info();
			//send_system_info();

			cnt = 0;
		}
		send_attitude_info();
		
		vTaskDelay(delay_t);

		mavlink_parse_received_cmd(&received_msg);
		cnt++;
		
	}
}

void mavlink_receiver_task(void)
{
	uint8_t buffer;

	while(1) {
		buffer = usart3_read();

		mavlink_parse_char(MAVLINK_COMM_0, buffer, &received_msg, &received_status); 
	}
}
