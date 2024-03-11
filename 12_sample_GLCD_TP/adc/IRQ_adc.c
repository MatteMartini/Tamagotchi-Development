/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD_advanced/GLCD.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/


int volatile volume = 0;

void ADC_IRQHandler(void) {
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){	
    volume = AD_current*4/0xFFF;
		if(volume==0) {
			LCD_DrawLine(29, 11, 29, 19, White);
	    LCD_DrawLine(30, 11, 30, 19, White);
			LCD_DrawLine(33, 9, 33, 21, White);
	    LCD_DrawLine(34, 9, 34, 21, White);
			LCD_DrawLine(37, 7, 37, 23, White);
	    LCD_DrawLine(38, 7, 38, 23, White);
			LCD_DrawLine(41, 5, 41, 25, White);
	    LCD_DrawLine(42, 5, 42, 25, White);
		}
		if(volume==1) {
			LCD_DrawLine(29, 11, 29, 19, Black);
	    LCD_DrawLine(30, 11, 30, 19, Black);
			LCD_DrawLine(33, 9, 33, 21, White);
	    LCD_DrawLine(34, 9, 34, 21, White);
			LCD_DrawLine(37, 7, 37, 23, White);
	    LCD_DrawLine(38, 7, 38, 23, White);
			LCD_DrawLine(41, 5, 41, 25, White);
	    LCD_DrawLine(42, 5, 42, 25, White);
		}
		if(volume==2) {
			LCD_DrawLine(29, 11, 29, 19, Black);
	    LCD_DrawLine(30, 11, 30, 19, Black);
			LCD_DrawLine(33, 9, 33, 21, Black);
	    LCD_DrawLine(34, 9, 34, 21, Black);
			LCD_DrawLine(37, 7, 37, 23, White);
	    LCD_DrawLine(38, 7, 38, 23, White);
			LCD_DrawLine(41, 5, 41, 25, White);
	    LCD_DrawLine(42, 5, 42, 25, White);
		}
		if(volume==3) {
			LCD_DrawLine(29, 11, 29, 19, Black);
	    LCD_DrawLine(30, 11, 30, 19, Black);
			LCD_DrawLine(33, 9, 33, 21, Black);
	    LCD_DrawLine(34, 9, 34, 21, Black);
			LCD_DrawLine(37, 7, 37, 23, Black);
	    LCD_DrawLine(38, 7, 38, 23, Black);
			LCD_DrawLine(41, 5, 41, 25, White);
	    LCD_DrawLine(42, 5, 42, 25, White);
		}
		if(volume==4) {
			LCD_DrawLine(29, 11, 29, 19, Black);
	    LCD_DrawLine(30, 11, 30, 19, Black);
			LCD_DrawLine(33, 9, 33, 21, Black);
	    LCD_DrawLine(34, 9, 34, 21, Black);
			LCD_DrawLine(37, 7, 37, 23, Black);
	    LCD_DrawLine(38, 7, 38, 23, Black);
			LCD_DrawLine(41, 5, 41, 25, Black);
	    LCD_DrawLine(42, 5, 42, 25, Black);
		}
		
		AD_last = AD_current;
  }
	
}

/*
	LCD_DrawLine(29, 11, 29, 19, Black);
	LCD_DrawLine(30, 11, 30, 19, Black);
	LCD_DrawLine(33, 9, 33, 21, Black);
	LCD_DrawLine(34, 9, 34, 21, Black);
	LCD_DrawLine(37, 7, 37, 23, Black);
	LCD_DrawLine(38, 7, 38, 23, Black);
	LCD_DrawLine(41, 5, 41, 25, Black);
	LCD_DrawLine(42, 5, 42, 25, Black);


*/
