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
*   Nom:  ccasm.c
*   Description: assembleur pour la console CHIPcon
*   auteur: Jacques Deschenes
*   Date:  2014-10-28
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ORG (512)

typedef struct data_node{
	char *name;
	union {
		unsigned addr;
		unsigned pc;
		unsigned value;
	};
	struct data_node *next;
}node_t;

typedef enum token {eNONE,eSYMBOL,eLABEL,eNUMBER,eSTRING,eADDOP,eMULOP,eCOMMA,eLPAREN,eRPAREN,eLBRACKET,eRBRACKET} token_t;

token_t tok_id;
char tok_value[256];


node_t *label_list=NULL;
node_t *forward_list=NULL;
node_t *symbol_list=NULL;

#define add_label(name,addr)   add_node(name,addr,label_list)
#define add_forward_ref(name,pc) add_node(name,pc,forward_list)
#define add_symbol(name,value)  add_node(name,value,symbol_list)
#define search_label(name)   search_list(name,label_list)
#define search_ref(name)  search_list(name,forward_list)
#define search_symbol(name) search_list(name,symbol_list)


FILE *bin;

int pc; // compteur ordinal
int line_no; //no de ligne en cours d'analyse

#define MEM_SIZE (4096)   
unsigned char binary[MEM_SIZE];


int inp; // pointeur d'analyse ligne d'entrée
char line[256]; // contient la ligne à analyser

#define KW_COUNT (28)

const char *mnemonics[KW_COUNT]={"CLS","RET","SCR","SCL","EXIT","LOW","HIGH","SCD","JP","CALL",
						 "SHR","SHL","SKP","SKNP","SE","SNE","ADD","SUB","SUBN","OR","AND","XOR",
						 "RND","TONE","PRT","PIXI","LD","DRW"};

typedef enum Mnemo {eCLS,eRET,eSCR,eSCL,eEXIT,eLOW,eHIGH,eSCD,eJP,eCALL,eSHR,eSHL,eSKP,eSKNP,eSE,eSNE,eADD,
                    eSUB,eSUBN,eOR,eAND,eXOR,eRND,eTONE,ePRT,ePIXI,eLD,eDRW} mnemo_t;
						 
#define DIR_COUNT (4)						 
const char *directives[]={"DB","DW","ASCII","EQU"};


int search_word(char *target, const char *list[], int list_count){
	int i=0;
	while (i<list_count){
		if (!strcmp(target,list[i])) break;
		i++;
	}
	return i;
	
}

bool letter(char c){
	return ((c>='A') && (c<='Z')) || ((c>='a') && (c<='z'));
}

bool digit(char c){
	return (c>='0') && (c<='9');
}

bool alnum(char c){
	return letter(c) || digit(c);
}

bool hex(char c){
	return digit(c) || ((c>='A')&&(c<='F'));
}

bool identifier(char *name){
	if (!letter(*name)) return false;
	name++;
	while (*name){
		if (!(alnum(*name) || *name=='_')) return false;
		name++;
	}
	return true;
}

bool separator(char c){
	strchr("()[]+-*/%,",c);
}

bool match_vx(char *w){
	return (*w=='V') && hex(*(w+1)); 
}


void memory_overflow(){
	printf("CHIPcon program memory overflow at line %d\n", line_no);
	exit(EXIT_FAILURE);
}

void store_code(unsigned char b1, unsigned char b2){
#if defined DEBUG
	printf("%d\t%04X\t%02X %02X\n", line_no,pc,b1,b2);
#endif
	if (pc<4095){
		binary[pc++]=b1&0xff;
		binary[pc++]=b2&0xff;
	}else{
		memory_overflow();
	}
}




void next_token();
void parse_identifier();
unsigned get_number();
unsigned expression();


void error(){
	puts(line);
	printf("Syntax error at line %d, position %d\n",line_no,inp-strlen(tok_value));
	exit(EXIT_FAILURE);
}

//convertie une chaine hexadécimale
//en entier positif
int htoi(char *hnbr){
	unsigned int n=0;
	while (hex(*hnbr)){
		n*=16;
		n+=*hnbr<='9'?*hnbr-'0':*hnbr-'A'+10;
		hnbr++;
	}
	return n;
}

