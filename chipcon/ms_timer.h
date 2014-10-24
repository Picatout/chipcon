/*
 * ms_timer.h
 *
 * Created: 2014-10-04 12:44:22
 *  Author: Jacques
 *  10msec par tick
 */ 


#ifndef MS_TIMER_H_
#define MS_TIMER_H_

#include "hardware.h"

#define TONE_TIMER (0)
#define GAME_TIMER (1)

typedef void (*timer_callback)();

void ms_timer_init();

void ms_timer_create(uint8_t tmr_id, uint16_t delay, timer_callback cb, uint8_t run);
void ms_timer_start(uint8_t tmr_id, uint16_t delay);
void ms_timer_stop(uint8_t tmr_id);
void ms_timer_set_callback(uint8_t tmr_id, timer_callback cb);
uint16_t ms_timer_get_counter(uint8_t tmr_id);

#endif /* MS_TIMER_H_ */