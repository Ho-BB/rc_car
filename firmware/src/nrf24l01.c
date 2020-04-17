

#include "inc/nrf24l01.h"


static void spi_enable(volatile f10xxx_spi_t *const spi_x,
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


void nrf24l01_init(nrf24l01_t *rf,
		   volatile f10xxx_spi_t *const spi_x,
		   
		   volatile f10xxx_gpio_t *nss_gpio,
		   f10xxx_gpio_port_e nss_port,
		   
		   volatile f10xxx_gpio_t *sck_gpio,
		   f10xxx_gpio_port_e sck_port,

		   volatile f10xxx_gpio_t *miso_gpio,
		   f10xxx_gpio_port_e miso_port,

		   volatile f10xxx_gpio_t *mosi_gpio,
		   f10xxx_gpio_port_e mosi_port,

		   volatile f10xxx_gpio_t *ce_gpio,
		   f10xxx_gpio_port_e ce_port){

  rf->spi_x = spi_x;
  rf->nss_gpio = nss_gpio;
  rf->nss_port = nss_port;
  rf->sck_gpio = sck_gpio;
  rf->sck_port = sck_port;
  rf->miso_gpio = miso_gpio;
  rf->miso_port = miso_port;
  rf->mosi_gpio = mosi_gpio;
  rf->mosi_port = mosi_port;
  rf->ce_gpio = ce_gpio;
  rf->ce_port = ce_port;

  f10xxx_gpio_configure_port(nss_gpio,
			     nss_port,
			     output_50mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(sck_gpio,
			     sck_port,
			     output_50mhz,
			     af_output_push_pull);
  f10xxx_gpio_configure_port(miso_gpio,
			     miso_port,
			     input,
			     input_floating);
  f10xxx_gpio_configure_port(mosi_gpio,
			     mosi_port,
			     output_50mhz,
			     af_output_push_pull);
  f10xxx_gpio_configure_port(ce_gpio,
			     ce_port,
			     output_50mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(nss_gpio, nss_port, set);

  spi_enable(spi_1,
	     lines_2,
	     crc_disabled,
	     frame_format_8,
	     msbit,
	     spi_clock_div_256,
	     spi_master_mode,
	     spi_ck0_transition1);
}

void nrf24l01_configure_rx(nrf24l01_t *rf,
			   u8 payload_width){

  u8 buf;

  buf = 0;
  nrf24l01_write(rf, NRF24L01_WRITE_REG(NRF24L01_REG_EN_AA), &buf, 1);

  buf = (1 << 6) | (1 << 5) | ( 1 << 4) | (1 << 1) | 1;
  nrf24l01_write(rf, NRF24L01_WRITE_REG(NRF24L01_REG_CONFIG), &buf, 1);

  buf = payload_width;
  nrf24l01_write(rf, NRF24L01_WRITE_REG(NRF24L01_REG_RX_PW_P0), &buf, 1);
}

void nrf24l01_configure_tx(nrf24l01_t *rf){

  u8 buf;

  buf = 0;
  nrf24l01_write(rf, NRF24L01_WRITE_REG(NRF24L01_REG_EN_AA), &buf, 1);

  buf = (1 << 6) | (1 << 5) | ( 1 << 4) | (1 << 1);
  nrf24l01_write(rf, NRF24L01_WRITE_REG(NRF24L01_REG_CONFIG), &buf, 1);
}

void nrf24l01_read(nrf24l01_t *rf,
		   u8 cmd,
		   u8 *buf,
		   u32 len){

  u32 i;

  f10xxx_gpio_write_port(rf->nss_gpio, rf->nss_port, reset);

  f10xxx_spi_write(rf->spi_x, cmd);
  
  while(!f10xxx_spi_status(rf->spi_x, spi_rx_not_empty));
  rf->status = (u8) f10xxx_spi_read(rf->spi_x);

  for(i = 0; i < len; i++){
    f10xxx_spi_write(rf->spi_x, NRF24L01_NOP);

    while(!f10xxx_spi_status(rf->spi_x, spi_rx_not_empty));
    buf[i] = (u8) f10xxx_spi_read(rf->spi_x);
  }

  f10xxx_gpio_write_port(rf->nss_gpio, rf->nss_port, set);
}

void nrf24l01_write(nrf24l01_t *rf,
		    u8 cmd,
		    u8 *buf,
		    u32 len){

  u32 i;

  f10xxx_gpio_write_port(rf->nss_gpio, rf->nss_port, reset);

  f10xxx_spi_write(rf->spi_x, cmd);
  
  while(!f10xxx_spi_status(rf->spi_x, spi_rx_not_empty));
  rf->status = (u8) f10xxx_spi_read(rf->spi_x);

  for(i = 0; i < len; i++){
    f10xxx_spi_write(rf->spi_x, buf[i]);

    while(!f10xxx_spi_status(rf->spi_x, spi_rx_not_empty));
    f10xxx_spi_read(rf->spi_x);
  }

  f10xxx_gpio_write_port(rf->nss_gpio, rf->nss_port, set);
}

void nrf24l01_write_tx(nrf24l01_t *rf,
		       u8 *buf,
		       u32 len){

  u32 i;
  
  nrf24l01_write(rf, NRF24L01_WRITE_TX, buf, len);

  f10xxx_gpio_write_port(rf->ce_gpio, rf->ce_port, set);

  i = 200;
  while(i--);
  
  f10xxx_gpio_write_port(rf->ce_gpio, rf->ce_port, reset);
}

void nrf24l01_ce_high(nrf24l01_t *rf){

  f10xxx_gpio_write_port(rf->ce_gpio, rf->ce_port, set);
}

void nrf24l01_ce_low(nrf24l01_t *rf){

  f10xxx_gpio_write_port(rf->ce_gpio, rf->ce_port, reset);
}
