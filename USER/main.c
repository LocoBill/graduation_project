#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#define callbuf  "13540328279"


 int main(void)
 {		
 	u8 t;
	u8 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
  USART2_Init(36,115200);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	//u2_printf("ATD%s;\r\n",callbuf);
 	while(1)
	{
		
		
		
		
	}	 
 }

