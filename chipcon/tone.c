/*
* Copyright 2014, Jacques Deschênes
* This file is part of CHIPcon.
*
*     CHIPcon is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     CHIPcon is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with CHIPcon.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * tones.c
 *
 * Created: 2014-09-26 10:17:59
 *  Author: Jacques Deschênes
 */ 


#include "tone.h"
#include "tvout.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>






void tone_init(){
	TONE_DDR |= TONE_OUT;
	TONE_TCCRB = 3; // CLKt0=CLKio/64
	TONE_TCCRA = (1<<6)| 2; // OC0A toggle, mode 2 CTC
}


// fréquence en hertz
// durée en  millisecondes
void tone(uint16_t freq, uint8_t length){
	TONE_OCRA = (F_CPU/128)/freq-1;
	tone_on();
	tone_length = length;
}

PROGMEM const uint16_t SCALE[16]={523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245};
	
inline void key_tone(uint8_t key, uint8_t length,bool wait_end){
	tone(pgm_read_word(&SCALE[key&15]),length);	
	if (wait_end) wait_tone_end();
}

