

/*gpio.h*/
#ifndef _GPIO_H_
#define _GPIO_H_
#if 0
#define GPIO_PIN_1				0xA3
#define GPIO_PIN_2				0xB4
#define GPIO_PIN_3				0xC3
#define GPIO_PIN_4				0xC5
#define GPIO_PIN_5				0xC6
#define GPIO_PIN_6				0xC7
#define GPIO_PIN_7				0xD2
#define GPIO_PIN_8				0xD4

#define GPIO_MODE_INPUT 		0x01
#define GPIO_MODE_PULL_UP		0x02
#define GPIO_MODE_FLOAT 		0x04
#define GPIO_MODE_INT			0x06//interrupt
#define GPIO_MODE_OUTPUT		0x08
#define GPIO_MODE_OD			0x10//open drain
#define GPIO_MODE_PP			0x20//pull-push
#define GPIO_MODE_FAST			0x40//high output speed

#define GPIO_VALUE_LOW			0x00
#define GPIO_VALUE_HIGH 		0x01
#define GPIO_VALUE_INVALID		0x02
#endif

uint8_t gpio_init(uint8_t port, uint8_t mode);
uint8_t gpio_close(uint8_t port);
uint8_t gpio_close_all();
uint8_t gpio_get_value(uint8_t pin);
uint8_t gpio_set_value(uint8_t pin, uint8_t value);

#endif

