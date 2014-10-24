// déassembleur pour programme CHIP-8 et SCHIP

#include <stdlib.h>
#include <stdio.h>

#define ORG (0x200)
#define caddr(b1,b2) (((b1&0xf)<<8)+b2)
#define r1(b1)  (b1&0xf)
#define r2(b2)  ((b2&0xf0)>>4)


typedef  struct data_loc{
	int addr;
	int count;
} data_loc_t;

data_loc_t  data[100];
int dx=0;

unsigned char bytecode[4096];
unsigned char registers[16];

int  target[256]; // call and jumps target address
int  tx=0;

int search_for_data(int addr){
	int i;
	for(i=0;i<tx;i++){
		if (data[i].addr==addr) return i;
	}
	return -1;
}

int search_for_target(int addr){
	int i;
	for(i=0;i<tx;i++){
		if (target[i]==addr) return i;
	}
	return -1;
}

int load_bytecode(char *name){
	FILE *fp;
	int i;
	for (i=0;i<0x200;i++) bytecode[i]=0;
	fp=fopen(name,"rb");
	i=ORG;
	while (!feof(fp)){
		bytecode[i++]=fgetc(fp);
	}
	close(fp);
	return i;
}

int main(int argc, char *argv[]){
	FILE  *out;
	unsigned char b1,b2,n1,n2,n3, unknown=0;
	int size, i=0,j, ix;
	unsigned int pc=ORG;
	
	if (argc<3) exit(EXIT_FAILURE);
	size=load_bytecode(argv[1]);
	out=fopen(argv[2],"w");
	do {
		if ((i=search_for_data(pc))>-1){
			fprintf(out,"0x%04X\t\t\t.DB 0x%02X",pc,bytecode[pc++]);
			for (j=0;j<data[i].count;j++){
				fprintf(out,", 0x%02X",bytecode[pc++]);
			}
			fputs("\n",out);
			continue;
		}else{
			b1=bytecode[pc++];
			if (search_for_target(pc)>-1){
				fprintf (out,"0x%04X\t\t\t.DB 0x%02X\n",pc-1,b1);
				continue;
			}else{
				b2=bytecode[pc++];
				fprintf (out,"0x%04X\t0x%04X\t",pc-2,b1*256+b2);
				switch((b1&0xf0)>>4){
				case 0:
					if ((b2&0xf0)==0xc0){ // 00CN, (modes schip et xchip seulement) glisse l'affichage N lignes vers le bas  
						fprintf(out,"SCRLDN %d\n",b2&0xf);
					}else{
						switch (b2){
						case 0xE0:  // 00E0, efface l'écran
							fprintf(out,"CLS\n");
							break;
						case 0xEE: // 00EE, sortie de sous-routine
							fprintf(out, "RET\n");
							break;
						case 0xFB: // 00FB, (modes schip et xchip seulement) glisse l'affichage vers la droite de 4 pixels
							fprintf(out,"SCRLRT 4\n");
							break;
						case 0xFC: // 00FC, (modes schip et xchip seulement) glisse l'affichage vers la gauche de 4 pixels
							fprintf(out,"SCRLLT 4\n");
							break;
						case 0xFD:// 00FD, (modes schip et xchip seulement) sortie de l'interpréteur.
							fprintf(out,"EXIT CHIP_EXIT_OK\n");
							break;
						case 0xFE: // 00FE  revient au mode CHIP-8
							fprintf(out,"CHIP8\n");
							break;
						case 0xFF: // 00FF passe en mode SCHIP
							fprintf(out,"SCHIP\n");
							break;
						default:
							unknown=1;
							break;
						}//switch
					}
					break;
				case 1: // 1NNN saut vers l'adresse NNN
					if (search_for_target(caddr(b1,b2))==-1) target[tx++]=caddr(b1,b2);
					fprintf(out,"JMP 0x%03X\n",caddr(b1,b2));
					break;
				case 2: // 2NNN  appelle la sous-routine à l'adresse NNN
					if (search_for_target(caddr(b1,b2))==-1) target[tx++]=caddr(b1,b2);
					fprintf(out,"CALL 0x%03X\n",caddr(b1,b2));
					break;
				case 3: // 3XKK     saute l'instruction suivante si VX == KK 
					fprintf(out,"SKPE V%X==%d\n",r1(b1),b2);
					break;
				case 4: // 4XKK     Saute l'instruction suivante si VX <> KK 
					fprintf(out,"SKPNE V%X==%d\n",r1(b1),b2);
					break;
				case 5: // 5XY0     Saute l'instruction suivante si VX == VY
					fprintf(out,"SKPE V%X==V%X\n",r1(b1),r2(b2));
					break;
				case 6: // 6XKK     VX := KK 
					fprintf(out,"V%X = %d\n", r1(b1),b2);
					break;
				case 7: // 7XKK     VX := VX + KK
					fprintf(out,"V%X += %d\n", r1(b1),b2);
					break;
				case 8: 
					switch(b2&0xf){
					case 0: // 8XY0     VX := VY
						fprintf(out,"V%X = V%X\n",r1(b1),r2(b2));
						break;
					case 1: // 8XY1     VX := VX or VY
						fprintf(out,"V%X = V%X OR V%X\n", r1(b1),r1(b1),r2(b2));
						break;
					case 2: // 8XY2     VX := VX and VY
						fprintf(out,"V%X = V%X AND V%X\n", r1(b1),r1(b1),r2(b2));
						break;
					case 3: // 8XY3     VX := VX xor VY
						fprintf(out,"V%X = V%X XOR V%X\n", r1(b1),r1(b1),r2(b2));
						break;
					case 4: // 8XY4     VX := VX + VY, VF := carry 
						fprintf(out,"V%X = V%X + V%X\n", r1(b1),r1(b1),r2(b2));
						break;
					case 5: // 8XY5     VX := VX - VY, VF := not borrow 
						fprintf(out,"V%X = V%X - V%X\n", r1(b1),r1(b1),r2(b2));
						break;
					case 6: // 8XY6     VX := VX shr 1, VF := carry 
						fprintf(out,"V%X = V%X SHR 1\n",r1(b1),r1(b1));
						break;
					case 7: // 8XY7     VX := VY - VX, VF := not borrow
						fprintf(out,"V%X = V%X - V%X\n", r1(b1),r2(b2),r1(b1));
						break;
					case 0xe: // 8XYE     VX := VX shl 1, VF := carry 
						fprintf(out,"V%X = V%X SHL 1\n",r1(b1),r1(b1));
						break;
					default:
						unknown=1;
						break;
					}//switch
					break;
				case 9: // 9XY0     Saute l'instruction suivante si VX <> VY
					fprintf(out,"SKPNE  V%X, V%X\n",r1(b1),r2(b2));
					break;
				case 0xA: // ANNN     I := NNN 
					ix=caddr(b1,b2);
					if (search_for_data(ix)==-1){data[dx].addr=ix; data[dx++].count=0;}
					fprintf(out,"I = 0x%03X\n",caddr(b1,b2));
					break;
				case 0xB: // BNNN     saut à NNN+V0
					if (search_for_target(caddr(b1,b2))==-1) target[tx++]=caddr(b1,b2);	
					fprintf(out,"JUMP [V0+0x%03X]\n",caddr(b1,b2));
					break;
				case 0xC: //CXKK VX=random_number&KK
					fprintf(out,"V%X = RAND() AND 0x%02X\n",r1(b1),b2);
					break;
				case 0xD: //DXYN dessine le sprite pointé par I
					if ((i=search_for_data(ix))>-1) data[i].count=b2&0xf;
					fprintf(out,"SPRITE V%X, V%X, %d\n",r1(b1),r2(b2),b2&0xf);
					break;
				case 0xE:
					switch(b2){
					case 0x9e: //EX9E, saute l'instruction suivante si la touche VX est enfoncée
						fprintf(out,"SKPKD V%X\n", r1(b1));
						break;
					case 0xa1: //EXA1, saute l'instruction suivante si la touche VX n'est pas enfoncée
						fprintf(out,"SKPKU, V%X\n",r1(b1));
						break;
					default:
						unknown=1;
						break;
					}//swtich
					break;
				case 0xF:
					switch(b2){
					case 0x07: // FX07     VX := GAME_TIMER
						fprintf(out," V%X = GAME_TIMER\n", r1(b1));
						break;
					case 0x0a: // FX0A     attend qu'une touche soit enfoncée et met sa valeur dans VX
						fprintf(out,"V%X = WAIT_KEY\n",r1(b1));
						break;
					case 0x15: // FX15     démarre la minuterie GAME_TIMER avec la valeur de délais VX*16 
						fprintf(out,"GAME_TIMER = V%X\n", r1(b1));
						break;
					case 0x18: // FX18     fait entendre un beep d'une durée VX
						fprintf(out,"BEEP  V%X\n",r1(b1));
						break;
					case 0x1e: // FX1E     ix := ix + VX 
						fprintf(out,"I += V%X\n",r1(b1));
						break;
					case 0x29: // FX29     fait pointé ix vers le caractère VX dans la police FONT_SHEX
						fprintf(out,"I = &FONT3x5+(V%X)\n",r1(b1));
						break;
					case 0x30: // FX30 (schip,xchip)    fait pointé ix vers le caractère dans  VX (0..9) pour la police FONT_LHEX
						fprintf(out,"I = &FONT8x10+(V%X)\n",r1(b1));
						break;
					case 0x33: // FX33     met la représentation BCD de VX dans M(ix)..M(ix+2)
						if ((i=search_for_data(ix))>-1) data[i].count=3;	
						fprintf(out,"M(I)[] = BCD(V%X)\n", r1(b1));
						break;
					case 0x55: // FX55     Sauvegarde les registres V0..VX dans la mémoire SRAM à l'adresse M(ix)
						if ((i=search_for_data(ix))>-1) data[i].count=r1(b1);
						fprintf(out,"M(I)[] = V0..V%X\n", r1(b1));
						break;
					case 0x65: // FX65     charge les registres V0..VX à partir de la mémoire SRAM à l'adresse M(ix)	
						if ((i=search_for_data(ix))>-1) data[i].count=r1(b1);
						fprintf(out,"V0..V%X = M(I)\n",r1(b1));
						break;
					case 0x75: // FX75 (modes schip et xchip seulement) sauvegarde les registres V0..VX dans rpl  (X <= 7)
						fprintf(out," RPL[] =  V0..V%X\n",r1(b1));
						break;
					case 0x85: // FX85 (modes schip et xchip seulement) charge les registres V0..VX à partir de rpl (X <= 7)
						fprintf(out,"V0..V%X = RPL[0..%X]\n",r1(b1),r1(b1));
						break;
					default:
						unknown=1;
						break;
					}//switch
					break;
				default:
					unknown=1;
					break;
				}//switch
				if (unknown){
					fprintf(out,".DB 0x%02X, 0x%02X  ; not opcode\n",b1,b2);
					unknown=0;
				}
			}
		}
	} while (pc<(ORG+size));
	fputs("\n",out);
	fclose(out);
	return EXIT_SUCCESS;
}

