#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "matrix_control.hpp"	// for controlling the motors

#define R_VAL right_light.reflected	// light sensor on right side
#define L_VAL left_light.reflected	// light sensor on left side

using namespace std;

BrickPi3 BP;	// instance of BrickPi3 class

int switch_val_light=2000; //2000
int switch_val_rgb=450; //450
int debug = 1;
int my_X = 1;
int my_Y = 1;
int32_t motorEncoderRight;
int32_t motorEncoderLeft;
int afstand;
unsigned int read_distance();

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo)
{
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}

int main()
{
	signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
	BP.detect(); // Check for BrickPi firmware
	BP.set_motor_limits(PORT_C, 30, 0);
	BP.set_motor_limits(PORT_B, 30, 0);
	int error;
	int encoder_value = BP.reset_motor_encoder(PORT_C);
	
	/* initialize sensor modes */
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON); 
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_light_t      right_light; // struct for sensors
	sensor_light_t      left_light;

	BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	// Main loop
	while (true)
  	{
		if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)
		{
			cout << "R_VAL: " << R_VAL << '\n';
			cout << "L_VAL: " << L_VAL << '\n';
			if (R_VAL > 2100 && L_VAL < 500) //Als beide waarden zwart zijn(kruispunt of T-splitsing)
			{
				BP.set_motor_power(PORT_B, 0);
				BP.set_motor_power(PORT_C, 0);
				motorEncoderLeft = BP.get_motor_encoder(PORT_C);
				motorEncoderRight = BP.get_motor_encoder(PORT_B);
				sleep(2);
				afstand = read_distance();
				cout << "afstand: " << afstand << '\n';
				cout << "read_distance: " << read_distance() << '\n';
				if(afstand < 10)
				{
					cout << "object op " << afstand << '\n';
					cout << "motorEncoderLeft " << motorEncoderLeft << '\n';
					cout << "motorEncoderRight " << motorEncoderRight << '\n';
					sleep(1);
					cout << "we gaan achteruit mattieeee!" << '\n';
					BP.set_motor_position_relative(PORT_C, -motorEncoderLeft);
					BP.set_motor_position_relative(PORT_B, -motorEncoderRight);
					sleep(2);

				}
				else
				{
					cout << "geen object" << '\n' ;
					sleep(1);
					BP.set_motor_position_relative(PORT_C, 50);
					BP.set_motor_position_relative(PORT_B, 50);
				}
			}
			else if(R_VAL < switch_val_light && L_VAL > switch_val_rgb)
			{
				if(debug)
				{
					cout << "right_light: " << setw(4) << R_VAL << '\n';
					cout << "left_light: " << setw(4) << L_VAL << '\n';
				}
				go_straight(PORT_B, PORT_C);
			}
			else if (L_VAL < switch_val_rgb)
			{
				if(debug)
				{
					cout << "right_light: " << setw(4) << R_VAL << '\n';
					cout << "left_light: " << setw(4) << L_VAL << '\n';
				}
				go_left(PORT_B, PORT_C);
			}
			else if (R_VAL > switch_val_light)
			{
				if(debug){
					cout << "right_light: " << setw(4) << R_VAL << '\n';
					cout << "left_light: " << setw(4) << L_VAL << '\n';
				}
				go_right(PORT_B, PORT_C);
			}
			else if (L_VAL < switch_val_rgb && R_VAL > switch_val_light)
			{
				go_straight(PORT_B,PORT_C);
			} 
		}  
	}	
}

unsigned int read_distance()            //Dit is de functie voor het lezen van de afstand.
{
        int a = 0, b = 0, t = 0;
        sensor_ultrasonic_t Ultrasonic2;

        if (BP.get_sensor(PORT_2, Ultrasonic2) == 0)
        {
                a = Ultrasonic2.cm;
                b = Ultrasonic2.cm;
                while (a ==0)
                {
                        a = Ultrasonic2.cm;
                }
                while (b==0)
                {
                        b = Ultrasonic2.cm;
                }
                return (a+b)/2;
        }
}
	  
