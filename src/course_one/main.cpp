/*======================================================================================================================================
Project: 		Lijnvolger. 
Projectgroep:		1.
			Marc Dirven, Cris van de Nolle, Nathan Hoekstra, Nick Snel en Ramon van Bemmel.
Klas:			V1B.
Gemaakt door:		Marc Dirven, Cris van de Nolle, Nathan Hoekstra, Nick Snel en Ramon van Bemmel.
Functie van de code:	Deze code laat de BrickPi een lijn volgen en stuurt proportioneel bij. Als er een object voor de ultrasoon sensor
			staat rijdt de bot hier omheen. 
=======================================================================================================================================*/

#include "../../lib/BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "motor_control.hpp"	// for controlling the motors

#define R_VAL right_light.reflected	// light sensor on right side
#define L_VAL left_light.reflected	// light sensor on left side

using namespace std;

BrickPi3 BP;	// instance of BrickPi3 class
void setup();
void dodge();
unsigned int read_distance();
void exit_signal_handler(int signo);

//DEBUG  
int debug = 0;//zet op 1 voor algemene debug. 
int debug_ultrasoon = 0;	//Zet op 1 voor het debuggen van de ultrasoon. 

//variabelen.
unsigned int switch_afstand = 10; 	//Deze variabele bepaald welke afstand een object minimaal van de bot af moet staan. 
unsigned int afstand = 100;		//Hier wordt steeds de ultrasoon waarde in opgeslagen.

int main()   // De main functie. 
{
	setup();  		   
	int error;

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
			if(debug_ultrasoon){			
				cout << afstand << endl;}
			
			afstand = read_distance();

			if (afstand <= switch_afstand && afstand) // als er een object is gaat die hem hier ontwijken. 
			{
				dodge();}
			if (debug_ultrasoon){
				cout << read_distance() << endl;
			}

			prop_power(R_VAL, false); // steer proportionally on the right
			prop_power(L_VAL, true);  // steer proportionally on the left
		}
	}
}

unsigned int read_distance() 		//Dit is de functie voor het lezen van de afstand via de ultrasoon. 
{
	int a = 0, b = 0, t = 0;
	sensor_ultrasonic_t Ultrasonic2;

	if (BP.get_sensor(PORT_2, Ultrasonic2) == 0)
	{
		a = Ultrasonic2.cm;
		b = Ultrasonic2.cm;

		if (debug_ultrasoon)
			cout << "ULTRASOON functie \t" << Ultrasonic2.cm << endl;
		while (a == 0)
			a = Ultrasonic2.cm;
		while (b == 0)
			b = Ultrasonic2.cm;

		return (a + b) / 2;
	}

}
void setup()			//Deze functie stelt alles jusit in. 
{
	signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
	BP.detect(); // Check for BrickPi firmware
}
void dodge() 			//Deze functie is de ontwijkprocedure als er een object is gevonden. 
{
	float delay = 1;	//hier kan de delaytijd tussen elke stap worden ingesteld. 
	sensor_light_t      right_light; // struct for sensors
	sensor_light_t      left_light;

	BP.set_motor_limits(PORT_B, 40, 2000);
	BP.set_motor_limits(PORT_C, 40, 2000);
	// maakt bocht van 90 graden naar rechts
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_position_relative(PORT_B, 360);
	sleep(delay);
	// klein stukje naar voren (1 slag = 1100)
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
	BP.set_motor_position_relative(PORT_C, 200);
	sleep(delay);
	//rechtdoor
	/*BP.set_motor_position_relative(PORT_B, 70);
	BP.set_motor_position_relative(PORT_C, 70);
	sleep(delay);*/
	/*// bochtje naar rechts 
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_position_relative(PORT_B, 100);
	sleep(delay);*/

	while ((R_VAL < 2000) || (L_VAL > 450))
	{
		if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)
		{
			run_motor(16, 26);
			cout << "right: " << right_light.reflected << " left: " << left_light.reflected << endl;
		}
	}
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_position_relative(PORT_B, 100);
	
}
// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
