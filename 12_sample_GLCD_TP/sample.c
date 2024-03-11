/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD_advanced/GLCD.h" 
#include "timer/timer.h"
//aggiunti io
#include "joystick/joystick.h"
#include "My_functions/functions.h"
#include "RIT/RIT.h"
#include "TP/TouchPanel.h"
#include "adc/adc.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();                 //INIZIALIZZA IL DISPLAY
	joystick_init();							/* Joystick Initialization  */
	TP_Init();              //INIZIALIZZA IL TOUCHSCREEN
	TouchPanel_Calibrate();
	ADC_init();
	
	//timer 3 usato per riprodurre una nota, timer 0 per stopparla
	init_timer(1, 0x017D7840);  //25Mhz * 1 sec, usato per diminuire di una tacca Satiety e Happiness
	init_timer(2, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */  //è QUELLO CON 125 KHz
	
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec * 100 MHz (il Rit usa il timer del processore, quindi 100 MHz)	*/  //IL RIT SI OCCUPERA DEL JOYSTICK (50 ms)
	
	LCD_Clear(White);   
	disegnaAmbiente();  //disegna tutto tranne il tamagotchi, che invece va disegnato come "frame" ogni secondo con la funzione disegna tamagotchi
	disegnaTamagotchi(0, 0);
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);

	enable_timer(1);
	enable_timer(2);  //IL TIMER 0 LEGGE IL PUNTO CHE STO TOCCANDO, E SE è NELLO SCHERMO ALLORA DISEGNA UN PUNTO (LO TROVI NELL'HANDLER)	
	enable_RIT();													/* RIT enabled												*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						

  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
