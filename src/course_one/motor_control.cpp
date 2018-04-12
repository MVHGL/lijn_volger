#include "motor_control.hpp"
#include "../../lib/BrickPi3.h"
#include <stdio.h>
#include <unistd.h>

BrickPi3 MyBP; // new instance of BrickPi3
int debug_motor=0;	//Zet op 1 om te debuggen

void run_motor(int left_power, int right_power)		//Zet de motorpower op ingegeven waarde. 
{
	MyBP.set_motor_power(PORT_C, right_power);
	MyBP.set_motor_power(PORT_B, left_power);
}


void prop_power(int sensor_value, bool left_sensor, int &left_offset, int &right_offset)	//Functie voor propotioneel bij sturen.
{
	int Kp = 2;	// Proportional constant
	//int right_offset = 24; // offset for right sensor
	//int left_offset = 71;   // offset for left sensor
	int Tp = 24;		 // Target power

	int motor_left = 0;
	int motor_right = 0;

	/* check which sensor */
	if (left_sensor){
		sensor_value = sensor_value / 6;
		int error = sensor_value - left_offset;
		printf("ERROR: %d\n", error);
		printf("SENSOR_V: %d\n", sensor_value);
		int turn = Kp * error;

		motor_left = Tp + turn;
		motor_right = Tp - turn;
	}
	else{
		sensor_value = sensor_value / 80;
		int error = sensor_value - right_offset;
		printf("ERROR: %d\n", error);
		printf("SENSOR_V: %d\n", sensor_value);
		int turn = Kp * error;

		motor_left = Tp + turn;
		motor_right = Tp - turn;

	}

	printf("Motor left: %d\n", motor_left);
	printf("Motor right: %d\n", motor_right);
	run_motor(motor_left, motor_right);
}
