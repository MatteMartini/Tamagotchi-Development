#include "functions.h"
#include "LPC17xx.h"
#include "../GLCD_advanced/GLCD.h"
#include "stdio.h"
#include "../timer/timer.h"


volatile int secondi=0;
volatile int minuti=0;
volatile int ore=0;
volatile int frame=0; //questa variabile indica in quale stato dell'animazione siamo
volatile int mealBattery=4;
volatile int snackBattery=4;
extern int staMangiando;
extern int flagReset;

void disegnaAmbiente(void) {
	disegnaAge(ore, minuti, secondi);
	disegnaBatterie();
	disegnaMealSnack();
	//per megafono
	LCD_DrawRectangle (10, 10, 10, 10, Black);
	LCD_DrawLine(20, 9, 20, 21, Black);
	LCD_DrawLine(21, 8, 21, 22, Black);
	LCD_DrawLine(22, 7, 22, 23, Black);
	LCD_DrawLine(23, 6, 23, 24, Black);
	LCD_DrawLine(24, 5, 24, 25, Black);
	LCD_DrawLine(25, 4, 25, 26, Black);
	LCD_DrawLine(26, 3, 26, 27, Black);
	
	return;
}


void disegnaAge(int ore, int minuti, int secondi) {
	char stringa[20];
	sprintf(stringa, "Age: %02u:%02u:%02u", ore, minuti, secondi);
	GUI_Text(65, 10, (uint8_t *) stringa, Blue, White);
}

void disegnaBatterie(void){
	GUI_Text(30, 30, (uint8_t *) "Happiness", Blue, White);
	GUI_Text(160, 30, (uint8_t *) "Satiety", Blue, White);
	//contorno batteria 1
	LCD_DrawLine(40, 50, 40, 70, Black);
	LCD_DrawLine(80, 50, 80, 70, Black);
	LCD_DrawLine(40, 70, 80, 70, Black);
	LCD_DrawLine(40, 50, 80, 50, Black);
	LCD_DrawRectangle (80, 55, 5, 10, Black);
	
	//contorno batteria 2
	LCD_DrawLine(170, 50, 170, 70, Black);
	LCD_DrawLine(210, 50, 210, 70, Black);
	LCD_DrawLine(170, 70, 210, 70, Black);
	LCD_DrawLine(170, 50, 210, 50, Black);
	LCD_DrawRectangle (210, 55, 5, 10, Black);
	
	//riempimento tacchette batteria 1
	LCD_DrawRectangle(42, 52, 8, 16, Black);
	LCD_DrawRectangle(52, 52, 8, 16, Black);
	LCD_DrawRectangle(62, 52, 8, 16, Black);
	LCD_DrawRectangle(72, 52, 7, 16, Black);
	
	//riempimento tacchette batteria 2
	LCD_DrawRectangle(172, 52, 8, 16, Black);
	LCD_DrawRectangle(182, 52, 8, 16, Black);
	LCD_DrawRectangle(192, 52, 8, 16, Black);
	LCD_DrawRectangle(202, 52, 7, 16, Black);
	return;
}
void disegnaMealSnack(void) {
	//bottone Meal
  LCD_DrawLine(0, 260, 119, 260, Black);
	LCD_DrawLine(0, 319, 119, 319, Black);
	LCD_DrawLine(0, 260, 0, 319, Black);
	LCD_DrawLine(119, 260, 119, 319, Black);
  GUI_Text(40, 280, (uint8_t *) "Meal", Black, White);
	
  //bottone Snack
  LCD_DrawLine(120, 260, 239, 260, Black);
	LCD_DrawLine(120, 319, 239, 319, Black);
	LCD_DrawLine(120, 260, 120, 319, Black);
	LCD_DrawLine(239, 260, 239, 319, Black);
	GUI_Text(160, 280, (uint8_t *) "Snack", Black, White);
  
	return;
}
	


//le immagini ricevono 2 parametri di offset cosi da poterle spostare all'interno dello schermo

