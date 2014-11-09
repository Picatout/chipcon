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
 * chip8.c
 *
 * Created: 2014-09-26 10:10:45
 *  Author: Jacques Deschênes
 *  Description: machine virtuelle qui exécute du byte code super CHIP
 */ 

// doit-être inclus avant <util/delay.h>
#include "hardware.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include "chip8.h"
#include "tvout.h"
#include "sram.h"
#include "text.h"
#include "keypad.h"
#include "tone.h"

#define caddr(b1,b2) (((b1&0xf)<<8)+b2)
#define rx(b1)  (b1&0xf)
#define ry(b2)  ((b2&0xf0)>>4)

// état de la machine virtuelle
static vm_state_t vms;

// stockage temporaire sprite
static uint8_t block[32];


void print_vms(const char *msg){
	screen_save();
	cls();
	select_font(FONT_ASCII);
	prt_pstr(msg);
	prt_pstr(PSTR("PC:"));
	print_hex(vms.pc-2,4);
	print_hex(vms.b1,2);
	print_hex(vms.b2,2);
	new_line();
	prt_pstr(PSTR("I:"));
	print_hex(vms.ix,4);
	prt_pstr(PSTR(" SP:"));
	print_hex(vms.sp,2);
	new_line();
	prt_pstr(PSTR("var[]:"));
	for (int i=0;i<16;i++){
		print_hex(vms.var[i],2);
	}
	new_line();
/*	
	prt_pstr(PSTR("rpl[]:"));
	for (int i=0;i<16;i++){
		print_hex(vms.rpl[i++],2);
	}
*/	
	prt_pstr(PSTR("'E'xit trace or any"));
	if (wait_key()==0xE) vms.trace=0;
	while (keypad_read()!=255);
	screen_restore();
}

#if FW_DEBUG

#define MAX_BREAK  (10)

static uint16_t break_points[MAX_BREAK]={0,0,0,0,0,0,0,0,0,0};

void set_break_point(uint16_t addr){
	uint8_t i;
	for (i=0;i<MAX_BREAK;i++)
		if (!break_points[i]) break_points[i]=addr;
}

void clr_break_point(uint16_t addr){
	uint8_t i;
	for (i=0;i<MAX_BREAK;i++){ 
		if (break_points[i]==addr){ 
			break_points[i]=0;
			break;
		}
	}
}


static uint8_t is_break_point(uint16_t addr){
	uint8_t i;
	for (i=0;i<MAX_BREAK;i++){
		if (break_points[i]==addr){
			return 1;
		}
	}
	return 0;	
}
#endif

