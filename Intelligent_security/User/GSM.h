#ifndef __GSM_H
#define __GSM_H
	 
#include "sys.h"	 
uint8_t Sim800cSend(uint8_t *buf,uint8_t keed);
uint8_t Alarm_system(uint8_t *text);
void GetMsg(void);

#endif
