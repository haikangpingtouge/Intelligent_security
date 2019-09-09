#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"	 


int Read_Encoder(void);
uint8_t setPWMpulse(int pulse);
int locationPIDcounter(int readangle,int targetargle);
uint8_t motor(void);
#endif
