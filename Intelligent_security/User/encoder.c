#include "encoder.h"
extern TIM_HandleTypeDef htim2;
    int Encoder_TIM;   
    int encode=0 ,pidout=0;
    float ki=0,kp=100,kd=0;
 extern   uint8_t tim3flag;
int Read_Encoder(void)
{
 
    Encoder_TIM= TIM1 -> CNT;

		return Encoder_TIM;
}


uint8_t setPWMpulse(int pulse)
{
       TIM_OC_InitTypeDef sConfigOC;  
     sConfigOC.OCMode = TIM_OCMODE_PWM1;  
   sConfigOC.Pulse = pulse;  
     sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;  
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;  
     HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
    if( HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1)!=HAL_OK)
    {
       return 1; 
    }
  
    return 0;
}


int locationPIDcounter(int readangle,int targetargle)
{
    

    static int error=0,lasterror=0,integral=0;
    int outpid;
    error =targetargle-readangle;
    integral +=error;
    
    
    outpid = kp*error+ki*integral+kd*(error-lasterror);
    outpid = outpid > 9000 ? 9000 : outpid;
    if(outpid<0)
      outpid = -outpid;
    lasterror = error;
    return  outpid;
    
}

uint8_t motor(void)
{

    
   
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
   pidout = locationPIDcounter(encode,800);
  

    setPWMpulse(pidout);
    return 0;
}

  void UserTIM3ITCallBack(void)//电机服务回调函数
  {
     encode = Read_Encoder();
//    setPWMpulse(8000);
//      setPWMpulse(500);
      if(tim3flag==1)
      {
        motor();
      }
      else if(tim3flag==0)
      {
         setPWMpulse(0);
      }
     
   
  }

  