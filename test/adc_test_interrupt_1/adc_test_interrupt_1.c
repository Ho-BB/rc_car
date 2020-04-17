

#include "inc/stm32f10xxx.h"

//TX = PA9 | RX = PA10


void usart_write_strn(volatile f10xxx_usart_t *usart_x,
		      u8 *str,
		      u32 n){

  while(n--){
    while(!f10xxx_usart_tx_empty(usart_x));
    f10xxx_usart_write(usart_x, *str++);
  }
}


u8 hta(u8 val){

  if(val >= 10)
    return val + 55;
  else
    return val + 48;
}


void adc_val_to_ascii(u16 val, u8 *buf){

  buf[0] = hta((val >> 8) & 0x0F);
  buf[1] = hta((val >> 4) & 0x0F);
  buf[2] = hta(val & 0x0F);
}


__attribute__((noreturn))
void f10xxx_reset_isr(){

  u32 i;
  
  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopc);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_apb2(adc1);
  f10xxx_rcc_enable_clock_apb1(tim2);

  f10xxx_rcc_reset_apb2(usart1);
  f10xxx_adc_simple_init(adc_1);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_ADC1_2);

  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_c,
			 port_13,
			 set);

  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  f10xxx_gpio_configure_port(gpio_a,
			     port_0,
			     input,
			     input_floating);

  f10xxx_gpio_configure_port(gpio_a,
			     port_1,
			     input,
			     input_floating);

  f10xxx_adc_enable_eoc_irq(adc_1);
  f10xxx_adc_regular_sequence_length(adc_1, 1);
  f10xxx_adc_set_channel_position(adc_1, 0, adc_channel_0);
  //f10xxx_adc_set_channel_position(adc_1, 1, adc_channel_1);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  while(!f10xxx_usart_rx_not_empty(usart_1));

  f10xxx_gpio_write_port(gpio_c, port_13, reset);

  while(1){
    
    i = 100000;
    while(i--);

    f10xxx_adc_start(adc_1);
  }
}

void f10xxx_adc1_2_isr(){

  u8 str[4];
  str[3] = '\n';

  f10xxx_gpio_write_port(gpio_c, port_13, set);
  
  adc_val_to_ascii(f10xxx_adc_get_regular_value(adc_1), str);

  usart_write_strn(usart_1, str, 4);
}
