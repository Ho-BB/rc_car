
.SECONDARY:

LIB:=~/src/stm/clean_1/build/libstm32f10xxx.a
LDSCRIPT:=~/src/stm/clean_1/ld/stm32f10xxx.ld
INCLUDE:=-I ~/src/stm/clean_1/inc -I ~/src/stm/clean_1/

AS:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-as
CC:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
LD:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld
OBJCOPY:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy


all : h_bridge_test_1.bin


h_bridge_test_1.bin : h_bridge_test_1.elf
	$(OBJCOPY) $< $@ -O binary

h_bridge_test_1.elf : h_bridge_test_1.o
	$(LD) -T $(LDSCRIPT) -o $@ $< $(LIB)

h_bridge_test_1.o : h_bridge_test_1.c
	$(CC) -o $@ -c $< -mcpu=cortex-m3 -mthumb -Wall -O0 $(INCLUDE)