// convertie une chaine binaire
// en entier positif
int btoi(char *bnbr){
	unsigned int n=0;
	while ((*bnbr=='1')||(*bnbr=='.')||(*bnbr=='0')){
		n <<=1;
		n += *bnbr=='1';
		bnbr++;
	}
	return n;
}

//convertie un token numérique
// en entier positif
unsigned token_to_i(){
	switch(tok_value[0]){
	case '#':
		return htoi(tok_value+1);
	case '$':
		return btoi(tok_value+1);
	default:
		return atoi(tok_value);
	}
}


node_t *add_node(char *name, unsigned value, node_t *list){
	node_t *n;
	
	n=malloc(sizeof(node_t));
	n->next=NULL;
	if (list) n->next=list; else list=n;
#if defined DEBUG
	printf("%d\t%d\t%d\t%d\t%d\n",line_no,label_list,forward_list,symbol_list);
#endif	
	n->value=value;
	n->name=malloc(strlen(name)+1);
	strcpy(n->name,name);
	return n;
}

node_t *search_list(char *name, node_t *list){
	node_t *node;
	node=list;
	while (node){
#if defined DEBUG
	printf("%d ",node);
#endif	
		if (!strcmp(name,node->name)) break;
		node=node->next;
	}
#if defined DEBUG
	puts("");
#endif	
	return node;
}

int parse_vx(){
	next_token();
	if (!(tok_id==eSYMBOL && tok_value[0]=='V' && hex(tok_value[1]))) error();
	return tok_value[1]<='9'?tok_value[1]-'0':tok_value[1]-'A'+10;
}

// codes sans arguments
// "CLS","RET","SCR","SCL","EXIT","LOW","HIGH"
void op0(mnemo_t code){
	unsigned b1,b2;
	
	b1=0;
	switch (code){
	case eCLS: // CLS
		b2=0xe0;
		break;
	case eRET: // RET
		b2=0xee;
		break;
	case eSCR: // SCR
		b2=0xfb;
		break;
	case eSCL: // SCL
		b2=0xfc;
		break;
	case eEXIT: // EXIT
		b2=0xfd;
		break;
	case eLOW: // LOW
		b2=0xfe;
		break;
	case eHIGH: // HIGH
		b2=0xff;
		break;
	}
	store_code(b1,b2);
}

// codes avec 1 arguments
//"SCD","JP","CALL","SHR","SHL","SKP","SKNP" 
void op1(mnemo_t code){
	unsigned b1,b2;
	node_t *n;
	
	switch (code){
	case eSCD: // SCD
		b2=expression();
		b1=0;
		b2=0xc0|(b2&0xf);
		break;
	case eJP: // JP
		b2=0;
		next_token();
		if (tok_id!=eSYMBOL) error();
		if (!strcmp(tok_value,"V0")){
			b1=0xB0;
			next_token();
			if (tok_id!=eCOMMA) error();
			next_token();
			if (tok_id!=eSYMBOL) error();
			n=search_label(tok_value);
			if (n){
				b1|=(n->addr&0xf00)>>8;
				b2=n->addr&0xff;
			}else{
				forward_list=add_forward_ref(tok_value,pc);
			}
		}else{
			b1=0x10;
			n=search_label(tok_value);
			if (n){
				b1|=(n->addr&0xf00)>>8;
				b2=n->addr&0xff;
			}else{
				forward_list=add_forward_ref(tok_value,pc);
			}
		}
		break;
	case eCALL: // CALL
		b1=0x20;
		next_token();
		if (tok_id!=eSYMBOL) error();
		n=search_label(tok_value);
		if (n){
			b1|=(n->addr&0xf00)>>8;
			b2&=n->addr&0xff;
		}else{
			b2=0;
			forward_list=add_forward_ref(tok_value,pc);
		}
		break;
	case eSHR: // SHR
		b1=0x80|parse_vx();
		b2=6;
		break;
	case eSHL: // SHL
		b1=0x80|parse_vx();
		b2=6;
		break;
	case eSKP: // SKP
		b1=0xe0|parse_vx();
		b2=0x9e;
		break;
	case eSKNP: // SKNP
		b1=0xe0|parse_vx();
		b2=0xa1;
		break;
	}
	store_code(b1,b2);
}

