/*
 * main.c
 *
 *  Created on: 15 gru 2017
 *      Author: Milo
 */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "SH1106/sh1106.h"
#include "UART/uart.h"
#include "ADC/ADC.h"

#define BUT1 (1<<PC4)
#define BUT2 (1<<PC5)
#define BUT1_DOWN !(PINC & BUT1)
#define BUT2_DOWN !(PINC & BUT2)

/******************deklaracja funkcji**********************/
void ramki_gps(void);
void show_status(void);
//void show_time(void);
//void show_date(void);
//void show_speed(void);
void GPRMC(void);
void GPGGA(void);
void GPVTG(void);
void big_time(void);
void big_speed(void);
void battery(void);
void big_temperature(void);
void big_altitde(void);
void big_long_lat(void);
void big_date(void);
/******************zmienne**********************/
extern volatile uint8_t flaga;
volatile uint16_t bufor_sei;
volatile uint8_t flaga_przerwania;

char data;
uint8_t tmp;
uint8_t comma_cnt;
char header[6]; //GP***
char time[10]; //hhmmss.00
char status; //A-active, V-not active
char latitude[11];
char north_south; //N-north S-south
char longitude[12];
char east_west; //E-east W-West
char speed[6]; //knots
char date[7]; // ddmmyy
char bufor[82]; //maks 82 znaki? 80?
char sat_num[3]; //
char altitude[6]; // xxx.x wysokosc nad poziomem morza
char speed_kmh[6]; // xxx.x predkosc km/h
//adc
char ADC_string[4]; //maks 3 cyfry?
char cz_d_string[3];
char cz_u_string[3];
int16_t temperatura;
extern uint8_t cz_d; //czesc dziesietna wyniku
extern uint8_t cz_u; //czesc ulamokowa wyniku

