

#include "inc/stm32f10xxx.h"
#include "inc/utils.h"

/*
  USART : TX = A9 
        | RX = A10

  ILI9341 : SPI : CSN  = A4
                | SCK  = A5
                | MISO = A6
                | MOSI = A7
          | RESET = A2
          | D/CX  = A3
*/

#define GPIO_CSN   gpio_a
#define PORT_CSN   port_4

#define GPIO_DCX   gpio_a
#define PORT_DCX   port_3

#define GPIO_RESET gpio_a
#define PORT_RESET port_2

void spi_enable(volatile f10xxx_spi_t *const spi_x,
		f10xxx_spi_num_lines_e num_lines,
		f10xxx_spi_crc_mode_e crc_mode,
		f10xxx_spi_frame_format_e frame_format,
		f10xxx_spi_bit_endianess_e bit_endianess,
		f10xxx_spi_clock_div_e clock_div,
		f10xxx_spi_mode_e spi_mode,
		f10xxx_spi_clock_pol_pha_e clock_pol_pha){

  spi_x->CR1 =
    (u32) num_lines
    | (u32) crc_mode
    | (u32) frame_format
    | (u32) bit_endianess
    | (u32) clock_div
    | (u32) spi_mode
    | (u32) clock_pol_pha
    | (u32) (1 << 6)
    | (u32) (1 << 9)
    | (u32) (1 << 8);
}


__attribute__((noreturn))
void f10xxx_reset_isr(){

  u32 i;

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
  f10xxx_gpio_configure_port(gpio_a, //CSN
			     port_4,
			     output_50mhz,
			     gp_output_push_pull);

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

  // D/CX conf
  f10xxx_gpio_configure_port(gpio_a, // D/CX
			     port_3,
			     output_50mhz,
			     gp_output_push_pull);

  //RESET conf
  f10xxx_gpio_configure_port(gpio_a, //RESET
			     port_2,
			     output_50mhz,
			     gp_output_push_pull);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  f10xxx_gpio_write_port(GPIO_CSN,
			 PORT_CSN,
			 set);

  f10xxx_gpio_write_port(GPIO_RESET,
			 PORT_RESET,
			 reset);

  i = 1000000;
  while(i--);

  f10xxx_gpio_write_port(GPIO_RESET,
			 PORT_RESET,
			 set);

  spi_enable(spi_1,
	     lines_2,
	     crc_disabled,
	     frame_format_8,
	     msbit,
	     spi_clock_div_256,
	     spi_master_mode,
	     spi_ck0_transition1);

  while(!f10xxx_usart_rx_not_empty(usart_1));
  f10xxx_usart_read(usart_1);

  u8 buf[4];

  f10xxx_gpio_write_port(GPIO_CSN,
			 PORT_CSN,
			 reset);

  f10xxx_gpio_write_port(GPIO_DCX,
			 PORT_DCX,
			 reset);
  
  f10xxx_spi_write(spi_1, 0x09);
  while(!f10xxx_spi_status(spi_1, spi_rx_not_empty));
  
  f10xxx_spi_read(spi_1);

  f10xxx_gpio_write_port(GPIO_DCX,
			 PORT_DCX,
			 set);

  // Dummy clock cycle

  f10xxx_gpio_configure_port(gpio_a,
			     port_5,
			     output_50mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_a,
			 port_5,
			 reset);
  // No delay for now
  f10xxx_gpio_write_port(gpio_a,
			 port_5,
			 set);

  f10xxx_gpio_configure_port(gpio_a,
			     port_5,
			     output_50mhz,
			     af_output_push_pull);
  
  // End dummy clock cycle

  for(i = 0; i < 4; i++){
    f10xxx_spi_write(spi_1, 0x00);
    while(!f10xxx_spi_status(spi_1, spi_rx_not_empty));

    buf[i] = f10xxx_spi_read(spi_1);
  }

  f10xxx_gpio_write_port(GPIO_CSN,
			 PORT_CSN,
			 set);

  usart_write_str(usart_1, (u8 *) "STATUS : ");
  for(i = 0; i < 4; i++)
    usart_write_byte(usart_1, buf[i]);
  usart_write_str(usart_1, (u8 *) "\n");

  while(1);
}
