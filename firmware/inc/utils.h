

#ifndef __UTILS__
#define __UTILS__

#include "inc/stm32f10xxx.h"

void usart_write_str(volatile f10xxx_usart_t *usart_x,
		     u8 *str);

void usart_write_strn(volatile f10xxx_usart_t *usart_x,
		      u8 *str,
		      u32 n);

void usart_write_byte(volatile f10xxx_usart_t *usart_x,
		      u8 byte);

void usart_setup_usart_1();

u8 ascii_hex_digit(u8 quartet);

#endif
