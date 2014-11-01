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
	}
	struct data_node *next;
}node_t;

typedef enum token {eNONE,eSYMBOL,eNUMBER,eSTRING,eADDOP,eMULOP,eCOMMA,eLPAREN,eRPAREN,eLBRACKET,eRBRACKET} token_t;

token_t tok_id;
char tok_value[256];


node_t *label_list=NULL;
node_t *forward_list=NULL;
node_t *symbol_list=NULL;

#define add_label(name,addr)   add_node(name,addr,label_list)
#define add_forward_ref(name,pc) add_node(name,pc,forward_list)
#define add_symbol(name,value)  add_node(name,value,symbol_symb)
#define search_label(name)   search_list(name,label_list)
#define search_ref(name)  search_list(name,forward_ref)
#define search_symbol(name) search_list(name,symbol_list)


FILE *bin;

int pc; // compteur ordinal
int line_no; //no de ligne en cours d'analyse

#define MEM_SIZE (4096)   
unsigned char binary[MEM_SIZE];


int inp; // pointeur d'analyse ligne d'entrée
char line[256]; // contient la ligne à analyser

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
	strchar("()[]+-*/%,:",c);
}

bool match_vx(char *w){
	return (*w=='V') && hex(*(w+1)); 
}

int get_register(){
	return line[start+1]<='9'?line[start+1]-'0':line[start+1]-'A'+10;
}

void memory_overflow(){
	printf("CHIPcon program memory overflow at line %d\n", line_no);
	exit(EXIT_FAILURE);
}

void store_code(unsigned char b1, unsigned char b2){
	if (pc<4095){
		binary[pc++]=b1&0xff;
		binary[pc++]=b2&0xff;
	}else{
		memory_overflow();
	}
}

#define KW_COUNT (27)

typedef  void (*kw_fn)();

void next_token();
void parse_identifier();
unsigned get_number();
unsigned expression();

void error(){
	puts(line);
	printf("Syntax error at line %d, position %d\n",line_no,start+1);
	exit(EXIT_FAILURE);
}

node_t *add_node(char *name, unsigned value, node_t *list){
	if (list==NULL){
		list=malloc(sizeof(node_t));
		list->next=NULL;
	}else{
		list->next=list;
	}
	list->value=value;
	list->name=malloc(strlen(name)+1);
	strcpy(list->name,name);
	return list;
}

node_t *search_list(char *name, node_t *list){
	node_t *node;
	node=list;
	while (node){
		if (!strcmp(name,node->name)) break;
		node=node->next;
	}
	return node;
}


void kw_scd(){
	int i;
	next_token();
	i=atoi(&line[start]);
	store_code(0,0xc0+(i&0xf));
}

void kw_cls(){
	store_code(0,0xe0);
}

void kw_ret(){
	store_code(0,0xee);
}

void kw_scr(){
	store_code(0,0xfb);
}

void kw_scl(){
	store_code(0,0xfc);
}

void kw_exit(){
	store_code(0,0xfd);
}

void kw_low(){
	store_code(0,0xfe);
}

void kw_high(){
	store_code(0,0xff);
}

void kw_jp(){
	unsigned b1,b2;
	label_t *lbl;
	next_token();
	if (!strcmp("V0",&line[start])){
		b1=0xB0;
		next_token();
		b2=get_number();
		b1|=(b2&0xf00)>>8;
		b2&=0xff;
	}else if (letter(line[start])){
			lbl=search_label(&line[start]);
			if (lbl){
				b1=0x10+((lbl->addr&0xf00)>>8);
				b2=lbl->addr&0xff;	
			}else{
				add_forward_ref();
				b1=0x10;
				b2=0;
			}
			}else{
				b2=get_number();
				b1=0x10|((b2&0xf00)>>8);
				b2&=0xff;
			}
	
	store_code(b1,b2);
}

void kw_call(){
	unsigned b1,b2;
	label_t *lbl;
	next_token();
	if (letter(line[start])){
		lbl=search_label(&line[start]);
		if (lbl){
			b1=0x20+((lbl->addr&0xf00)>>8);
			b2=lbl->addr&0xff;	
		}else{
			add_forward_ref(&line[start]);
			b1=0x20;
			b2=0;
		}
	}else{
		b2=get_number();
		b1=0x20|((b2&0xf00)>>8);
		b2&=0xff;
	}
	store_code(b1,b2);
}


void kw_se(){
	unsigned b1,b2;

	next_token();
	if (match_vx(&line[start])){
			b1=get_register();
			next_token();
			if (match_vx(&line[start])){
				b2=get_register();
				b1|=0x50;
				b2<<=4;
			}else{
				b1|=0x30;
				b2=get_number();
			}
			store_code(b1,b2);
	}else{
		error();
	}
}

void kw_sne(){
	unsigned b1,b2;

	next_token();
	if (match_vx(&line[start])){
			b1=get_register();
			next_token();
			if (match_vx(&line[start])){
				b2=get_register();
				b1|=0x90;
				b2<<=4;
			}else{
				b1|=0x40;
				b2=get_number();
			}
			store_code(b1,b2);
	}else{
		error();
	}
}

