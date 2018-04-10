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
int my_x = 1;
int my_y = 1;
int direction = 90;
int32_t motorEncoderRight;
int32_t motorEncoderLeft;
int distance_obstacle;
unsigned int read_distance();
bool obstacle = false;

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
			distance_obstacle = read_distance();
			set_encoders(motorEncoderLeft, motorEncoderRight);
			if (R_VAL > 2300 && L_VAL < 500 && obstacle == false) //Als beide waarden zwart zijn(kruispunt of T-splitsing)
			{
				cout << "Kruispuunt maar geen obstakel" << '\n';
				stop_engines();
				while (R_VAL > 2300 && L_VAL < 500 && distance_obstacle > 10)
				{
					if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)	
					{	
						go_straight(PORT_C, PORT_B);			
						cout << "RVAL: " << R_VAL << endl;
					}
				}
				if(distance_obstacle <= 10 && distance_obstacle) // als er een obstakel is, ga achterui
				{
					cout << "object op " << distance_obstacle << '\n';
					cout << "motorEncoderLeft " << motorEncoderLeft << '\n';
					cout << "motorEncoderRight " << motorEncoderRight << '\n';
					sleep(1);
					cout << "we gaan achteruit mattieeee!" << '\n';
					BP.set_motor_position_relative(PORT_C, -motorEncoderLeft);
					BP.set_motor_position_relative(PORT_B, -motorEncoderRight);
					sleep(4);
				}
				else
				{
					cout << "geen object" << '\n' ;
					BP.set_motor_position_relative(PORT_C, 360);
					cout << "PORT C 100" << endl;
					BP.set_motor_position_relative(PORT_B, 360);
					update_location(my_x, my_y, direction);
				}
			}
			else if (R_VAL > 2300 && L_VAL < 500 && obstacle)
			{
				cout << "Kruispunt en obstakel" << '\n';	
				if(direction == 90)
				{
					go_right();
					direction = 180;
					obstacle = false;
				}
				else if(direction == 0)
				{
					go_right();
					direction = 90;
					obstacle = false;
				}
				else if(direction == 270)
				{
					go_left();
					direction = 180;
					obstacle = false;
				}
				else if(direction == 180)
				{
					go_left();
					direction = 90;
					obstacle = false;
				}	
			}
			else if(distance_obstacle <= 10 && distance_obstacle) // als er een obstakel is, ga achterui
                        {
				cout << "object op " << distance_obstacle << '\n';
                                cout << "motorEncoderLeft " << motorEncoderLeft << '\n';
                                cout << "motorEncoderRight " << motorEncoderRight << '\n';
                                stop_engines();
				cout << "we gaan achteruit mattieeee!" << '\n';
                                BP.set_motor_position_relative(PORT_C, -motorEncoderLeft);
                                BP.set_motor_position_relative(PORT_B, -motorEncoderRight);
				obstacle = true;
                                sleep(4);
                        }
			else if(R_VAL < switch_val_light && L_VAL > switch_val_rgb)
			{
				go_straight(PORT_B, PORT_C);
			}
			else if (L_VAL < switch_val_rgb)
			{
				go_left(PORT_B, PORT_C);
			}
			else if (R_VAL > switch_val_light)
			{
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
                while (b==0)
                {
                        b = Ultrasonic2.cm;
                }
                return (a+b)/2;
        }
}
