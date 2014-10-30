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

typedef struct label{
	int addr;
	struct label *next;
	char *name;
}label_t;

typedef struct forward_ref{
	int pc;
	struct forward_ref *next;
	char *name;
}forward_ref_t;


label_t *label_list=NULL;
label_t *last_lbl=NULL;

forward_ref_t *fref=NULL;
forward_ref_t *last_ref=NULL;


FILE *bin;

int pc; // program counter
int line_no; //source file line number

unsigned char binary[4096];


int start,end; // word start and end position
char line[256]; // input line buffer

bool digit(char c){
	return (c>='0') && (c<='9');
}

bool hex(char c){
	return digit(c) || ((c>='A')&&(c<='F'));
}

bool match_vx(char *w){
	return (*w=='V') && hex(*(w+1)); 
}

int get_register(){
	return line[start+1]<='9'?line[start+1]-'0':line[start+1]-'A'+10;
}

void store_code(unsigned char b1, unsigned char b2){
	binary[pc++]=b1&0xff;
	binary[pc++]=b2&0xff;
}

#define KW_COUNT (27)

typedef  void (*kw_fn)();

void next_word();
int get_number();

void error(){
	printf("Syntax error at line %d, position %d\n",line_no,start+1);
	exit(EXIT_FAILURE);
}

void add_forward_ref(){
	if (fref==NULL){
		fref=malloc(sizeof(forward_ref_t));
		fref->pc=pc;
		fref->next=NULL;
		fref->name=malloc(strlen(&line[start])+1);
		strcpy(fref->name,&line[start]);
		last_ref=fref;
	}else{
		last_ref->next=malloc(sizeof(forward_ref_t));
		last_ref=last_ref->next;
		last_ref->pc=pc;
		last_ref->name=malloc(strlen(&line[start])+1);
		strcpy(last_ref->name,&line[start]);
		last_ref->next=NULL;
	}
}

void add_label(){
	if (label_list==NULL){
		label_list=malloc(sizeof(label_t));
		label_list->addr=pc;
		label_list->name=malloc(strlen(&line[start])+1);
		strcpy(label_list->name,&line[start]);
		label_list->next=NULL;
		last_lbl=label_list;
	}else{
		last_lbl->next=malloc(sizeof(label_t));
		last_lbl=last_lbl->next;
		last_lbl->addr=pc;
		last_lbl->name=malloc(strlen(&line[start])+1);
		strcpy(last_lbl->name,&line[start]);
		last_lbl->next=NULL;
	}
}

label_t *search_label(char *name){
	label_t *lbl;
	lbl=label_list;
	while (lbl){
		if (!strcmp(name,lbl->name)) break;
		lbl=lbl->next;
	}
	return lbl;
}

void kw_scd(){
	int i;
	next_word();
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
	next_word();
    lbl=search_label(&line[start]);
	if (lbl){
		b1=0x10+((lbl->addr&0xf00)>>8);
		b2=lbl->addr&0xff;	
	}else{
		add_forward_ref();
		b1=0x10;
		b2=0;
	}
	store_code(b1,b2);
}

void kw_call(){
	unsigned b1,b2;
	label_t *lbl;
	next_word();
    lbl=search_label(&line[start]);
	if (lbl){
		b1=0x20+((lbl->addr&0xf00)>>8);
		b2=lbl->addr&0xff;	
	}else{
		add_forward_ref(&line[start]);
		b1=0x20;
		b2=0;
	}
	store_code(b1,b2);
}