void disegnaTamagotchi(int x, int y) {  //matrice di intere su 8 bit, ogni numero un colore
	
  // 1=bianco, 2=verde, 3=yellow, 4=blue2
  uint8_t Tama [21][13] = 
  {0,0,0,0,0,1,1,1,0,0,0,0,0,
   0,0,0,0,1,2,2,2,1,0,0,0,0,
   0,0,0,0,1,1,2,1,1,0,0,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,1,2,2,3,2,3,2,2,1,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,2,4,4,4,2,1,0,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,1,1,2,1,1,1,0,0,0,
   0,0,1,2,2,2,2,2,2,2,1,0,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,0,1,1,2,2,2,2,2,1,1,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,2,1,0,1,2,1,0,0,0,
   0,0,0,1,2,1,0,1,2,1,0,0,0,
	 0,0,0,1,2,1,0,1,2,1,0,0,0,
	 0,0,1,2,2,1,0,1,2,2,1,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0};

  int i,j, x0 = 86, y0 = 159;
 //le immagini sono disegnate con pixel 5*5, quindi la nostra immagine non è solo 21x13! Mentre snack e meal saranno in grandezza "naturale"
  for(i=0; i<21; i++){
    for(j=0; j<13; j++){
      switch (Tama[i][j]){
        case 1:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, White);
          break;
        case 2:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Green);
          break;
        case 3:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Yellow);
          break;
        case 4:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Blue2);
          break;
      }
    }
  }
		frame=1;
 return;
}

void disegnaTamagotchi2(int x, int y) {
		
  // 1=bianco, 2=verde, 3=yellow, 4=blue2
  uint8_t Tama [21][13] = 
  {0,0,0,0,0,1,1,1,0,0,0,0,0,
   0,0,0,0,1,2,2,2,1,0,0,0,0,
   0,0,0,0,1,1,2,1,1,0,0,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,1,2,2,3,2,3,2,2,1,0,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,1,2,1,2,4,4,4,2,1,2,1,0,
   0,1,2,1,2,2,2,2,2,1,2,1,0,
   0,1,2,1,1,1,2,1,1,1,2,1,0,
   0,0,1,2,2,2,2,2,2,2,1,0,0,
   0,1,0,1,2,2,2,2,2,1,0,0,0,
   0,1,0,1,2,2,2,2,2,1,0,0,0,
   0,1,0,1,2,2,2,2,2,1,0,0,0,
   0,1,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,2,2,2,2,2,1,0,0,0,
   0,0,0,1,2,1,0,1,2,1,0,0,0,
   0,0,0,1,2,1,0,1,2,1,0,0,0,
	 0,0,0,1,2,1,0,1,2,1,0,0,0,
	 0,0,1,2,2,1,0,1,2,2,1,0,0,
	 0,0,0,0,0,0,0,0,0,0,0,0,0};

  int i,j, x0 = 86, y0 = 159;
 
  for(i=0; i<21; i++){
    for(j=0; j<13; j++){
      switch (Tama[i][j]){
        case 1:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, White);
          break;
        case 2:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Green);
          break;
        case 3:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Yellow);
          break;
        case 4:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Blue2);
          break;
      }
    }
  }
  frame=0;	
return;
}

void cancellaTama(int x, int y) {
	int x0 = 86, y0 = 159;
	LCD_DrawRectangle(x0+x, y0+y, 13*5, 20*5, White);  //le misure sono prese dalle funzioni precedenti di scrittura 

return;
}

