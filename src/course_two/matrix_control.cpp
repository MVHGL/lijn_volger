#include "matrix_control.hpp"
#include "../../lib/BrickPi3.h" // for BrickPi3
#include <stdio.h>
#include <unistd.h>

BrickPi3 MyBP; // new instance of BrickPi3

void go_left(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 30); //27
	MyBP.set_motor_power(left_port, 7); //5
}

void go_right(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 7);
	MyBP.set_motor_power(left_port,30);
}

void go_straight(uint8_t left_port, uint8_t right_port)
{
	MyBP.set_motor_power(right_port, 45);
	MyBP.set_motor_power(left_port, 45);
	sleep(0.5);
}

void update_location(int &my_x, int &my_y, int &direction){
	if(direction == 90)
	{
		 my_x++;
	}else if(direction == 180)
	{
		my_y--;
	}else if(direction == 270)
	{
		my_x--;
	}else if(direction == 0)
	{
		my_y++;
	}
}

void set_encoders(int &motorEncoderLeft, int &motorEncoderRight)
{
        motorEncoderLeft = MyBP.get_motor_encoder(PORT_C);
        motorEncoderRight = MyBP.get_motor_encoder(PORT_B);
}

void reset_encoders()
{
	 MyBP.offset_motor_encoder(PORT_B, MyBP.get_motor_encoder(PORT_B));
	 MyBP.offset_motor_encoder(PORT_C, MyBP.get_motor_encoder(PORT_C));
}

void stop_engines()
{
	MyBP.set_motor_power(PORT_C, 0);
	MyBP.set_motor_power(PORT_B, 0);
	sleep(1);
}

void go_hard_left(int &direction)
{
	MyBP.set_motor_position_relative(PORT_C, 360);
	MyBP.set_motor_position_relative(PORT_B, -100);
	sleep(1.5);
	
	if (direction == 0)
		direction = 270;
	else
		direction -= 90;
}

void go_hard_right(int &direction)
{
	MyBP.set_motor_position_relative(PORT_C, -100);
	MyBP.set_motor_position_relative(PORT_B, 360);
	sleep(1.5);
	
	if (direction == 270)
		direction = 0;
	else
		direction += 90;
}

void go_back(bool &obstacle, const int &motor_encoder_left, const int &motor_encoder_right)
{
	MyBP.set_motor_position_relative(PORT_C, motor_encoder_left);
	MyBP.set_motor_position_relative(PORT_B, motor_encoder_right);
	obstacle = true;
	sleep(4);
}