void kw_ld(){
	unsigned b1,b2;
	label_t *lbl;
	next_token();
	switch (line[start]){
	case 'V':
		if (hex(line[start+1])){
			b1=get_register();
			next_token();
			switch(line[start]){
			case 'V':
				b1|=0x80;
				b2=get_register();
				b2<<=4;
				break;
			case 'R':
				b1|=0xF0;
				b2=0x85;
				break;
			case 'D':
				b1|=0xF0;
				b2=0x07;
				break;
			case '[':
				if (line[start+1]=='I' && line[start+2]==']'){
					b1|=0xF0;
					b2=0x65;
				}else{
					error();
				}
				break;
			default:
				b1|=0x60;
				b2=get_number();
			}		
			}else{
			error();
		}
		break;
	case 'I':
		next_token();
		if (letter(line[start])){
			lbl=search_label(&line[start]);
			if (lbl){
				b1=0xA0|((lbl->addr&0xf00)>>8);
				b2=lbl->addr&0xff;
			}else{
				add_forward_ref();
				b1=0xA0;
				b2=0;
			}
		}else{
			b2=get_number();
			b1=0xA0|((b2&0xf00)>>8);
			b2 &= 0xff;
		}
		break;
	case 'D':
	    if (line[start+1]=='T'){
			b1=0xF0;
			b2=0x15;
			next_token();
			if (match_vx(&line[start])){
				b1|=get_register();
			}else{
				error();
			}
		}else{
			error();
		}
		break;
	case 'S':
	    if (line[start+1]=='T'){
			b1=0xF0;
			b2=0x18;
			next_token();
			if (match_vx(&line[start])){
				b1|=get_register();
			}else{
				error();
			}
		}else{
			error();
		}
		break;
	case 'R':
		b1=0xF0;
		b2=0x75;
		next_token();
		if (match_vx(&line[start])){
			b1|=get_register();
		}else{
			error();
		}
		break;
	case 'B':
		b1=0xF0;
		b2=0x33;
		next_token();
		if (match_vx(&line[start])){
			b1|=get_register();
		}else{
			error();
		}
		break;
	case 'F':
		b1=0xF0;
		b2=0x29;
		next_token();
		if (match_vx(&line[start])){
			b1|=get_register();
		}else{
			error();
		}
		break;
	case 'L':
	    if (line[start+1]=='F'){
			b1=0xF0;
			b2=0x30;
			next_token();
			if (match_vx(&line[start])){
				b1|=get_register();
			}else{
				error();
			}
		}else{
			error();
		}
		break;
	case '[':
		if ((line[start+1]=='I') && (line[start+2]==']')){
			next_token();
			if (match_vx(&line[start])){
				b1=0xF0+get_register();
				b2=0x55;
			}else{
				error();
			}
		}else{
			error();
		}
		break;
	}
	store_code(b1,b2);
}

