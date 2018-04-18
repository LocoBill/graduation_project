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
	delay_init();	    	 //延时函数初始化	  
  USART2_Init(36,115200);
	usart3_init(36,115200);
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
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
		
		
		A6_CONNECT();//A6连接onenet，检查连接状态
		memset(USART3_RX_BUF, 0, strlen((const char *)USART3_RX_BUF));
        usart3_rcv_len = 0;  
		Connect_RequestType1(src_dev, src_api_key);
		Ping_Server();   
        Save_Pm2p5ToOneNet();
		Ping_Server();
		
		
	}	 
 }

