

#include "inc/nRF24L01.h"


/* 0 <= reg <= 23

   the first byte of a read_cmd is status so buf should be len+1 length
*/
void nrf24l01_write_cmd(volatile f10xxx_spi_t *const spi_x,
			u8 cmd, u8 *buf, u32 len, u8 *status){

  u32 i;
  
  f10xxx_spi_simple_enable(spi_x);
  
  f10xxx_spi_write(spi_x, cmd);

  for(i = 0; i < len; i++){
    while(!f10xxx_spi_status(spi_x, spi_tx_empty));

    f10xxx_spi_write(spi_x, buf[i]);
  }

  //read the status we received when sending the cmd to prevent future overrun
  *status = (u8) f10xxx_spi_read(spi_x);

  f10xxx_spi_disable(spi_x);
}

void nrf24l01_read_cmd(volatile f10xxx_spi_t *const spi_x,
		       u8 cmd, u8 *buf, u32 len){

  u32 i;
  
  f10xxx_spi_simple_enable(spi_x);

  //f10xxx_spi_write(spi_x, cmd);

  for(i = 0; i <= len; i++){
    f10xxx_spi_write(spi_x, cmd);// I think i have to write to receive
    while(!f10xxx_spi_status(spi_x, spi_rx_not_empty));

    buf[i] = (u8) f10xxx_spi_read(spi_x);
  }

  f10xxx_spi_disable(spi_x);
}


void nrf24l01_configure_spi(volatile f10xxx_spi_t *const spi_x){

  f10xxx_spi_enable_ss_output(spi_x);
  f10xxx_spi_enable(spi_x,
		    lines_2,
		    crc_disabled,
		    frame_format_8,
		    msbit,
		    spi_clock_div_256,
		    spi_master_mode,
		    spi_ck0_transition1);

  //f10xxx_spi_disable(spi_x);//to avoid shift left 1 on first transaction
}

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

void nrf24l01_simple_configure_rx(volatile f10xxx_spi_t *const spi_x){

  u8 status;
  u8 buf[1];

  buf[0] = (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1) | 1;
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_CONFIG),
		     buf,
		     1,
		     &status);

  buf[0] = 0;
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_SETUP_RETR),
		     buf,
		     1,
		     &status);
}

void nrf24l01_simple_configure_tx(volatile f10xxx_spi_t *const spi_x){

  u8 status;
  u8 buf[1];

  buf[0] = (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1);
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_CONFIG),
		     buf,
		     1,
		     &status);

  buf[0] = 0;
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_SETUP_RETR),
		     buf,
		     1,
		     &status);
}

u32 nrf24l01_receive_poll(volatile f10xxx_spi_t *const spi_x,
			  u8 *buf){

  u8 status;
  u8 buf2[33];
  u32 len;
  u32 i;

  do{
    nrf24l01_write_cmd(spi_x,
		       NRF24L01_NOP,
		       NULL,
		       0,
		       &status);
  }while(NRF24L01_RX_FIFO_EMPTY(status));
  
  nrf24l01_read_cmd(spi_x,
		    NRF24L01_READ_REG(NRF24L01_REG_RX_PW_P0),
		    buf2,
		    1);

  len = (u32) buf2[1];

  nrf24l01_read_cmd(spi_x,
		    NRF24L01_READ_RX,
		    buf2,
		    len);

  for(i = 0; i < len; i++){
    buf[i] = buf2[i + 1];
  }

  buf2[0] = (1 << 6);
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_STATUS),
		     buf2,
		     1,
		     &status);

  return len;
}

void nrf24l01_send_poll(volatile f10xxx_spi_t *const spi_x,
			u8 *buf,
			u32 len){

  u8 status;
  u8 buf2[1];

  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_TX,
		     buf,
		     len,
		     &status);
  
  /*
  do{
    nrf24l01_write_cmd(spi_x,
		       NRF24L01_NOP,
		       NULL,
		       0,
		       &status);
  }while(!NRF24L01_DATA_SENT(status));
  
  buf2[0] = (1 << 5);
  nrf24l01_write_cmd(spi_x,
		     NRF24L01_WRITE_REG(NRF24L01_REG_STATUS),
		     buf2,
		     1,
		     &status);
  */
}
