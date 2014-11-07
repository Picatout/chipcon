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
 * tvout.c
 *
 * Created: 2014-09-25 12:35:29
 *  Author: Jacques Deschênes
 */ 

#include "hardware.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "tvout.h"
#include "text.h"
#include "tone.h"
#include "sram.h"

void tvout_init(){
	// met broche sync en mode sortie
	NTSC_SYNC_DDR |= NTSC_SYNC_OUT;
	//configuration USART0 pour mode SPI
	NSTC_VIDEO_CONFIG();
	//configuration PWM 16 bits pour sync video
	NTSC_SYNC_OCRA=HTIME;
	NTSC_SYNC_OCRB=VPULSE;
	//configuration mode PWM
	NTSC_SYNC_CONFIG();
	//activation interruption
	NTSC_SYNC_INT_ENABLE();
	
}

static uint8_t video_buffer[HBYTES*VRES];

// dessine un point à l'écran
void plot(int8_t x, int8_t y, optype op){
	int8_t xbyte,xbit;
	
	if ((x<0)||(x>=HRES)||(y<0)||(y>=VRES)) return;
	xbyte= x/8;
	xbit = 7-x%8;
	switch (op){
		case WHITE:
		video_buffer[y*HBYTES+xbyte] |= (1<<xbit);
		break;
		case BLACK:
		video_buffer[y*HBYTES+xbyte] &= ~(1<<xbit);
		break;
		case INVERT:
		video_buffer[y*HBYTES+xbyte] ^= (1<<xbit);
		break;
	}
}


// dessine un sprite 8 pixels par n pixels
// s'il y a collision retourne une valeur 1
// autrement 0.
int8_t put_sprite(int8_t x, int8_t y, uint8_t n, const uint8_t *sprite, int8_t memory){
	int8_t xbyte, shl, shr, row, collision;
	uint8_t sprite_row;

	
	// contrôle des limites
	if ((x<-7)||(x>=HRES)||(y<-14)||y>=VRES){return 0;} 
	collision=0;
	if (x<0){
		shl=-x;
		for (row=0;row<n;row++){
			if (y<0 || y>=VRES){y++;continue;}
			if (memory==RAM_MEM){
				sprite_row=sprite[row];
			}else{
				sprite_row= pgm_read_byte(sprite+row);
			}
			collision |= video_buffer[y*HBYTES] & (sprite_row<<shl);
			video_buffer[y*HBYTES] ^= sprite_row<<shl;
			y++;
		}//for
	}else{
		shr= x&7;
		shl=(8-shr)&7;
		xbyte=x>>3;
		for (row=0;row<n;row++){
			if (y<0 || y>=VRES){y++;continue;}
			if (memory==RAM_MEM){
				sprite_row=sprite[row];
				}else{
				sprite_row= pgm_read_byte(sprite+row);
			}
			collision |= video_buffer[y*HBYTES+xbyte] & (sprite_row>>shr);
			video_buffer[y*HBYTES+xbyte] ^= sprite_row>>shr;
			if (shl && (xbyte+1<HBYTES)){
				collision |= video_buffer[y*HBYTES+xbyte+1] & (sprite_row<<shl);
				video_buffer[y*HBYTES+xbyte+1] ^= sprite_row<<shl;
			}
			y++;
		}
	}
	if (collision) collision=1;
	return collision;
}

//dessine un sprite 16x16pixels
// le sprite est en mémoire RAM
// s'il y a collision retourne 1,
// autrement 0.
int8_t put_big_sprite(int8_t x, int8_t y,const uint8_t *sprite){
	int8_t xbyte, shl, shr, row, collision;
	uint8_t sprite_row;

	// contrôle des limites
	if ((x<-15)||(x>=HRES)||(y<-15)||(y>=VRES)) return 0;
	collision=0;
	if (x<-7){ // x {-7..-15}
		shl=-x-8;
		for (row=0;row<32;row+=2){
			if ((y<0) || (y>= VRES)){y++; continue;}
			sprite_row=sprite[row+1];
			collision |= video_buffer[y*HBYTES] & (sprite_row<<shl);
			video_buffer[y*HBYTES] ^= sprite_row<<shl;
			y++;
		}
	}else if (x<0){ // x {-1..-7}
		shl=-x;
		shr=8-shl;
		for (row=0;row<32;row+=2){
			if ((y<0) || (y>= VRES)){y++; continue;}
			sprite_row=sprite[row];
			collision|=video_buffer[y*HBYTES] & (sprite_row<<shl);
			video_buffer[y*HBYTES] ^= sprite_row<<shl;
			sprite_row=sprite[row+1];
			collision|=video_buffer[y*HBYTES] & (sprite_row>>shr);
			video_buffer[y*HBYTES] ^= sprite_row>>shr;
			collision|=video_buffer[y*HBYTES+1] & (sprite_row<<shl);
			video_buffer[y*HBYTES+1] ^= sprite_row<<shl;
			y++;
		}
	}else{ // x {0..HRES-1}
		shr= x&7;
		shl=(8-shr)&7;
		xbyte=x>>3;
		for (row=0;row<32;row+=2){
			if ((y<0) || (y>=VRES)){y++; continue;}
			sprite_row=sprite[row];
			collision |= video_buffer[y*HBYTES+xbyte] & (sprite_row>>shr);
			video_buffer[y*HBYTES+xbyte] ^= sprite_row>>shr;
			if ((xbyte+1)<HBYTES){
				if (shl){
					collision |= video_buffer[y*HBYTES+xbyte+1] & (sprite_row<<shl);
					video_buffer[y*HBYTES+xbyte+1] ^= sprite_row<<shl;
				}
				sprite_row=sprite[row+1];
				collision |= video_buffer[y*HBYTES+xbyte+1] & (sprite_row>>shr);
				video_buffer[y*HBYTES+xbyte+1] ^= (sprite_row>>shr);
				if (shl && ((xbyte+2)<HBYTES)){
					collision |= video_buffer[y*HBYTES+xbyte+2] & (sprite_row<<shl);
					video_buffer[y*HBYTES+xbyte+2] ^=(sprite_row<<shl);
				}
			}
			y++;
		}
	}
	if (collision) collision=1;
	return collision;
}


