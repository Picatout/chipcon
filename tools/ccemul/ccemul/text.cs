﻿/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-07
 * Time: 11:47
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;

namespace ccemul
{
	/// <summary>
	/// définition polices de caractères
	/// et fonction d'impression écran
	/// </summary>
	

	
	public class Text
	{

		// police 4x5 hex
		const byte SHEX_HEIGHT=6;
		const byte SHEX_WIDTH=4;
		const byte FONT_SHEX=0;
		
		byte[] font_hex_4x6=new byte[16*SHEX_HEIGHT] {
		0xe0,0xa0,0xa0,0xa0,0xe0,0x00, //0
		0xc0,0x40,0x40,0x40,0xe0,0x00, //1
		0xe0,0x20,0xe0,0x80,0xe0,0x00, //2
		0xe0,0x20,0xe0,0x20,0xe0,0x00, //3
		0xa0,0xa0,0xe0,0x20,0x20,0x00, //4
		0xe0,0x80,0xe0,0x20,0xe0,0x00, //5
		0x80,0x80,0xe0,0xa0,0xe0,0x00, //6
		0xe0,0x20,0x20,0x20,0x20,0x00, //7
		0xe0,0xa0,0xe0,0xa0,0xe0,0x00, //8
		0xe0,0xa0,0xe0,0x20,0x20,0x00, //9
		0xe0,0xa0,0xe0,0xa0,0xa0,0x00, //A
		0xc0,0xa0,0xc0,0xa0,0xc0,0x00, //B
		0xe0,0x80,0x80,0x80,0xe0,0x00, //C
		0xc0,0xa0,0xa0,0xa0,0xc0,0x00, //D
		0xe0,0x80,0xe0,0x80,0xe0,0x00, //E
		0xe0,0x80,0xe0,0x80,0x80,0x00,  //F
		};
		
		// police 8x10 hex
		const byte LHEX_HEIGHT=10;
		const byte LHEX_WIDTH=8;
		const byte FONT_LHEX=1;
		
		byte[] font_hex_8x10=new byte[16*LHEX_HEIGHT]{
		0x7c,0x82,0x86,0x8a,0x92,0xa2,0xc2,0x82,0x7c,0x00, // 0
		0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x08,0x3e,0x00, // 1
		0x38,0x44,0x82,0x04,0x08,0x10,0x20,0x40,0xfe,0x00, // 2
		0x38,0x44,0x82,0x02,0x3c,0x02,0x82,0x44,0x38,0x00, // 3
		0x84,0x84,0x84,0x84,0x7e,0x04,0x04,0x04,0x04,0x00, // 4
		0x7e,0x80,0x80,0x80,0x7c,0x02,0x02,0x02,0xfe,0x00, // 5
		0x78,0x84,0x80,0x80,0xfc,0x82,0x82,0x82,0x7c,0x00, // 6
		0x7e,0x82,0x04,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // 7
		0x38,0x44,0x82,0x82,0x7c,0x82,0x82,0x44,0x38,0x00, // 8
		0x38,0x44,0x82,0x82,0x46,0x3a,0x02,0x04,0x38,0x00, // 9
		0x38,0x44,0x82,0x82,0xfe,0x82,0x82,0x82,0x82,0x00, // A
		0xfc,0x82,0x82,0x82,0xfc,0x82,0x82,0x82,0xfc,0x00, // B
		0x3e,0x40,0x80,0x80,0x80,0x80,0x80,0x40,0x3e,0x00, // C
		0xf8,0x84,0x82,0x82,0x82,0x82,0x82,0x84,0xf8,0x00, // D
		0xfe,0x80,0x80,0x80,0xfe,0x80,0x80,0x80,0xfe,0x00, // E
		0xfe,0x80,0x80,0x80,0xfe,0x80,0x80,0x80,0x80,0x00, // F
		};
		
		// police 6x8
		const byte FONT_SIZE=101;
		const byte CHAR_HEIGHT=8;
		const byte CHAR_WIDTH=6;
		const byte FONT_ASCII=2;
		
