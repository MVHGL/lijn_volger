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

int read_c_sensor()
{

	int error;
	sensor_color_t	rgb_sensor;

	error = 0;
	if(BP.get_sensor(PORT_4, rgb_sensor) == 0){
		return rgb_sensor.reflected_red;
	}
}

int read_bw_sensor()
{
	int error;
	sensor_light_t	light;
	
	error = 0;
	if(BP.get_sensor(PORT_3, light) == 0){
		return light.reflected;
	}
}

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

	while(true){
		int rgb_value = read_c_sensor();
		int bw_value = read_bw_sensor();
		cout << "RGB sensor waarde:" << setw(4) << (rgb_value / 6) << endl;
		cout << "Black/white sensor: " << setw(4) << (bw_value / 80) << endl;
		cout << "----" << endl;
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
