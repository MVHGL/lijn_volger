#ifndef _MOTOR_CONTROL_HPP
#define _MOTOR_CONTROL_HPP

/* macro definitions */
#define HARD_TURN 90	// degrees to make a hard turn

/* function definitions */
/* function to make a sharp 90 degree left turn */
void hard_left(uint8_t left_port, uint8_t right_port);
/* function to make a sharp 90 degree right turn */
void hard_right(uint8_t left_port, uint8_t right_port);

#endif