		const byte RECTANGLE=127;
		const byte RIGHT_ARROW= 128;
		const byte LEFT_ARROW=129;
		const byte UP_ARROW=130;
		const byte DOWN_ARROW=131;
		const byte CIRCLE=132;
		
		byte[] font_6x8= new byte[FONT_SIZE*CHAR_HEIGHT]{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // espace
		0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00, // !
		0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00, // "
		0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00, // #
		0x20,0x78,0xA0,0x70,0x28,0xF0,0x20,0x00, // $
		0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00, // %
		0x60,0x90,0xA0,0x40,0xA8,0x90,0x68,0x00, // &
		0x60,0x20,0x40,0x00,0x00,0x00,0x00,0x00, // '
		0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00, // (
		0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00, // )
		0x00,0x20,0xA8,0x70,0xA8,0x20,0x00,0x00, // *
		0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00, // +
		0x00,0x00,0x00,0x00,0x60,0x20,0x40,0x00, // ,
		0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00, // -
		0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00, // .
		0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00, // /
		0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00, // 0
		0x20,0x60,0x20,0x20,0x20,0x20,0xF8,0x00, // 1
		0x70,0x88,0x10,0x20,0x40,0x80,0xF8,0x00, // 2
		0xF0,0x08,0x08,0xF0,0x08,0x08,0xF0,0x00, // 3
		0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00, // 4
		0xF8,0x80,0x80,0xF0,0x08,0x08,0xF0,0x00, // 5
		0x30,0x40,0x80,0xF0,0x88,0x88,0x70,0x00, // 6
		0xF8,0x08,0x10,0x20,0x40,0x40,0x40,0x00, // 7
		0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
		0x70,0x88,0x88,0x70,0x08,0x08,0x70,0x00, // 9
		0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00, // :
		0x00,0x60,0x60,0x00,0x60,0x20,0x40,0x00, // ;
		0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00, // <
		0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00, // =
		0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00, // >
		0x70,0x88,0x08,0x10,0x20,0x00,0x20,0x00, // ?
		0x70,0x88,0x08,0x68,0xA8,0xA8,0x70,0x00, // @
		0x70,0x88,0x88,0xF8,0x88,0x88,0x88,0x00, // A
		0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00, // B
		0x78,0x80,0x80,0x80,0x80,0x80,0x78,0x00, // C
		0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00, // D
		0xF8,0x80,0x80,0xF8,0x80,0x80,0xF8,0x00, // E
		0xF8,0x80,0x80,0xF8,0x80,0x80,0x80,0x00, // F
		0x78,0x80,0x80,0xB0,0x88,0x88,0x70,0x00, // G
		0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00, // H
		0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // I
		0x78,0x08,0x08,0x08,0x08,0x90,0x60,0x00, // J
		0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00, // K
		0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00, // L
		0x88,0xD8,0xA8,0x88,0x88,0x88,0x88,0x00, // M
		0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00, // N
		0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // O
		0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00, // P
		0x70,0x88,0x88,0x88,0xA8,0x98,0x78,0x00, // Q
		0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00, // R
		0x78,0x80,0x80,0x70,0x08,0x08,0xF0,0x00, // S
		0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00, // T
		0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // U
		0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00, // V
		0x88,0x88,0x88,0xA8,0xA8,0xD8,0x88,0x00, // W
		0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00, // X
		0x88,0x88,0x88,0x50,0x20,0x20,0x20,0x00, // Y
		0xF8,0x10,0x20,0x40,0x80,0x80,0xF8,0x00, // Z
		0x60,0x40,0x40,0x40,0x40,0x40,0x60,0x00, // [
		0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00, // '\'
		0x18,0x08,0x08,0x08,0x08,0x08,0x18,0x00, // ]
		0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00, // ^
		0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00, // _
		0x40,0x20,0x10,0x00,0x00,0x00,0x00,0x00, // `
		0x00,0x00,0x70,0x08,0x78,0x88,0x78,0x00, // a
		0x80,0x80,0x80,0xB0,0xC8,0x88,0xF0,0x00, // b
		0x00,0x00,0x70,0x80,0x80,0x88,0x70,0x00, // c
		0x08,0x08,0x08,0x68,0x98,0x88,0x78,0x00, // d
		0x00,0x00,0x70,0x88,0xF8,0x80,0x70,0x00, // e
		0x30,0x48,0x40,0xE0,0x40,0x40,0x40,0x00, // f
		0x00,0x00,0x78,0x88,0x88,0x78,0x08,0x70, // g
		0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00, // h
		0x00,0x20,0x00,0x20,0x20,0x20,0x20,0x00, // i
		0x10,0x00,0x30,0x10,0x10,0x90,0x60,0x00, // j
		0x80,0x80,0x90,0xA0,0xC0,0xA0,0x90,0x00, // k
		0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // l
		0x00,0x00,0xD0,0xA8,0xA8,0x88,0x88,0x00, // m
		0x00,0x00,0xB0,0xC8,0x88,0x88,0x88,0x00, // n
		0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00, // o
		0x00,0x00,0xF0,0x88,0x88,0xF0,0x80,0x80, // p
		0x00,0x00,0x68,0x90,0x90,0xB0,0x50,0x18, // q
		0x00,0x00,0xB0,0xC8,0x80,0x80,0x80,0x00, // r
		0x00,0x00,0x70,0x80,0x70,0x08,0xF0,0x00, // s
		0x40,0x40,0xE0,0x40,0x40,0x48,0x30,0x00, // t
		0x00,0x00,0x88,0x88,0x88,0x98,0x68,0x00, // u
		0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00, // v
		0x00,0x00,0x88,0x88,0xA8,0xA8,0x50,0x00, // w
		0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00, // x
		0x00,0x00,0x88,0x88,0x88,0x78,0x08,0x70, // y
		0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00, // z
		0x20,0x40,0x40,0x80,0x40,0x40,0x20,0x00, // {
		0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00, // |
		0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00, // }
		0x00,0x00,0x40,0xA8,0x10,0x00,0x00,0x00, // ~
		0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC, // 95 rectangle
		0x40,0x20,0x10,0xF8,0x10,0x20,0x40,0x00,// 96 flèche droite
		0x10,0x20,0x40,0xF8,0x40,0x20,0x10,0x00,// 97 flèche gauche
		0x20,0x70,0xA8,0x20,0x20,0x20,0x00,0x00,// 98 flèche haut
		0x00,0x20,0x20,0x20,0xA8,0x70,0x20,0x00,// 99 flèche bas
		0x00,0x70,0xF8,0xF8,0xF8,0x70,0x00,0x00,// 100 rond			
		};

