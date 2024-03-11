#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

void disegnaTamagotchi(int x, int y);
void disegnaAmbiente(void);
void disegnaAge(int ore, int minuti, int secondi);
void disegnaBatterie(void);
void disegnaMealSnack(void);
void cancellaTama(int x, int y);
void disegnaTamagotchi2(int x, int y);  //per il frame con le braccia alzate
void disegnaMeal(void);
void disegnaSnack(void);
void aggiornaBatteria(int J_down1, int J_down2);
void gameOver(void);
void disegnaReset(void);
void resetTama(void);
void disegnaTamagotchiMorto(int x, int y);
void cancellaMeal(void);
void cancellaSnack(void);

#endif
