#include "motor_control.hpp"
#include "BrickPi3.h"
#include <stdio.h>
#include <unistd.h>

BrickPi3 MyBP; // new instance of BrickPi3

void go_left(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 40);
	MyBP.set_motor_power(left_port, 20);
	printf("Left turn\n");
}

void go_right(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 20);
	MyBP.set_motor_power(left_port, 40);
	printf("Right turn\n");
}
