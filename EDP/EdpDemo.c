#include <stdio.h>
#include <stdlib.h>
#if defined(_LINUX)
//#include <unistd.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include <libgen.h>
#endif
#include <string.h>
#include "EdpKit.h"
#include "stm32f10x.h"
#include "usart3.h"
#include "utils.h"
#include "EdpDemo.h"
#include "a6.h"
#include "delay.h"
#include "sys.h" 
#include "led.h"
char buf[]={0x1A,0x0D,0x0A};
extern int8_t  src_api_key[] ;
extern int8_t  src_dev[] ;
extern volatile uint32_t   usart3_rcv_len;
 u16 Conce_PM2_5=30 ; 
u8 cntflag=1;
u8 sendflag=1;

/**
 * @brief  EDP数据包发送
 * @param  buffer: 要发送的数据缓冲区地址
* @param  len: 要发送的数据缓长度
* @param  sockfd：兼容linux socket api: STM32下无意义
 * @retval 发送的数据长度
 **/
int32_t DoSend(int32_t sockfd, const uint8_t *buffer, uint32_t len)
{   

    uint32_t i;
	memset(USART3_RX_BUF,0,strlen((const char *)USART3_RX_BUF));
	usart3_rcv_len=0;
    for(i = 0; i < len; i++)
    {
         while((USART3->SR&0X40)==0);
		  USART3->DR =buffer[i];
		
    }	
	delay_ms(20);	
//    hexdump((const uint8_t *)buffer, len);
    return len;
}

/*
 *  @brief  串口接收处理线程
 */
