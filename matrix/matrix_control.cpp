#include "matrix_control.hpp"
#include "BrickPi3.h"
#include <stdio.h>
#include <unistd.h>

BrickPi3 MyBP; // new instance of BrickPi3

void go_left(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 27);
	MyBP.set_motor_power(left_port, 5);
	printf("Left turn\n");
}

void go_right(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 5);
	MyBP.set_motor_power(left_port,27);
	printf("Right turn\n");
}

void go_straight(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 30);
	MyBP.set_motor_power(left_port, 30);
}


