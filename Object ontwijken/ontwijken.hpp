#ifndef _ONTWIJKEN_HEADER_HPP
#define _ONTWIJKEN_HEADER_HPP

#include "BrickPi3.h"
#include <stdint.h>
using namespace std;

unsigned int read_distance();
void dodge(); 
void exit_signal_handler(int signo);
void setup();

#endif
