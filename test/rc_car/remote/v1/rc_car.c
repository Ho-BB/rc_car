

#include "inc/stm32f10xxx.h"
#include "inc/nrf24l01.h"

extern u32 __start_ram[];


__attribute__((noreturn))
void f10xxx_reset_isr(){

  nrf24l01_t rf;
  u8 buf[32];
  buf[1] = 'y';
  buf[2] = 'o';
  
  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_apb2(spi1);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopc);
  
  f10xxx_rcc_reset_apb2(usart1);
  f10xxx_rcc_reset_apb2(spi1);

  cortex_m3_enable_all_interrupts();

  //led conf
  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_c, port_13, reset);

  //usart ports conf
  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);
  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  //usart conf
  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  nrf24l01_init(&rf,
		spi_1,
		gpio_a,
		port_4,
		gpio_a,
		port_5,
		gpio_a,
		port_6,
		gpio_a,
		port_7,
		gpio_a,
		port_3);

  nrf24l01_configure_tx(&rf);
  
  while(1){

    while(!f10xxx_usart_rx_not_empty(usart_1));
    buf[0] = f10xxx_usart_read(usart_1);

    nrf24l01_write_tx(&rf, buf, 3);

    do{
      nrf24l01_write(&rf, NRF24L01_NOP, NULL, 0);
    }while(!NRF24L01_DATA_SENT(rf.status));
  }
}
