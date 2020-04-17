
.SECONDARY:

LIB:=~/src/stm/clean_1/build/libstm32f10xxx.a
LDSCRIPT:=~/src/stm/clean_1/ld/stm32f10xxx.ld
INCLUDE:=-I ~/src/stm/clean_1/inc -I ~/src/stm/clean_1/

AS:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-as
CC:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
LD:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld
OBJCOPY:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy


all : adc_test_read_1.bin


adc_test_read_1.bin : adc_test_read_1.elf
	$(OBJCOPY) $< $@ -O binary

adc_test_read_1.elf : adc_test_read_1.o
	$(LD) -T $(LDSCRIPT) -o $@ $< $(LIB)

adc_test_read_1.o : adc_test_read_1.c
	$(CC) -o $@ -c $< -mcpu=cortex-m3 -mthumb -Wall -O0 $(INCLUDE)

