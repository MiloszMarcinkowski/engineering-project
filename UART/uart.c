/*
 * uart.c
 *
 *  Created on: 27 gru 2017
 *      Author: Milo
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"

volatile uint8_t flaga;

// definiujemy w ko�cu nasz bufor UART_RxBuf
volatile char UART_RxBuf[UART_RX_BUF_SIZE];
// definiujemy indeksy okre�laj�ce ilo�� danych w buforze
volatile uint8_t UART_RxHead; // indeks oznaczaj�cy �g�ow� w�a�
volatile uint8_t UART_RxTail; // indeks oznaczaj�cy �ogon w�a�

void USART_Init( unsigned int baud ) {
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	UCSRB = (1<<RXEN)|(1<<RXCIE); //RXEN- start odbiornika, RXCIE- uruchomienie przerwania Rx
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); //8bit�w danych, 1 bit stopu
}
char uart_getc(void) {
    // sprawdzamy czy indeksy s� r�wne
    if ( UART_RxHead == UART_RxTail ) return 0;

    // obliczamy i zapami�tujemy nowy indeks �ogona w�a� (mo�e si� zr�wna� z g�ow�)
    UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji

    return UART_RxBuf[UART_RxTail];
}

// definiujemy procedur� obs�ugi przerwania odbiorczego, zapisuj�c� dane do bufora cyklicznego
ISR( USART_RXC_vect ) {

	extern volatile uint8_t flaga;
    uint8_t tmp_head;
    char data;

    data = UDR; //pobieramy natychmiast bajt danych z bufora sprz�towego

    if(data == 0x0a)
    {
    	flaga++;
    }
    // obliczamy nowy indeks �g�owy w�a�
    tmp_head = ( UART_RxHead + 1) & UART_RX_BUF_MASK;
    // sprawdzamy, czy w�� nie zacznie zjada� w�asnego ogona
    if ( tmp_head == UART_RxTail ) {
		//LED_ON; //przepelniony bufor
    } else {
	UART_RxHead = tmp_head;
	UART_RxBuf[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
    }
}

