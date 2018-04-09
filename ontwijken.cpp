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

//DEBUG  
int debug = 0;//zet op 1 voor debug. 
int debug_ultrasoon=0; 

//variabelen.

unsigned int switch_afstand=10;
unsigned int afstand=100;

//Gebruikte functies. 
unsigned int read_distance();
void dodge(); 
void exit_signal_handler(int signo);
void setup();


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
			if (afstand <= switch_afstand && afstand) // als er een object is gaat die hem hier ontwijken. 
			{	
				dodge();
			}
			
			if(debug_ultrasoon)
				cout << read_distance() << endl;				
			cout<<"Volgt de lijn" <<endl;		
			prop_power(R_VAL, false); // steer proportionally on the right
			prop_power(L_VAL, true);  // steer proportionally on the left
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
		if(debug_ultrasoon)
			cout<<"ULTRASOON functie \t" <<Ultrasonic2.cm <<endl; 
		while (a ==0)
		{
			a=Ultrasonic2.cm;
		}
		while (b==0)
		{
			b=Ultrasonic2.cm;
		}
		return (a+b)/2;
	}
    
  }
void setup()
{
	signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
	BP.detect(); // Check for BrickPi firmware
}
void dodge()
{
	float delay=1;
	//cout << read_distance() << endl;
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
	sleep(delay);
	// maakt bocht naar links 90 graden. 
	BP.set_motor_power(PORT_B, 0);
	BP.set_motor_position_relative(PORT_C, 360);
	sleep(delay);
	//bepaalde afstand naar voren
	BP.set_motor_position_relative(PORT_B, 500);
	BP.set_motor_position_relative(PORT_C, 500);
	sleep(delay);
	//bocht naar links 90 graden
	BP.set_motor_power(PORT_B, 0);
	BP.set_motor_position_relative(PORT_C, 360);
	sleep(delay);
	//rechtdoor
	BP.set_motor_position_relative(PORT_B, 70);
	BP.set_motor_position_relative(PORT_C, 70);
	sleep(delay);
	// bochtje naar rechts 
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_position_relative(PORT_B, 100);		
	sleep(delay);
	//while (sensor1 != zwart || sensor2 != zwart) 
		//rechtdoor
}
// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo)
{
	if(signo == SIGINT)
	{
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
