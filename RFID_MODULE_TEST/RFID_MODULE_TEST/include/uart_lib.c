﻿/*
 * uart_lib.c
 *
 * Created: 2020-07-16 오전 2:27:39
 *  Author: 최희우
 
 최종 수정일 : 2020 08 04 : uart0, 1 사용 확장
 */ 

//#include <avr/io.h>
#include "uart_lib.h"



ISR(USART0_TX_vect)
{//전송시 딜레이 여유가 있어야함.. 안그러면 다른 구문으로 넘어가버리네 안좋은 함수같음.
	
	_uart_tx_string_in_isr();
}

//ISR(USART0_TX_vect)에 넣어주는 함수.
void _uart_tx_string_in_isr()
{
	if(uart0.tx0Buf[uart0.tx0Cnt]=='\0') //문자열 마지막부분은 보내지 않는 것이 특징.
	{
		UCSR0B &= ~0x40; //Tx 송신완료 interrupt disabled.
		memset((char*)uart0.tx0Buf,0,sizeof(uart0.tx0Buf));
	}
	else UDR0 = uart0.tx0Buf[uart0.tx0Cnt++]; // 0은 이미 보냈고 1이 보내지고 2로 증가, 2보내지고 3으로 증가되고 다음 루틴에서 인터럽트 비활성화
}


void uart_init(int select_ch,unsigned int ubrr)
{
	unsigned int buff_baud= MYUBRR(ubrr);
	if(select_ch==0){
		DDRE &= ~(0x01);
		//set Baud Rate
		UBRR0H= (unsigned char )(buff_baud>>8);
		UBRR0L= (unsigned char )(buff_baud&0xff);
	
		// Tx, Rx Enabled
		UCSR0B = ((1<<TXEN0));// | (1<<RXEN0)
		//UCSR0B |= ((1<<RXCIE0)); //수신 완료 인터럽트 허용
	
		// Set Frame format: data 8 bit, 1 stop bit
		UCSR0C = (0<<USBS0) |(1<<UCSZ01)| (1<<UCSZ00 );
	}else if(select_ch==1){
		//set Baud Rate
		UBRR1H= (unsigned char )(buff_baud>>8);
		UBRR1L= (unsigned char )(buff_baud&0xff);
		
		// Tx, Rx Enabled
		UCSR1B = ((1<<TXEN1));// | (1<<RXEN0)
		//UCSR0B |= ((1<<RXCIE0)); //수신 완료 인터럽트 허용
		
		// Set Frame format: data 8 bit, 1 stop bit
		UCSR1C = (0<<USBS1) |(1<<UCSZ11)| (1<<UCSZ10 );
	}
}

void uart0_tx_char(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}
void uart0_tx_string(char * data)
{
	while(*data) uart0_tx_char(*data++);
}


void uart1_tx_char(unsigned char data)
{
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1=data;
}
void uart1_tx_string(char * data)
{
	while(*data) uart1_tx_char(*data++);
}



void uart0_tx_string_IT(char * data)
{
	
	int _len=strlen(data);
	strncpy((char*)uart0.tx0Buf,data,_len);
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=uart0.tx0Buf[0];
	
	uart0.tx0Cnt=1;
	uart0.tx0CntMax=_len+1;
	UCSR0B |= (1<<TXCIE0);
	
}

void uart1_tx_string_IT(char * data)
{
	
	int _len=strlen(data);
	strncpy((char*)uart1.tx1Buf,data,_len);
	
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1=uart1.tx1Buf[0];
	
	uart1.tx1Cnt=1;
	uart1.tx1CntMax=_len+1;
	UCSR1B |= (1<<TXCIE1);
	
}

char * IntToString(int num)
{
	/** Dummy Function*/
	char buf[20];
	sprintf(buf,"%d,",num); //고유 자릿값
	
	strcpy(buff.itos_buff,buf);
	
	return (buff.itos_buff);
}
 
 char * HexToString(char num)
 {
	 /** Dummy Function*/
	 char buf[20];
	 sprintf(buf,"0x%x,",num); //고유 자릿값
	 
	 strcpy(buff.itos_buff,buf);
	 
	 return (buff.itos_buff);
 }
 
 
 


