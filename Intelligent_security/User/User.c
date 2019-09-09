#include "User.h"
#include "string.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
    uint8_t t=0;	
    uint8_t temperature=0;  	    
	uint8_t humidity=0; 
    uint8_t zt=0;
   uint8_t text=1;
   	uint16_t ADC1_Value;
     	uint16_t ADC2_Value;

    
uint8_t temp_flag=0;
uint16_t temper_flag=0;
uint16_t ADC1_Value_flag=0;
uint16_t ADC2_Value_flag=0;
uint8_t tim3flag=0;
uint16_t motorencode=0;
uint8_t rxtemp[30]={0};
char  temp11[20] = "AT+CMGF=1\r\n";
char  temp12[20] = "AT+CSCS=\"GSM\"\r\n";
char  temp13[20] = "AT+CNMI=2,1\r\n";
  void UserInitCallBack(void)
  {
      
      delay_init(72);
      HAL_Delay(400);
      HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
    HAL_Delay(400);
    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
    HAL_Delay(200);
    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
      HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL); //编码器模式开启
         HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  //pwm开启
      	HAL_TIM_Base_Start_IT(&htim3);               //处理中断开启
      
      
   	while(DHT11_Init())	//DHT11初始化	        //温度模块初始化	成功
    {
            HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
            HAL_Delay(200);
    }
   //sim800c模块初始化
    HAL_UART_Transmit(&huart1,(uint8_t*)temp11,20,100);
    memset(rxtemp,0,30);
    HAL_UART_Receive(&huart1,rxtemp,30,300);
     memset(rxtemp,0,30);
    HAL_UART_Transmit(&huart1,(uint8_t*)temp12,20,100);
      HAL_UART_Receive(&huart1,rxtemp,30,300);
     memset(rxtemp,0,30);
HAL_UART_Transmit(&huart1,(uint8_t*)temp13,20,100);
      HAL_UART_Receive(&huart1,rxtemp,30,300);
  }
  
  
  void UserServiceCallBack(void)//用户服务函数回调函数
  {



      //温度模块检测
   
        if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值		
               if(temperature>40)
               {
                   if(temper_flag==10)
                   {
//                          tim3flag=1;//电机开启标志
                        Alarm_system("Sir,you may have a fire in your home.please go home hurry. \n");
                       BeepCall(3);
                      tim3flag=1;
                       temper_flag=0;
                       
                   }
                   temper_flag++;
                    
               }
               else
                 {
                     temper_flag=0;
//                          tim3flag=0;//电机开启标志
                 }
		}				   
	 	HAL_Delay(5);
		t++;
		if(t==20)
		{
			t=0;
            HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
		}
      
      
      //MQ-2模块检测
       HAL_ADC_Start(&hadc1);//adc开启
      HAL_ADC_PollForConversion(&hadc1, 50);
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
     {
           ADC1_Value = HAL_ADC_GetValue(&hadc1);//读取adc的值
         if(ADC1_Value>1000)  //判断浓度值是否达到危险值
         {
                  if(ADC1_Value_flag==10)//判断达到危险值持续时间，消除抖动
                   {
                       temper_flag=1;
                        Alarm_system("Sir,the gas may not be turned off in your house.please go home hurry. \n");//启动发送信息提示和报警
                       BeepCall(4);//开启蜂鸣器，报警
       tim3flag=1;
                       ADC1_Value_flag=0;
                       temper_flag=0;//开启门窗电机
                       
                   }
                   ADC1_Value_flag++;
                    
               }
               else
                 {
                     ADC1_Value_flag=0;
                 }
         }
     
         
         //MQ-5模块检测
       HAL_ADC_Start(&hadc2);
      HAL_ADC_PollForConversion(&hadc2, 50);
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))
     {
           ADC2_Value = HAL_ADC_GetValue(&hadc2);
         if(ADC2_Value>1200)  //判断浓度值是否达到危险值
         {
                  if(ADC2_Value_flag==10)//判断达到危险值持续时间，消除抖动
                   {
                       temper_flag=1;
                     Alarm_system("Sir,the gas may not be turned off in your house.please go home hurry. \n");//启动发送信息提示和报警
                       BeepCall(4);//开启蜂鸣器，报警
  tim3flag=1;
                       ADC2_Value_flag=0;
                       temper_flag=0;//开启门窗电机
                       
                   }
                   ADC2_Value_flag++;
                    
               }
               else
                 {
                     ADC2_Value_flag=0;
                 }
         }
    GetMsg();//读取sim800c模块信息，用以短信控制开关门窗
}

     
//    HAL_Delay(100);
//    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
//         HAL_Delay(100);
//    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
//         HAL_Delay(100);
//    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);


  
  void BeepCall(uint8_t keeds)
  {
      while(keeds)
      {
          HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);
          HAL_Delay(50); 
          keeds--;
      }

  }
  
  
  
