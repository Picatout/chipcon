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
 * keypad.c
 *
 * Created: 2014-10-03 12:29:17
 *  Author: Jacques Deschênes
 */ 

#include "keypad.h"
#include "tvout.h"
#include "text.h"
#include "tone.h"

#include <avr/pgmspace.h>

#define  out_pin(r) pgm_read_byte(&outputs[r])
#define  inp_pin(c) pgm_read_byte(&inputs[c])
#define  hex_key(r,c) pgm_read_byte(&key_matrix[r][c])
#define  is_pin_low(c)  !(KP_PIN & (c))
#define  set_row_low(r)  KP_OPORT &= ~(r)
#define  set_row_high(r) KP_OPORT |= (r) 
#define  is_break()  !(KP_PIN & (KP_INP5|KP_INP7))
#define  settle_delay()  	__asm__ __volatile__("nop");

// chaque bit représente une touche
// 1 touche relâché
// 0 touche enfoncée.
uint16_t keys_state;

void keypad_init(){
	KP_DDR |= KP_OMASK; // broches en sortie
	set_row_high(KP_OMASK); // mettre les broches à 1
	KP_IPORT |= KP_IMASK; // activation pullup	sur les entrées
	keys_state=0xffff;
}

// transcription position switch clavier à valeur
PROGMEM const int8_t key_matrix[4][4]={
	{KP1,KP2,KP3,KPA},
	{KP4,KP5,KP6,KPB},
	{KP7,KP8,KP9,KPC},
	{KPE,KP0,KPF,KPD}			
};
	
PROGMEM const uint8_t outputs[4]={KP_OUT1,KP_OUT2,KP_OUT3,KP_OUT4};
PROGMEM const uint8_t inputs[4]={KP_INP5,KP_INP6,KP_INP7,KP_INP8};

#define DEBOUNCED  (8000U)

uint8_t keypad_read(){
	register uint8_t r,c;
	uint8_t key;
	register int16_t debounce;
	
	key=NO_KEY;
	for (r=0;r<4;r++){
		set_row_low(out_pin(r));
		for(c=0;c<4;c++){
			if (is_pin_low(inp_pin(c))){
				debounce=0;
				frame_delay=2;
				while (frame_delay && (debounce<DEBOUNCED)){
				   if (is_pin_low(inp_pin(c))) debounce++; else debounce--;				
				}//while
				if (debounce==DEBOUNCED){
					key=hex_key(r,c);
					break;
				}// if
			}// if
		}// for
		set_row_high(KP_OMASK);
		if (key!=NO_KEY) break;
	}// for
	return key;
}

uint8_t wait_key(){
	uint8_t key;
	while ((key=keypad_read())==NO_KEY);
	return key;	
}

void prompt_key(){
	uint8_t key;
	
	select_font(FONT_ASCII);
	while (keypad_read()!=NO_KEY);
	prt_pstr(PSTR("une touche..."));
	wait_key();
	while ((key=keypad_read())!=NO_KEY) tone(key,4);
}


uint8_t keypad_break(){
	uint8_t break_down;
	
	set_row_low(KP_OUT4);
	settle_delay();
	break_down=is_break();
	if (break_down){
		while (is_break()){
			key_tone(0xe,1,true);
			key_tone(0xf,1,true);
		}
	}//if 
	set_row_high(KP_OUT4);
	return break_down;
}


uint8_t key_down(uint8_t key){
	uint8_t r,c;
	
	keys_state=0xffff;
	
	for (r=0;r<4;r++){
		set_row_low(out_pin(r));
		for (c=0;c<4;c++){
			if (is_pin_low(inp_pin(c))){
				keys_state &= ~(1<<(hex_key(r,c)));
			}
		}
		set_row_high(KP_OMASK);
	}
	return (keys_state&(1<<key))==0;
}

