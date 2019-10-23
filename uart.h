/*uart.h*/
enum UART_IDS   
{ 
	UART_1 = 1
};

uint8_t uart_init(uint8_t id, uint16_t baudrate);
uint8_t uart_send(char *data, uint8_t len);
uint8_t uart_getchar(char *data);

#define UART_INTERPRUPT_MODE 0





