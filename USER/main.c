#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "SIM808.h"

#define callbuf  "13540328279"


 int main(void)
 {		
 	u8 key=0;
	delay_init();	    	 //��ʱ������ʼ��	  
  USART2_Init(36,115200);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
// u2_printf("ATD%s;\r\n",callbuf);
	//gprs_init();
 
  SIM808_DevLink(DEVICEID,APIKEY,20);
	 // send_cmd("AT+CIPMODE=1","OK",10);	
  //  send_cmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",\"876\"","OK",10);
 	while(1)
	{
					key=KEY_Scan(0); 
			if(key)
			{
				switch(key)
				{
					case KEY_RIGHT:
					
						break;
					case KEY_DOWN:
					
						break;
					case KEY_UP:
						DS0_ON;
						gprs_init();
					  DS0_OFF;
						break;
				}
			
				
			} 			
		
		
		
		
	}	 
 }

