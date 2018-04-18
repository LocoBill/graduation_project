#include "timer.h"
extern vu16 USART3_RX_STA;
//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	    		    
	if(TIM7->SR&0X01)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM7->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM7->CR1&=~(1<<0);		//�رն�ʱ��7	  
	}	      											 
} 
//������ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;//TIM7ʱ��ʹ��    
 	TIM7->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM7->PSC=psc;  	//Ԥ��Ƶ��  
	TIM7->DIER|=1<<0;   //��������ж�	 
	TIM7->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(0,1,TIM7_IRQn,2);//��ռ0�������ȼ�1����2									 
}

