void cls(){
	memset(video_buffer,0,VRES*HBYTES);	
	set_cursor(0,0);
}


// lines {1..31}
void scroll_down(uint8_t lines){
	if (!lines) return;
	lines &=0x1f;
	memmove(video_buffer+HBYTES*lines,video_buffer,HBYTES*VRES-HBYTES*lines);
	memset(video_buffer,0,HBYTES*lines);	
}


// lines {1..31}
void scroll_up(uint8_t lines){
	if (!lines) return;
	lines &=0x1f;
	memmove(video_buffer,video_buffer+HBYTES*lines,HBYTES*VRES-HBYTES*lines);	
	memset(video_buffer+HBYTES*VRES-HBYTES*lines,0,HBYTES*lines);
}

// pixels {1..8}
void scroll_right(uint8_t pixels){
	uint8_t line, column;
	uint8_t *line_buffer;
	
	if (pixels==8){
		for (line=0;line<VRES;line++){
			line_buffer=video_buffer+HBYTES*line;
			memmove(line_buffer+1,line_buffer,HBYTES-1);
			*line_buffer=0;
		}		
	}else{
		pixels &=0x7;
		for (line=1;line<=VRES;line++){
			line_buffer=video_buffer+HBYTES*line-1;
			for (column=HBYTES-1;column;column--){
				*line_buffer=(*line_buffer>>pixels)|(*(line_buffer-1))<<(8-pixels);
				--line_buffer;
			}
			*line_buffer>>=pixels;
		}
	}
}

// optimisé pour la VM chip8()
// décale de 4 pixels
void chip_scroll_right(){
	uint8_t line, column;
	uint8_t *line_buffer;

	for (line=1;line<=VRES;line++){
		line_buffer=video_buffer+HBYTES*line-1;
		for (column=HBYTES-1;column;column--){
			*line_buffer=(*line_buffer>>4)|((*(line_buffer-1))<<4);
			--line_buffer;
		}
		*line_buffer>>=4;
	}
}

//pixels {1..8}
void scroll_left(uint8_t pixels){
	uint8_t line, column;
	uint8_t  *line_buffer;

	if (pixels==8){
		for (line=0;line<VRES;line++){
			line_buffer=video_buffer+HBYTES*line;
			memmove(line_buffer,line_buffer+1,HBYTES-1);
			*(line_buffer+HBYTES-1)=0;
		}
	}else{
		pixels &=0x7;
		for (line=0;line<VRES;line++){
			line_buffer=video_buffer+HBYTES*line;
			for (column=0;column<(HBYTES-1);column++){
				*line_buffer=(*line_buffer<<pixels)|(*(line_buffer+1)>>(8-pixels));
				++line_buffer;
			}
			*line_buffer<<=pixels;
		}
	}
}

// optimisé pour la VM chip8()
// décale de 4 pixels
void chip_scroll_left(){
	uint8_t line, column;
	uint8_t  *line_buffer;
	for (line=0;line<VRES;line++){
		line_buffer=video_buffer+HBYTES*line;
		for (column=0;column<(HBYTES-1);column++){
			*line_buffer=(*line_buffer<<4)|((*(line_buffer+1))>>4);
			++line_buffer;
		}
		*line_buffer<<=4;
	}
}



void screen_save(){
	sram_store_block(SAVE_SCREEN_ADDR,HBYTES*VRES,video_buffer);
}

void screen_restore(){
	sram_load_block(SAVE_SCREEN_ADDR,HBYTES*VRES,video_buffer);
}



volatile uint8_t frame_delay=0;
volatile uint8_t tone_length=0;
volatile uint8_t speed_delay=0;

static volatile uint16_t line_count=0;
static volatile uint8_t video=0;

// interruption à la fin de
// chaque pulse de synchronisation
ISR(TIMER1_COMPB_vect){
	register unsigned i;
	register unsigned char *buff;

	line_count++;
	switch(line_count){
		case 1:
			if (frame_delay) frame_delay--;
			break;
		case 2:
		if (tone_length){
			tone_length--;
			if (!tone_length) tone_off();
		}
		break;
		case 3:
			NTSC_SYNC_OCRB=HPULSE;
			break;
		case 4:
			if (speed_delay) speed_delay--;
			break;
		case 5:
			ADCSRA |= (1<<ADSC);
			break;
		case FIRST_VISIBLE:
			video=1;
			break;
		case LAST_VISIBLE:
			video=0;
			break;
		case 262:
			line_count=0;
			NTSC_SYNC_OCRB=VPULSE;
			break;
		default:
			if (video){
				buff=video_buffer+(((line_count-FIRST_VISIBLE)>>1)*HBYTES);
				NTSC_VIDEO_UBRR=0;
				NTSC_VIDEO_UCSRB=(1<<TXEN0);
				NTSC_VIDEO_UBRR=2;
				NTSC_VIDEO_UDR=0x0;
				for (i=HBYTES;i;i--){
					while ( !( UCSR0A & (1<<UDRE0)) );
					NTSC_VIDEO_UDR=*buff++;
				}
				while (!(NTSC_VIDEO_UCSRA & (1<<TXC0)));
				NTSC_VIDEO_UCSRB &= ~(1<<TXEN0);
			}
			break;
	}//switch
}

