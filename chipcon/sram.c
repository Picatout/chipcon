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
 * sram.c
 *
 * Created: 2014-09-25 12:33:24
 *  Author: Jacques Deschênes
 */ 

#include "hardware.h"
//#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#include "sram.h"

#define sram_enable()  {SRAM_PORT &= ~SRAM_SEL;}
#define sram_disable() {SRAM_PORT |= SRAM_SEL;}


inline void sram_send_byte(unsigned char b){
	SPI_SR|=(1<<SPI2X);
	SPI_DR = b;
	while (!(SPI_SR&(1<<SPIF)));
}

#define sram_cmd(cmd, addr) sram_send_byte(cmd);\
                            sram_send_byte(addr>>8);\
							sram_send_byte(addr&0xff)
							
							
inline void sram_write_block(uint8_t *block,uint16_t size){
	uint16_t i;
	for (i=0;i<size;i++) sram_send_byte(*block++);
}

inline void sram_read_block(uint8_t *block, uint16_t size){
	while (size){
		sram_send_byte(0);
		*block++=SPI_DR;
		--size;
	}
}

void sram_wrmode(unsigned char mode){
	sram_enable();
	sram_send_byte(SRAM_WRMR);
	sram_send_byte(mode);
	sram_disable();
}

unsigned char sram_rdmode(){
	sram_enable();
	sram_send_byte(SRAM_RDMR);
	sram_send_byte(0);
	sram_disable();
	return SPI_DR;
}

void sram_clear(){
	uint32_t i;

	sram_enable();
	sram_cmd(SRAM_WRITE,0);
	for (i=0;i<SRAM_SIZE;i++){
		sram_send_byte(0);
	}
	sram_disable();

}

// initialisation de la SRAM
void sram_init(){
	SRAM_DDR |= SRAM_SEL;
	sram_disable(); 
	//met la SRAM en mode séquentiel
	sram_wrmode(SRAM_SQMD);
	//efface la SRAM
	sram_clear();
}


// écriture d'un octet dans la SRAM
void sram_write_byte(uint16_t addr, uint8_t data){
	sram_enable();
	sram_cmd(SRAM_WRITE,addr);
	sram_send_byte(data);
	sram_disable();
}
/*
void sram_write_word(uint16_t addr, uint16_t data){
}
*/

// lecture d'un octet de la SRAM
uint8_t sram_read_byte(uint16_t addr){
	sram_enable();
	sram_cmd(SRAM_READ,addr);
	sram_send_byte(0);
	sram_disable();
	return SPI_DR;
}

// lecture d'un mot 16 bits
// ordre little indian
uint16_t sram_read_word(uint16_t addr){
	uint16_t w;
	sram_enable();
	sram_cmd(SRAM_READ,addr);
	sram_send_byte(0);
	w=SPI_DR;
	sram_send_byte(0);
	w |= SPI_DR<<8;
	sram_disable();
	return w;
}

// charge un bloc de mémoire SRAM dans un variable tableau.
void sram_load_block(uint16_t addr,uint16_t size, uint8_t *b){
	sram_enable();
	sram_cmd(SRAM_READ,addr);
	sram_read_block(b,size);
	sram_disable();
}

// transfert le contenu d'une variable tableau dans 
// la mémoire SRAM
void sram_store_block(uint16_t addr, uint16_t size, uint8_t *b){
	sram_enable();
	sram_cmd(SRAM_WRITE,addr);
	sram_write_block(b,size);
	sram_disable();
}

