#include "music.h"
#include "../timer/timer.h"

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(3);
		init_timer(3, note.freq);
		enable_timer(3);
	}
	reset_timer(0);
	init_timer(0, note.duration);
	enable_timer(0);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM0->TCR != 0) || (LPC_TIM1->TCR != 0));
}
