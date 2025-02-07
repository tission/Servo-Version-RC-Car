#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/************************************************
 ALIENTEK��ӢSTM32������ʵ��10
 ���벶��ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 

extern u32  Frequency1,Frequency2;
extern u32  HIGH1,LOW1,HIGH2,LOW2;
int main(void)
 {		
	 
	 delay_init();	    	 //��ʱ������ʼ��	  
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	 LED_Init();

	 TIM2_Cap_Init();
	 TIM3_PWM_Init(19999,71);
 	
	 while(1)
	 {
	 
		 delay_ms(10);
		 
		 if(HIGH1>1450 && HIGH1<1550)
			 TIM_SetCompare2(TIM3,450);
		 else if(HIGH1>1600 && HIGH1<1700)
			 TIM_SetCompare2(TIM3,700);
		 else if(HIGH1>1750 && HIGH1<1850)
			 TIM_SetCompare2(TIM3,950);
		 else if(HIGH1>1400 && HIGH1<1450)
			 TIM_SetCompare2(TIM3,1200);
		 else if(HIGH1>1300 && HIGH1<1400)
			 TIM_SetCompare2(TIM3,1450);
		 else if(HIGH1>1150 && HIGH1<1250)
			 TIM_SetCompare2(TIM3,1700);
		 else if(HIGH1>1000 && HIGH1<1100)
			 TIM_SetCompare2(TIM3,1950);
		 
		 
		 if(HIGH2>1000 && HIGH2<1400)
		 {
			 PBout(0) = 1;
			 PBout(1) = 0;
		 }
		 else if(HIGH2>1500 && HIGH2<1850)
		 {
			 PBout(0) = 0;
			 PBout(1) = 1;
		 } 
		 else if(HIGH2>1400 && HIGH2<1500)
		 {
			 PBout(0) = 0;
			 PBout(1) = 0;
		 } 
		
		}
		  			
 }


 
 
 
 
 
 
 