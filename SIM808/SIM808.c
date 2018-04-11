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

//sim808���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//

//sim808���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


/**
  * @brief  ����һ��ATָ��
	//��sim808��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
**/
u8 send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	printf("%s  ====%d \n",cmd,res);   //���ڴ�ӡָ������ loco4.11
	return res;
} 

/*
   * @brief gprs init
*/
void gprs_init()
{
	 send_cmd("AT","OK",10);		
  send_cmd("AT+CGCLASS=\"B\"","OK",10);				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
  send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",10);//����PDP������,��������Э��,��������Ϣ
  send_cmd("AT+CGATT=1","OK",10);					//����GPRSҵ��
  send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",10);	 	//����ΪGPRS����ģʽ
  send_cmd("AT+CLPORT=\"TCP\"\,\"2000\"","OK",10);	 				//���ý���������ʾIPͷ(�����ж�������Դ)

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
