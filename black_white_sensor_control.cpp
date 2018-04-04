
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
#include <iomanip>              // for setw and setprecision
#include <motor_control.hpp>
using namespace std;

BrickPi3 BP;
void exit_signal_handler(int signo);

int main()
{
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

  int error;
  BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON); // Set sensor type with corresponding port
  sensor_light_t      Light; // Struct

  while (true)
  {
          if (BP.get_sensor(PORT_3, Light) == 0)
          {
                if (Light.reflected > 2000) // Black value is approx 2000 - 2500. White value is approx 1700 - 2000
                {
                        cout << "Black detected @: " << setw(4) << Light.reflected << endl
                        hard_right(PORT_B, PORT_C);
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
