#ifndef _MOTOR_CONTROL_HPP
#define _MOTOR_CONTROL_HPP

#include <stdint.h>	// for uint8_t
#include "../../lib/BrickPi3.h"


/* function definitions */
/* function to measure proportional turn*/
void prop_power(int sensor_value, bool left_sensor);
/* run the motors */
void run_motor(int left_power, int right_power);
#endif
