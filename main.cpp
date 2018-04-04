 /*  Example compile command:
 *    g++ -o program senxor_nxt.cpp BrickPi3.cpp
 *  Example run command:
 *    ./program
 *
 */

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "motor_control.hpp"
using namespace std;

void exit_signal_handler(int signo);

BrickPi3 BP;

int main()
{
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

  int error;
  BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON); // Set sensor type with corresponding port
  BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
  sensor_light_t      right_light; // Struct
  sensor_light_t      left_light;
  
  while (true)
  {
	  if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)
	  {
		if(right_light.reflected < 1950 && left_light.reflected > 470){
			cout << "right_light: " << setw(4) << right_light.reflected << '\n';
			cout << "left_light: " << setw(4) << left_light.reflected << '\n';
			go_straight(PORT_B, PORT_C);
		}
		else if (left_light.reflected < 450){
			while(left_light.reflected < 450){
				cout << "right_light: " << setw(4) << right_light.reflected << '\n';
				cout << "left_light: " << setw(4) << left_light.reflected << '\n';
				go_left(PORT_B, PORT_C);
				if (BP.get_sensor(PORT_3)
			}
		}
		else if (right_light.reflected > 2050){
			while (right_light.reflected > 2050){
				cout << "right_light: " << setw(4) << right_light.reflected << '\n';
				cout << "left_light: " << setw(4) << left_light.reflected << '\n';
				go_right(PORT_B, PORT_C);
			}
		}
	  }
  }
  
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
