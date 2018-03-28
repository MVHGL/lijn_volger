#include "BrickPi3.h"
#include "motor_control.hpp"
#include <stdio.h>

void hard_left(uint8_t left_port, uint8_t right_port)
{
	set_motor_power(right_port, 50);	// turn clockwise
	set_motor_power(left_port, -50);	// turn counter clockwise

	for (int i=0; i < (HARD_TURN / 2); i++){
		set_motor_position_relative(right_port, 1);
		set_motor_position_relative(left_port, 1);
	}
	printf("Left turn completed\n");
}

void hard_right(uint8_t left_port, uint8_t right_port)
{
	set_motor_power(right_port, -50);	// turn counter clockwise
	set_motor_power(left_port, 50);		// turn clockwise

	for(int i=0; i < (HARD_TURN / 2); i++){
		set_motor_position_relative(right_port, 1);
		set_motor_position_relative(left_port, 1);
	}
	printf("Right turn completed\n");
}
