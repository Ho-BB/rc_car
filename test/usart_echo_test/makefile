
.SECONDARY:

LIB:=~/src/stm/clean_1/build/libstm32f10xxx.a
LDSCRIPT:=~/src/stm/clean_1/ld/stm32f10xxx.ld
INCLUDE:=-I ~/src/stm/clean_1/inc -I ~/src/stm/clean_1/

AS:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-as
CC:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
LD:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld
OBJCOPY:=~/arm/2019/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy


all : usart_echo_test.bin


usart_echo_test.bin : usart_echo_test.elf
	$(OBJCOPY) $< $@ -O binary

usart_echo_test.elf : usart_echo_test.o
	$(LD) -T $(LDSCRIPT) -o $@ $< $(LIB)

usart_echo_test.o : usart_echo_test.c
	$(CC) -o $@ -c $< -mcpu=cortex-m3 -mthumb -Wall -O0 $(INCLUDE)