// codes avec 2 arguments
//"SE","SNE","ADD","SUB","SUBN","OR","AND","XOR","RND","TONE","PRT"
void op2(unsigned code){
	unsigned b1,b2,mark;
	bool reg2;
	
	next_token();
	if (tok_id!=eSYMBOL) error();
	if (strlen(tok_value)==1 && tok_value[0]=='I'){
		// ADD I,VX  FX1E
		b2=0x1E;
		next_token();
		if (tok_id!=eCOMMA) error();
		b1=0xF0|parse_vx();
		goto op2_done;
	}else if (strlen(tok_value)==2 && tok_value[0]=='V' && hex(tok_value[1])){
		b1=(tok_value[1]<='9'?tok_value[1]-'0':tok_value[1]-'A'+10);
	}else error();
	next_token();
	if (tok_id!=eCOMMA) error();
	mark=inp;
	next_token();
	if ((tok_id==eSYMBOL) && strlen(tok_value)==2 && tok_value[0]=='V' && hex(tok_value[1])){
		reg2=true;
		b2=(tok_value[1]<='9'?tok_value[1]-'0':tok_value[1]-'A'+10)<<4;
	}else{
		reg2=false;
		inp=mark;
		b2=expression()&0xff;
	}
	switch (code){
	case eSE: // SE
		if (reg2){ // 5XY0
			b1|=0x50;
		}else{ // 3XKK
			b1|=0x30;
		}
		break;
	case eSNE: // SNE
		if (reg2){ // 9XY0
			b1|=0x90;
		}else{ // 4XKK
			b1|=0x40;
		}
		break;
	case eADD: // ADD
		if (reg2){ // 8XY4
			b1|=0x80;
			b2|=4;
		}else{
			b1|=0x70;
		}		
		break;
	case eSUB: // SUB  8XY5
		b1|=0x80;
		b2|=5;
		break;
	case eSUBN: // SUBN  8XY7
		b1|=0x80;
		b2|=7;
		break;
	case eOR: // OR 8XY1
		b1|=0x80;
		b2|=1;
		break; 
	case eAND: // AND 8XY2
		b1|=0x80;
		b2|=2;
		break;
	case eXOR: // XOR  8XY3
		b1|=0x80;
		b2|=3;
		break;
	case eRND: // RND CXKK
		b1|=0xC0;
		break;
	case eTONE: // TONE  9XY1
		b1|=0x90;
		b2|=1;
		break;
	case ePRT: // PRT  9XY2
		b1|=0x90;
		b2|=2;
		break;
	case ePIXI:
		b1|=0x90;
		b2|=3;
		break;
	}
op2_done:	
	store_code(b1,b2);
}

// DRW DXYN
void draw(){
	unsigned b1,b2,n;
	b1=0xD0|parse_vx();
	next_token();
	if (tok_id!=eCOMMA) error();
	b2=parse_vx()<<4;
	next_token();
	if (tok_id!=eCOMMA) error();
	n=expression();
	b2|=n&0xf;
	store_code(b1,b2);
}

