#include "GSM.h"
#include "string.h"
extern UART_HandleTypeDef huart1;
void usart2_send_char(uint8_t c,UART_HandleTypeDef* huart); 
extern uint8_t tim3flag;
uint16_t USART2_RX_STA=0;   
uint8_t flag=0;
    uint16_t i=0;
    uint8_t temp_keed;

    uint8_t resetInit=0x1A;
     uint8_t rxbuf[500];
     uint16_t userdelay=0;
     uint8_t temp_rcbuf[10];
         uint8_t errorflag=1;
         uint8_t readrxbuff[20]={0};
          uint8_t readrxbuff1[100]={0};
//          uint8_t readrxbuff2[20]={0};
//           uint8_t readrxbuff3[20]={0};
            uint8_t num=0;
//         char temptxmsg[20]= "AT+CMGR=";
uint8_t Sim800cSend(uint8_t *buf,uint8_t keed)
{
    
   
    temp_keed =keed;

switch (keed)
{
        case 0:

          for(i=0;buf[i]!='\n';i++)  
            {
                if(i>100) 
                    return 1;
            }
    
            HAL_UART_Transmit(&huart1,buf,i+1,50);
          HAL_Delay(5);
            if(HAL_UART_Receive(&huart1,rxbuf,50,1000)!=HAL_OK)
            {
//                 HAL_UART_Transmit(&huart1,buf,i+1,50); 
                if(userdelay>20)
                {
                    userdelay=0;
                    return 1;
                }
                 HAL_Delay(5);
                userdelay++;
               
            }

            if(rxbuf[i+2]=='O'&&rxbuf[i+3]=='K')
             {

//                 rxbuf[i+2]=0;
//                 rxbuf[i+3]=0;
                return 0;
             }
            if(rxbuf[i]=='>')
             {
                return 2;
             }
        break;
    case 2:
           for(i=0;buf[i]!='\n';i++)  
            {
                if(i>500) 
                    return 1;
            }
            HAL_UART_Transmit(&huart1,buf,i-1,500);
            HAL_Delay(100);

//            if( HAL_UART_Receive(&huart1,rxbuf,i+50,1000)!=HAL_OK)
//            {
////               HAL_UART_Transmit(&huart1,buf,i-1,500);
//                HAL_Delay(100);
////                HAL_UART_Transmit(&huart1,&resetInit,1,50); 
//            }
//   
//            if(rxbuf[i+2]=='+'&&rxbuf[i+3]=='C')
//            {
//                 rxbuf[i+2]=0;
//                 rxbuf[i+3]=0;
//                return 0;
//            }
//            else
//             {
//                 HAL_Delay(500);
////                 HAL_UART_Receive(&huart1,rxbuf,2,1000);
//             }
        
        break;
    default :
        return 1;
    
}
  
    return 1;

} 

uint8_t Alarm_system(uint8_t *text)
{

         errorflag= Sim800cSend("AT\n",0);
                errorflag= Sim800cSend("AT+CMGF=1\n",0);
          Sim800cSend("AT+CSCS=\"GSM\"\n",0);
          Sim800cSend("AT+CMGS=\"18420016634\"\n",0);
          Sim800cSend("Sir,you may have a fire in your home.please go home hurry. \n",2);
           HAL_UART_Transmit(&huart1,&resetInit,1,50);
            HAL_Delay(1000);
 return 0;
   
//    if(Sim800cSend("AT\r\n",0)==0)
//    {
//        if(Sim800cSend("AT+CMGF=1\n",0)==0)
//        {
//         if(Sim800cSend("AT+CSCS=\"GSM\"\n",0)==0)
//         {
//            if(Sim800cSend("AT+CMGS=\"18875960660\"\n",0)==0)
//             {
//                 if(Sim800cSend(text,2)==0)
//                 {
//                     return 0;
//                 }else return 5;
//             }else return 4;
//         }else return 3;
//     }else return 2;
//        

//           
//    }else return 1;
    
    
}

/* -------------------------------- end -------------------------------- */
void usart2_send_char(uint8_t c,UART_HandleTypeDef* huart)
{
    while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TXE)==0);//等待上一次发送完毕  
    huart->Instance->DR=c;   
}

/*---------------------------------80字符限制-----------------------------------*/
  /**
  * @Data    2019-02-21 23:32
  * @brief   重定向C库函数 printf 到USART2
  * @param   void
  * @retval  void
  */
	int fputc(int ch,FILE *f)  
  {   
    usart2_send_char((unsigned char)ch,&huart1);
    return (ch);    
  }
void GetMsg(void)
{
  uint8_t i=20;
  uint8_t flaggg=1;
 memset(readrxbuff,0,20);
  HAL_UART_Receive(&huart1,readrxbuff,20,200);
 
    if(readrxbuff[0]==0x0D&&readrxbuff[1]==0x0A&&readrxbuff[10]==0x53&&readrxbuff[11]==0x4D)
    {
      if(readrxbuff[15]==0x0D)
      {
        num =( readrxbuff[14]&0x0f);
      }
      else  num =(((readrxbuff[14]&0x0f)*10)+(readrxbuff[15]&0x0f)); 
         printf("AT+CMGR=%d\r\n",num);
//      printf("AT+CMGR=1\r\n");
//       HAL_UART_Receive(&huart1,readrxbuff2,20,200);
      if( HAL_UART_Receive(&huart1,readrxbuff1,100,1000))
      {
        do{
          if(readrxbuff1[i]==0x0D)
          {
            if(readrxbuff1[i+1]==0x0A)
            {
              if(readrxbuff1[i+2] =='o'&&readrxbuff1[i+3] =='p'&&readrxbuff1[i+4] =='e'&&readrxbuff1[i+5] =='n')
                  tim3flag=1;
              else if(readrxbuff1[i+2] =='n'&&readrxbuff1[i+3] =='o')
                tim3flag=0;
                   flaggg=0;
            }
              
          }
          i++;
           
        }while(flaggg);
        
      }
//        HAL_UART_Receive(&huart1,readrxbuff3,20,200);
    

    }

}