// machine virtuelle CHIPcon
uint8_t schip(uint8_t flags){
	uint8_t x,y,n,ipf;
	uint16_t code;
	
    if (flags&F_RESET){	
		vms.pc=CODE_BASE_ADDR;
		vms.sp=0;
		vms.ix=0;
	}
#if FW_DEBUG
	if (flags&F_DEBUG) vms.debug=1;else vms.debug=0;
	if (flags&F_TRACE) vms.trace=1;else vms.trace=0;
#endif	
	ipf=255;
	speed_delay=1;
 	while (1){
		if (!(--ipf)){
			while (speed_delay){
			}
			speed_delay=1;
			ipf=ADCH;
		}
#if FW_DEBUG
		if (vms.debug && keypad_break()){
			vms.trace^=1;
		}
#else		
		if (keypad_break()) return CHIP_EXIT_OK;
#endif		
		vms.opcode=sram_read_word(vms.pc);
		vms.pc+=2;
#if FW_DEBUG		
		if (vms.trace || (vms.debug && is_break_point(vms.pc-2))){ 
			vms.trace=1;
			print_vms(PSTR("Trace print\n"));
		}
#endif		
		x=rx(vms.b1);
		y=ry(vms.b2);
		// décodeur d'instruction
		code=(vms.b1&0xf0)<<4;
		switch (code){
			case 0x000:
				if ((vms.b2&0xf0)==0xc0) code=0xc0; else code=vms.b2;
				break;
			case 0xe00:
			case 0xf00:
				code |= vms.b2;
				break;
			case 0x800:
			case 0x900:
				code |= vms.b2&0xf;
				break;
		}
		// exécution de l'instruction
		switch(code){
			case 0xc0: // 00CN, (mode schip seulement) glisse l'affichage N lignes vers le bas
				scroll_down(vms.b2&0xf);
				break;
			case 0xe0: // 00E0, efface l'écran
				cls();
				break;
			case 0xee: // 00EE, sortie de sous-routine
				vms.pc=vms.stack[vms.sp--];
				break;
			case 0xfb: // 00FB, (mode schip seulement) glisse l'affichage vers la droite de 4 pixels
				chip_scroll_right();
				break;
			case 0xfc: // 00FC, (mode schip seulement) glisse l'affichage vers la gauche de 4 pixels
				chip_scroll_left();
				break;
			case 0xfd:// 00FD, (mode schip seulement) sortie de l'interpréteur.
				return CHIP_EXIT_OK;
			case 0xfe: //00FE, (mode schip seulement) revient au vms.mode par défaut chip-8 résolution 64x32
				break; // ignore ce code
			case 0xff:  //00FF, (mode schip seulement) passe en vms.mode schip résolution 128x64
				break; // ignore ce code
			case 0x100: // 1NNN saut vers l'adresse NNN
				vms.pc=caddr(vms.b1,vms.b2);
				break;
			case 0x200: // 2NNN  appelle la sous-routine à l'adresse NNN
				vms.stack[++vms.sp]=vms.pc;
				vms.pc=caddr(vms.b1,vms.b2);
				break;
			case 0x300: // 3XKK   saute l'instruction suivante si VX == KK
				if (vms.var[x]==vms.b2) vms.pc+=2;
				break;
			case 0x400: // 4XKK  Saute l'instruction suivante si VX <> KK
				if (vms.var[x]!=vms.b2)vms.pc+=2;
				break;
			case 0x500: // 5XY0     Saute l'instruction suivante si VX == VY
				if (vms.var[x]==vms.var[y]) vms.pc+=2;
				break;
			case 0x600: // 6XKK     VX := KK
				vms.var[x]=vms.b2;
				break;
			case 0x700: // 7XKK     VX := VX + KK
				vms.var[x]+=vms.b2;
				break;
			case 0x800: // 8XY0     VX := VY
				vms.var[x]=vms.var[y];
				break;
			case 0x801: // 8XY1     VX := VX OR VY
				vms.var[x]|=vms.var[y];
				break;
			case 0x802: // 8XY2     VX := VX AND VY
				vms.var[x]&=vms.var[y];
				break;
			case 0x803: // 8XY3     VX := VX XOR VY
				vms.var[x]^=vms.var[y];
				break;
			case 0x804: // 8XY4     VX := VX + VY, VF := carry
				n=(vms.var[x]+vms.var[y])>255;
				vms.var[x]+=vms.var[y];
				vms.var[15]=n;
				break;
			case 0x805: // 8XY5     VX := VX - VY, VF := not borrow
				n=vms.var[x]>=vms.var[y];
				vms.var[x]-=vms.var[y];
				vms.var[15]=n;
				break;
			case 0x806: // 8XY6     VX := VX shr 1, VF := carry
				n=(vms.var[x]&1u);
				vms.var[x]>>=1;
				vms.var[15]=n;
				break;
			case 0x807: // 8XY7     VX := VY - VX, VF := not borrow
				n=vms.var[y]>=vms.var[x];
				vms.var[x]=vms.var[y]-vms.var[x];
				vms.var[15]=n;
				break;
			case 0x80e: // 8XYE     VX := VX shl 1, VF := carry
				n=(vms.var[x]&128)>>7;
				vms.var[x]<<=1;
				vms.var[15]=n;
				break;
			case 0x900: // 9XY0     Saute l'instruction suivante si VX <> VY
				if (vms.var[x]!=vms.var[y]) vms.pc+=2;
				break;
			case 0x901: // 9XY1  TONE VX, VY  joue une note de la gamme tempérée.
				key_tone(vms.var[x],vms.var[y],false);
				break;
			case 0x902: // 9XY2  PRT VX, VY  imprime le texte pointé par I. I est incrémenté.
				select_font(FONT_ASCII);
				set_cursor(vms.var[x],vms.var[y]);
				n=sram_read_byte(vms.ix++);
				while (n){
					put_char(n);
					n=sram_read_byte(vms.ix++);
				}
				break;
			case 0x903: // 9XY3 PIXI VX, VY  inverse le pixel aux coordonnées indiquées par VX,VY
				plot(vms.var[x],vms.var[y],INVERT);
				break;
			case 0x905: // 9XY5 TONE VX, VY, WAIT  joue une note de la gamme tempérée attend la fin avant de poursuivre
				key_tone(vms.var[x],vms.var[y],true);
				break;
			case 0xa00: // ANNN     I := NNN
				vms.ix=caddr(vms.b1,vms.b2);  // chip-8 et schip adressse de 12 bits
				vms.src_mem=RAM_MEM;
				break;
			case 0xb00: // BNNN     saut à NNN+V0
				vms.pc=vms.var[0]+caddr(vms.b1,vms.b2);
				break;
			case 0xc00: //CXKK VX=random_number&KK
				vms.var[x]=rand()&vms.b2;
				break;
			case 0xd00: //DXYN dessine un sprite
				n=vms.b2&0xf;
				if (!n){
					sram_load_block(vms.ix,32,block);
					vms.var[15]=put_big_sprite((int8_t)vms.var[x],(int8_t)vms.var[y],(const uint8_t*)block);
				}else{
					if (vms.src_mem==FLASH_MEM){
						vms.var[15]=put_sprite((int8_t)vms.var[x],(int8_t)vms.var[y],n,(const uint8_t *)vms.ix,FLASH_MEM);
					}else{
						sram_load_block(vms.ix,n,block);
						vms.var[15]=put_sprite((int8_t)vms.var[x],(int8_t)vms.var[y],n,(const uint8_t*)block,RAM_MEM);
					}
				}
				break;
			case 0xe9e: //EX9E, saute l'instruction suivante si la touche VX est enfoncée
				//n=keypad_read();
				//if (vms.var[x]==n) vms.pc+=2;
				if (key_down(vms.var[x])) vms.pc+=2;
				break;
			case 0xea1: //EXA1, saute l'instruction suivante si la touche VX n'est pas enfoncée
				//n=keypad_read();
				//if (vms.var[x]!=n) vms.pc+=2;
				if (!key_down(vms.var[x])) vms.pc+=2;
				break;
			case 0xf07: // FX07     VX := delay_cntr
				vms.var[x]=frame_delay;
				break;
			case 0xf0a: // FX0A     attend qu'une touche soit enfoncée et met sa valeur dans VX
				vms.var[x]=wait_key();
				break;
			case 0xf15: // FX15     démarre la minuterie delay_cntr avec la valeur de délais VX
				frame_delay=vms.var[x];
				break;
			case 0xf18: // FX18     fait entendre un beep d'une durée VX (multiple de 16.7 msec)
				tone(523,vms.var[x]);
				break;
			case 0xf1e: // FX1E     I := I + VX
				vms.ix += vms.var[x];
				break;
			case 0xf29: // FX29     fait pointé vms.ix vers le caractère VX dans la police FONT_SHEX
				vms.ix=(int16_t)font_hex_4x6+vms.var[x]*SHEX_HEIGHT;
				select_font(FONT_SHEX);
				vms.src_mem=FLASH_MEM;
				break;
			case 0xf30: // FX30 (schip)    fait pointé vms.ix vers le caractère dans  VX (0..9) pour la police FONT_LHEX
				vms.ix=(int16_t)font_hex_8x10+vms.var[x]*LHEX_HEIGHT;
				select_font(FONT_LHEX);
				vms.src_mem=FLASH_MEM;
				break;
			case 0xf33: // FX33     met la représentation BCD de VX dans M(vms.ix)..M(vms.ix+2)
				n=vms.var[x];
				block[2]=n%10;
				n /=10;
				block[1]=n%10;
				block[0]=n/10;
				sram_store_block(vms.ix,3,block);
				break;
			case 0xf55: // FX55     Sauvegarde les registres V0..VX dans la mémoire SRAM à l'adresse M(vms.ix)
				sram_store_block(vms.ix,x+1,vms.var);
				break;
			case 0xf65: // FX65     charge les registres V0..VX à partir de la mémoire SRAM à l'adresse M(vms.ix)
				sram_load_block(vms.ix,x+1,vms.var);
				break;
			case 0xf75: // FX75 (mode schip seulement) sauvegarde les registres V0..VX dans rpl
				for (n=0;n<=x;n++){
					vms.rpl[n]=vms.var[n];
				}
				break;
			case 0xf85: // FX85 (mode schip seulement) charge les registres V0..VX à partir de rpl
				for (n=0;n<=x;n++){
					vms.var[n]=vms.rpl[n];
				}
				break;
			default:
#if FW_DEBUG
				print_vms(PSTR("BAD OPCODE\n"));
#endif
				return CHIP_BAD_OPCODE;
		}//switch
	}//while(1)
}