void Recv_Thread_Func(void)
{  
    int32_t error = 0;
    int32_t rtn;
    uint8_t mtype, jsonorbin;
    uint8_t buffer[128];
    RecvBuffer *recv_buf = NewBuffer();
    EdpPacket *pkg;

    int8_t *src_devid;
    int8_t *push_data;
    uint32_t push_datalen;

    cJSON *desc_json;
    int8_t *desc_json_str;
    int8_t *save_bin;
    uint32_t save_binlen;
    int8_t *json_ack;

    int8_t *cmdid;
    uint16_t cmdid_len;
    int8_t *cmd_req;
    uint32_t cmd_req_len;
    EdpPacket *send_pkg;
    int8_t *ds_id;
    double dValue = 0;

    int8_t *simple_str = NULL;
    int8_t cmd_resp[] = "ok";
    uint32_t cmd_resp_len = 0;
    uint8_t timeout_t=200;
//    printf("\n[%s] recv thread start ...\r\n", __func__);
    while (error == 0)
    {
       
       
		/* 试着接收1024个字节的数据 */
        int32_t rcv_len = 0;

        rcv_len = USART3_GetRcvNum();
        if ((rcv_len <= 0)&&timeout_t)
        {
//                printf("%s %d No Data\n", __func__, __LINE__);
						delay_ms(100);
						timeout_t--;
            continue;
        }
				if(timeout_t<=0)
				{
					break;
				}
				delay_ms(50);
	    rcv_len = USART3_GetRcvNum();
        USART3_GetRcvData(buffer, rcv_len);
//        printf("recv from server, bytes: %d\r\n", rcv_len);
//        hexdump((const uint8_t *)buffer, rcv_len);
//        printf("\n");
        /* 成功接收了n个字节的数据 */
        WriteBytes(recv_buf, buffer, rcv_len);
        while (1)
        {
            /* 获取一个完成的EDP包 */
            if ((pkg = GetEdpPacket(recv_buf)) == 0)
            {
//                printf("need more bytes...\n");
                break;
            }
            /* 获取这个EDP包的消息类型 */
            mtype = EdpPacketType(pkg);
//            printf("mtype=%d\n", mtype);
            /* 根据这个EDP包的消息类型, 分别做EDP包解析 */
            switch (mtype)
            {
                case CONNRESP:
                    /* 解析EDP包 - 连接响应 */
                    rtn = UnpackConnectResp(pkg);
//                    printf("recv connect resp, rtn: %d\n", rtn);
                    break;
                case PUSHDATA:
                    /* 解析EDP包 - 数据转发 */
                    UnpackPushdata(pkg, &src_devid, &push_data,
                                   &push_datalen);
//                    printf
//                    ("recv push data, src_devid: %s, push_data: %s, len: %d\n",
//                     src_devid, push_data, push_datalen);
                    free(src_devid);
                    free(push_data);
                    break;
                case SAVEDATA:
                    /* 解析EDP包 - 数据存储 */
                    if (UnpackSavedata(pkg, &src_devid, &jsonorbin)
                        == 0)
                    {
                        if (jsonorbin == kTypeFullJson
                            || jsonorbin ==
                            kTypeSimpleJsonWithoutTime
                            || jsonorbin ==
                            kTypeSimpleJsonWithTime)
                        {
                            //printf("json type is %d\n",
                             //      jsonorbin);
                            /* 解析EDP包 - json数据存储 */
                            /* UnpackSavedataJson(pkg, &save_json); */
                            /* save_json_str=cJSON_Print(save_json); */
                            /* printf("recv save data json, src_devid: %s, json: %s\n", */
                            /*     src_devid, save_json_str); */
                            /* free(save_json_str); */
                            /* cJSON_Delete(save_json); */

                            /* UnpackSavedataInt(jsonorbin, pkg, &ds_id, &iValue); */
                            /* printf("ds_id = %s\nvalue= %d\n", ds_id, iValue); */

                            UnpackSavedataDouble((SaveDataType)jsonorbin,
                                                 pkg,
                                                 (char **)&ds_id,
                                                 &dValue);
//                        printf
//                            ("ds_id = %s\nvalue = %f\n",
//                             ds_id, dValue);

                            /* UnpackSavedataString(jsonorbin, pkg, &ds_id, &cValue); */
                            /* printf("ds_id = %s\nvalue = %s\n", ds_id, cValue); */
                            /* free(cValue); */
                            free(ds_id);
                        }
                        else if (jsonorbin == kTypeBin)     /* 解析EDP包 - bin数据存储 */
                        {
                            UnpackSavedataBin(pkg,
                                              &desc_json,
                                              (uint8_t **) &
                                              save_bin,
                                              &save_binlen);
                            desc_json_str =(int8_t*)
                                cJSON_Print(desc_json);
//                            printf
//                            ("recv save data bin, src_devid: %s, desc json: %s, bin: %s, binlen: %d\n",
//                             src_devid, desc_json_str,
//                             save_bin, save_binlen);
                            free(desc_json_str);
                            cJSON_Delete(desc_json);
                            free(save_bin);
                        }
                        else if (jsonorbin == kTypeString)
                        {
                            UnpackSavedataSimpleString(pkg,
                                                       &simple_str);
//                            printf("%s\n", simple_str);
                            free(simple_str);
                        }
                        free(src_devid);
                    }
                    else
                    {
//                        printf("error\n");
                    }
                    break;
                case SAVEACK:
                    json_ack = NULL;
                    UnpackSavedataAck(pkg, &json_ack);
//                    printf("save json ack = %s\n", json_ack);
                    free(json_ack);
                    break;
                case CMDREQ:
                    if (UnpackCmdReq
                        (pkg, &cmdid, &cmdid_len, &cmd_req,
                         &cmd_req_len) == 0)
                    {
                        /*
                         * 用户按照自己的需求处理并返回，响应消息体可以为空，此处假设返回2个字符"ok"。
                         * 处理完后需要释放
                         */
                        cmd_resp_len = strlen((const char *)cmd_resp);
                        send_pkg =
                            PacketCmdResp(cmdid, cmdid_len,
                                          cmd_resp,
                                          cmd_resp_len);
#ifdef _ENCRYPT
                        if (g_is_encrypt)
                        {
                            SymmEncrypt(send_pkg);
                        }
#endif
                        DoSend(0, (const uint8_t *)
                               send_pkg->_data,
                               send_pkg->_write_pos);
                        DeleteBuffer(&send_pkg);

                        free(cmdid);
                        free(cmd_req);
                    }
                    break;
                case PINGRESP:
                    /* 解析EDP包 - 心跳响应 */
                    UnpackPingResp(pkg);
//                    printf("recv ping resp\n");
                    break;
                default:
                    /* 未知消息类型 */
                    error = 1;
//                    printf("recv failed...\n");
                    break;
            }
            DeleteBuffer(&pkg);
        }
    }
    DeleteBuffer(&recv_buf);

#ifdef _DEBUG
//    printf("[%s] recv thread end ...\n", __func__);
#endif
}

