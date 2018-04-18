#ifndef   SIM808_H_H
#define   SIM808_H_H

#define   MAX_SEND_BUF_LEN  1024

#define   AT              "AT\r\n"	
#define   CGCLASS         "AT+CGCLASS=\"B\""
#define   CGDCONT         "AT+CGDCONT=1,\"IP\",\"CMNET\""
#define   CGATT           "AT+CGATT=1"
#define   CIPCSGP         "AT+CIPCSGP=1,\"CMNET\""
#define   CLPORT          "AT+CLPORT=\"TCP\"\,\"2000\""
#define   CIPMODE         "AT+CIPMODE=1"
#define   CIPSTART        "AT+CIPSTART=\"TCP\",\"183.230.40.39\",\"876\""

#define   DEVICEID   "29547520"
#define   APIKEY     "28QOImqxBAIP6Vx34h9WqWyrsDo="

extern char send_buf[MAX_SEND_BUF_LEN];

extern void sim_at_response(u8 mode);
extern u8 send_cmd(u8 *cmd,u8 *ack,u16 waittime);
extern u8* sim_check_cmd(u8 *str);
extern void gprs_init();
extern void SIM808_DevLink(const char* devid, const char* auth_key, int timeOut);
#endif
