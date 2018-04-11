#include "matrix_control.hpp"
#include "../../lib/BrickPi3.h"
#include <stdio.h>
#include <unistd.h>

BrickPi3 MyBP; // new instance of BrickPi3

void go_left(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 27);
	MyBP.set_motor_power(left_port, 5);
	//printf("Left turn\n");
}

void go_right(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 5);
	MyBP.set_motor_power(left_port,27);
	//printf("Right turn\n");
}

void go_straight(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 30);
	MyBP.set_motor_power(left_port, 30);
}

void update_location(int &my_x, int &my_y, int &direction){
	if(direction == 90)
	{
		 my_x++;
	}else if(direction == 180)
	{
		my_y++;
	}else if(direction == 270)
	{
		my_x--;
	}else if(direction == 0)
	{
		my_y--;
	}
}

void set_encoders(int &motorEncoderLeft, int &motorEncoderRight)
{
        motorEncoderLeft = MyBP.get_motor_encoder(PORT_C);
        motorEncoderRight = MyBP.get_motor_encoder(PORT_B);
}

void stop_engines()
{
	MyBP.set_motor_power(PORT_C, 0);
	MyBP.set_motor_power(PORT_B, 0);
	sleep(1);
}

void go_left()
{
        MyBP.set_motor_position_relative(PORT_B, 360);
        MyBP.set_motor_position_relative(PORT_C, -50);
	sleep(1);
}

void go_right()
{
        MyBP.set_motor_position_relative(PORT_B, -50);
        MyBP.set_motor_position_relative(PORT_C, 360);
	sleep(1);
}