int main(void)
{
	//ADC
    ADCSRA |= (1<<ADEN); //wlaczenie ADC
    ADCSRA |= (1<<ADPS0) | (1<<ADPS1); //preskaler ADC na 64
    ADMUX |= (1<<REFS1) | (1<<REFS0); //wew. zr. odenisienia Vcc=2.56
    //DIODA
    DDRD |= LED_PIN; //stan wyjsciowy, podciagniecie portu pod vcc
    LED_ON;
    _delay_ms(10);
    LED_OFF;
    //PRZYCISKI
    DDRC &= ~BUT1;
    DDRC &=	~BUT2;
    PORTC |= BUT1;
    PORTC |= BUT2;
    //OLED
	sh1106_begin();
	sh1106_clear(oled_buf);
	//TIMER
    TCCR0 |=(1<<WGM01); //TRYB CTC
    TCCR0 |=(1<<CS02)|(1<<CS00); //PRESKALER 1024
    OCR0 = 72; //podzial przez 73? 100hz
    TIMSK |=(1<<OCIE0); //compare match enable
	//UART
	USART_Init( __UBRR ); //inicjalizacja UART
	sei(); //odblokowanie przerwan

	while(1)
	{
		/****************************** SPEED ******************************/
		big_speed();
		speed:
		if(BUT2_DOWN) _delay_ms(50);{
		if(BUT2_DOWN) goto date;
		}
		if(BUT1_DOWN)_delay_ms(50);{
		if(BUT1_DOWN){
			while(1){
				/****************************** GEOGRAPHIC ******************************/
				big_long_lat();
				geo:
				if(BUT2_DOWN) _delay_ms(50);{
				if(BUT2_DOWN) goto speed;
				}
				if(BUT1_DOWN)_delay_ms(50);{
				if(BUT1_DOWN){
					while(1){
						/****************************** WYSOKOSC ******************************/
						big_altitde();
						altitude:
						if(BUT2_DOWN) _delay_ms(80);{
						if(BUT2_DOWN) goto geo;
						}
						if(BUT1_DOWN)_delay_ms(80);{
						if(BUT1_DOWN){
							while(1){
								/****************************** TEMP ******************************/
								big_temperature();
								temp:
								if(BUT2_DOWN) _delay_ms(80);{
								if(BUT2_DOWN) goto altitude;
								}
								if(BUT1_DOWN)_delay_ms(80);{
								if(BUT1_DOWN){
									while(1){
										/******************************	TIME ******************************/
										big_time();
										time:
										if(BUT2_DOWN) _delay_ms(80);{
										if(BUT2_DOWN) goto temp;
										}
										if(BUT1_DOWN)_delay_ms(80);{
										if(BUT1_DOWN){
											while(1){
												/******************************	DATE ******************************/
												big_date();
												date:
												if(BUT2_DOWN) _delay_ms(80);{
												if(BUT2_DOWN) goto time;
												}
												if(BUT1_DOWN)_delay_ms(80);{
												if(BUT1_DOWN) goto speed;
												}
											}
										}
										}
									}
								}
								}
							}
						}
						}
					}
				}
			}
			}
		}
		}
		//ramki_gps();
		//SH1106_string(112, 16, sat_num , 16, 1, oled_buf);
		//SH1106_string(88 , 32, altitude , 16, 1, oled_buf); //wysokosc
		//SH1106_string(88 , 48, speed_kmh , 16, 1, oled_buf); //km/h
		//show_status();
		//battery();
		//temperature();
		//show_date();
		//show_time();
		//show_speed();
		//big_speed();
		//big_temperature();
		//big_altitde();
		//big_time();
		//big_long_lat();
		//big_date();
		//display_ada();
		//sh1106_clear(oled_buf);

	}//while
}//main
void battery(void) {
	if(flaga_przerwania == 1){ //10 sekund
	sprintf(cz_d_string, "%d", cz_d);
	sprintf(cz_u_string, "%d", cz_u);
	flaga_przerwania = 0;
	}
	adc_to_volt((pomiar6()));
	SH1106_string(80 , 00, "}" , 16, 1, oled_buf); //ZNAK BATERII
	SH1106_string(88, 0, cz_d_string , 16, 1, oled_buf);
	SH1106_string(96, 0, "," , 16, 1, oled_buf);
	SH1106_string(104, 0, cz_u_string , 16, 1, oled_buf);
	SH1106_string(120, 0, "V" , 16, 1, oled_buf);
}
void show_status(void){
	if(status == 'A'){
		SH1106_string(0, 0, sat_num , 16, 1, oled_buf);
		SH1106_string(24, 0, "GPS OK" , 16, 1, oled_buf);
	}
	if(status == 'V') {
		SH1106_string(0, 0, sat_num , 16, 1, oled_buf);
		SH1106_string(24, 0, "no GPS" , 16, 1, oled_buf);
	}
}
void big_time(void){
	ramki_gps();
	battery();
	show_status();
	SH1106_char3216(0, 20, time[0], oled_buf);
	SH1106_char3216(16, 20, time[1], oled_buf);
	SH1106_char3216(32, 20, ':', oled_buf);
	SH1106_char3216(48, 20, time[2], oled_buf);
	SH1106_char3216(64, 20, time[3], oled_buf);
	SH1106_char3216(80, 20, ':', oled_buf);
	SH1106_char3216(96, 20, time[4], oled_buf);
	SH1106_char3216(112, 20, time[5], oled_buf);
	SH1106_string(48, 52, "TIME" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void big_speed(void){
	ramki_gps();
	battery();
	show_status();
	SH1106_char3216(8, 20, speed_kmh[0], oled_buf);
	SH1106_char3216(24, 20, speed_kmh[1], oled_buf);
	SH1106_char3216(40, 20, speed_kmh[2], oled_buf);
	SH1106_char3216(56, 20, speed_kmh[3], oled_buf);
	SH1106_char3216(72, 20, speed_kmh[4], oled_buf);
	SH1106_string(88, 36, "KM/H" , 16, 1, oled_buf);
	SH1106_string(34, 52, "SPEED" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void big_temperature(void){
	ramki_gps();
	battery();
	show_status();
	if(flaga_przerwania == 1){ //10 sekund
	sprintf(ADC_string, "%d", adc_to_temp(temperatura));
	flaga_przerwania = 0;
	}
	temperatura = pomiar4();
	SH1106_char3216(48, 20, ADC_string[0], oled_buf);
	SH1106_char3216(64, 20, ADC_string[1], oled_buf);
	//SH1106_char3216(64, 20, ADC_string[2], oled_buf);
	SH1106_string(80, 36, "~C" , 16, 1, oled_buf);
	SH1106_string(31, 52, "TEMPERATURE" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void big_altitde(void){
	ramki_gps();
	battery();
	show_status();
	SH1106_char3216(4, 20, altitude[0], oled_buf);
	SH1106_char3216(20, 20, altitude[1], oled_buf);
	SH1106_char3216(36, 20, altitude[2], oled_buf);
	SH1106_char3216(52, 20, altitude[3], oled_buf);
	SH1106_char3216(68, 20, altitude[4], oled_buf);
	SH1106_string(84, 36, "m npm" , 16, 1, oled_buf);
	SH1106_string(40, 52, "ALTITUDE" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void big_long_lat(void){
	ramki_gps();
	battery();
	show_status();
	SH1106_char(20, 20, latitude[0] , 16, 1, oled_buf);
	SH1106_char(28, 20, latitude[1] , 16, 1, oled_buf);
	SH1106_string(36, 20, "~" , 16, 1, oled_buf);
	SH1106_char(44, 20, latitude[2] , 16, 1, oled_buf);
	SH1106_char(52, 20, latitude[3] , 16, 1, oled_buf);
	SH1106_string(60, 20, "'" , 16, 1, oled_buf);
	SH1106_char(68, 20, latitude[5] , 16, 1, oled_buf);
	SH1106_char(76, 20, latitude[6] , 16, 1, oled_buf);
	SH1106_char(84, 20, latitude[7] , 16, 1, oled_buf);

	SH1106_string(92, 20, "''" , 16, 1, oled_buf);
	SH1106_char(108, 20, north_south , 16, 1, oled_buf);

	SH1106_char(20, 36, longitude[1] , 16, 1, oled_buf);
	SH1106_char(28, 36, longitude[2] , 16, 1, oled_buf);
	SH1106_string(36, 36, "~" , 16, 1, oled_buf);
	SH1106_char(44, 36, longitude[3] , 16, 1, oled_buf);
	SH1106_char(52, 36, longitude[4] , 16, 1, oled_buf);
	SH1106_string(60, 36, "'" , 16, 1, oled_buf);
	SH1106_char(68, 36, longitude[6] , 16, 1, oled_buf);
	SH1106_char(76, 36, longitude[7] , 16, 1, oled_buf);
	SH1106_char(84, 36, longitude[8] , 16, 1, oled_buf);

	SH1106_string(92, 36, "''" , 16, 1, oled_buf);
	SH1106_char(108, 36, east_west , 16, 1, oled_buf);

	SH1106_string(1, 52, "GEOGRAPGIC COORDINATE" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void big_date(void){
	ramki_gps();
	battery();
	show_status();
	SH1106_char3216(8, 20, date[0] ,oled_buf);
	SH1106_char3216(24, 20, date[1] ,oled_buf);
	SH1106_char(40, 36, '/' ,16, 1, oled_buf);
	SH1106_char3216(48, 20, date[2] ,oled_buf);
	SH1106_char3216(64, 20, date[3] ,oled_buf);
	SH1106_char(80, 36, '/' ,16, 1, oled_buf);
	SH1106_char3216(88, 20, date[4] ,oled_buf);
	SH1106_char3216(104, 20, date[5] ,oled_buf);

	SH1106_string(48, 52, "DATE" , 12, 1, oled_buf);
	display_ada();
	sh1106_clear(oled_buf);
}
void show_time(void){
	if(time[5]%2) SH1106_char(12, 52, ':' , 12, 1, oled_buf);
	SH1106_char(0, 52, time[0] , 12, 1, oled_buf);
	SH1106_char(6, 52, time[1] , 12, 1, oled_buf);
	SH1106_char(18, 52, time[2] , 12, 1, oled_buf);
	SH1106_char(24, 52, time[3] , 12, 1, oled_buf);
}
void show_date(void){
	SH1106_char(36, 52, date[0] , 12, 1, oled_buf);
	SH1106_char(42, 52, date[1] , 12, 1, oled_buf);
	SH1106_char(48, 52, '/' , 12, 1, oled_buf);
	SH1106_char(54, 52, date[2] , 12, 1, oled_buf);
	SH1106_char(60, 52, date[3] , 12, 1, oled_buf);
	SH1106_char(66, 52, '/' , 12, 1, oled_buf);
	SH1106_char(72, 52, date[4] , 12, 1, oled_buf);
	SH1106_char(78, 52, date[5] , 12, 1, oled_buf);
}
void show_speed(void){
	SH1106_string(0, 36, speed , 12, 1, oled_buf);
	SH1106_string(30, 36, "knots" , 12, 1, oled_buf);
}
void ramki_gps(void) {
	if(flaga) {
		data = uart_getc();
		/****************************** HEADER NMEA ******************************/
		if(data == 0x24) {
			while(1) {
				data = uart_getc();
				if(data == ',') break;
				header[tmp++] = data;
			}
			header[tmp++] = '\0';
			tmp = 0;
			/****************************** GPRMC ******************************/
			if((strcmp(header,"GPRMC")) == 0) {
				GPRMC();
			}
			/****************************** GPGGA ******************************/
			if((strcmp(header,"GPGGA")) == 0){
				GPGGA();
			}
			/****************************** GPVTG ******************************/
			if((strcmp(header,"GPVTG")) == 0){
				GPVTG();
			}
		} //if $
		/****************************** FRAME END ******************************/
		while(1) {
			data = uart_getc();
			if (data == 0x0a) {
				flaga--;
				break;
			}
		}
	}//if flaga
}//ramki_gps
void GPRMC(void) {
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		time[tmp++] = data;
	}
	time[tmp++] = '\0';
	tmp = 0;
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		status = data;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		latitude[tmp++] = data;
	}
	latitude[tmp++] = '\0';
	tmp = 0;
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		north_south = data;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		longitude[tmp++] = data;
	}
	longitude[tmp++] = '\0';
	tmp = 0;
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		east_west = data;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		speed[tmp++] = data;
	}
	speed[tmp++] = '\0';
	tmp = 0;
	while(1) {
		data = uart_getc();
		if(data == ',') break;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		date[tmp++] = data;
	}
	date[tmp++] = '\0';
	tmp = 0;
}
void GPGGA(void) {
	while(1) {
		data = uart_getc();
		if(data == ',') {
			comma_cnt++;
		}
		if(comma_cnt == 6) break;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		sat_num[tmp++] = data;
	}
	sat_num[tmp++] = '\0';
	tmp = comma_cnt = 0;
	while(1) {
		data = uart_getc();
		if(data == ',') break;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		altitude[tmp++] = data;
	}
	//altitude[tmp++] = '\0';
	tmp = 0;
}
void GPVTG(void) {
	while(1) {
		data = uart_getc();
		if(data == ',') {
			comma_cnt++;
		}
		if(comma_cnt == 6) break;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		speed_kmh[tmp++] = data;
	}
	//speed_kmh[tmp++] = '\0';
	tmp = comma_cnt = 0;
	/*
	while(1) {
		data = uart_getc();
		if(data == ',') break;
	}
	while(1) {
		data = uart_getc();
		if(data == ',') break;
		altitude[tmp++] = data;
	}
	altitude[tmp++] = '\0';
	tmp = 0;
	*/
}
ISR(TIMER0_COMP_vect)
{
	if(bufor_sei++ == 300) //10 sekund
	{
		flaga_przerwania = 1;
		bufor_sei = 0;
	}
}
/*
			test = uart_getc();
			SH1106_char(0, 0, test , 12, 1, oled_buf);
			//SH1106_char(12, 0, &test , 12, 1, oled_buf);

			SH1106_char(0, 32, test , 12, 1, oled_buf);
			display_ada();
*/
			//uart_putc(test);
			/*
			LED_ON;
			_delay_ms(10);
			LED_OFF;
			_delay_ms(1000);
			test = uart_getc();

			uart_putc(test);	// wyœlij tekst
			uart_putc('\r');			// wyœlij znak CR (enter)
			uart_putc('\n');			// wyœlij znak LF (nowa linia)
			*/
			/*uart_putint(licznik, 10);	// wyœlij liczbê
			uart_putc('\r');			// wyœlij znak CR (enter)
			uart_putc('\n');			// wyœlij znak LF (nowa linia)
			_delay_ms(500);				// odczekaj 0,5 sekundy
			OSCCAL = licznik++;			// zwiêksz wartoœæ bajtu kalibracyjnego o 1

			// badamy kalibracjê tylko w granicach +- 20 w stosunku do
			// tej jaka by³a ustawiona fabrycznie
			if(licznik > pm+40) licznik=pm;
	*/

/*
	//uint8_t i;
	//for(i=0; i<128; i++) sh1106_pixel_mirek(i, 3, 1, oled_buf);
	//bitmap(0, 0, bitmap_1, 128, 64, oled_buf);

		SH1106_char3216(0, 16, '9', oled_buf);

		LED_ON;
		_delay_ms(1000);
		LED_OFF;
		_delay_ms(1000);
*/
