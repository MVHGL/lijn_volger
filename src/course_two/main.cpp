#include "../../lib/BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision
#include "matrix_control.hpp"	// for controlling the motors

#define R_VAL right_light.reflected	// light sensor on right side
#define L_VAL left_light.reflected	// light sensor on left side

using namespace std;

BrickPi3 BP;	// instance of BrickPi3 class

int switch_val_light = 2000; //2000
int switch_val_rgb = 450; //450
int inter_val_light = 2300;
int inter_val_rgb = 300;
int t_section_val_light = 2400;
int t_section_val_rgb = 500;
int debug = 1;
int my_x = 0;
int my_y = 0;
int target_x = 3;
int target_y = 2;
int direction = 90;
int32_t motorEncoderRight;
int32_t motorEncoderLeft;
int32_t motorEncoderRight_old;
int32_t motorEncoderLeft_old;
int32_t motorEncoderRight_new;
int32_t motorEncoderLeft_new;
int distance_obstacle;
unsigned int read_distance();
bool obstacle = false;
bool dodge_obstacle = false;

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
	/*
	cout << "Geef uw X waarde in waar de robot naartoe moet. " << '\n';
	cin >> target_x;
	cout << "Geef uw Y waarde in waar de robot naartoe moet. " << '\n';
	cin >> target_y;
	*/
	signal(SIGINT, exit_signal_handler); // Ctrl-c signal handler
	BP.detect(); // Check for BrickPi firmware
	BP.set_motor_limits(PORT_C, 35, 0);
	BP.set_motor_limits(PORT_B, 35, 0);
	int error;
	int encoder_value = BP.reset_motor_encoder(PORT_C);
	/* initialize sensor modes */
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON); 
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_RED);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_light_t      right_light; // struct for sensors
	sensor_light_t      left_light;

	reset_encoders();
	// Main loop
	while (true)
  	{
		if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)
		{	
			//cout << "LVAL: " << L_VAL << '\n';
			//cout << "RVAL: " << R_VAL << '\n';
			distance_obstacle = read_distance();
			set_encoders(motorEncoderLeft, motorEncoderRight);
			if(my_x == target_x && my_y == target_y)
			{
			cout << "klaar. " << '\n';
			stop_engines();
			BP.reset_all();
			break;
			}
			else if(R_VAL > inter_val_light && L_VAL < inter_val_rgb && obstacle == false) //Als beide waarden zwart zijn(kruispunt of T-splitsing)
			{
				cout << "Kruispuunt maar geen obstakel" << '\n';
				cout << "LVAL: " << L_VAL << '\n';
				cout << "RVAL: " << R_VAL << '\n';
				reset_encoders();
				stop_engines();
				set_encoders(int &motorEncoderLeft_new, int &motorEncoderRight_new);
				if(motorEncoderLeft_new > motorEncoderLeft_old +10 && motorEncoderLeft_new < motorEncoderLeft_old -10)
				{
					set_encoders(int &motorEncoderLeft_old, int &motorEncoderRight_old);
					update_location(my_x, my_y, direction);
					cout << "Location updated." << '\n';
				}
				while (R_VAL > inter_val_light && L_VAL < inter_val_rgb && distance_obstacle > 10)
				{	
					if (BP.get_sensor(PORT_3, right_light) == 0 && BP.get_sensor(PORT_4, left_light) == 0)	
					{
						cout << "x : " << my_x << " target x: " << target_x << '\n';
						cout << "y : " << my_y << " target y: " << target_y << '\n';
						if (my_x == target_x && my_y == target_y)
						{
							cout << "op eindpunt gearriveerd." << '\n';
							stop_engines();
						}
						else if(my_x < target_x) //deze if else  bepalen waar de robot naartoe moet(t.o.v. richting
						{
							if (direction == 90)
							{
								go_straight(PORT_C, PORT_B);
							}
							else if (direction == 0)
							{
								go_hard_right(direction);
								break;
							}
							else if (direction == 180)
							{
								go_hard_left(direction);
								break;
							}
						}
						else if(my_x == target_x && my_y < target_y)
						{	
							cout << "else if 1" << '\n';
							if(direction == 90)
							{
								BP.set_motor_position_relative(PORT_C, 100);
								BP.set_motor_position_relative(PORT_B, 100);
								sleep(2);
								go_hard_left(direction);
								cout << "Dir: " << direction << '\n';
								sleep(2);
								break;
							}
							else if(direction == 270)
							{
								go_hard_right(direction);
								break;
							}
							else if(direction == 0) 
							{
								BP.set_motor_position_relative(PORT_C, 100);
								BP.set_motor_position_relative(PORT_B, 100);
								sleep(2);
								go_straight(PORT_C, PORT_B);
								break;
							}
						}
						else if(my_y > target_y)
						{
							cout << "else if 4" << '\n';
							if(direction == 0)
								go_hard_right(direction);
							else if(direction == 180)
								go_hard_left(direction);
							else
								go_hard_left(direction);
						}
					}
				}
				if(distance_obstacle <= 10 && distance_obstacle) // als er een obstakel is, ga achteruit
				{
					cout << "object op " << distance_obstacle << '\n';
					cout << "motorEncoderLeft " << motorEncoderLeft << '\n';
					cout << "motorEncoderRight " << motorEncoderRight << '\n';
					sleep(1);
					cout << "we gaan achteruit mattieeee!" << '\n';
					go_back(obstacle, (-motorEncoderLeft+100), (-motorEncoderRight+100));
				}
				else //kruispunt maar geen object.
				{
					go_straight(PORT_C, PORT_B);
					cout << "x: " << my_x << " y: " << my_y << '\n';
					cout << "direction: " << direction << '\n';
				}
			}
			else if (R_VAL > inter_val_light && L_VAL < inter_val_rgb && obstacle) //obstakel waargenomen op na het kruispunt
			{
				cout << "Kruispunt en obstakel" << '\n';	
				if(direction == 90 || direction == 0)
				{
					if(my_y < 4)
					{
						go_hard_left(direction);
						obstacle = false;
					}
					else
					{
						go_hard_right(direction);
						obstacle = false;
					}
				}
				else if(direction == 270 || direction == 180)
				{
					go_hard_left(direction);
					obstacle = false;
				}
			}
			else if(distance_obstacle <= 10 && distance_obstacle) // als er een obstakel is, ga achteruit
                        {
				cout << "object op " << distance_obstacle << '\n';
                                cout << "motorEncoderLeft " << motorEncoderLeft << '\n';
                                cout << "motorEncoderRight " << motorEncoderRight << '\n';
                                stop_engines();
				cout << "we gaan achteruit mattieeee!" << '\n';
				go_back(obstacle, (-motorEncoderLeft-100), (-motorEncoderRight-100));
                        }
			else if(R_VAL < switch_val_light && L_VAL > switch_val_rgb) // Volgen van de lijn
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
