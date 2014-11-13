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
 * Date: 2014-11-06
 * Time: 17:06
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Threading;

namespace ccemul
{
	/// <summary>
	/// Description of ChipCon.
	/// </summary>
	/// 
	
	enum vm_error {VM_OK,BAD_OPCODE,INVALID_CHAR_VALUE,INVALID_TONE_VALUE,
		           PC_OUT_OF_BOUND,STACK_UNDERFLOW,STACK_OVERFLOW,VM_BREAK,VM_END}

	internal class ChipConVM
	{
		const int ORG=512;
		const int CODE_SIZE=4096;
		const int STACK_SIZE=32;
		const int MAX_BREAK=10;
		
		
		internal TVout tv;
		internal Text text;
		internal Tone tone;
		internal Keypad kpad;
		
		Random rnd=new Random();
		internal byte speed;
		internal byte dt; // durée délais jeux
		internal byte st; // durée tonalité
		internal bool codeInStore=false;
		internal ushort pc; // compteur ordinal
		internal short sp; // pointeur de pile
		internal ushort ix; // registre pointeur de donnée
		internal ushort[] stack=new ushort[STACK_SIZE]; //pile des retours
		internal byte[] var=new byte[16];  // variables
		internal byte[] rpl=new byte[16];  // sauvegarde des variables
		internal byte[] code= new byte[CODE_SIZE]; // espace de code programme.
		byte[] block= new byte[32]; // transfert sprite
		byte font_char=0;
		byte kx;
		BreakPoint[] breaks;
		
		internal ChipConVM()
		{
			tv=new TVout();
			tone=new Tone();
			kpad=new Keypad();
			sp=-1;
			pc=ORG;
			dt=0;
			st=0;
			breaks= new BreakPoint[MAX_BREAK] {null,null,null,null,null,null,null,null,null,null};
		}
		
		internal void load(byte[] program)
		{
			for (int i=0;i<program.Length;i++) code[i+ORG]=program[i];
			codeInStore=true;
			sp=-1;
			pc=ORG;
			dt=0;
			st=0;
			tv.cls();
			text.set_cursor(0,0);
			text.select_font(Text.FONT_ASCII);
			speed=0;
			ClearBreakPoints();
		}
		
