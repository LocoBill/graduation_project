#include "a6.h"
#include "usart.h"		
#include "delay.h"	 	 
#include "string.h"    	
#include "usart3.h" 
//a6发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8 tcpflag=1;//未连接或者closed
u8* check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向a6发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 SendCmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//等待上一次数据发送完成  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(check_cmd(ack))
				{ 
				printf("%s %d cmd:%s,rsp:%s\n", __func__, __LINE__, cmd, USART3_RX_BUF);
				break;//得到有效数据 
				}
				USART3_RX_STA=0;
				
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
void A6_Init(void)
{	  
	  SendCmd("AT","OK",100);
	  SendCmd("ATE0","OK",200);
	  SendCmd("AT+CGATT=1","OK",1000);
	  SendCmd("AT+CGACT=1,1","OK",1000);
      SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876","OK", 1000);
 	  tcpflag=SendCmd("at+cipstatus?","CONNECT OK", 1000);
}	
void A6_CONNECT()
{	
  A6_Init();	   
}


