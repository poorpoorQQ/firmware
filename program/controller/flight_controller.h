#ifndef FILE_FLIGHT_CONTROLLER_H
#define FILE_FLIGHT_CONTROLLER_H

#include <stdio.h>
#include "attitude_estimator.h"
#include "vertical_estimator.h"
#include "estimator.h"
#include "controller.h"
#include "pwm.h"
#include "radio_control.h"
#include "test_common.h"
#include "hmc5983.h"
#include "lea6h_ubx.h"// Should link to higher level like GPS.h but so far we have one GPS receiver only.
#include "usart.h"
#include "system_time.h"

#include "FreeRTOS.h"
#include "task.h"

#include "global.h"

void flight_control_task(void);
UBXvelned_t get_UBXvelned_data(UBXvelned_t *UBXvelned_data);
UBXsol_t get_UBXsol_data(UBXsol_t *UBXsol_data);
UBXposLLH_t get_UBXposLLH_data(UBXposLLH_t *UBXposLLH_data);




#endif