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
      // ���ò����ʣ�����ע�����¼��㣺
      // (1) ������дBRR2
      // (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
      // (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
      // ����0λ
      // ������ڲ�����λ9600ʱ����Ƶϵ��=2000000/9600=208
      // ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00

      UART1_BRR2=0x03;
      UART1_BRR1=0x02;

      UART1_CR2=0x2c;//������գ����ͣ��������ж�
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
		rcv = UART1_DR;	//��ȡ����
	}
    else
    {
        rcv = UART1_DR;	//��ȡ����
    }	
        if(rcv != 0x0d && rcv != 0x0a)
        {
	
           rx_buffer[rx_wr_index++] = rcv;	//������д����ջ����� ����дָ��++
      
                  if (rx_wr_index == RX_BUFFER_SIZE)  rx_wr_index = 0;	//���д��ָ����� ��������С д��ָ��ָ���һλ	
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
	while (rx_counter == 0);    //����������ݸ���Ϊ0 ��ȴ�
	data = rx_buffer[rx_rd_index++];
	
	#if RX_BUFFER_SIZE != 256
		if (rx_rd_index == RX_BUFFER_SIZE)  rx_rd_index = 0;
	#endif		
		
	asm("sim"); 	//�����ж�
	--rx_counter;
	
	asm("rim");	//�����ж�
	return data;
}
 
/***********************************************************
*	�������ƣ�int getStr(char *Buf,int count)
*
*	�����������ӽ��ջ�������ȡһ֡����
*
*	�����б�*Buf  �洢�յ������� ����
*			  count Ҫ��ȡ�����ݳ���
*
*	���ؽ�������ػ�ȡ�������ݳ���
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
