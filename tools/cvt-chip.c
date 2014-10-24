// convert schip files binaries (*.SC) to C file
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void usage(){
  puts("\nConvert CHIP/SCHIP binary to 'C' and 'H' files");
  puts("USAGE: cvt-chip.exe input_file.");
  puts("input_file is SCHIP binary file.");
  puts("exemple: cvt_sc.exe sokoban.sc");
  puts("create  sokoban.c and sokoban.h");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
	FILE *inp, *hf, *cf;
	unsigned char c;
	unsigned char array_name[32],header_var[32],h_file[32], c_file[32], *bslash;
	int size,  i=0;
	if (argc<2) usage();
	inp=fopen(argv[1],"rb");
	if (!inp) {
		printf("failed to open %s file.\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	bslash=argv[1]+strlen(argv[1])-1;
	while (bslash>=argv[1] && *bslash!='\\') --bslash;
	bslash++;
	strcpy(c_file,bslash); 
	i=0;
	while (c_file[i] && c_file[i]!='.'){
		array_name[i]=tolower(c_file[i]);
		header_var[i]=toupper(c_file[i]);
		h_file[i]=tolower(c_file[i]);
		c_file[i]=tolower(c_file[i]);
		i++;
	}
	array_name[i]=0;
	header_var[i]=0;
	h_file[i]='.';
	c_file[i]='.';
	i++;
	h_file[i]='h';
	c_file[i]='c';
	i++;
	h_file[i]=0;
	c_file[i]=0;
	hf=fopen(h_file,"w");
	if (!hf){
		printf("failed to create %s file.\n",h_file);
		exit(EXIT_FAILURE);
	}
	cf=fopen(c_file,"w");
	if (!cf){
		printf("failed to create %s file.\n",c_file);
		exit(EXIT_FAILURE);
	}
	fseek(inp,0,SEEK_END);
	size=ftell(inp);
	if (!size){
		puts("fichier vide");
		fclose(inp);
		fclose(hf);
		fclose(cf);
		exit(EXIT_SUCCESS);
	}
	fprintf(cf,"#include \"%s.h\"\n\n",array_name);		
	fprintf(cf,"PROGMEM const uint8_t %s_info[]=\"\";\n\n", array_name);
	fprintf(cf,"PROGMEM const uint8_t %s[%s_SIZE]={",array_name,header_var);
	fseek(inp,0,SEEK_SET);
	i=0;
	c=fgetc(inp);
	while (!feof(inp)) {
		if (!(i%16)) fputs("\n\t",cf);
		fprintf(cf,"0x%02x,",c);
		c=fgetc(inp);
		i++;
	}
	fputs("\n};\n",cf);
	fclose(cf);
	fclose(inp);
	fprintf(hf,"#ifndef %s_\n",header_var); 
	fprintf(hf,"#define %s_\n\n",header_var);
	fprintf(hf,"#include <avr/pgmspace.h>\n");
	fprintf(hf,"#include <stdint.h>\n\n");
	fprintf(hf,"#define %s_SIZE (%d)\n\n",header_var, size);
	fprintf(hf,"extern const uint8_t %s[%s_SIZE];\n\n",array_name,header_var);
	fprintf(hf,"extern const uint8_t %s_info[];\n\n", array_name);
	fprintf(hf,"#endif\n");
	fclose(hf);
	printf("%d octets\n",size);
	return EXIT_SUCCESS;
}