		// machine virtuelle CHIPcon
		internal vm_error ccVM()
		{
			byte x,y,n,b1,b2;
			ushort addr,opCode;
			int j;
			
			if (kpad.fWaitKey){
				var[kx]=kpad.keypadRead();
				if (var[kx]<16)
					kpad.fWaitKey=false;
				else
				{
					speed=0;
					return vm_error.VM_OK;
				}
			}
			while (speed>0)
			{
				if (pc>(CODE_SIZE-2)){
					speed=0;
					return vm_error.PC_OUT_OF_BOUND;
				}
				if ((j=isBreakPoint(pc))>-1)
				{
					if (breaks[j].isEnabled())
					{
						breaks[j].disable();
						speed=0;
						return vm_error.VM_BREAK;
					    	
					}else
					{
						breaks[j].enable();
					}
				}
				b1=code[pc++];
				b2=code[pc++];
				addr=(ushort)(((b1&0xf)<<8)|b2);
				x=(byte)(b1&0xf);
				y=(byte)((b2&0xf0)>>4);
				// décodeur d'instruction
				opCode=(ushort)((b1&0xf0)<<4);
				switch (opCode){
					case 0x000:
						if ((b2&0xf0)==0xc0) opCode=0xc0; else opCode=b2;
						break;
					case 0xe00:
					case 0xf00:
						opCode |= b2;
						break;
					case 0x800:
					case 0x900:
						opCode |= (ushort)(b2&0xf);
						break;
				}
				// exécution de l'instruction
				switch(opCode){
					case 0xc0: // 00CN, (mode schip seulement) glisse l'affichage N lignes vers le bas
						tv.scrollDown(b2&0xf);
						break;
					case 0xe0: // 00E0, efface l'écran
						tv.cls();
						break;
					case 0xee: // 00EE, sortie de sous-routine
						if (sp==-1){
							return vm_error.STACK_UNDERFLOW;
						}
						pc=stack[sp--];
						break;
					case 0xfb: // 00FB, glisse l'affichage vers la droite de 4 pixels
						tv.scrollRight(4);
						break;
					case 0xfc: // 00FC, glisse l'affichage vers la gauche de 4 pixels
						tv.scrollLeft(4);
						break;
					case 0xfd:// 00FD, Sortie de l'interpréteur.
						speed=0;
						return vm_error.VM_END;
					case 0xfe: //00FE, revient au vms.mode par défaut chip-8 résolution 64x32
						break; // ignore ce code
					case 0xff:  //00FF, passe en vms.mode schip résolution 128x64
						break; // ignore ce code
					case 0x100: // 1NNN saut vers l'adresse NNN
						pc=addr;
						break;
					case 0x200: // 2NNN  appelle la sous-routine à l'adresse NNN
						if (sp+1==STACK_SIZE){
							return vm_error.STACK_OVERFLOW;
						}
						stack[++sp]=pc;
						pc=addr;
						break;
					case 0x300: // 3XKK   saute l'instruction suivante si VX == KK
						if (var[x]==b2) pc+=2;
						break;
					case 0x400: // 4XKK  Saute l'instruction suivante si VX <> KK
						if (var[x]!=b2)pc+=2;
						break;
					case 0x500: // 5XY0     Saute l'instruction suivante si VX == VY
						if (var[x]==var[y]) pc+=2;
						break;
					case 0x600: // 6XKK     VX := KK
						var[x]=b2;
						break;
					case 0x700: // 7XKK     VX := VX + KK
						var[x]= (byte)((var[x]+b2)&0xff);
						break;
					case 0x800: // 8XY0     VX := VY
						var[x]=var[y];
						break;
					case 0x801: // 8XY1     VX := VX OR VY
						var[x]|=var[y];
						break;
					case 0x802: // 8XY2     VX := VX AND VY
						var[x]&=var[y];
						break;
					case 0x803: // 8XY3     VX := VX XOR VY
						var[x]^=var[y];
						break;
					case 0x804: // 8XY4     VX := VX + VY, VF := carry
						if ((var[x]+var[y])>255) n=1; else n=0;
						var[x]=(byte)((var[x]+var[y])&0xff);
						var[15]=n;
						break;
					case 0x805: // 8XY5     VX := VX - VY, VF := not borrow
						if (var[x]>=var[y]) n=1; else n=0;
						var[x]=(byte)((var[x]-var[y])&0xff);
						var[15]=n;
						break;
					case 0x806: // 8XY6     VX := VX shr 1, VF := carry
						n=(byte)(var[x]&1);
						var[x]>>=1;
						var[15]=n;
						break;
					case 0x807: // 8XY7     VX := VY - VX, VF := not borrow
						if (var[y]>=var[x]) n=1;else n=0;
						var[x]=(byte)((var[y]-var[x])&0xff);
						var[15]=n;
						break;
					case 0x80e: // 8XYE     VX := VX shl 1, VF := carry
						n=(byte)((var[x]&128)>>7);
						var[x]=(byte)((var[x]<<1)&0xff);
						var[15]=n;
						break;
					case 0x900: // 9XY0     Saute l'instruction suivante si VX <> VY
						if (var[x]!=var[y]) pc+=2;
						break;
					case 0x901: // 9XY1  TONE VX, VY  joue une note de la gamme tempérée.
						if (var[x]>15){
							return vm_error.INVALID_TONE_VALUE;
						}
						tone.key_tone(var[x],var[y],false);
						break;
					case 0x902: // 9XY2  PRT VX, VY  imprime le texte pointé par I. I est incrémenté.
						text.select_font(Text.FONT_ASCII);
						text.set_cursor(var[x],var[y]);
						n=code[ix++];
						while (n>0){
							if (n-32>=Text.FONT_SIZE){
								return vm_error.INVALID_CHAR_VALUE;
							}
							text.put_char(n);
							n=code[ix++];
						}
						break;
					case 0x903: // 9XY3 PIXI VX, VY  inverse le pixel aux coordonnées indiquées par VX,VY
						tv.plot(var[x],var[y],eOP.eINVERT);
						break;
					case 0x905: // 9XY5 TONE VX, VY, WAIT  joue une note de la gamme tempérée attend la fin avant de poursuivre
						if (var[x]>15){
							return vm_error.INVALID_TONE_VALUE;
						}
						tone.key_tone(var[x],var[y],true);
						break;
					case 0xa00: // ANNN     I := NNN
						ix=addr;  // chip-8 et schip adressse de 12 bits
						break;
					case 0xb00: // BNNN     saut à NNN+V0
						pc=(ushort)(var[0]+addr);
						if (pc>=4096){
							return vm_error.PC_OUT_OF_BOUND;
						}
						break;
					case 0xc00: //CXKK VX=random_number&KK
						var[x]=(byte)(rnd.Next()&b2);
						break;
					case 0xd00: //DXYN dessine un sprite
						n=(byte)(b2&0xf);
						int cx,cy;
						if (var[x]>127) cx=-(256-var[x]);  else cx=var[x];
						if (var[y]>127) cy=-(256-var[y]); else cy=var[y];
						if (n==0){
							for (int i=0;i<32;i++) block[i]=code[i+ix];
							var[15]=tv.putSprite(cx,cy,16,16,block);
						}else{
							if (font_char==0) for (int i=0;i<n;i++)block[i]=code[ix+i];
							var[15]=tv.putSprite(cx,cy,8,n,block);
						}
						font_char=0;
						break;
					case 0xe9e: //EX9E, saute l'instruction suivante si la touche VX est enfoncée
						if (kpad.keyDown(var[x])) pc+=2;
						break;
					case 0xea1: //EXA1, saute l'instruction suivante si la touche VX n'est pas enfoncée
						if (!kpad.keyDown(var[x])) pc+=2;
						break;
					case 0xf07: // FX07     VX := dt
						var[x]=dt;
						break;
					case 0xf0a: // FX0A     attend qu'une touche soit enfoncée et met sa valeur dans VX
						var[x]=kpad.waitKey();
						if (var[x]==255){
							kpad.fWaitKey=true;
							kx=x;
							speed=0;
							return vm_error.VM_OK;
						}
						break;
					case 0xf15: // FX15     démarre la minuterie delay_cntr avec la valeur de délais VX
						dt=var[x];
						break;
					case 0xf18: // FX18     fait entendre un beep d'une durée VX (multiple de 16.7 msec)
						tone.play_tone(523,var[x],false);
						break;
					case 0xf1e: // FX1E     I := I + VX
						ix += var[x];
						break;
					case 0xf29: // FX29     fait pointé ix vers le caractère VX dans la police FONT_SHEX
						if (var[x]>15){
							return vm_error.INVALID_CHAR_VALUE;
						}
						text.select_font(Text.FONT_SHEX);
						for (int i=0;i<Text.SHEX_HEIGHT;i++)block[i]=text.font_hex_4x6[var[x]*Text.SHEX_HEIGHT+i];
						font_char=5;
						break;
					case 0xf30: // FX30 (schip)    fait pointé ix vers le caractère dans  VX (0..9) pour la police FONT_LHEX
						if (var[x]>15){
							return vm_error.INVALID_CHAR_VALUE;
						}
						text.select_font(Text.FONT_LHEX);
						for (int i=0;i<Text.LHEX_HEIGHT;i++)block[i]=text.font_hex_8x10[var[x]*Text.LHEX_HEIGHT+i];
						font_char=9;
						break;
					case 0xf33: // FX33     met la représentation BCD de VX dans M(ix)..M(ix+2)
						n=var[x];
						code[ix+2]=(byte)(n%10);
						n /=10;
						code[ix+1]=(byte)(n%10);
						code[ix]=(byte)(n/10);
						break;
					case 0xf55: // FX55     Sauvegarde les registres V0..VX dans la mémoire SRAM à l'adresse M(ix)
						for (int i=0;i<=x;i++){
							code[ix+i]=var[i];
						}
						break;
					case 0xf65: // FX65     charge les registres V0..VX à partir de la mémoire SRAM à l'adresse M(ix)
						for (int i=0;i<=x;i++){
							var[i]=code[ix+i];
						}
						break;
					case 0xf75: // FX75 (mode schip seulement) sauvegarde les registres V0..VX dans rpl
						for (int i=0;i<=x;i++){
							rpl[i]=var[i];
						}
						break;
					case 0xf85: // FX85 (mode schip seulement) charge les registres V0..VX à partir de rpl
						for (int i=0;i<=x;i++){
							var[i]=rpl[i];
						}
						break;
					default:
						return vm_error.BAD_OPCODE;
				}//switch
				speed--;
			}//while(speed>0)
			return vm_error.VM_OK;
		}

		
		
