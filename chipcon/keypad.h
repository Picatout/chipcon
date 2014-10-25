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
 * keypad.h
 *
 * Created: 2014-10-03 12:29:38
 *  Author: Jacques Deschênes
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "hardware.h"

#define KP0  (0)
#define KP1  (1)
#define KP2  (2)
#define KP3  (3)
#define KP4  (4)
#define KP5  (5)
#define KP6  (6)
#define KP7  (7)
#define KP8  (8)
#define KP9  (9)
#define KPA  (10)
#define KPB  (11)
#define KPC  (12)
#define KPD  (13)
#define KPE  (14)
#define KPF  (15) 
#define NO_KEY (255)

#define wait_key_release()  while (keypad_read()!=NO_KEY)

void keypad_init();
uint8_t keypad_read();
uint8_t wait_key();
void prompt_key();
uint8_t keypad_break();
uint8_t key_down(uint8_t key);

#endif /* KEYPAD_H_ */