// LD VX,KK  6XKK
// LD VX,VY  8XY0
// LD I,label  ANNN
// LD VX,DT  FX07
// LD DT,VX  FX15
// LD ST,VX  FX18
// LD F,VX   FX29
// LD LF,VX  FX30
// LD B,VX   FX33
// LD [I],VX FX55  
// LD VX,[I] FX65
// LD R,VX   FX75
// LD VX,R   FX85
// LD VX,K   FX0A
void load(){
	unsigned b1,b2,mark;
	node_t *n;
	char c=0,c1;
	
	next_token();
	if (tok_id==eLBRACKET){
		next_token();
		if (!(tok_id==eSYMBOL && strlen(tok_value)==1 && tok_value[0]=='I')) error();
		next_token();
		if (tok_id!=eRBRACKET) error();
	}else if (!((tok_id==eSYMBOL) && ((strlen(tok_value)==1)||(strlen(tok_value)==2)))) error();
	if (strlen(tok_value)==1){
		c=tok_value[0];
		next_token();
		if (tok_id!=eCOMMA) error();
		switch (c){
		case ']': //LD [I],VX  FX55
			b1=0xf0|parse_vx();
			b2=0x55;
			break;
		case 'I': // LD I,label  ANNN
			b1=0xa0;
			next_token();
			if (tok_id!=eSYMBOL) error();
			n=search_label(tok_value);
			if (n){
				b1|=(n->addr&0xf00)>>8;
				b2=n->addr&0xff;
			}else{
				b2=0;
				forward_list=add_forward_ref(tok_value,pc);
			}
			break;
		case 'R': // LD R,VX  FX75
			b1=0xf0|parse_vx();
			b2=0x75;
			break;
		case 'F': // LD F,VX  FX29
			b1=0xf0|parse_vx();
			b2=0x29;
			break;
		case 'B': // LD B,VX  FX33
			b1=0xf0|parse_vx();
			b2=0x33;
			break;
		default:
			error();
		}
		goto load_done;
	}else if (strlen(tok_value)==2){
		if (tok_value[0]=='V' && hex(tok_value[1])){
			c=tok_value[1];
			b1=c<='9'?c-'0':c-'A'+10;
			next_token();
			if (tok_id!=eCOMMA) error();
			mark=inp;
			next_token();
			if (tok_id==eNUMBER || ((tok_id==eSYMBOL) && (search_symbol(tok_value)))){
				inp=mark;
				b2=expression()&0xff;
				b1|=0x60;
				goto load_done;
			}
			if (tok_id==eLBRACKET || (tok_id==eSYMBOL  && strlen(tok_value)==1)){
				c=tok_value[0];
				switch (c){
				case 'R': // LD VX,R  FX85
					b1|=0xf0;
					b2=0x85;
				case 'K': // LD VX,K  FX0A
					b1|=0xf0;
					b2=0x0a;
				case '[':  // LD VX,[I] FX65
					next_token();
					if (!(tok_id==eSYMBOL && strlen(tok_value)==1 && tok_value[0]=='I')) error();
					next_token();
					if (tok_id!=eRBRACKET) error();
					b1|=0xf0;
					b2=0x65;
				default:
					error();
				}
				goto load_done;
			}
			if (tok_id==eSYMBOL && strlen(tok_value)==2){
				if (!strcmp(tok_value,"DT")){ // LD VX,DT FX07
					b1|=0xf0;
					b2=0x07;
				}else if (tok_value[0]='V' && hex(tok_value[1])){ // LD VX,VY  8XY0
					b1|=0x80;
					c=tok_value[1];
					b2=(c<='9'?c-'0':c-'A'+10)<<4;
				}else error();
				goto load_done;
			}
			error();
		}else{
			c=tok_value[0];
			c1=tok_value[1];
			next_token();
			if (tok_id!=eCOMMA) error();
			b1=0xf0|parse_vx();
			switch(c){
			case 'D':  // LD DT,VX   FX15
				if (c1!='T') error();
				b2=0x15;
				break;
			case 'L': // LD LF,VX   FX30
				if (c1!='F') error();
				b2=0x30;
				break;
			case 'S': // LD ST, VX  FX18
			    if (c1!='T') error();
				b2=0x18;
				break;
			default:
				error();
			}
		}
	}else error();
load_done:	
	store_code(b1,b2);
}

void usage(){
	puts("CHIPcon assembler");
	puts("USAGE: ccasm source binary");
	puts("'source' is CHIPcon source code file.");
	puts("'binary' is generated binary file to be executed on CHIPcon.");
	exit (EXIT_FAILURE);
}


bool space(char c){
   return (c==' ')||(c=='\t')||(c=='\n');
}

void skip_white(){
	while(line[inp] && space(line[inp])) inp++;
}

unsigned parse_string(){
	bool quote=false;
	bool escape=false;
	unsigned i=0;
	
	while (line[inp] && !quote){
		switch(line[inp]){
		case '\\':
			if (!escape) escape=true;else{tok_value[i++]=line[inp];escape=false;}
			break;
		case 'n':
			if (!escape) tok_value[i++]=line[inp];else{tok_value[i++]='\n';escape=false;}
			break;
		case '"':
			if (!escape) quote=true;else{tok_value[i++]=line[inp];escape=false;} 
			break;
		default:
			if ((line[inp]<32)||line[inp]>127) error();else{tok_value[i++]=line[inp];} 
			break;
		}//switch
		inp++;
	}//while
	if (quote){tok_value[i]=0;inp--;} else error();
	return i;
}