		byte font;
		byte xpos;
		byte ypos;
		
		ChipConVM vm;
		
		internal Text(MainForm Parent)
		{
			this.vm=Parent.vm;
			font=FONT_ASCII;
			xpos=0;
			ypos=0;
		}
		
		internal void select_font(byte font_id){
			font=font_id;
		}
		
		internal void new_line(){
			xpos=0;
			switch (font){
			case FONT_SHEX:
				if ((ypos+SHEX_HEIGHT)<=(TVout.VRES-SHEX_HEIGHT+1)){
					ypos+=SHEX_HEIGHT;
				}else{
					this.vm.tv.scrollUp(SHEX_HEIGHT);
				}
				break;
			case FONT_LHEX:
				if ((ypos+LHEX_HEIGHT)<=(TVout.VRES-LHEX_HEIGHT+1)){
					ypos+=LHEX_HEIGHT;
				}else{
					this.vm.tv.scrollUp(LHEX_HEIGHT);
				}
				break;
			case FONT_ASCII:
				if ((ypos+CHAR_HEIGHT)<=(TVout.VRES-CHAR_HEIGHT+1)){
					ypos+=CHAR_HEIGHT;
				}else{
					this.vm.tv.scrollUp(CHAR_HEIGHT);
				}
				break;	
			}//swtich
		
		}
		
