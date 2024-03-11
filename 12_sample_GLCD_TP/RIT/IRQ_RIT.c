/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../My_functions/functions.h"
#include "../GLCD_advanced/GLCD.h"
#include "../adc/adc.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int mealBattery;  //definite nel functions.c per indicare il livello delle batterie, il frame dell'animazione in corso
extern int snackBattery;
extern int frame;
volatile int staMangiando=0;   //serve perche nella fase in cui sta mangiando uno snack o un meal il joystick non deve essere usato per decidere tra meal e snack, ne per selezionare
volatile int flagReset=0;
volatile int flagclick=0;
volatile int flagcibo=0;
volatile int flagcuddle=0;
volatile int flagmorte=0;
volatile int next=0;
extern NOTE click;
extern NOTE cibo[4];

void RIT_IRQHandler (void)
{					
	static int J_down1 = 0; //per left
	static int J_down2 = 0; //per right
  int i;
	ADC_start_conversion();
	
	if (flagReset==0) {
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	 //joystick sinistro
		J_down1++;
		switch(J_down1){
			case 1:
				J_down2=0;

			  LCD_DrawLine(0, 260, 119, 260, Red);  //coloro di rosso meal
			  LCD_DrawLine(0, 319, 119, 319, Red);
				LCD_DrawLine(0, 260, 0, 319, Red);
				LCD_DrawLine(119, 260, 119, 319, Red);
				GUI_Text(40, 280, (uint8_t *) "Meal", Red, White);
			
		    LCD_DrawLine(120, 260, 239, 260, Black); //serve anche colorare l'altro in nero, perche magari prima era lui ad essere selezionato e quindi rosso
				LCD_DrawLine(120, 319, 239, 319, Black);
				LCD_DrawLine(120, 260, 120, 319, Black);
				LCD_DrawLine(239, 260, 239, 319, Black);
				GUI_Text(160, 280, (uint8_t *) "Snack", Black, White); 
			
				break;
			default:
				break;
		}
	}
   if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	 //joystick destro
		J_down2++;
		switch(J_down2){
			case 1:
				J_down1=0;
			 	LCD_DrawLine(0, 260, 119, 260, Black);  //coloro di nero meal
			  LCD_DrawLine(0, 319, 119, 319, Black);
				LCD_DrawLine(0, 260, 0, 319, Black);
				LCD_DrawLine(119, 260, 119, 319, Black);
				GUI_Text(40, 280, (uint8_t *) "Meal", Black, White);
			
		    LCD_DrawLine(120, 260, 239, 260, Red); //rosso snack 
				LCD_DrawLine(120, 319, 239, 319, Red);
				LCD_DrawLine(120, 260, 120, 319, Red);
				LCD_DrawLine(239, 260, 239, 319, Red);
				GUI_Text(160, 280, (uint8_t *) "Snack", Red, White);
		
			
				break;
			default:
				break;
		}
	}
	/* button management */
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {  // SELECT con il joystick, per dargli da mangiare e in fondo anche per decidere se riniziare da capo dopo che il tamagotchi è scappato
		disable_RIT();
		disable_timer(1);
		disable_timer(2);
		flagclick=1;
		playNote(click);
		if(J_down1>0) { 
			 //dargli da mangiare se ha le batterie piene è inutile
				staMangiando=1;
				disegnaMeal();
				cancellaTama(0,0);
			for(i=0; i<=55; i=i+5) { //per spostarlo a destra con il meal
			 disegnaTamagotchi(i,0);
			 cancellaTama(i,0);
		}
			playNote(cibo[0]);
			cancellaMeal();
		  for(i=0; i<=50; i=i+5) {  //per spostarlo a sinistra
			disegnaTamagotchi(55-i,0);
			cancellaTama(55-i,0);
		}
			aggiornaBatteria(J_down1, J_down2);
			mealBattery++;
		  disegnaTamagotchi(0,0);
	}
		if(J_down2>0) {

			staMangiando=1;
		  disegnaSnack();
			cancellaTama(0,0);
			for(i=0; i<=55; i=i+5) { // per spostarlo a destra con lo snack
			 disegnaTamagotchi(i,0);
			 cancellaTama(i,0);
		}
			playNote(cibo[0]);
			cancellaSnack();
		  for(i=0; i<=50; i=i+5) {  //per spostarlo a sinistra
			disegnaTamagotchi(55-i,0);
			cancellaTama(55-i,0);
		}
			aggiornaBatteria(J_down1, J_down2);
			snackBattery++;
		  disegnaTamagotchi(0,0);
	}
		staMangiando=0;
	  next=0;
		disable_timer(3);
	
		enable_RIT();
		enable_timer(1);
	  enable_timer(2);
 }
}
	if (flagReset==1) {  //nel caso in cui si sta decidendo se riniziare con un nuovo tamagotchi oppure no, lo si controlla sempre con il joystick in polling, e il joystick è gestito dal RIT
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {
			disable_RIT();
			resetTama();
			next=0;
			enable_RIT();
	}
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */

  return;
 }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
