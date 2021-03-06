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
#include "stdio.h"
#include "lcd.h"
#include "tsensor.h"
#include "ds18b20.h" 

#define callbuf  "13540328279"
 int8_t  src_api_key[] = "4=FIl6GSKGpTo5MFQrDHuxVrlUA=";
 int8_t  src_dev[] = "29547977"; 
extern volatile uint32_t   usart3_rcv_len;
	u16 adcx;
	short b_temp;
 	float cpu_temp;
void get_temp();


 int main(void)
 {		
	int data1=400,data2=352;
 	u8 key=0;
	delay_init();	    	 //延时函数初始化	 
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 
  USART2_Init(36,115200);
	usart3_init(36,115200);
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	LCD_Init();			 	
 	T_Adc_Init();		  		//ADC初始化 
	DS18B20_Init();
	
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"Grad-proj for Loco");	
	LCD_ShowString(60,70,200,16,16,"Data Collection");	
// 	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
// 	LCD_ShowString(60,110,200,16,16,"2012/9/7");	
	//显示提示信息											      
	POINT_COLOR=BLUE;//设置字体为蓝色
		LCD_ShowString(60,130,200,16,16,"Temp:   . C");	 	      
//	LCD_ShowString(60,150,200,16,16,"TEMP_VOL:0.000V");	      
	LCD_ShowString(60,170,200,16,16,"CPU TEMPERATE:00.00C");	
	 
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
// 						SIM808_CONNECT();
// 					  DS0_OFF;
// 						break;
// 				}
// 			
// 				
// 			} 			
// 		
// 		
 	//		delay_ms(2000);
// 			Connect_RequestType1(src_dev, src_api_key);
			
 		SIM808_CONNECT();//A6连接onenet，检查连接状态

 //		memset(USART3_RX_BUF, 0, strlen((const char *)USART3_RX_BUF));
   //      usart3_rcv_len = 0;  
 	
	//	while(1);
	//	Ping_Server();   
 //       Save_Pm2p5ToOneNet();
	//	Ping_Server();
		get_temp();
		restful_send(src_dev, src_api_key,"cpu_temp",(int)cpu_temp);
 		delay_ms(200);
		restful_send(src_dev, src_api_key,"18b_temp",(int)(b_temp/10));
 		delay_ms(200);
	}	 
 }


void get_temp()
{
	float temp,temp1,temp2;
	
	adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);
//		LCD_ShowxNum(132,130,adcx,4,16,0);//显示ADC的值
		temp=(float)adcx*(3.3/4096);
		cpu_temp=temp;//保存温度传感器的电压值
	
 		adcx=temp;
// 		LCD_ShowxNum(132,150,adcx,1,16,0);     		//显示电压值整数部分
// 		temp1=temp-(u8)temp;				    			//减掉整数部分		  
// 		LCD_ShowxNum(148,150,temp1*1000,3,16,0X80);	//显示电压小数部分
			
		b_temp=DS18B20_Get_Temp();	
			if(b_temp<0)
			{
				LCD_ShowChar(60+40,130,'-',16,0);			//显示负号
				b_temp=-b_temp;					//转为正数
			}else LCD_ShowChar(60+40,130,' ',16,0);			//去掉负号
			LCD_ShowNum(60+40+8,130,b_temp/10,2,16);	//显示正数部分	    
   		LCD_ShowNum(60+40+32,130,b_temp%10,1,16);	//显示小数部分 	
	
		cpu_temp=(1.43-cpu_temp)/0.0043+25;		//计算出当前温度值	 
		LCD_ShowxNum(60+14*8,170,(u8)cpu_temp,2,16,0); //显示温度整数部分
		temp2=cpu_temp-(u8)cpu_temp;	  
		LCD_ShowxNum(60+24+14*8,170,temp2*100,2,16,0X80);//显示温度小数部分
		LED0=!LED0;
		delay_ms(250);
	
} 
 
