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

#define callbuf  "13540328279"
 int8_t  src_api_key[] = "4=FIl6GSKGpTo5MFQrDHuxVrlUA=";
 int8_t  src_dev[] = "29547977"; 
extern volatile uint32_t   usart3_rcv_len;
	u16 adcx;
	float temp;
 	float temperate;
void get_temp();


 int main(void)
 {		
	int data1=400,data2=352;
 	u8 key=0;
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 
  USART2_Init(36,115200);
	usart3_init(36,115200);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	LCD_Init();			 	
 	T_Adc_Init();		  		//ADC��ʼ�� 
	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"Loco��s Grad-proj");	
	LCD_ShowString(60,70,200,16,16,"CPU Internal temperature");	
// 	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
// 	LCD_ShowString(60,110,200,16,16,"2012/9/7");	
	//��ʾ��ʾ��Ϣ											      
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,130,200,16,16,"TEMP_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"TEMP_VOL:0.000V");	      
	LCD_ShowString(60,170,200,16,16,"TEMPERATE:00.00C");	
	 
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
			
 		SIM808_CONNECT();//A6����onenet���������״̬

 //		memset(USART3_RX_BUF, 0, strlen((const char *)USART3_RX_BUF));
   //      usart3_rcv_len = 0;  
 	
	//	while(1);
	//	Ping_Server();   
 //       Save_Pm2p5ToOneNet();
	//	Ping_Server();
		get_temp();
		restful_send(src_dev, src_api_key,"my_data",(int)temp);
 		delay_ms(1000);
		restful_send(src_dev, src_api_key,"sys_time",(int)temperate);
 		delay_ms(1000);
	}	 
 }


void get_temp()
{
	float temp1,temp2;
	
	adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);
		LCD_ShowxNum(132,130,adcx,4,16,0);//��ʾADC��ֵ
		temp=(float)adcx*(3.3/4096);
		temperate=temp;//�����¶ȴ������ĵ�ѹֵ
		adcx=temp;
		LCD_ShowxNum(132,150,adcx,1,16,0);     		//��ʾ��ѹֵ��������
		temp1=temp-(u8)temp;				    			//������������		  
		LCD_ShowxNum(148,150,temp1*1000,3,16,0X80);	//��ʾ��ѹС������
										  
		temperate=(1.43-temperate)/0.0043+25;		//�������ǰ�¶�ֵ	 
		LCD_ShowxNum(140,170,(u8)temperate,2,16,0); //��ʾ�¶���������
		temp2=temperate-(u8)temperate;	  
		LCD_ShowxNum(164,170,temp2*100,2,16,0X80);//��ʾ�¶�С������
		LED0=!LED0;
		delay_ms(250);
	
} 
 
