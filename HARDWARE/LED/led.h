#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#define  DS0_ON      GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define  DS1_ON      GPIO_ResetBits(GPIOE,GPIO_Pin_5)



#define  DS0_OFF      GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define  DS1_OFF      GPIO_SetBits(GPIOE,GPIO_Pin_5)


void LED_Init(void);//��ʼ��

		 				    
#endif
