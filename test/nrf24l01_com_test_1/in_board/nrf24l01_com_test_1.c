

#include "inc/stm32f10xxx.h"
#include "inc/nRF24L01.h"

//TX = PA9 | RX = PA10

// NSS = PA4 | SCK = PA5 | MISO = PA6 | MOSI = PA7

// CE on a3

__attribute__((noreturn))
void f10xxx_reset_isr(){

  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopc);//for the inboard led
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_apb2(spi1);

  f10xxx_rcc_reset_apb2(usart1);
  f10xxx_rcc_reset_apb2(spi1);

  //LED conf
  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  //USART conf
  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  //SPI conf
  f10xxx_gpio_configure_port(gpio_a, //NSS
			     port_4,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a, //SCK
			     port_5,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a, //MISO
			     port_6,
			     input,
			     input_floating);

  f10xxx_gpio_configure_port(gpio_a, //MOSI
			     port_7,
			     output_50mhz,
			     af_output_push_pull);

  //CE conf
  f10xxx_gpio_configure_port(gpio_a, //CE
			     port_3,
			     output_50mhz,
			     gp_output_push_pull);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  nrf24l01_configure_spi(spi_1);

  nrf24l01_simple_configure_tx(spi_1);

  //LED off
  f10xxx_gpio_write_port(gpio_c,
			 port_13,
			 set);

  u8 buf;

  while(1){
    while(!f10xxx_usart_rx_not_empty(usart_1));
    //LED on
    f10xxx_gpio_write_port(gpio_c, port_13, reset);

    buf = f10xxx_usart_read(usart_1);

    f10xxx_gpio_write_port(gpio_a, //CE
			   port_3,
			   reset);
    nrf24l01_send_poll(spi_1, &buf, 1); // REMOVE POLL TX (done!)
    f10xxx_gpio_write_port(gpio_a, //CE
			   port_3,
			   set);
  }
}