		internal void put_char(byte c){
			byte[] character=new Byte[10];
			switch(font){
			case FONT_SHEX:	
				if (c<16){
					for (int i=0;i<SHEX_HEIGHT;i++) character[i]=font_hex_4x6[c*SHEX_HEIGHT+i];
					this.vm.tv.putSprite(xpos,ypos,8,SHEX_HEIGHT,character);
					xpos+=SHEX_WIDTH;
					if (xpos>(TVout.HRES-SHEX_WIDTH+1)){
						new_line();
					}
				}
				break;
			case FONT_LHEX:
				if (c<16){
					for (int i=0;i<LHEX_HEIGHT;i++) character[i]=font_hex_4x6[c*LHEX_HEIGHT+i];
					this.vm.tv.putSprite(xpos,ypos,LHEX_WIDTH,LHEX_HEIGHT,character);
					xpos+=LHEX_WIDTH;
					if (xpos>(TVout.HRES-LHEX_WIDTH+1)){
						new_line();
					}
				}
				break;
			case FONT_ASCII:
				if (c=='\n'){
					new_line();
				}else if ((c>31) && (c<(FONT_SIZE+32))){
					c-=32;
					for (int i=0;i<CHAR_HEIGHT;i++) character[i]=font_6x8[c*CHAR_HEIGHT+i];
					this.vm.tv.putSprite(xpos,ypos,8,CHAR_HEIGHT,character);
					xpos+=CHAR_WIDTH;
					if (xpos>(TVout.HRES-CHAR_WIDTH+1)){
						new_line();
					}
				}
				break;		
			}
		}
		
		internal void set_cursor(byte x, byte y){
			xpos=x;
			ypos=y;
		}
		
		ushort get_cursor(){
			return (ushort)((xpos<<8)+ypos);
		}
		
		internal void print(byte[] str){
			int i=0;
			while (str[i]>0) put_char(str[i++]);
		}
		
		
		internal void print_int(ushort n){
			byte[] nstr=new byte[12];
			byte i=11;
		
			do {
				nstr[i--]=(byte)(n%10+'0');
				n/=10;
			}while (n>0);
			put_char((byte)' ');
			i++;
			while (i<12) put_char(nstr[i++]);
		}
		
		internal void print_hex(ushort n,byte width){
			byte[] hex=new byte[4];
			int i=3,h;
			
			do{
				h=n&0xf;
				if (h<10) hex[i]=(byte)(h+'0'); else hex[i]=(byte)(h+'A'-10);
				n>>=4;
				i--;
				width--;
			}while (n>0);
			put_char((byte)' ');
			while (width>0){hex[i--]=(byte)'0'; width--;}
			i++;
			while (i<4) put_char(hex[i++]);
		}
		
		internal void cursor_left(){
			switch(font){
			case FONT_SHEX:
				if (xpos-SHEX_WIDTH>=0) xpos-=SHEX_WIDTH;
				break;
			case FONT_LHEX:
				if (xpos-LHEX_WIDTH>=0) xpos-=LHEX_WIDTH;
				break;
			case FONT_ASCII:
				if (xpos-CHAR_WIDTH>=0) xpos-=CHAR_WIDTH;
				break;	
			}
		}
		
		internal byte[] str2byte(string str)
		{
			byte[] newStr=new Byte[str.Length+1];
			for (int i=0;i<str.Length;i++) newStr[i]=(byte)str[i];
			return newStr;
		}

		
		/*
		internal void text_scroller(byte[] text, byte speed){
			byte c,j;
			int i=0;
			
			vm.tv.cls();
			select_font(FONT_ASCII);
			c=text[i++];
			while (c){
				set_cursor(0,4*CHAR_HEIGHT);
				while (c && c!='\n'){
					put_char(c);
					c=text[i++];
					if (keypad_break()) {goto break_out;}
				}
				for (j=0;j<CHAR_HEIGHT;j++){
					frame_delay=speed;
					while(frame_delay);
					scroll_up(1);
					if (keypad_break()) {goto break_out;}
				}
				c=text[i++];
			}//while
			for (c=0;c<4*CHAR_HEIGHT;c++){
				frame_delay=speed;
				while(frame_delay);
				scroll_up(1);
				if (keypad_break()) { break;}
			}//for
		break_out:	
			vm.tv.cls();
		}
		*/
		
	}
}