void Connect_RequestType1(int8_t *devid, int8_t *api_key)
{
     EdpPacket *send_pkg;
 	int t=20,dosend_flag=0;
 	u8 s[2]="OK";
     send_pkg = PacketConnect1(devid, api_key);
     if(send_pkg == NULL)
     {
         return;
     }
     printf("send connect to server, bytes: %d\n", send_pkg->_write_pos);
 		 SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80","OK", 1000);
		 SendCmd("at+cipstatus?","CONNECT OK", 1000);
    cntflag=SendCmd("AT+CIPSEND",">",1000);
 		printf("AT+CIPSEND----%d",cntflag);
		printf("pocet: %s\n",send_pkg->_data);
   dosend_flag = DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    delay_ms(500);//加上此句OK
		printf("dosend_flag: %d\n  ", dosend_flag);
	DeleteBuffer(&send_pkg);//加上此句OK
	USART3_RX_STA=0;


}

void restful_send(int8_t *devid, int8_t *api_key)
{
		int t=20,dosend_flag=0;
	  SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80","OK", 1000);
		SendCmd("at+cipstatus?","CONNECT OK", 1000);
    cntflag=SendCmd("AT+CIPSEND",">",1000);
 		printf("AT+CIPSEND----%d",cntflag);
		SendCmd("POST /devices/29547977/datapoints HTTP/1.1\r\napi-key:4=FIl6GSKGpTo5MFQrDHuxVrlUA=\r\nHost:api.hecloud.com\r\nContent-Length:64\r\n\r\n{\"datastreams\":[{\"id\":\"sys_time\",\"datapoints\":[{\"value\":20}]}]}\r\n",NULL,100);
//	 u3_printf("%x\r\n",26);
		DoSend(0, buf,strlen(buf));
}
/*
 *  @brief  发送PING包维持心跳
 */
void Ping_Server(void)
{   
	EdpPacket *send_pkg;
    printf("%s %d\n", __func__, __LINE__);
    /* 组装ping包 */
    send_pkg = PacketPing();
    SendCmd("AT+CIPSEND=2",">",1000);
    DoSend(0, (const uint8_t *)send_pkg->_data,send_pkg->_write_pos);
    delay_ms(50);
    DeleteBuffer(&send_pkg);
    USART3_RX_STA=0;
	  DS1_OFF;
}

void Save_Pm2p5ToOneNet(void)
{
    char buf3[30];
	EdpPacket* send_pkg;
    send_pkg = PacketSavedataInt(kTypeSimpleJsonWithoutTime, NULL, "test", (uint32_t) Conce_PM2_5, 0, NULL);
    sprintf(buf3,"AT+CIPSEND=%d",send_pkg->_write_pos );
   sendflag=SendCmd(buf3,">",1000);
    DoSend(0, (const uint8_t *)send_pkg->_data,send_pkg->_write_pos);	
	delay_ms(50);
    DeleteBuffer(&send_pkg);
	USART3_RX_STA=0;
	DS1_ON;

}

