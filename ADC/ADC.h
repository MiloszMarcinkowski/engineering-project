/*
 * ADC.h
 *
 *  Created on: 11 sty 2018
 *      Author: Milo
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

uint16_t pomiar4(void);
uint16_t pomiar6(void);
int16_t adc_to_temp(uint16_t);
void adc_to_volt(uint16_t);

#endif /* ADC_ADC_H_ */
