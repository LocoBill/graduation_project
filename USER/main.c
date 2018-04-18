#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "SIM808.h"
#include "a6.h"
#include "usart3.h"	
#include <string.h>
#include "EdpDemo.h"

#define callbuf  "13540328279"
int8_t  src_api_key[] = "28QOImqxBAIP6Vx34h9WqWyrsDo=";
int8_t  src_dev[] = "29547977";
extern volatile uint32_t   usart3_rcv_len;
 int main(void)
 {		
 	u8 key=0;
	delay_init();	    	 //��ʱ������ʼ��	  
  USART2_Init(36,115200);
	usart3_init(36,115200);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
// u2_printf("ATD%s;\r\n",callbuf);
//	gprs_init();
 
 //SIM808_DevLink(DEVICEID,APIKEY,20);
	 // send_cmd("AT+CIPMODE=1","OK",10);	
  //  send_cmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",\"876\"","OK",10);
 	while(1)
	{
// 					key=KEY_Scan(0); 
// 			if(key)
// 			{
// 				switch(key)
// 				{
// 					case KEY_RIGHT:
// 					
// 						break;
// 					case KEY_DOWN:
// 					
// 						break;
// 					case KEY_UP:
// 						DS0_ON;
// 						gprs_init();
// 					  DS0_OFF;
// 						break;
// 				}
// 			
// 				
// 			} 			
		
		
		A6_CONNECT();//A6����onenet���������״̬
		memset(USART3_RX_BUF, 0, strlen((const char *)USART3_RX_BUF));
        usart3_rcv_len = 0;  
		Connect_RequestType1(src_dev, src_api_key);
		Ping_Server();   
        Save_Pm2p5ToOneNet();
		Ping_Server();
		
		
	}	 
 }

