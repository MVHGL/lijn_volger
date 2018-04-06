#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>              // for setw and setprecision
#include "motor_control.hpp"

#define R_VAL right_light.reflected
#define L_VAL left_light.reflected

using namespace std;

BrickPi3 BP;

int switch_val_light_low=1960;
int switch_val_light_high=2000;
int switch_val_rgb_low=380;
int switch_val_rgb_high=400;
int switch_distance=10;
int debug = 0;
int richting=0;


//Gebruikte functies.
int read_distance();
void exit_signal_handler(int);

void setup()
{
        BP.detect();// Check for BrickPi firmware
        BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
        signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
        //int error;
        int encoder_value = BP.reset_motor_encoder(PORT_C);
        /* initialize sensor modes */
        BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
        BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
}
int main()
{
        setup();
        sensor_light_t      right_light; // struct for sensors
        sensor_light_t      left_light;
        while(true)
        {
                if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0){ // read value from sensors
                        cout << "Rechts:\n";
                        prop_power(R_VAL, false); // steer proportionally on the right
                        cout << "Links:\n";
                        prop_power(L_VAL, true);  // steer proportionally on the left/
                        }
                        if (R_VAL > switch_val_light_high && L_VAL < switch_val_rgb_low)  //kruispunt
                        {
                                cout << "kruispunt" << endl;
                                BP.set_motor_power(PORT_B, 0);
                                BP.set_motor_power(PORT_C,0);
                                sleep(2);

                                /*if (read_distance() > switch_distance)
                                {
                                        go_straight(PORT_B, PORT_C);
                                }*/
                                if (read_distance() < switch_distance &&richting==0)
                                {
                                        BP.set_motor_position(PORT_C, 1100);
                                        richting=1;

                                }
                                else if (read_distance() < switch_distance &&richting==1)
                                {
                                        BP.set_motor_position(PORT_C, 1100);
                                        richting=1;
                                }

                        }
                }
}

int read_distance()             //Dit is de functie voor het lezen van de afstand.
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

void exit_signal_handler(int signo)
{
        if(signo == SIGINT){
                BP.reset_all();    // Reset everything so there are no run-away motors
                exit(-2);
        }
}
