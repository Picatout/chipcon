/*
 * ms_timer.c
 *
 * Created: 2014-10-04 12:44:40
 *  Author: Jacques
 */ 

#include "ms_timer.h"

#include <avr/interrupt.h>
#include <stddef.h>

//intervalle d'interruption 10msec
void ms_timer_init(){
	MS_TMR_TCCRA = 2; //mode TCT
	MS_TMR_TCCRB = 7; // préscaler CLKt2s/1024
	MS_TMR_OCRA = 155;
}

typedef struct timer_struct{
		uint16_t cntr;
		timer_callback callback;
		} timer_t;

static volatile timer_t timers[2]={{0,NULL},{0,NULL}};


void ms_timer_create(uint8_t tmr_id, uint16_t delay, timer_callback cb, uint8_t run){
	timers[tmr_id].cntr=delay;
	timers[tmr_id].callback=cb;
	if (run) MS_TMR_TIMSK |= MS_TMR_IE; // activation de l'interruption
}

inline void ms_timer_start(uint8_t tmr_id, uint16_t delay){
	timers[tmr_id].cntr=delay;
	MS_TMR_TIMSK |= MS_TMR_IE; // activation de l'interruption
}

inline void ms_timer_stop(uint8_t tmr_id){
	timers[tmr_id].cntr=0;
}

inline void ms_timer_set_callback(uint8_t tmr_id, timer_callback cb){
	timers[tmr_id].callback=cb;
}

inline uint16_t ms_timer_get_counter(uint8_t tmr_id){
	return timers[tmr_id].cntr;
}

//intervalle d'interruption 10msec
ISR(TIMER2_COMPA_vect){
	sei();
	if (timers[TONE_TIMER].cntr){
		timers[TONE_TIMER].cntr--;
		if (!timers[TONE_TIMER].cntr && timers[TONE_TIMER].callback) timers[TONE_TIMER].callback();
	}
	if (timers[GAME_TIMER].cntr){
		timers[GAME_TIMER].cntr--;
		if (!timers[GAME_TIMER].cntr && timers[GAME_TIMER].callback) timers[GAME_TIMER].callback();
	}
	if (!(timers[GAME_TIMER].cntr || timers[TONE_TIMER].cntr)) MS_TMR_TIMSK &= ~MS_TMR_IE;
}
