#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart.h"
#include "usart2.h"
#include "edpkit.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "SIM808.h"
 EdpPacket* send_pkg;
char send_buf[MAX_SEND_BUF_LEN];

volatile unsigned char  rcv_cmd_start=0;
volatile unsigned char  rcv_cmd_flag=0;

unsigned char  usart2_rcv_buf[MAX_RCV_LEN];
volatile unsigned int   usart2_rcv_len=0;

unsigned char  usart2_cmd_buf[MAX_CMD_LEN];
volatile unsigned int   usart2_cmd_len=0;

//sim808发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//

//sim808发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


/**
  * @brief  发送一条AT指令
	//向sim808发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
**/
u8 send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	printf("%s  ====%d \n",cmd,res);   //串口打印指令发送情况 loco4.11
	return res;
} 

/*
   * @brief gprs init
*/
void gprs_init()
{
	 send_cmd("AT","OK",10);		
  send_cmd("AT+CGCLASS=\"B\"","OK",10);				//设置GPRS移动台类别为B,支持包交换和数据交换 
  send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",10);//设置PDP上下文,互联网接协议,接入点等信息
  send_cmd("AT+CGATT=1","OK",10);					//附着GPRS业务
  send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",10);	 	//设置为GPRS连接模式
  send_cmd("AT+CLPORT=\"TCP\"\,\"2000\"","OK",10);	 				//设置接收数据显示IP头(方便判断数据来源)

}

/**
  * @brief  
**/
void SIM808_DevLink(const char* devid, const char* auth_key, int timeOut)
{
		int32 count=0;

		memset(usart2_rcv_buf,0,strlen((const char *)usart2_rcv_buf));
		usart2_rcv_len=0;			
		
	  send_cmd("AT+CIPMODE=1","OK",100);	
    send_cmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",\"876\"","OK",100); 
		for(count=0;count<timeOut;count++)
		{
				delay_ms(100);
				if((NULL != strstr((const char *)usart2_rcv_buf,">")))
				{
						break;
				}
		}	

// 		send_pkg = PacketConnect1(devid,auth_key);
// 		delay_ms(200);
// 		//usart2_write(USART2,send_pkg->_data,send_pkg->_write_pos);  //??????????
// 		u2_printf("%s",send_pkg->_data);
// 		delay_ms(500);
// 		DeleteBuffer(&send_pkg);
// 		delay_ms(200);
// 		//usart2_write(USART2,"+++",3);  //?ESP8266??+++????,?ESP8266??????
// 		u2_printf("+++");
// 		delay_ms(50);
}
