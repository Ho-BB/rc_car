

#ifndef __NRF24L01__
#define __NRF24L01__

#include "inc/stm32f10xxx.h"


#define NRF24L01_READ_REG(x)            (x & 0x1F)
#define NRF24L01_WRITE_REG(x)           ((x & 0x1F) | (1 << 5))
#define NRF24L01_READ_RX                97
#define NRF24L01_WRITE_TX               160
#define NRF24L01_NOP                    255

#define NRF24L01_REG_CONFIG             0
#define NRF24L01_REG_EN_AA              1
#define NRF24L01_REG_EN_RXADDR          2
#define NRF24L01_REG_SETUP_AW           3
#define NRF24L01_REG_SETUP_RETR         4
#define NRF24L01_REG_RF_CH              5
#define NRF24L01_REG_RF_SETUP           6
#define NRF24L01_REG_STATUS             7
#define NRF24L01_REG_OBSERVE_TX         8
#define NRF24L01_REG_CD                 9
#define NRF24L01_REG_RX_ADDR_P0         10
#define NRF24L01_REG_RX_ADDR_P1         11
#define NRF24L01_REG_RX_ADDR_P2         12
#define NRF24L01_REG_RX_ADDR_P3         13
#define NRF24L01_REG_RX_ADDR_P4         14
#define NRF24L01_REG_RX_ADDR_P5         15
#define NRF24L01_REG_TX_ADDR            16
#define NRF24L01_REG_RX_PW_P0           17
#define NRF24L01_REG_RX_PW_P1           18
#define NRF24L01_REG_RX_PW_P2           19
#define NRF24L01_REG_RX_PW_P3           20
#define NRF24L01_REG_RX_PW_P4           21
#define NRF24L01_REG_RX_PW_P5           22
#define NRF24L01_REG_FIFO_STATUS        23

#define NRF24L01_RX_FIFO_EMPTY(status)  ((status & 0x0E) == 0x0E)
#define NRF24L01_DATA_SENT(status)      (status & 0x20)


typedef struct nrf24l01{
  volatile f10xxx_spi_t *spi_x;
  
  volatile f10xxx_gpio_t *nss_gpio;
  f10xxx_gpio_port_e nss_port;

  volatile f10xxx_gpio_t *sck_gpio;
  f10xxx_gpio_port_e sck_port;

  volatile f10xxx_gpio_t *miso_gpio;
  f10xxx_gpio_port_e miso_port;

  volatile f10xxx_gpio_t *mosi_gpio;
  f10xxx_gpio_port_e mosi_port;

  volatile f10xxx_gpio_t *ce_gpio;
  f10xxx_gpio_port_e ce_port;

  u8 status;
} nrf24l01_t;


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
		   f10xxx_gpio_port_e ce_port);

void nrf24l01_configure_rx(nrf24l01_t *rf,
			   u8 payload_width);

void nrf24l01_configure_tx(nrf24l01_t *rf);

void nrf24l01_read(nrf24l01_t *rf,
		   u8 cmd,
		   u8 *buf,
		   u32 len);

void nrf24l01_write(nrf24l01_t *rf,
		    u8 cmd,
		    u8 *buf,
		    u32 len);

void nrf24l01_write_tx(nrf24l01_t *rf,
		       u8 *buf,
		       u32 len);

void nrf24l01_ce_high(nrf24l01_t *rf);
void nrf24l01_ce_low(nrf24l01_t *rf);

#endif
