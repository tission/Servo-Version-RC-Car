#include "timer.h"
#include "led.h"
#include "usart.h"

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��                                                                      
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 
}


//��ʱ��2ͨ��1���벶�����ò�Ƶ�� ռ�ձ�
void TIM2_Cap_Init(void)
{	 

    GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (72000000 / 1000000) - 1 ; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseStructure.TIM_Period = 0XFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM_ICInitStructure.TIM_ICFilter = 0x00; 
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM_ICInitStructure.TIM_ICFilter = 0x00; 
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
  TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2, ENABLE); 
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
  TIM_Cmd(TIM2, ENABLE); //���� TIM2

}
	

u32  Frequency1,Frequency2;
static int starttime1=0,starttime2=0;
static int endtime1=0,endtime2=0;
static int a1=0,b1=0,a2=0,b2=0;
int c1=0,c2=0;
uint32_t HIGH1,LOW1,HIGH2,LOW2;


void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		c1++;
		c2++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
	//ͨ��1
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == SET)
		{
			a1 = TIM_GetCapture1(TIM2);
			starttime1 = c1*0xFFFF + a1;
			LOW1 = starttime1 - b1;
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);
		}
		else
		{
			b1 = TIM_GetCapture1(TIM2); 
			endtime1 = c1*0xFFFF + b1;
			HIGH1 = endtime1 - a1;
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);
		}
		
		Frequency1 = 1000000/(HIGH1 + LOW1);
		c1 = 0;

		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); //����жϱ�־λ

	}
	
	//ͨ��2
	if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == SET)
		{
			a2 = TIM_GetCapture2(TIM2);
			starttime2 = c2*0xFFFF + a2;
			LOW2 = starttime2 - b2;
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);
		}
		else
		{
			b2 = TIM_GetCapture2(TIM2); 
			endtime2 = c2*0xFFFF + b2;
			HIGH2 = endtime2 - a2;
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);
		}
		
		Frequency2 = 1000000/(HIGH2 + LOW2);
		c2 = 0;

		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2); //����жϱ�־λ

	}
	
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
