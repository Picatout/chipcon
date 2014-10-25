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
 * sram.h
 *
 * Created: 2014-09-25 12:32:48
 *  Author: Jacques Deschênes
 */ 


#ifndef SRAM_H_
#define SRAM_H_

#include "hardware.h"


void sram_init();
void sram_wrmode(unsigned char mode);
unsigned char sram_rdmode();
void sram_write_byte(uint16_t addr, uint8_t data);
//void sram_write_word(uint16_t addr, uint16_t data);
uint8_t sram_read_byte(uint16_t addr);
uint16_t sram_read_word(uint16_t addr);
void sram_load_block(uint16_t addr,uint16_t size, uint8_t *b);
void sram_store_block(uint16_t addr, uint16_t size, uint8_t *b);
void sram_clear();

#endif /* SRAM_H_ */
