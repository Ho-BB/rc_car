

#ifndef __F10XXX_SPI__
#define __F10XXX_SPI__

#include "inc/stdinclude.h"

//__attribute__((packed))
typedef struct f10xxx_spi{
  u32 CR1;
  u32 CR2;
  u32 SR;
  u32 DR;
  u32 CRCPR;
  u32 RXCRCR;
  u32 TXCRCR;
  u32 I2SCFGR;
  u32 I2SPR;
} f10xxx_spi_t;

extern volatile f10xxx_spi_t *const spi_1;
extern volatile f10xxx_spi_t *const spi_2;

typedef enum f10xxx_spi_num_lines
  {
   lines_2 = 0,
   lines_1_input = 2 << 14,
   lines_1_output = 3 << 14
  }f10xxx_spi_num_lines_e;

typedef enum f10xxx_spi_crc_mode
  {
   crc_disabled = 0,
   crc_enabled = 1 << 13
  }f10xxx_spi_crc_mode_e;

typedef enum f10xxx_spi_frame_format
  {
   frame_format_8 = 0,
   frame_format_16 = 1 << 11
  }f10xxx_spi_frame_format_e;

typedef enum f10xxx_spi_bit_endianess
  {
   msbit = 0,
   lsbit = 1 << 7
  }f10xxx_spi_bit_endianess_e;

typedef enum f10xxx_spi_clock_div
  {
   spi_clock_div_2 = 0,
   spi_clock_div_4 = 1 << 3,
   spi_clock_div_8 = 2 << 3,
   spi_clock_div_16 = 3 << 3,
   spi_clock_div_32 = 4 << 3,
   spi_clock_div_64 = 5 << 3,
   spi_clock_div_128 = 6 << 3,
   spi_clock_div_256 = 7 << 3
  }f10xxx_spi_clock_div_e;

typedef enum f10xxx_spi_mode
  {
   spi_slave_mode = 0,
   spi_master_mode = 1 << 2
  }f10xxx_spi_mode_e;

typedef enum f10xxx_spi_clock_pol_pha
  {
   spi_ck0_transition1 = 0,
   spi_ck0_transition2 = 1,
   spi_ck1_transition1 = 2,
   spi_ck1_transition2 = 3
  }f10xxx_spi_clock_pol_pha_e;

typedef enum f10xxx_spi_irq
  {
   spi_tx_empty_irq = 1 << 7,
   spi_rx_not_empty_irq = 1 << 6,
   spi_error_irq = 1 << 5
  }f10xxx_spi_irq_e;

typedef enum f10xxx_spi_dma
  {
   spi_dma_tx = 1 << 1,
   spi_dma_rx = 1
  }f10xxx_spi_dma_e;

typedef enum f10xxx_spi_status
  {
   spi_busy = 1 << 7,
   spi_overrun = 1 << 6,
   spi_mode_fault = 1 << 5,
   spi_crc_error = 1 << 4,
   spi_underrun = 1 << 3,
   spi_tx_empty = 1 << 1,
   spi_rx_not_empty = 1
  }f10xxx_spi_status_e;


void f10xxx_spi_enable_irq(volatile f10xxx_spi_t *const spi_x,
			   f10xxx_spi_irq_e irq);

void f10xxx_spi_disable_irq(volatile f10xxx_spi_t *const spi_x,
			    f10xxx_spi_irq_e irq);

void f10xxx_spi_enable_dma(volatile f10xxx_spi_t *const spi_x,
			   f10xxx_spi_dma_e dma);

void f10xxx_spi_disable_dma(volatile f10xxx_spi_t *const spi_x,
			    f10xxx_spi_dma_e dma);

u32 f10xxx_spi_status(volatile f10xxx_spi_t *const spi_x,
		      f10xxx_spi_status_e status);

void f10xxx_spi_enable(volatile f10xxx_spi_t *const spi_x,
		       f10xxx_spi_num_lines_e num_lines,
		       f10xxx_spi_crc_mode_e crc_mode,
		       f10xxx_spi_frame_format_e frame_format,
		       f10xxx_spi_bit_endianess_e bit_endianess,
		       f10xxx_spi_clock_div_e clock_div,
		       f10xxx_spi_mode_e spi_mode,
		       f10xxx_spi_clock_pol_pha_e clock_pol_pha);

void f10xxx_spi_simple_enable(volatile f10xxx_spi_t *const spi_x);

void f10xxx_spi_disable(volatile f10xxx_spi_t *const spi_x);

void f10xxx_spi_enable_ss_output(volatile f10xxx_spi_t *const spi_x);

void f10xxx_spi_disable_ss_output(volatile f10xxx_spi_t *const spi_x);

u16 f10xxx_spi_read(volatile f10xxx_spi_t *const spi_x);

void f10xxx_spi_write(volatile f10xxx_spi_t *const spi_x,
		      u16 data);

#endif
