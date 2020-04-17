

#ifndef __F10XXX_USART__
#define __F10XXX_USART__

#include "inc/stdinclude.h"

//__attribute__((packed))
typedef struct f10xxx_usart{
  u32 SR;
  u32 DR;
  u32 BRR;
  u32 CR1;
  u32 CR2;
  u32 CR3;
  u32 GTPR;
} f10xxx_usart_t;

extern volatile f10xxx_usart_t *usart_1;
extern volatile f10xxx_usart_t *usart_2;
extern volatile f10xxx_usart_t *usart_3;

typedef enum f10xxx_usart_baud_rate
  {
   B9600
  }f10xxx_usart_baud_rate_e;

typedef enum f10xxx_usart_frame_format
  {
   data8_parity0,
   data7_parity1,
   data9_parity0,
   data8_parity1
  }f10xxx_usart_frame_format_e;

typedef enum f10xxx_usart_parity
  {
   even = 0,
   odd = 1 << 9
  }f10xxx_usart_parity_e;

typedef enum f10xxx_usart_stop_bits
  {
   stop05 = 1 << 12,
   stop1 = 0,
   stop15 = 3 << 12,
   stop2 = 2 << 12
  }f10xxx_usart_stop_bits_e;

typedef enum f10xxx_usart_irq
  {
   frame_error_irq = 1,
   idle_line_irq = 1 << 4,
   rx_not_empty_irq = 1 << 5,
   transmission_complete_irq = 1 << 6,
   tx_empty_irq = 1 << 7,
   parity_error_irq = 1 << 8
  }f10xxx_usart_irq_e;

typedef enum f10xxx_usart_status
  {
   parity_error = 1,
   frame_error = 1 << 1,
   noise_error = 1 << 2,
   overrun_error = 1 << 3,
   idle_line = 1 << 4,
   rx_not_empty = 1 << 5,
   transmission_complete = 1 << 6,
   tx_empty = 1 << 7,
   lin_break = 1 << 8,
   cts = 1 << 9,
   no_status = 1 << 10
  }f10xxx_usart_status_e;

typedef enum f10xxx_usart_dma_mode
  {
   transmission = 1 << 7,
   reception = 1 << 6
  }f10xxx_usart_dma_mode_e;

void f10xxx_usart_start(volatile f10xxx_usart_t *usart_x,
			f10xxx_usart_baud_rate_e baud_rate,
			f10xxx_usart_frame_format_e frame_format,
			f10xxx_usart_parity_e parity,
			f10xxx_usart_stop_bits_e stop_bits);

u32 f10xxx_usart_read(volatile f10xxx_usart_t *usart_x);

void f10xxx_usart_write(volatile f10xxx_usart_t *usart_x,
			u32 data);

f10xxx_usart_status_e f10xxx_usart_status(volatile f10xxx_usart_t *usart_x);

void f10xxx_usart_enable_irq(volatile f10xxx_usart_t *usart_x,
			     f10xxx_usart_irq_e irq);

void f10xxx_usart_disable_irq(volatile f10xxx_usart_t *usart_x,
			      f10xxx_usart_irq_e irq);

u32 f10xxx_usart_tx_empty(volatile f10xxx_usart_t *usart_x);
u32 f10xxx_usart_rx_not_empty(volatile f10xxx_usart_t *usart_x);

void f10xxx_usart_enable_dma(volatile f10xxx_usart_t *usart_x,
			     f10xxx_usart_dma_mode_e dma_mode);

void f10xxx_usart_disable_dma(volatile f10xxx_usart_t *usart_x,
			      f10xxx_usart_dma_mode_e dma_mode);

#endif
