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
 * chip8.h
 *
 * Created: 2014-09-26 09:38:22
 *  Author: Jacques Deschênes
 */ 



#ifndef CHIP8_H_
#define CHIP8_H_

#define CODE_BASE_ADDR (0x200)

#define CHIP_CONTINUE (0)
#define CHIP_EXIT_OK  (1)
#define CHIP_BAD_OPCODE (2)
#define CHIP_BREAK (3)

#define F_RESET (2)
#define F_DEBUG (4)
#define F_TRACE (8)

typedef struct vm_state{
	uint16_t pc;
	uint16_t ix;
	uint8_t  sp;
	uint8_t  var[16];
	uint8_t  rpl[16];
	union {
		uint16_t opcode;	
	struct{	
		uint8_t b1;
		uint8_t b2;
		};
	};
	uint16_t stack[32];
	uint8_t  src_mem:1 ;
	uint8_t  debug:1;
	uint8_t  trace:1;
	}vm_state_t;


void print_vms(const char *msg);
	
uint8_t schip(uint8_t flags);
#if FW_DEBUG
void set_break_point(uint16_t addr);
void clr_break_point(uint16_t addr);
#endif
#endif /* CHIP8_H_ */