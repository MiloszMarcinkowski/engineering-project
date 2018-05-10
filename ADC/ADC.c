/*
 * ADC.c
 *
 *  Created on: 11 sty 2018
 *      Author: Milo
 */
#include <avr/io.h>
#include <util/delay.h>

#include "ADC.h"

uint8_t cz_d; //czesc dziesietna wyniku
uint8_t cz_u; //czesc ulamokowa wyniku

uint16_t pomiar4(void)
   {
	static uint16_t sr0[32]; //tablica do przechowywania ostanich wartosci
	static uint8_t idx0; //indeks chodzacy po tablicy
	uint32_t sr=0; //srednia
	uint8_t i;

   	ADMUX = (ADMUX & 0xF8) | 4; //maskowanie nie uzywanych bitow i wlaczenie kanalu do pomiaru
   	ADCSRA |= (1<<ADSC); //start pomiaru
   	while(ADCSRA & (1<<ADSC)); //petla czekajaca na koniec pomiaru
   	sr0[idx0] = ADCW;
   	idx0++;
   	if(idx0==32)idx0=0;

   	for(i=0 ; i<32 ; i++) sr += sr0[i];
   	sr/=32;
   	return sr; //makro zwraca rejestr z wynikiem jako zmienna 16 bitow
   }
uint16_t pomiar6(void)
   {
	static uint16_t sr0[32]; //tablica do przechowywania ostanich wartosci
	static uint8_t idx0; //indeks chodzacy po tablicy
	uint32_t sr=0; //srednia
	uint8_t i;

   	ADMUX = (ADMUX & 0xF8) | 6; //maskowanie nie uzywanych bitow i wlaczenie kanalu do pomiaru
   	ADCSRA |= (1<<ADSC); //start pomiaru
   	while(ADCSRA & (1<<ADSC)); //petla czekajaca na koniec pomiaru
   	sr0[idx0] = ADCW;
   	idx0++;
   	if(idx0==32)idx0=0;

   	for(i=0 ; i<32 ; i++) sr += sr0[i];
   	sr/=32;
   	return sr; //makro zwraca rejestr z wynikiem jako zmienna 16 bitow
   }
int16_t adc_to_temp(uint16_t sr)
{
	    uint16_t wynik_volt; //odczyt w voltach
	   	int16_t wynik_temp;
	    //uint8_t cz_d; //czesc dziesietna wyniku
	    //uint8_t cz_u; //czesc ulamokowa wyniku

		wynik_volt = sr * 26UL; //wedlug wzoru: Vin = ADC*26(10000*(2,56)/1024)*wspolczynnik dzielnika(0)
	    wynik_temp = wynik_volt/100 - 63; //wzor temp=(V*10000-0,5*10000)/0,01*100 (wynik mnozoony *100
	  //  if(wynik_temp<0) {
	    //	wynik_temp = wynik_temp*(-1);
	    //}
	    return wynik_temp;
}
void adc_to_volt(uint16_t sr)
{
	    uint16_t wynik_volt; //odczyt w voltach
	   	//int16_t wynik_temp;


		wynik_volt = sr * 37UL; //wedlug wzoru: Vin = ADC*26(10000*(2,56)/1024)*wspolczynnik dzielnika(0)
	    //wynik_temp = wynik_volt/100 - 63; //wzor temp=(V*10000-0,5*10000)/0,01*100 (wynik mnozoony *100

	    //return wynik_temp;
		cz_d = wynik_volt/10000; //wynik czesci dziesietnej
	    cz_u = wynik_volt/100 % 100; //wynik czesci ulamkowej

}
