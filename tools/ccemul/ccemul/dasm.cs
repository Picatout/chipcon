/*---------------------------------------------------------------------------
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
*
*--------------------------------------------------------------------------
*/
/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-10
 * Time: 19:52
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;

namespace ccemul
{
	/// <summary>
	/// Description of dasm.
	/// </summary>
	public class dasm
	{
		public dasm()
		{
		}
		
		static internal string deassemble(ushort pc, ushort code)
		{
			byte b1,b2;
			string s;
			b1=(byte)(code>>8);
			b2=(byte)(code&0xff);
			
			s="";
			switch((b1&0xf0)>>4){
			case 0:
				if ((b2&0xf0)==0xc0){ // 00CN, glisse l'affichage N lignes vers le bas  
					s=string.Format("{0:S}SCD {1:D} ",s,b2&0xf);
				}else{
					switch (b2){
					case 0xE0:  // 00E0, efface l'écran
						s=string.Format("{0:S}CLS ",s);
						break;
					case 0xEE: // 00EE, sortie de sous-routine
						s=string.Format("{0:S}RET ",s);
						break;
					case 0xFB: // 00FB, glisse l'affichage vers la droite de 4 pixels
						s=string.Format("{0:S}SCR ",s);
						break;
					case 0xFC: // 00FC, glisse l'affichage vers la gauche de 4 pixels
						s=string.Format("{0:S}SCL ",s);
						break;
					case 0xFD:// 00FD, sortie de l'interpréteur.
						s=string.Format("{0:S}EXIT ",s);
						break;
					case 0xFE: // 00FE  revient au mode CHIP-8
						s=string.Format("{0:S}LOW ",s);
						break;
					case 0xFF: // 00FF passe en mode SCHIP
						s=string.Format("{0:S}HIGH ",s);
						break;
					default:
						s=string.Format("{0:S}??? ",s);
						break;
					}//switch
				}
				break;
			case 1: // 1NNN saut vers l'adresse NNN
				s=string.Format("{0:S}JP #{1:X3} ",s,((b1&0xf)<<8)|b2);
				break;
			case 2: // 2NNN  appelle la sous-routine à l'adresse NNN
				s=string.Format("{0:S}CALL #{1:X3} ",s,((b1&0xf)<<8)|b2);
				break;
			case 3: // 3XKK     saute l'instruction suivante si VX == KK 
				s=string.Format("{0:S}SE V{1:X}, {2:D2} ",s,(b1&0xf),b2);
				break;
			case 4: // 4XKK     Saute l'instruction suivante si VX <> KK 
				s=string.Format("{0:S}SNE V{1:X}, {2:D2} ",s,(b1&0xf),b2);
				break;
			case 5: // 5XY0     Saute l'instruction suivante si VX == VY
				s=string.Format("{0:S}SE V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
				break;
			case 6: // 6XKK     VX := KK 
				s=string.Format("{0:S}LD V{1:X}, {2:D2} ",s,(b1&0xf),b2);
				break;
			case 7: // 7XKK     VX := VX + KK
				s=string.Format("{0:S}ADD V{1:X}, {2:D2} ",s,(b1&0xf),b2);
				break;
			case 8: 
				switch(b2&0xf){
				case 0: // 8XY0     VX := VY
					s=string.Format("{0:S}LD V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 1: // 8XY1     VX := VX or VY
					s=string.Format("{0:S}OR V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 2: // 8XY2     VX := VX and VY
					s=string.Format("{0:S}AND V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 3: // 8XY3     VX := VX xor VY
					s=string.Format("{0:S}XOR V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 4: // 8XY4     VX := VX + VY, VF := carry 
					s=string.Format("{0:S}ADD V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 5: // 8XY5     VX := VX - VY, VF := not borrow 
					s=string.Format("{0:S}SUB V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 6: // 8XY6     VX := VX shr 1, VF := carry 
					s=string.Format("{0:S}SHR  V{1:X} ",s,b1&0xf);
					break;
				case 7: // 8XY7     VX := VY - VX, VF := not borrow
					s=string.Format("{0:S}SUBN V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 0xe: // 8XYE     VX := VX shl 1, VF := carry 
					s=string.Format("{0:S}SHL  V{1:X} ",s,b1&0xf);
					break;
				default:
					s=string.Format("{0:S}??? ",s);
					break;
				}//switch
				break;
			case 9:
				switch(b2&0xf){
				case 0:  // 9XY0     Saute l'instruction suivante si VX <> VY   
					s=string.Format("{0:S}SNE V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 1:  // 9XY1  tonalilité fréquence V[X], durée V[Y]
					s=string.Format("{0:S}TONE V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 2: // 9XY2  imprime chaine à l'écran
					s=string.Format("{0:S}PRT V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 3: // 9XY3  inverse pixel à la coordonnée V[X],V[Y]
					s=string.Format("{0:S}PIXI V{1:X}, V{2:X} ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				case 5: // tonalité  fréquence V[X], durée V[Y], attend la fin.
					s=string.Format("{0:S}TONE V{1:X}, V{2:X}, WAIT ",s,(b1&0xf),(b2&0xf0)>>4);
					break;
				}
				break;
			case 0xA: // ANNN     I := NNN 
				s=string.Format("{0:S}LD I, {1:X3} ",s,code&0xfff);
				break;
			case 0xB: // BNNN     saut à NNN+V0
				s=string.Format("{0:S}JP V0, {1:X3} ",s,code&0xfff);
				break;
			case 0xC: //CXKK VX=random_number&KK
				s=string.Format("{0:S}RND V{1:X}, {2:D3} ",s,b1&0xf,b2);
				break;
			case 0xD: //DXYN dessine le sprite pointé par I
				s=string.Format("{0:S}DRW V{1:X}, V{2:X}, {3:D} ",s,b1&0xf,(b2&0xf0)>>4,b2&0xf);
				break;
			case 0xE:
				switch(b2){
				case 0x9e: //EX9E, saute l'instruction suivante si la touche VX est enfoncée
					s=string.Format("{0:S}SKP V{1:X} ",s,b1&0xf);
					break;
				case 0xa1: //EXA1, saute l'instruction suivante si la touche VX n'est pas enfoncée
					s=string.Format("{0:S}SKNP V{1:X} ",s,b1&0xf);
					break;
				default:
					s=string.Format("{0:S}??? ",s);
					break;
				}//swtich
				break;
			case 0xF:
				switch(b2){
				case 0x07: // FX07     VX := DELAY_TIMER
					s=string.Format("{0:S}LD V{1:X}, DT ",s,b1&0xf);
					break;
				case 0x0a: // FX0A     attend qu'une touche soit enfoncée et met sa valeur dans VX
					s=string.Format("{0:S}LD V{1:X}, K ",s,b1&0xf);
					break;
				case 0x15: // FX15     démarre la minuterie DELAY_TIMER avec la valeur de délais VX*16 
					s=string.Format("{0:S}LD DT, V{1:X} ",s,b1&0xf);
					break;
				case 0x18: // FX18     charge sound timer avec VX
					s=string.Format("{0:S}LD ST, V{1:X} ",s,b1&0xf);
					break;
				case 0x1e: // FX1E     ix := ix + VX 
					s=string.Format("{0:S}ADD I, V{1:X} ",s,b1&0xf);
					break;
				case 0x29: // FX29     fait pointé ix vers le caractère VX dans la police FONT_SHEX
					s=string.Format("{0:S}LD F, V{1:X} ",s,b1&0xf);
					break;
				case 0x30: // FX30 (schip,xchip)    fait pointé ix vers le caractère dans  VX (0..9) pour la police FONT_LHEX
					s=string.Format("{0:S}LD LF, V{1:X} ",s,b1&0xf);
					break;
				case 0x33: // FX33     met la représentation BCD de VX dans M(ix)..M(ix+2)
					s=string.Format("{0:S}LD B, V{1:X} ",s,b1&0xf);
					break;
				case 0x55: // FX55     Sauvegarde les registres V0..VX dans la mémoire SRAM à l'adresse M(ix)
					s=string.Format("{0:S}LD [I], V{1:X} ",s,b1&0xf);
					break;
				case 0x65: // FX65     charge les registres V0..VX à partir de la mémoire SRAM à l'adresse M(ix)	
					s=string.Format("{0:S}LD V{1:X}, [I]  ",s,b1&0xf);
					break;
				case 0x75: // FX75 (modes schip et xchip seulement) sauvegarde les registres V0..VX dans rpl  (X <= 7)
					s=string.Format("{0:S}LD R, V{1:X} ",s,b1&0xf);
					break;
				case 0x85: // FX85 (modes schip et xchip seulement) charge les registres V0..VX à partir de rpl (X <= 7)
					s=string.Format("{0:S}LD V{1:X}, R  ",s,b1&0xf);
					break;
				default:
					s=string.Format("{0:S}??? ",s);
					break;
				}//switch
				break;
			default:
				s=string.Format("{0:S}??? ",s);
				break;
			}//switch
			return s;
		}
	}
}
