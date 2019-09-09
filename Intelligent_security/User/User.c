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
      HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL); //������ģʽ����
         HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  //pwm����
      	HAL_TIM_Base_Start_IT(&htim3);               //�����жϿ���
      
      
   	while(DHT11_Init())	//DHT11��ʼ��	        //�¶�ģ���ʼ��	�ɹ�
    {
            HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
            HAL_Delay(200);
    }
   //sim800cģ���ʼ��
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
  
  
  void UserServiceCallBack(void)//�û��������ص�����
  {



      //�¶�ģ����
   
        if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ		
               if(temperature>40)
               {
                   if(temper_flag==10)
                   {
//                          tim3flag=1;//���������־
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
//                          tim3flag=0;//���������־
                 }
		}				   
	 	HAL_Delay(5);
		t++;
		if(t==20)
		{
			t=0;
            HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
		}
      
      
      //MQ-2ģ����
       HAL_ADC_Start(&hadc1);//adc����
      HAL_ADC_PollForConversion(&hadc1, 50);
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
     {
           ADC1_Value = HAL_ADC_GetValue(&hadc1);//��ȡadc��ֵ
         if(ADC1_Value>1000)  //�ж�Ũ��ֵ�Ƿ�ﵽΣ��ֵ
         {
                  if(ADC1_Value_flag==10)//�жϴﵽΣ��ֵ����ʱ�䣬��������
                   {
                       temper_flag=1;
                        Alarm_system("Sir,the gas may not be turned off in your house.please go home hurry. \n");//����������Ϣ��ʾ�ͱ���
                       BeepCall(4);//����������������
       tim3flag=1;
                       ADC1_Value_flag=0;
                       temper_flag=0;//�����Ŵ����
                       
                   }
                   ADC1_Value_flag++;
                    
               }
               else
                 {
                     ADC1_Value_flag=0;
                 }
         }
     
         
         //MQ-5ģ����
       HAL_ADC_Start(&hadc2);
      HAL_ADC_PollForConversion(&hadc2, 50);
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))
     {
           ADC2_Value = HAL_ADC_GetValue(&hadc2);
         if(ADC2_Value>1200)  //�ж�Ũ��ֵ�Ƿ�ﵽΣ��ֵ
         {
                  if(ADC2_Value_flag==10)//�жϴﵽΣ��ֵ����ʱ�䣬��������
                   {
                       temper_flag=1;
                     Alarm_system("Sir,the gas may not be turned off in your house.please go home hurry. \n");//����������Ϣ��ʾ�ͱ���
                       BeepCall(4);//����������������
  tim3flag=1;
                       ADC2_Value_flag=0;
                       temper_flag=0;//�����Ŵ����
                       
                   }
                   ADC2_Value_flag++;
                    
               }
               else
                 {
                     ADC2_Value_flag=0;
                 }
         }
    GetMsg();//��ȡsim800cģ����Ϣ�����Զ��ſ��ƿ����Ŵ�
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
  
  
  
