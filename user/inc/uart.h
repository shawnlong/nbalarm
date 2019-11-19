/*uart.h*/
#ifndef _UART_H_
#define _UART_H_
typedef enum {UART_SEND_MODE_CHAR = 0, UART_SEND_MODE_HEX = 1} UART_Send_Mode_Typedef;
uint8_t uart_init(uint8_t id, uint16_t baudrate);
uint8_t uart_send(char *data, uint8_t len, UART_Send_Mode_Typedef mode);
uint8_t uart_getchar(char *data);
#define UART1_RX_BUFFER_SIZE 32


#endif


