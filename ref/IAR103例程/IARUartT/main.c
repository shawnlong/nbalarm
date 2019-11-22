#include"iostm8s103F3.h"

#define MAX_AT_RESPONSE  50
#define RX_BUFFER_SIZE 100
unsigned char rcv;
unsigned char rx_buffer[RX_BUFFER_SIZE];
unsigned char rx_wr_index = 0, rx_rd_index = 0, rx_counter;

_Bool rx_buffer_overflow;


void Init_UART1(void)
{
      UART1_CR1=0x00;
      UART1_CR2=0x00;
      UART1_CR3=0x00;
      // 设置波特率，必须注意以下几点：
      // (1) 必须先写BRR2
      // (2) BRR1存放的是分频系数的第11位到第4位，
      // (3) BRR2存放的是分频系数的第15位到第12位，和第3位
      // 到第0位
      // 例如对于波特率位9600时，分频系数=2000000/9600=208
      // 对应的十六进制数为00D0，BBR1=0D,BBR2=00

      UART1_BRR2=0x03;
      UART1_BRR1=0x02;

      UART1_CR2=0x2c;//允许接收，发送，开接收中断
      asm("rim");
}


void UART1_sendchar(unsigned char c)
{
      while((UART1_SR & 0x80)==0x00);
      UART1_DR=c;
}

void UART1_sendstring(unsigned char *string)
{
  while(*string != '\0'){
    UART1_sendchar(*string);
    string++;
  }
}

#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_Recv_IRQHandler (void)
{
  if (UART1_SR & 0X0F)
	{
		rcv = UART1_SR;
		rcv = UART1_DR;	//读取数据
	}
    else
    {
        rcv = UART1_DR;	//读取数据
    }	
        if(rcv != 0x0d && rcv != 0x0a)
        {
	
           rx_buffer[rx_wr_index++] = rcv;	//把数据写入接收缓冲区 接收写指针++
      
                  if (rx_wr_index == RX_BUFFER_SIZE)  rx_wr_index = 0;	//如果写入指针等于 缓冲区大小 写入指针指向第一位	
                  if (++rx_counter == RX_BUFFER_SIZE)
                  {
                          rx_counter = 0;
                          rx_buffer_overflow = 1;
                  }
        }
}

char getchar(void)
{
	char data;
	while (rx_counter == 0);    //如果接收数据个数为0 则等待
	data = rx_buffer[rx_rd_index++];
	
	#if RX_BUFFER_SIZE != 256
		if (rx_rd_index == RX_BUFFER_SIZE)  rx_rd_index = 0;
	#endif		
		
	asm("sim"); 	//关总中断
	--rx_counter;
	
	asm("rim");	//开总中断
	return data;
}
 
/***********************************************************
*	函数名称：int getStr(char *Buf,int count)
*
*	功能描述：从接收缓冲区获取一帧数据
*
*	参数列表：*Buf  存储收到的数据 数组
*			  count 要获取的数据长度
*
*	返回结果：返回获取到的数据长度
***********************************************************/
int getStr(char *Buf, int count)
{
	int i = 0;
	for (i=0; i<count; i++)
	{ 
		Buf[i] = getchar();
                  
	}    
		
	Buf[i] = '\0';
	return i;
}    


/*
  function:
        send at command to DTU and get responsse
  param:
        command:a string contains the at command 
  return:
        a string contains the repsonse from DTU
*/
unsigned char *DTU_AT(unsigned char *command, int responsesize)
{
  static unsigned char response[MAX_AT_RESPONSE];

  UART1_sendstring(command);
   getStr(response, responsesize);
   return response;
}

/*
  function:
        check if the DTU(M5313) is working OK
  param:
        timeout:seconds to wait, 0 for wait infinitely
  return:
        1 - DTU works OK
        0 - timeout
*/
unsigned char checkDTU(unsigned int timeout)
{
  unsigned char *response;
  int seconds = timeout;
  //disable the echo mode
  //DTU_AT("ATE0\r\n",0);
  while(seconds != 0 || timeout == 0)
  {
    response = DTU_AT("AT\r\n",2);
    //should receive "OK" from DTU
    if(response[0] == 'O' && response[1] == 'K')
      return 1;
      seconds --;
  }
  
  return 0;
}

main()
{
        //1.Initialize the BSP
	Init_UART1();
        //2.Check the DTU works OK
        checkDTU(0);
        //3.Detect chanels
       
        //4.Send alarm message
        
        //5.goto sleep/shut down
        
        while(1);
        //6. Never goes here
}
