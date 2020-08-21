

/*dtu.h
*/
#ifndef _DTU_H_
#define _DTU_H_
#define AT_CMD_AT				"AT"
#define AT_CMD_CIMI				"AT+CIMI"
#define AT_CMD_ATE				"ATE"
#define AT_CMD_ATI				"ATI"
#define AT_CMD_GMI				"AT+GMI"
#define AT_CMD_GMM				"AT+GMM"
#define AT_CMD_GMR				"AT+GMR"
#define AT_CMD_GSN				"AT+GSN=1"
#define AT_CMD_CGACT			"AT+CGACT"
#define AT_CMD_CGATT			"AT+CGATT"
#define AT_CMD_HTTPCREATE		"AT+HTTPCREATE="
#define AT_CMD_HTTPHEADER		"AT+HTTPHEADER=0,"
#define AT_CMD_HTTPCONTENT		"AT+HTTPCONTENT=0,"
#define AT_CMD_HTTPSEND 		"AT+HTTPSEND=0,1,"
#define AT_CMD_HTTPCLOSE		"AT+HTTPCLOSE"
#define AT_CMD_CSQ				"AT+CSQ"
#define AT_CMD_CEREG			"AT+CEREG"
#define AT_CMD_CPIN 			"AT+CPIN?"
#define AT_CMD_PING 			"AT+CMPING="
#define AT_CMD_COPS				"AT+COPS"
#define AT_CMD_NVSETPM			"AT+NVSETPM="
#define AT_CMD_CSCLK			"AT+CSCLK="
#define AT_CMD_CPSMS			"AT+CPSMS=0"
#define AT_CMD_W				"AT&W"
#define AT_CMD_PING				"AT+CMPING="
#define AT_CMD_CFGDUALMODE		"AT+CFGDUALMODE=1,"
#define AT_CMD_CFGRATRRIO		"AT+CFGRATPRIO="
#define AT_CMD_CLOCK			"AT+CCLK?"
#define RX_BUFFER_SIZE			100
#define DTU_IMEI_LEN			15
#define ACK_OK					0
#define ACK_FAIL				1
#define ACK_UNKNOWN 			3
#define HTTP_ACK_OK 			"+HTTPNMIC:0,0,2,2\r\nOK"
#define HTTP_ACK_FAIL			"+HTTPNMIC:0,0,4,4\r\nFAIL\r\n"

#define HTTP_CONNECT_ERROR		"CONNECT ERROR"
#define DTU_ONLINE				"+CTZV:"//"OK"

#define DTU_POWER_PIN		GPIO_PIN_3
#define DTU_RESET_PIN		GPIO_PIN_7
//#define DTU_WAKEUP_PIN		GPIO_PIN_8

#define DTU_NET_TYPE_NB		1
#define DTU_NET_TYPE_GPRS	2
#define DTU_CONTENT_MAX_SIZE	127
#define DTU_CHECKREG_RETRY	15


uint8_t dtu_init();
uint8_t dtu_reset();
uint8_t dtu_close(uint8_t state);

uint8_t dtu_get_imei(uint8_t * imei);
uint8_t dtu_check_reg();
uint8_t dtu_get_ntype(uint8_t *network_type);
uint8_t	dtu_get_signal(uint8_t *signal_strength);
uint8_t dtu_add_content(uint8_t *index, uint8_t *content);
char* dtu_at_cmd(char * cmd, char *param);
uint8_t dtu_waitfor_response(uint8_t timeout_seconds, char * ok_pattern);

#define DEBUG_WO_DTU 0


#endif