		internal void Reset()
		{
			sp=-1;
			pc=ORG;
			tv.cls();
			text.set_cursor(0,0);
			text.select_font(Text.FONT_ASCII);
			speed=0;
			kpad.keys_state=0;
			kpad.fWaitKey=false;
		}
		
		internal void AddBreakPoint(ushort addr)
		{
			BreakPoint bp=new BreakPoint(addr);
			
			for (int i=0;i<MAX_BREAK;i++)
			{
				if (breaks[i]==null)
				{
					breaks[i]=bp;
					return;
				}
			}
		}
		
		internal void RemoveBreakPoint(ushort addr)
		{
			for (int i=0;i<MAX_BREAK;i++)
				if (breaks[i].GetAddress()==addr)
				{	
					breaks[i]=null;
					return;
				}
		}
		
		internal void ClearBreakPoints()
		{
			for (int i=0;i<MAX_BREAK;i++) breaks[i]=null;
		}
		
		internal BreakPoint[] GetBreakPointList()
		{
			return breaks;
		}
		
		int isBreakPoint(ushort addr)
		{
			for (int i=0;i<MAX_BREAK;i++)
			{
				if (breaks[i]!=null && breaks[i].GetAddress()==addr) return i;
			}
			return -1;
		}
		
	}

	class BreakPoint
	{
		ushort address;
		bool enabled;
		
		internal BreakPoint(ushort addr)
		{
			address=addr;
			enabled=true;
		}
		
		internal ushort GetAddress()
		{
			return address;
		}
		
		internal bool AddressMatch(ushort addr)
		{
			return address==addr;
		}
		
		internal void enable()
		{
			enabled=true;
		}
		
		internal void disable()
		{
			enabled=false;
		}
		
		internal bool isEnabled()
		{
			return enabled;
		}
	}
		

}
