/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD_advanced/GLCD.h" 
#include "../My_functions/functions.h"
#include "../RIT/RIT.h"
#include "../TP/TouchPanel.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
	
	
	uint16_t SinTable[45] =                                       /*                      */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

	
	extern int secondi;
	extern int minuti;
	extern int ore;
	extern int staMangiando;
	extern int frame;        //indica quale dei 2 frame dell'animazione si sta usando
	extern int mealBattery;  //ci dice il livello di una delle due batterie
	extern int snackBattery;  //livello dell'altra batteria
	extern int flagReset;    //ci dice se ci troviamo nella situazione in cui c'è il reset, e quindi il tamagotchi è scappato
  extern int flagclick;
  extern int flagcibo;
  extern int flagcuddle;
  extern int flagmorte;
  extern int next;
  extern int volume;

NOTE click={a4, time_croma};
NOTE cibo[3]={{c3, time_minima},{e4, time_minima},{b2, time_minima}};
NOTE cuddle[6]={{c3, time_croma},{c4, time_croma},{g3, time_croma},{e4, time_croma},{a4, time_semiminima}};
NOTE morte[9]={{b2, time_minima},{g3, time_minima},{b4, time_minima},{e3, time_semiminima},{b3, time_croma},{g3, time_minima},{b2, time_semiminima},{b3, time_croma}};

	void TIMER0_IRQHandler (void)
{ 
  disable_timer(3);
	if (flagclick==1) {
		next=0;
		flagclick=0;
	}
		if (flagcibo==1) {
		  if (next<3) {
				next++;
				playNote(cibo[next]);
			}
			else {
				flagcibo=0;
			}
	}
		if (flagcuddle==1) {
			if (next<6) {
				next++;
				playNote(cuddle[next]);
			}
			else {
				flagcuddle=0;
			}
	}
		if (flagmorte==1) {
			if (next<9) {
				next++;
				playNote(morte[next]);
			}
			else {
				flagmorte=0;
			}
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(secondi<59)
		secondi++;
	else {
		secondi=0;
		if (minuti<59)
			 minuti++;
		else {
			minuti=0;
			ore++;
		}
	}
	disegnaAge(ore, minuti, secondi);
	
	//serve per l'animazione, si decide quale dei 2 frame stampare in base alla variabile frame. Se sta mangiando non alza le braccia mentre cammina
	if(staMangiando==0) {
		cancellaTama(0,0);
		if(frame==0)  //se non sta alzando le braccia
		  disegnaTamagotchi(0,0);
		else 
			disegnaTamagotchi2(0,0);
	}
	
	if(secondi%5==0) {
		disable_RIT();
	  disable_timer(2);
  if(mealBattery==1) {  //nella prima parte si va a controllare se eliminando la prossima barretta di vita da una delle due batterie si arrivi a 0 e quindi al gameover, nella seconda parte si considera il caso in cui ci siano ancor almeno 2 barrette x entrambe le batterie
		flagReset=1;
		mealBattery=0;
		LCD_DrawRectangle(172, 52, 8, 16, White);
	if(snackBattery==1) {
		LCD_DrawRectangle(42, 52, 8, 16, White);
		snackBattery=0;
		flagReset=1;
	}
		gameOver();
	  flagmorte=1;
	  playNote(morte[0]);
	}
	if(snackBattery==1 && !flagReset) {
		flagReset=1;
		snackBattery=0;
		LCD_DrawRectangle(42, 52, 8, 16, White);
		if(mealBattery==1) {
			mealBattery=0;
		  LCD_DrawRectangle(172, 52, 8, 16, White);	
		}
		gameOver();
		flagmorte=1;
		playNote(morte[0]);
	}
		if(snackBattery>1) {
			switch(snackBattery) {
			case 2:
				LCD_DrawRectangle(52, 52, 8, 16, White);
				break;
			case 3:
				LCD_DrawRectangle(62, 52, 8, 16, White);
				break;
		  case 4:
				LCD_DrawRectangle(72, 52, 7, 16, White);
			  break;
	  	}
			snackBattery--; 
	  }
		if(mealBattery>1) {
			switch(mealBattery) {
				case 2:
					LCD_DrawRectangle(182, 52, 8, 16, White);
					break;
				case 3:
					LCD_DrawRectangle(192, 52, 8, 16, White);
					break;
				case 4:
					LCD_DrawRectangle(202, 52, 7, 16, White);
					break;
			}
			mealBattery--;
		}
		if(mealBattery>0 && snackBattery>0) {  //se il tamagotchi non era scappato e si stavano solo aggiornando le batterie, i timer ripartono da dove si erano fermati
			enable_timer(1);
			enable_timer(2);
		}
		enable_RIT();
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	
  if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){  //legge il punto che sto toccando
		if(display.y >= 159 && display.y < 260 && display.x>=86 && display.x<155 ){  //se sta nel mio schermo, disegna qualcosina, in questo caso un punto 
			disable_timer(1);
			disable_RIT();
			cancellaTama(0,0);
			disegnaTamagotchi2(0, -40);
			flagcuddle=1;
			playNote(cuddle[0]);
			DelayUS(2000 * 1000); //2 secondi senza emulatore
			//DelayUS(2000 * 10); //con emulatore deve essere piu breve per non dover aspettare un'eternità
			cancellaTama(0,-40);
			disegnaTamagotchi(0,0);
			aggiornaBatteria(0,1);
			if(snackBattery<4)
				snackBattery++;
			next=0;
			disable_timer(3);
	    enable_timer(1);
			enable_RIT();
		}
	}
	else{
		//do nothing if touch returns values out of bounds
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER3_IRQHandler (void)
{
	static int sineticks=0;
	/* DAC management */	

	LPC_DAC->DACR = (((SinTable[sineticks])*1023/819*volume/4)<<6);
	sineticks++;
	if(sineticks==45) sineticks=0;

	
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
