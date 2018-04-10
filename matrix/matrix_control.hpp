#ifndef _MATRIX_CONTROL_HPP
#define _MATRIX_CONTROL_HPP

#include <stdint.h>	// for uint8_t
#include "BrickPi3.h"


/* function definitions */
/* function to turn left, right or go straight */
void go_right(uint8_t left_port, uint8_t right_port);

void go_left(uint8_t left_port, uint8_t right_port);

void go_straight(uint8_t left_port, uint8_t right_port);

#endif
