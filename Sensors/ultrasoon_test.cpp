/*========================================================================================================================================================================
 * Gemaakt door: Cris van der Nolle & Ramon van Bemmel. 
 * Klas: V1B. 
 * Datum: 3-4-2018
 * Wat doet het programma: Het programma leest een ulrasoon sensor uit en meet de afstand. 
 * Deze code mag niet gekopieerd en/of gebruikt worden zonder toestemming van de makers. 
 * Versie:   0.0.1
 * ========================================================================================================================================================================*/

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "BrickPi3.cpp"

using namespace std;

BrickPi3 BP;

//void exit_signal_handler(int signo);
int read_distance(); 

int main()
{
	BP.detect();
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC); //Dit moet in de setup komen 
	
	while(true)
	{
		signal(SIGINT, exit_signal_handler);
		cout <<read_distance() << endl; 			//Dit is een tijdelijke loop voor het continu meten van de afstand. 
		sleep(1);
	}
}


int read_distance() 		//Dit is de functie voor het lezen van de afstand. 
{
	sensor_ultrasonic_t Ultrasonic2;
	while(true)
	{
		if(BP.get_sensor(PORT_2, Ultrasonic2) == 0)
		{
			return Ultrasonic2.cm;
		}
    }
  }
/*	
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
} */
