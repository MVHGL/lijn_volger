#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "motor_control.hpp"	// for controlling the motors

#define R_VAL right_light.reflected	// light sensor on right side
#define L_VAL left_light.reflected	// light sensor on left side

using namespace std;

BrickPi3 BP;	// instance of BrickPi3 class

int debug = 1;
unsigned int switch_afstand=5;
int onoff=0;
unsigned int afstand=100;

//function read_distance init
unsigned int read_distance();

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo)
{
	if(signo == SIGINT)
	{
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}


void setup()
{
	signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
	BP.detect(); // Check for BrickPi firmware
}
int main()
{
	setup();
	int error;
	int encoder_value = BP.reset_motor_encoder(PORT_C);

	/* initialize sensor modes */
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON); 
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_light_t      right_light; // struct for sensors
	sensor_light_t      left_light;  

	// Main loop
	while (true)
	{

	

		if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)
		{
			afstand=read_distance(); 
		//	sleep(0.8);
			if (afstand <= switch_afstand && afstand) // als er een object is gaat die hem hier ontwijken. 
			{	
				cout << read_distance() << endl;
				cout <<"in object gevonden if" <<endl;
				BP.set_motor_limits(PORT_B, 40, 2000);
				BP.set_motor_limits(PORT_C, 40, 2000);
				// maakt bocht van 90 graden naar rechts
				BP.set_motor_power(PORT_C, 0);
				BP.set_motor_position_relative(PORT_B, 360);
				sleep(1);	
				//m klein stukje naar voren ( 1 slag = 1100)
				BP.set_motor_position_relative(PORT_B, 180);
				BP.set_motor_position_relative(PORT_C, 180);
				sleep(1);
				// maakt bocht naar links 90 graden. 
				BP.set_motor_power(PORT_B, 0);
				BP.set_motor_position_relative(PORT_C, 360);
				sleep(1);
				//bepaalde afstand naar voren
				BP.set_motor_position_relative(PORT_B, 50);
				BP.set_motor_position_relative(PORT_C, 50);
				sleep(1);
				//bocht naar links 90 graden
				BP.set_motor_power(PORT_B, 0);
				BP.set_motor_position_relative(PORT_C, 360);
				sleep(1);
				//rechtdoor
				BP.set_motor_position_relative(PORT_B, 360);
				BP.set_motor_position_relative(PORT_C, 360);
				sleep(1);
				// bochtje naar rechts 
				BP.set_motor_power(PORT_C, 0);
				BP.set_motor_position_relative(PORT_B, 100);		
				sleep(1);
			}
			//cout << "in init if" << endl;	// read value from sensors
			cout << read_distance() << endl;				
			cout<<"Volgt de lijn" <<endl;		
			//cout << "Rechts:\n";
			prop_power(R_VAL, false); // steer proportionally on the right
			//cout << "Links:\n";
			prop_power(L_VAL, true);  // steer proportionally on the left
	//		sleep(1);
			//onoff=1
				

		}
	}
}

unsigned int read_distance() 		//Dit is de functie voor het lezen van de afstand. 
{
	int a=0,b=0, t=0;
	sensor_ultrasonic_t Ultrasonic2;
	if(BP.get_sensor(PORT_2, Ultrasonic2) == 0)
	{
		a=Ultrasonic2.cm;

		b=Ultrasonic2.cm;
		
		cout<<"ULTRASOON functie \t" <<Ultrasonic2.cm <<endl; 
		while (a ==0)
		{
			a=Ultrasonic2.cm;

			/*t++;
			if (t==7)
			{
				break; 
			}*/
		}
		//t=0;
		while (b==0)
		{
			b=Ultrasonic2.cm;

			//t++;
			/*if (t==7)
			{
				break;
			}*/
		}
		return (a+b)/2;
	}
    
  }
