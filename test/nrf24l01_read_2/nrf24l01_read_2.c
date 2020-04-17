

#include "inc/stm32f10xxx.h"
#include "inc/nRF24L01.h"

//TX = PA9 | RX = PA10

// NSS = PA4 | SCK = PA5 | MISO = PA6 | MOSI = PA7

// CE on a3

void usart_write_string(volatile f10xxx_usart_t *usart_x,
			u8 *str){

  while(*str != '\0'){
    while(!f10xxx_usart_tx_empty(usart_x));
    f10xxx_usart_write(usart_x, *str);
    str++;
  }
}

u8 hex_digit(u8 val){

  if(val < 10)
    return val + 48;
  else
    return val + 55;
}

//hex needs to be of length 2
void byte_to_hex(u8 byte, u8 *hex){

  hex[0] = hex_digit((byte & 0x0F0) >> 4);
  hex[1] = hex_digit(byte & 0x0F);
}

void read_cmd(volatile f10xxx_spi_t *const spi_x,
	      volatile f10xxx_gpio_t *gpio_x,
	      f10xxx_gpio_port_e nss_port,
	      u8 cmd, u8 *buf, u32 len){

  u32 i;

  f10xxx_gpio_write_port(gpio_x, nss_port, reset);

  for(i = 0; i <= len; i++){
    f10xxx_spi_write(spi_x, cmd);
    while(!f10xxx_spi_status(spi_x, spi_rx_not_empty));

    buf[i] = (u8) f10xxx_spi_read(spi_x);
  }

  f10xxx_gpio_write_port(gpio_x, nss_port, set);
}

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
			     gp_output_push_pull);//gp now

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

  f10xxx_gpio_write_port(gpio_a,
			 port_4,
			 set);

  spi_enable(spi_1,
	     lines_2,
	     crc_disabled,
	     frame_format_8,
	     msbit,
	     spi_clock_div_256,
	     spi_master_mode,
	     spi_ck0_transition1);

  //nrf24l01_configure_spi(spi_1);

  //f10xxx_spi_disable(spi_1);

  //nrf24l01_simple_configure_tx(spi_1);

  //LED off
  f10xxx_gpio_write_port(gpio_c,
			 port_13,
			 set);

  /*f10xxx_gpio_write_port(gpio_a, //CE
			 port_3,
			 set);
  */
  u8 buf[33];
  u8 hex[3];
  hex[2] = '\0';
  
  while(!f10xxx_usart_rx_not_empty(usart_1));
  f10xxx_usart_read(usart_1);

  usart_write_string(usart_1, (u8 *) "hello\n");

  read_cmd(spi_1,
	   gpio_a,
	   port_4,
	   NRF24L01_READ_REG(NRF24L01_REG_CONFIG),
	   buf,
	   1);

  f10xxx_gpio_write_port(gpio_c,
			 port_13,
			 reset);

  byte_to_hex(0x1C, hex);
  usart_write_string(usart_1, (u8 *) "TEST CONVERSION : ");
  usart_write_string(usart_1, hex);
  usart_write_string(usart_1, (u8 *) "\n");

  byte_to_hex(buf[0], hex);
  usart_write_string(usart_1, (u8 *) "STATUS : ");
  usart_write_string(usart_1, hex);
  usart_write_string(usart_1, (u8 *) "\n");

  byte_to_hex(buf[1], hex);
  usart_write_string(usart_1, (u8 *) "CONFIG : ");
  usart_write_string(usart_1, hex);
  usart_write_string(usart_1, (u8 *) "\n");

  read_cmd(spi_1,
	   gpio_a,
	   port_4,
	   NRF24L01_READ_REG(NRF24L01_REG_EN_AA),
	   buf,
	   1);

  byte_to_hex(0xEA, hex);
  
  byte_to_hex(buf[0], hex);
  usart_write_string(usart_1, (u8 *) "STATUS : ");
  usart_write_string(usart_1, hex);
  usart_write_string(usart_1, (u8 *) "\n");

  byte_to_hex(buf[1], hex);
  usart_write_string(usart_1, (u8 *) "EN_AA : ");
  usart_write_string(usart_1, hex);
  usart_write_string(usart_1, (u8 *) "\n");

  while(1);

  /*
  while(1){
    //while(!f10xxx_usart_rx_not_empty(usart_1));
    //LED on
    //f10xxx_gpio_write_port(gpio_c, port_13, reset);
    
    f10xxx_gpio_write_port(gpio_a, //CE
			   port_3,
			   reset);
    nrf24l01_send_poll(spi_1, &buf, 1); // REMOVE POLL TX (done!)
    f10xxx_gpio_write_port(gpio_a, //CE
			   port_3,
			   set);
  }
  */
}