void next_token(){
	unsigned state=0;
	unsigned i=0;
	
	skip_white();
	tok_id=eNONE;
	tok_value[0]=0;
	while ((state<5) && line[inp] && !space(line[inp])){
		switch(state){
		case 0:
			switch(line[inp]){
			case ';':
				tok_id=eNONE;
				tok_value[i++]=0;
				inp=strlen(line);
				state=5;
				break;
			case '+':
			case '-':
				tok_id=eADDOP;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case '*':
			case '/':
			case '%':
				tok_id=eMULOP;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case '[':
				tok_id=eLBRACKET;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case '(':
				tok_id=eLPAREN;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case ')':
				tok_id=eRPAREN;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case ']':
				tok_id=eRBRACKET;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case ',':
				tok_id=eCOMMA;
				tok_value[i++]=line[inp];
				state=5;	
				break;
			case '"':
				inp++;
				i=parse_string();
				tok_id=eSTRING;
				state=5;
				break;
			case '#':
				tok_id=eNUMBER;
				tok_value[i++]=line[inp];
				state=1; // nombre hexadécimal
				break;
			case '$':
				tok_id=eNUMBER;
				tok_value[i++]=line[inp];
				state=2; // nombre binaire
			default:
				if (letter(line[inp])||(line[inp]=='_')){
					tok_id=eSYMBOL;
					tok_value[i++]=toupper(line[inp]);
					state=4; // symbole alphanumérique
				}else if (digit(line[inp])){
					tok_id=eNUMBER;
					tok_value[i++]=line[inp];
					state=3; // nombre décimal
				}
			}//switch
			break;
		case 1: // nombre hexadécimal
			if (hex(line[inp])){
				tok_value[i++]=toupper(line[inp]);
			}else if (separator(line[inp])){
				inp--;
				state=5;
			}else{
				error();
			}
			break;
		case 2: // nombre binaire
			if ((line[inp]=='1') || (line[inp]=='0') || (line[inp]=='.')){
				tok_value[i++]=line[inp];
			}else if (separator(line[inp])){
				inp--;
				state=5;
			}else{
				error();
			}
			break;
		case 3: // nombre décimal
			if (digit(line[inp])){
				tok_value[i++]=line[inp];
			}else if (separator(line[inp])){
				inp--;
				state=5;
			}else{
				error();
			}
			break;
		case 4: // symbole alphanumérique
			if (alnum(line[inp]) || line[inp]=='_'){
				tok_value[i++]=toupper(line[inp]);
			}else if (line[inp]==':'){
				tok_id=eLABEL;
				inp--;
				state=5;
			}else if (separator(line[inp])){
				inp--;
				state=5;
			}else{
				error();
			}
			break;

		}//switch
		inp++;
	}//while
	tok_value[i]=0;
}

void data_byte(){
	unsigned n;
	
	do{
	   n=expression();
	   binary[pc++]=n&0xff;
	   next_token();
	   if (tok_id && tok_id!=eCOMMA) error();	
   }while((pc<MEM_SIZE) && tok_id);
}

void data_word(){
	unsigned n;
	
	do{
		n = expression();
		binary[pc++]=(n>>8)&0xff;
		binary[pc++]=n&0xff;
		next_token();
		if (tok_id && tok_id!=eCOMMA) error();
	}while((pc<MEM_SIZE-1) && tok_id);
}


void data_ascii(){
	unsigned i=0;
	
	next_token();
	if (tok_id!=eSTRING) error();
	while ((pc<MEM_SIZE-1) && tok_value[i]) binary[pc++]=tok_value[i++];
	if (pc<MEM_SIZE) binary[pc++]=0; else {puts("memory overflow");error();}
}


void equate(){
	node_t *symbol;
	
	next_token();
	if (tok_id==eSYMBOL){
		symbol=search_symbol(tok_value);
		if (!symbol){ symbol = add_symbol(tok_value,0);}
		symbol->value = expression();
		symbol_list=symbol;
	}else{
		error();
	}
}

unsigned factor(){
	unsigned n;
	node_t *symbol;
	
	next_token();
	switch(tok_id){
	case (eNUMBER):
		n=token_to_i();
		break;
	case (eSYMBOL):
		symbol=search_symbol(tok_value);
		if (symbol){
			n=symbol->value;
		}else{
			puts("Unknown symbol");
			error();
		}
		break;
	case (eLPAREN):
		n=expression();
		next_token();
		if (tok_id!=eRPAREN) error();
		break;
	default:
		error();
	}
#if defined DEBUG
	printf("%d\tfactor=%d\n",line_no,n);
#endif	
	return n;
}