void disegnaMeal(void) {
	// 1=bianco, 2=nero, 3=yellow
  uint8_t Meal [27][17] = 
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
   0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,
   0,0,0,1,2,2,3,3,3,3,3,2,2,1,0,0,0,
   0,0,1,2,3,3,3,3,3,3,3,3,3,2,1,0,0,
	 0,1,2,3,3,3,3,3,3,3,3,3,3,3,2,1,0,
   0,1,2,3,3,3,3,3,3,3,3,3,3,3,2,1,0,
   1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
   1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
   1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
   1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
   1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
   0,1,2,3,3,3,3,2,3,2,3,3,3,3,2,2,1,
   0,1,2,3,3,3,2,3,2,3,2,3,3,3,2,1,0,
   0,0,1,2,3,3,2,3,3,3,2,3,3,2,1,0,0,
   0,0,0,1,2,3,3,2,3,2,3,3,2,1,0,0,0,
   0,0,0,1,2,3,3,2,3,2,3,3,2,1,0,0,0,
   0,0,0,0,1,2,3,2,3,2,3,2,1,0,0,0,0,
   0,0,0,0,1,2,3,2,3,2,3,2,1,0,0,0,0,
	 0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,
	 0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,
	 0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,
	 0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,
	 0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,
	 0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,
	 0,0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,0,
	 0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
	};

  int i,j, x0 = 190, y0 = 170;

  for(i=0; i<27; i++){
    for(j=0; j<17; j++){
      switch (Meal[i][j]){
        case 1:
          LCD_DrawRectangle(x0+(j), y0+(i), 1, 1, White);
          break;
        case 2:
          LCD_DrawRectangle(x0+(j), y0+(i), 1, 1, Black);
          break;
        case 3:
          LCD_DrawRectangle(x0+(j), y0+(i), 1, 1, Yellow);
          break;
      }
    }
  }
return;
}

void disegnaSnack(void) {
	// 1=bianco, 2=nero, 3=yellow
  uint8_t Snack [21][17] = 
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,
   0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,1,
   0,0,0,0,0,0,1,2,3,3,3,3,3,3,3,2,1,
   0,0,0,0,0,1,2,3,3,3,3,3,3,3,2,1,0,
	 0,0,0,0,1,2,3,3,3,3,3,3,3,2,1,0,0,
   0,0,0,1,2,3,3,3,3,3,3,3,2,1,0,0,0,
   0,0,1,2,3,3,3,3,3,3,3,2,1,1,1,0,0,
   0,1,2,3,3,3,3,3,3,3,2,2,2,2,2,1,0,
   1,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,0,
   1,2,3,3,3,3,3,3,3,3,3,3,3,3,2,1,0,
   1,2,2,2,2,2,3,3,3,3,3,3,3,2,1,0,0,
   0,1,1,1,1,2,3,3,3,3,3,3,2,1,0,0,0,
   0,0,0,0,1,2,3,3,3,3,3,2,1,0,0,0,0,
   0,0,0,1,2,3,3,3,3,3,2,1,0,0,0,0,0,
   0,0,0,1,2,3,3,3,3,2,1,0,0,0,0,0,0,
   0,0,0,1,2,3,3,3,2,1,0,0,0,0,0,0,0,
   0,0,1,2,3,3,3,2,1,0,0,0,0,0,0,0,0,
   0,0,1,2,3,3,2,1,0,0,0,0,0,0,0,0,0,
	 0,0,1,2,3,2,1,0,0,0,0,0,0,0,0,0,0,
	 0,0,1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0};

  int i,j, x0 = 190, y0 = 170;

  for(i=0; i<21; i++){
    for(j=0; j<17; j++){
      switch (Snack[i][j]){
        case 1:
          LCD_DrawRectangle(x0+(1*j), y0+(i), 1, 1, White);
          break;
        case 2:
          LCD_DrawRectangle(x0+(j), y0+(i), 1, 1, Black);
          break;
        case 3:
          LCD_DrawRectangle(x0+(j), y0+(i), 1, 1, Yellow);
          break;
      }
    }
  }	
return;
}


void aggiornaBatteria(int J_down1, int J_down2) {
	if(J_down2>0) { //qui se devo aumentare la batteria di snack, cioe happiness
		switch(snackBattery) {
			case 1:
				LCD_DrawRectangle(52, 52, 8, 16, Black);
				break;
			case 2:
				LCD_DrawRectangle(62, 52, 8, 16, Black);
				break;
		  case 3:
				LCD_DrawRectangle(72, 52, 7, 16, Black);
			  break;
			default:
				break;
	}
}
   if(J_down1>0) {
		switch(mealBattery) {  //qui per aumentare satiety
			case 1:
					LCD_DrawRectangle(182, 52, 8, 16, Black);
				break;
			case 2:
					LCD_DrawRectangle(192, 52, 8, 16, Black);
				break;
		  case 3:
					LCD_DrawRectangle(202, 52, 7, 16, Black);
			  break;
			default:
				break;
		}
	}
		return;
}
	

