/*
 * RFID_MODULE_TEST.c
 *
 * Created: 2020-07-10 오전 10:53:34
 * Author : 최희우
 */ 
/**/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "spi_lib.h"
#include "uart_lib.h"
#include "rc522.h"
#include <avr/interrupt.h>
#include <util/delay.h>
 
 #define ASCII_TYPE 0
 #define DECIMAL_TYPE 1 
 #define HEXDECIMAL_TYPE 2 
 
 
  unsigned char byte;
  uint8_t str[MAX_LEN];
 void mfrc_print_serial(int _type);

int main()
{

	//PORTB=0x00;
	//DDRB=0x07;
	
	
	sei();
	
	spi_init(_SPI_MASTER_MODE,_SPI_CLK_PRESC_16,_SPI_CLK_LO_LEADING);
	spi_master_tx(0x67);
	
	mfrc522_init(); //아직 덜 구현
	
	uart_init(0,9600);
	
	//uart0_tx_string_IT("RFID module TEST ...\n");
	uart0_tx_string("RFID module TEST ...\n");
	
	_delay_ms(1000);
	
	//version check function
	mfrc522_version_check();
	
	
	//비트마스킹
	byte=mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg,byte|0x20);
	byte=mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg,byte|0x80);
	
	
	
	_delay_ms(1500);
	while (1)
	{
		// Place your code here
		
		//spi_master_tx(0x6b);
		byte = mfrc522_request(PICC_REQALL,str);//
		//uart0_tx_string_IT(IntToString(byte));
		_delay_ms(20);
		if(byte==CARD_FOUND){
			byte=mfrc522_get_card_serial(str);
			if(byte==CARD_FOUND){
				
				//uart0_tx_string_IT("\nuid: ");
				uart0_tx_string("\n[charck uid]: ");
				//_delay_ms(20);	
				mfrc_print_serial(ASCII_TYPE);
				mfrc_print_serial(DECIMAL_TYPE);
				mfrc_print_serial(HEXDECIMAL_TYPE);						
			}
			else {
				uart0_tx_string("error\n");
				//uart0_tx_string_IT("error\n");	
			}
		}
		
		_delay_ms(500);
		//uart0_tx_string_IT("\nloop\n");	
				_delay_ms(500);
		
	}
}




void mfrc_print_serial(int _type)
{
	switch(_type)
	{
		case ASCII_TYPE:
		
		uart0_tx_string("\n    ascii: ");
		for(int i=0;i<4;i++){
			//uart0_tx_string_IT(IntToString(str[i]));
			uart0_tx_char(str[i]);
			//_delay_ms(10);
		}
		
		break;
		case DECIMAL_TYPE:
		
		uart0_tx_string("\n    dec: ");
		for(int i=0;i<4;i++){
			//uart0_tx_string_IT(IntToString(str[i]));
			//uart0_tx_char(str[i]);
			uart0_tx_string(IntToString(str[i]));
			//_delay_ms(10);
		}
		
		
		break;
		case HEXDECIMAL_TYPE:
		
		uart0_tx_string("\n    hex: ");
		for(int i=0;i<4;i++){
			//uart0_tx_string_IT(IntToString(str[i]));
			//uart0_tx_char(str[i]);
			uart0_tx_string(HexToString(str[i]));
			//_delay_ms(10);
		}
		
		break;
		
	}
	
}