unsigned term(){
	unsigned n, mark;

	n=factor();
	mark=inp;
	next_token();
	while (tok_id==eMULOP){
		switch (tok_value[0]){
		case '*':
			n*=factor();
			break;
		case '/':
			n/=factor();
			break;
		case '%':
			n%=factor();
			break;
		default:
			error();
		}//switch
		mark=inp;
		next_token();
	}//while
	if (tok_id) inp=mark;
#if defined DEBUG
	printf("%d\tterm=%d\n",line_no,n);
#endif	
	return n;
}


unsigned expression(){
	unsigned n,mark;
	
	n=term();
	mark=inp;
	next_token();
	while (tok_id==eADDOP){
		switch(tok_value[0]){
		case '+':
			n += term();
			break;
		case '-':
			n -= term();
			break;
		}//switch
		mark=inp;
		next_token();
	}//while
	if (tok_id) inp=mark;
#if defined DEBUG
	printf("%d\texpression=%d\n",line_no,n);
#endif	
	return n;
}

void parse_line(){
	int i;
		next_token();
		while (tok_id){
			if ((tok_id==eSYMBOL) && (i=search_word(tok_value,mnemonics,KW_COUNT))<KW_COUNT){
				//operation code
				switch(i){
				case eCLS:
				case eRET:
				case eSCL:
				case eEXIT:
				case eLOW:
				case eHIGH:
					op0(i);
					break;
			    case eSCD:
				case eJP:
				case eCALL:
				case eSHR:
				case eSHL:
				case eSKP:
				case eSKNP:
					op1(i);
					break;
				case eSE:
				case eSNE:
				case eADD:
                case eSUB:
				case eSUBN:
				case eOR:
				case eAND:
				case eXOR:
				case eRND:
				case eTONE:
				case ePRT:
				case ePIXI:
					op2(i);
					break;
				case eDRW:
					draw();
					break;
				case eLD:
					load();
					break;
				}
			}else if ((tok_id==eSYMBOL) && (i=search_word(tok_value,directives,DIR_COUNT))<DIR_COUNT){
				// assembler directive
				switch(i){
				case 0:
					data_byte();
					break;
				case 1:
					data_word();
					break;
				case 2:
					data_ascii();
					break;
				case 3:	
					equate();
					break;
				}
			}else if (tok_id==eLABEL){
				// label
				inp++;
				label_list=add_label(tok_value,pc);
			}else{
				//unknown code
				error();
			}
			next_token();
		}//while
	
}

void fix_forward_ref(){
	node_t *fwd;
	node_t *lbl;
	
	fwd=forward_list;
#if defined DEBUG
	printf("fwd=%d\n",fwd);
#endif	
	while (fwd && pc<MEM_SIZE){
		lbl=search_label(fwd->name);
		if (lbl){
			binary[fwd->pc] |= (lbl->addr&0xf00)>>8;
			binary[fwd->pc+1] = lbl->addr&0xff;
		}else{
			printf("undefined reference: %s\n", fwd->name);
			exit(EXIT_FAILURE);
		}
		fwd=fwd->next;
#if defined DEBUG
	printf("fwd=%d\n",fwd);
#endif	
	}
}

int main(int argc, char **argv){
	FILE *src;
	
	if (argc < 3) usage();
	if (!(src=fopen(argv[1],"r"))){ printf("Failed to open %s\n",argv[1]); exit(EXIT_FAILURE);}
	if (!(bin=fopen(argv[2],"wb"))) {printf("Failed to open %s\n",argv[2]); exit(EXIT_FAILURE);}
     
	pc=ORG;
	memset(line,0,256);
	line_no=0;
	while (pc<MEM_SIZE && fgets(line,256,src)){
		line_no++;
		line[strlen(line)-1]=0;
		inp=0;
		if (strlen(line)) parse_line();
		memset(line,0,256);
	}
	if (pc>4095 && !feof(src)) memory_overflow();
	fclose(src);
	fix_forward_ref();
	int i;
	for (i=ORG;i<pc;i++){
		fputc(binary[i],bin);
	}
	fclose(bin);
	printf("Total lines read: %d\ncode size: %d\n",line_no, pc);
	return EXIT_SUCCESS;
}