void kw_add(){
	unsigned b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+4;
		}else{
			b2=get_number();
			b1|=0x70;
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_sub(){
	unsigned b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+5;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_subn(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+7;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_or(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+1;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_and(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+2;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_xor(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=get_register();
		next_token();
		if (match_vx(&line[start])){
			b1|=0x80;
			b2=(get_register()<<4)+3;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_shl(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0x80|get_register();
		b2=0x0e;
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_shr(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0x80|get_register();
		b2=0x06;
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_drw(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0xd0|get_register();
		next_token();
		if (match_vx(&line[start])){
			b2=(get_register()<<4);
			next_token();
			b2|=get_number()&0xf;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_skp(){
	unsigned int b1,b2;
	next_token();
	if(match_vx(&line[start])){
		b1=0xE0|get_register();
		b2=0x9e;
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_sknp(){
	unsigned int b1,b2;
	next_token();
	if(match_vx(&line[start])){
		b1=0xE0|get_register();
		b2=0xa1;
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_rnd(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0xe0|get_register();
		next_token();
		b2=get_number();
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_tone(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0x90|get_register();
		next_token();
		if (match_vx(&line[start])){
			b2=(get_register()<<4)+1;
		}else{
			error();
		}
	}else{
		puts(&line[start]);
		error();
	}
	store_code(b1,b2);
}

void kw_prt(){
	unsigned int b1,b2;
	next_token();
	if (match_vx(&line[start])){
		b1=0x90|get_register();
		next_token();
		if (match_vx(&line[start])){
			b2=(get_register()<<4)+2;
		}else{
			error();
		}
	}else{
		error();
	}
	store_code(b1,b2);
}

const char *mnemonics[KW_COUNT]={"SCD",	"CLS","RET","SCR","SCL","EXIT","LOW","HIGH","JP","CALL",
						 "SE","SNE","LD","ADD","OR","AND","XOR","SUB","SHR","SUBN","SHL",
						 "RND","DRW","TONE","PRT","SKP","SKNP"};

const kw_fn opcode[KW_COUNT]={kw_scd,kw_cls,kw_ret,kw_scr,kw_scl,kw_exit,kw_low,kw_high,kw_jp,kw_call,
							   kw_se,kw_sne,kw_ld,kw_add,kw_or,kw_and,kw_xor,kw_sub,kw_shr,kw_subn,kw_shl,
							   kw_rnd,kw_drw,kw_tone,kw_prt,kw_skp,kw_sknp};

						 
#define DIR_COUNT (3)						 
const char *directives[]={"DB","DW","ASCII","EQU"};


int search_word(char *target, const char *list[], int list_count){
	int i=0;
	while (i<list_count){
		if (!strcmp(target,list[i])) break;
		i++;
	}
	return i;
	
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
	while(line[start] && space(line[start])) start++;
}

void parse_string(){
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
	if (quote) inp--; else error();
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
			}else if (separator(line[inp]){
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
			}else if (separator(line[inp]){
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

int htoi(char *hnbr){
	unsigned int n=0;
	while (hex(*hnbr)){
		n*=16;
		n+=*hnbr<='9'?*hnbr-'0':*hnbr-'A'+10;
		hnbr++;
	}
	return n;
}

int btoi(char *bnbr){
	unsigned int n=0;
	while ((*bnbr=='1')||(*bnbr=='.')||(*bnbr=='0')){
		n <<=1;
		n += *bnbr=='1';
		bnbr++;
	}
	return n;
}

void data_byte(){
	unsigned n;
	
	do{
	   n=expression();
	   binary[pc++]=n&0xff;
	   next_token();
	   if (tok_id && tok_id!=eCOMMA) error();	
   }while((pc<MEM_SIZE) && tok_id){
}

void data_word(){
	unsigned n;
	
	do{
		n = expression();
		binary[pc++]=(n>>8)&0xff;
		binary[pc++]=n&0xff;
		next_token();
		if (tok_id && tok_id!=eCOMMA) error();
	}while((pc<MEM_SIZE-1) && tok_id){
}


void data_ascii(){
	unsigned i=0;
	
	next_token();
	if (tok_id!=eSTRING) error();
	while ((pc<MEM_SIZE-1) && tok_value[i]) binary[pc++]=tok_value[i++];
	if (pc<MEM_SIZE) binary[pc++]=0; else {puts("memory overflow";error();}
}


void equate(){
	node_t symbol;
	
	next_token();
	if (tok_id==eSYMBOL){
		symbol=search_symbol(tok_value);
		if (!symbol) symbol = add_symbol(tok_value,0);
		symbol->value = expression();
	}else{
		error();
	}
}

unsigned factor(){
	unsigned n;
	node_t symbol;
	
	next_token();
	switch(token_id){
	case (eNUMBER):
		switch(tok_value[0]{
		case '#':
			n=htoi(tok_value+1);
			break;
		case '$':
			n=btoi(tok_value+1);
			break;
		default:
			n=atoi(tok_value);
		}
		break;
	case (eSYMBOL):
		symbol=search_symbol(tok_value);
		if (symbol){
			n=sumbol->value;
		}else{
			puts("Unknown symbol");
			error();
		}
		break;
	case (eLPAREN):
		n=expression();
		next_token();
		if (token_id!=RPAREN) error();
	default:
		error();
	}
	return n;
}

unsigned (term){
	unsigned n;
	
	n=factor();
	next_token();
	if (tok_id!=eMULOP) error();
	switch(tok_value[0]){
	case '*':
		n *=factor();
		break;
	case '/':
		n /= factor();
		break;
	case '%':
		n %= factor();
		break;
	default:
		error();
	}
	return n;
}


unsigned expression(){
	unsigned n;
	
	n=term();
	next_token();
	if (tok_id!=eADDOP) error();
	switch(tok_value[0]){
	case '+':
		n += term();
		break;
	case '-':
		n -= term();
		break;
	default:
		error();
	}
	return n;
}

void parse_line(){
	int i;
		next_token();
		while (tok_id){
			if ((tok_id==eSYMBOL) && (i=search_word(tok_value,mnemonics,KW_COUNT))<KW_COUNT){
				//operation code
				opcode[i]();
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
					data_text();
				case 3:	
					equate();
					break;
				}
			}else if ((tok_id==eSYMBOL) && line[inp]==':'){
				// label
				inp++;
				add_label(tok_value,pc);
			}else{
				//unknown code
				error();
			}
		}//while
	
}

void resolve_forward(){
	forward_ref_t *fwd;
	label_t *lbl;
	
	fwd=fref;
	while (fwd && pc<MEM_SIZE){
		lbl=search_label(fwd->name);
		if (lbl){
			binary[fwd->pc] += (lbl->addr&0xf00)>>8;
			binary[fwd->pc+1] = lbl->addr&0xff;
		}else{
			printf("undefined reference: %s\n", fwd->name);
			exit(EXIT_FAILURE);
		}
		fwd=fwd->next;
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
		start=0;
		inp=0;
		if (strlne(line)) parse_line();
		memset(line,0,256);
	}
	if (pc>4095 && !feof(src)) memory_overflow();
	fclose(src);
	resolve_forward();
	int i;
	for (i=ORG;i<pc;i++){
		fputc(binary[i],bin);
	}
	fclose(bin);
	printf("Total lines read: %d\ncode size: %d\n",line_no, pc);
	return EXIT_SUCCESS;
}
