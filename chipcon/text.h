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
 * text.h
 *
 * Created: 2014-09-30 22:31:06
 *  Author: Jacques Deschênes
 */ 


#ifndef TEXT_H_
#define TEXT_H_


// police 4x5 hex
#define SHEX_HEIGHT (6)
#define SHEX_WIDTH (4)
#define FONT_SHEX  (0)

extern const uint8_t font_hex_4x6[16*SHEX_HEIGHT];

// police 8x10 hex
#define LHEX_HEIGHT (10)
#define LHEX_WIDTH (8)
#define FONT_LHEX (1)

extern const uint8_t font_hex_8x10[16*LHEX_HEIGHT];

// police 6x8
#define FONT_SIZE   (101)
#define CHAR_HEIGHT (8)
#define CHAR_WIDTH  (6)
#define FONT_ASCII  (2)

#define RECTANGLE   (127)
#define RIGHT_ARROW (128)
#define LEFT_ARROW  (129)
#define UP_ARROW    (130)
#define DOWN_ARROW  (131)
#define CIRCLE      (132)

extern const uint8_t font_6x8[FONT_SIZE*CHAR_HEIGHT];

// détermine la police active
void select_font(uint8_t font_id);
//imprime un caractère à l'écran
void put_char(uint8_t c);
//positionne le curseur texte à l'écran
void set_cursor(uint8_t x, uint8_t y);
// retourne la position du curseur texte
// entier 16 bits = x<<8|y
uint16_t get_cursor();
void new_line();
//imprime une chaine à l'écran
void print(const unsigned char* str);
//imprime une chaine qui est dans la mémoire flash
void prt_pstr(const char *str);
// imprime un entier
void print_int(uint16_t n);
// imprime un entier en hexadécimal
void print_hex(uint16_t n,uint8_t width);
// déplace le curseur ver la gauche
void cursor_left();
// défile un text à l'écran
// le texte doi-être en mémoire flash
void text_scroller(const uint8_t *text, uint8_t speed);
#endif /* TEXT_H_ */
