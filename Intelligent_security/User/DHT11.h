#ifndef __DHT11_H
#define  __DHT11_H

#include "stm32f1xx_hal.h"
#include "sys.h"
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
#define DHT11_IO_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}


#define	DHT11_DQ_OUT PBout(11) //数据端口	PA0 
#define	DHT11_DQ_IN  PBin(11)  //数据端口	PA0 

void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void) ;
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);
uint8_t DHT11_Init(void);
   
#endif 

