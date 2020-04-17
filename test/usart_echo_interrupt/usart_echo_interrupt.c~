

#include "inc/stm32f10xxx.h"

//TX = PA9 | RX = PA10

__attribute__((noreturn))
void f10xxx_reset_isr(){

  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

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

  while(1){
    while(!f10xxx_usart_rx_not_empty(usart_1));
    while(!f10xxx_usart_tx_empty(usart_1));

    f10xxx_usart_write(usart_1, f10xxx_usart_read(usart_1));
  }
}
