

#ifndef __NRF24L01__
#define __NRF24L01__


#include "inc/stdinclude.h"
#include "inc/f10xxx_spi.h"

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

void nrf24l01_write_cmd(volatile f10xxx_spi_t *const spi_x,
			u8 cmd, u8 *buf, u32 len, u8 *status);
void nrf24l01_read_cmd(volatile f10xxx_spi_t *const spi_x,
		       u8 cmd, u8 *buf, u32 len);

/* configure spi_x to communicate with nrf24l01
*/
void nrf24l01_configure_spi(volatile f10xxx_spi_t *const spi_x);

/* configure nrf24l01
*/
void nrf24l01_simple_configure_rx(volatile f10xxx_spi_t *const spi_x);

void nrf24l01_simple_configure_tx(volatile f10xxx_spi_t *const spi_x);

/* polls the nrf24l01 until some data is available and writes it in buf,
   returns the number of bytes received (max 32, so buf should point to at least
   32 bytes of free memory
*/
u32 nrf24l01_receive_poll(volatile f10xxx_spi_t *const spi_x,
			  u8 *buf);

/* polls the nrf24l01 until data can be sent then sends the data contained in
   buf and of length len (max 32, 0 is an error too).
*/
void nrf24l01_send_poll(volatile f10xxx_spi_t *const spi_x,
			u8 *buf,
			u32 len);


#endif