void disegnaReset(void) {
	
	LCD_DrawRectangle(0,260,240,60,White);
	LCD_DrawLine(0, 260, 0, 319, Red);
	LCD_DrawLine(239, 260, 239, 319, Red);
	LCD_DrawLine(0, 260, 239, 260, Red);
	LCD_DrawLine(0, 319, 239, 319, Red);
	GUI_Text(100, 280, (uint8_t *) "Reset", Black, White);
}


void gameOver(void) {  //inserisco il frame di fine, la scritta game over e il bottone di reset
	disable_timer(1);  //il rit invece lo lasci acceso perche devi vedere se premi il tasto reset, ed è deciso col SELECT del joystick
	disable_timer(2);
	cancellaTama(0,0);
	disegnaTamagotchiMorto(0, 0);
	GUI_Text(87, 120, (uint8_t *) "GAME OVER", Blue2, White);
	disegnaReset(); 
	return;
}
	
void resetTama(void) {  //rinizializzo tutte le variabili al valore iniziale, cancello la scritta gameover, il frame di fine e faccio ripartire dall'inizio, con batterie piene e conteggio dell'age da zero
	staMangiando=0;
	frame=0;
	mealBattery=4;
	snackBattery=4;
	flagReset=0;
	ore=0;
	minuti=0;
	secondi=0;
	LCD_DrawRectangle(0, 260, 240, 60, White);
	GUI_Text(87, 120, (uint8_t*) "GAME_OVER", White, White);
	LCD_DrawRectangle(86, 159, 16*5, 16*5, White); //per cancellare la x del gameover
	disegnaAmbiente();
	disegnaTamagotchi(0,0);
	enable_timer(1);
	enable_timer(2);
	return;
}

void cancellaSnack(void) {
	int x0 = 190, y0 = 170;
	LCD_DrawRectangle(x0, y0, 21, 17, White);
	
return;
}

void cancellaMeal(void) {
	int x0 = 190, y0 = 170;
	LCD_DrawRectangle(x0, y0, 27, 17, White);
	
return;
}

void disegnaTamagotchiMorto(int x, int y) {
  // 1=rosso, 2 nero
  uint8_t Tama [16][16] = 
  {0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,
	 0,0,2,1,2,0,0,0,0,0,0,2,1,2,0,0,
   0,2,1,1,1,2,0,0,0,0,2,1,1,1,2,0,
   2,1,1,1,1,1,2,0,0,2,1,1,1,1,1,2,
   0,2,1,1,1,1,1,2,2,1,1,1,1,1,2,0,
   0,0,2,1,1,1,1,1,1,1,1,1,1,2,0,0,
   0,0,0,2,1,1,1,1,1,1,1,1,2,0,0,0,
   0,0,0,0,2,1,1,1,1,1,1,2,0,0,0,0,
   0,0,0,0,2,1,1,1,1,1,1,2,0,0,0,0,
   0,0,0,2,1,1,1,1,1,1,1,1,2,0,0,0,
   0,0,2,1,1,1,1,1,1,1,1,1,1,2,0,0,
   0,2,1,1,1,1,1,2,2,1,1,1,1,1,2,0,
   2,1,1,1,1,1,2,0,0,2,1,1,1,1,1,2,
   0,2,1,1,1,2,0,0,0,0,2,1,1,1,2,0,
   0,0,2,1,2,0,0,0,0,0,0,2,1,2,0,0,
	 0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0};

  int i,j, x0 = 86, y0 = 159;

  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      switch (Tama[i][j]){
        case 1:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Red);
          break;
        case 2:
          LCD_DrawRectangle(x0+(5*j)+x, y0+(5*i)+y, 5, 5, Black);
          break;
      }
    }
  }
return;
}
