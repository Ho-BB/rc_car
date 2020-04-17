

#include "inc/utils.h"

void usart_write_str(volatile f10xxx_usart_t *usart_x,
		     u8 *str){

  while(*str){
    while(!f10xxx_usart_tx_empty(usart_x));
    f10xxx_usart_write(usart_x, *str++);
  }
}

void usart_write_strn(volatile f10xxx_usart_t *usart_x,
		      u8 *str,
		      u32 n){

  while(n--){
    while(!f10xxx_usart_tx_empty(usart_x));
    f10xxx_usart_write(usart_x, *str++);
  }
}

void usart_write_byte(volatile f10xxx_usart_t *usart_x,
		      u8 byte){

  while(!f10xxx_usart_tx_empty(usart_x));
  f10xxx_usart_write(usart_x, ascii_hex_digit(byte >> 4));

  while(!f10xxx_usart_tx_empty(usart_x));
  f10xxx_usart_write(usart_x, ascii_hex_digit(byte));
}

void usart_setup_usart_1(){

  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(usart1);

  f10xxx_rcc_reset_apb2(usart1);
  
  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);
}

u8 ascii_hex_digit(u8 quartet){

  quartet &= 0x0F;
  if(quartet < 10)
    return quartet + 48;
  
  return quartet + 55;
}
