/*
 * uart.h
 *
 *  Created on: 27 gru 2017
 *      Author: Milo
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#define UART_BAUD 9600		// tu definiujemy interesuj¹c¹ nas prêdkoœæ
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1) // obliczamy UBRR dla U2X=0

#define UART_DE_ODBIERANIE  UART_DE_PORT |= UART_DE_BIT
#define UART_DE_NADAWANIE  UART_DE_PORT &= ~UART_DE_BIT

#define UART_RX_BUF_SIZE 256 // definiujemy bufor o rozmiarze 32 bajtów
// definiujemy maskê dla naszego bufora
#define UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1)

//DIODA
#define LED_PIN (1<<PD2) // definicja pinu do ktorego podlaczona jest dioda
#define LED_ON PORTD |= LED_PIN
#define LED_OFF PORTD &= ~LED_PIN

// deklaracje funkcji publicznych

void USART_Init( uint16_t baud );
char uart_getc(void);

#endif /* UART_UART_H_ */
