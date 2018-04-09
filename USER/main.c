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
	delay_init();	    	 //延时函数初始化	  
  USART2_Init(36,115200);
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	//u2_printf("ATD%s;\r\n",callbuf);
 	while(1)
	{
		
		
		
		
	}	 
 }

