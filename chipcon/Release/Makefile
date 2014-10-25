################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../blinky.c \
../byteordering.c \
../chip8.c \
../chipcon.c \
../fat.c \
../field.c \
../filesys.c \
../joust23.c \
../keypad.c \
../mines.c \
../partition.c \
../sd_raw.c \
../sokoban.c \
../spacefig.c \
../splash.c \
../square.c \
../text.c \
../sram.c \
../tone.c \
../tvout.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
blinky.o \
byteordering.o \
chip8.o \
chipcon.o \
fat.o \
field.o \
filesys.o \
joust23.o \
keypad.o \
mines.o \
partition.o \
sd_raw.o \
sokoban.o \
spacefig.o \
splash.o \
square.o \
text.o \
sram.o \
tone.o \
tvout.o

OBJS_AS_ARGS +=  \
blinky.o \
byteordering.o \
chip8.o \
chipcon.o \
fat.o \
field.o \
filesys.o \
joust23.o \
keypad.o \
mines.o \
partition.o \
sd_raw.o \
sokoban.o \
spacefig.o \
splash.o \
square.o \
text.o \
sram.o \
tone.o \
tvout.o

C_DEPS +=  \
blinky.d \
byteordering.d \
chip8.d \
chipcon.d \
fat.d \
field.d \
filesys.d \
joust23.d \
keypad.d \
mines.d \
partition.d \
sd_raw.d \
sokoban.d \
spacefig.d \
splash.d \
square.d \
text.d \
sram.d \
tone.d \
tvout.d

C_DEPS_AS_ARGS +=  \
blinky.d \
byteordering.d \
chip8.d \
chipcon.d \
fat.d \
field.d \
filesys.d \
joust23.d \
keypad.d \
mines.d \
partition.d \
sd_raw.d \
sokoban.d \
spacefig.d \
splash.d \
square.d \
text.d \
sram.d \
tone.d \
tvout.d

OUTPUT_FILE_PATH +=CHIPCON.elf

OUTPUT_FILE_PATH_AS_ARGS +=CHIPCON.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler









































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DNDEBUG  -I"../../../sdCard+fat_driver/sd-reader_source_20120612"  -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="CHIPCON.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "CHIPCON.elf" "CHIPCON.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "CHIPCON.elf" "CHIPCON.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "CHIPCON.elf" > "CHIPCON.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "CHIPCON.elf" "CHIPCON.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "CHIPCON.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "CHIPCON.elf" "CHIPCON.a" "CHIPCON.hex" "CHIPCON.lss" "CHIPCON.eep" "CHIPCON.map" "CHIPCON.srec"
	