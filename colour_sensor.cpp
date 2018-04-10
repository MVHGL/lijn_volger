/*
 *  Example compile command:
 *    g++ -o program colour_sensor.cpp BrickPi3.cpp
 *  Example run command:
 *    ./program
 *
 */

#include "BrickPi3.h"		// for BrickPi3
#include <iostream>      	// for cout
#include <unistd.h>     	// for sleep
#include <signal.h>     	// for catching exit signals
#include <iomanip>		// for setw and setprecision

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	int error;
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
	sensor_color_t      rgb_sensor;

	int treshold = 360;
	
	while(true){
		error = 0;
		if(BP.get_sensor(PORT_4, rgb_sensor) == 0){
			cout << "Rood waarde:" << setw(4) << rgb_sensor.reflected_red << endl;
			if(rgb_sensor.reflected_red < treshold){
				cout << "De kleursensor staat op de lijn!" << endl;
			}else{
				cout << "De kleursensor staat NIET op de lijn!" << endl;
			}
		}

    		cout << "....." << endl;

	sleep(1);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
		}
}