void kw_se(){
	unsigned b1,b2;

	next_word();
	if (match_vx(&line[start])){
			b1=get_register();
			next_word();
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

	next_word();
	if (match_vx(&line[start])){
			b1=get_register();
			next_word();
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
	next_word();
	switch (line[start]){
	case 'V':
		if (hex(line[start+1])){
			b1=get_register();
			next_word();
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
				b1|=0x70;
				b2=get_number();
			}		
			}else{
			error();
		}
		break;
	case 'I':
		next_word();
		b2=get_number();
		b1=0xA0|((b2&0xf00)>>8);
		b2 &= 0xff;
		break;
	case 'D':
	    if (line[start+1]=='T'){
			b1=0xF0;
			b2=0x15;
			next_word();
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
			next_word();
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
		next_word();
		if (match_vx(&line[start])){
			b1|=get_register();
		}else{
			error();
		}
		break;
	case 'B':
		b1=0xF0;
		b2=0x33;
		next_word();
		if (match_vx(&line[start])){
			b1|=get_register();
		}else{
			error();
		}
		break;
	case 'F':
		b1=0xF0;
		b2=0x29;
		next_word();
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
			next_word();
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
			next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=get_register();
		next_word();
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
	next_word();
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
	next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=0xd0|get_register();
		next_word();
		if (match_vx(&line[start])){
			b2=(get_register()<<4);
			next_word();
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
	next_word();
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
	next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=0xe0|get_register();
		next_word();
		b2=get_number();
	}else{
		error();
	}
	store_code(b1,b2);
}

void kw_tone(){
	unsigned int b1,b2;
	next_word();
	if (match_vx(&line[start])){
		b1=0x90|get_register();
		next_word();
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
	next_word();
	if (match_vx(&line[start])){
		b1=0x90|get_register();
		next_word();
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

						 
#define DIR_COUNT (5)						 
const char *directives[]={"DB","DW","OPTION","ALIGN","BINARY","OFF","ON"};


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

void next_word(){
	if (start<end) start=end+1;
	skip_white();
	if (!line[start]){
		end=start;
	}else{
		end=start;
		while (line[end] && !(space(line[end])||(line[end]==','))){
			if ((line[end]>='a') && (line[end]<='z')) line[end] -= 32;
			end++;
		}
	}
	line[end]=0;
}

int htoi(char *hnbr){
	int n=0;
	while (hex(*hnbr)){
		n*=16;
		n+=*hnbr<='9'?*hnbr-'0':*hnbr-'A'+10;
		hnbr++;
	}
	return n;
}

int btoi(char *bnbr){
	int n=0;
	while ((*bnbr=='1')||(*bnbr=='.')||(*bnbr=='0')){
		n <<=1;
		n += *bnbr=='1';
	}
	return n;
}

int get_number(){
	if (digit(line[start])){
		return atoi(&line[start]);
	}else if (line[start]=='#'){
		return htoi(&line[start+1]);
	}else if (line[start]=='$'){
		return btoi(&line[start+1]);
	}else {
		error();
	}
}

void parse_line(){
	int i;
	while (line[start]){
		next_word();
		if (line[start]==';') break;
		if (strlen(&line[start])){
			if ((i=search_word(&line[start],mnemonics,KW_COUNT))<KW_COUNT){
				//operation opcode
				opcode[i]();
			}else if ((i=search_word(&line[start],directives,DIR_COUNT))<DIR_COUNT){
				// assemble directvie
			}else if (line[end-1]==':'){
				// label
				line[end-1]=0;
				add_label();
			}else{
				//unknown code
				error();
			}
			
		}
	}
}

void resolve_forward(){
	forward_ref_t *fwd;
	label_t *lbl;
	
	fwd=fref;
	while (fwd){
		lbl=search_label(fwd->name);
		if (lbl){
			binary[fwd->pc] += (lbl->addr&0xf00)>>8;
			binary[fwd->pc+1] = lbl->addr&0xff;
		}else{
			error();
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
	while (fgets(line,256,src)){
		line_no++;
		line[strlen(line)-1]=0;
		start=0;
		end=0;
		parse_line();
		memset(line,0,256);
	}
	fclose(src);
	resolve_forward();
	int i;
	for (i=ORG;i<pc;i++){
		fputc(binary[i],bin);
	}
	fclose(bin);
	return EXIT_SUCCESS;
}
