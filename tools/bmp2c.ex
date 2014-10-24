-- euphoria script
-- convert splash bitmap to c file
integer fh,fo, j, c, r
sequence bits=repeat(repeat(0,16),64)
r=64
fo=open("splash.h","wb")
printf(fo,"#ifndef SPLASH_\n")
printf(fo,"#define SPLASH_\n\n")
printf(fo,"#include <stdint.h>\n")
printf(fo,"#include <avr/pgmspace.h>\n\n")
printf(fo,"#define SPLASH_SIZE (1024)\n\n")
printf(fo,"extern const uint8_t splash[SPLASH_SIZE];\n\n")
printf(fo,"#endif\n")
close(fo)
fo=open("splash.c","wb")
printf(fo,"#include \"splash.h\"\n\n")
printf(fo,"PROGMEM const uint8_t splash[SPLASH_SIZE]={\n")
j=1
fh=open("splash.bmp","rb")
for i=0 to 0x3d do c=getc(fh) end for
c=getc(fh)
while  c!=-1 do
	bits[r][j]=c
    c=getc(fh)
	j+=1
	if j=17 then
		r-=1
		j=1
	end if
end while
for i=1 to 64 do
	for k=1 to 16 do
		printf(fo,"0x%02x, ",bits[i][k])
	end for
	puts(fo,'\n')
end for
puts(fo,"};\n")
close(fh)
close(fo)

