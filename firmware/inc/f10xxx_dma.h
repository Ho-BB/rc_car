

#ifndef __F10XXX_DMA__
#define __F10XXX_DMA__

#include "inc/stdinclude.h"

//__attribute__((packed))
typedef struct f10xxx_dma{
  u32 ISR;
  u32 IFCR;
} f10xxx_dma_t;

//__attribute__((packed))
typedef struct f10xxx_dma_channel{
  u32 CCR;
  u32 CNDTR;
  u32 CPAR;
  u32 CMAR;
} f10xxx_dma_channel_t;

extern volatile f10xxx_dma_t *const dma;

extern volatile f10xxx_dma_channel_t *const dma_channel_1;
extern volatile f10xxx_dma_channel_t *const dma_channel_2;
extern volatile f10xxx_dma_channel_t *const dma_channel_3;
extern volatile f10xxx_dma_channel_t *const dma_channel_4;
extern volatile f10xxx_dma_channel_t *const dma_channel_5;
extern volatile f10xxx_dma_channel_t *const dma_channel_6;
extern volatile f10xxx_dma_channel_t *const dma_channel_7;

typedef enum f10xxx_dma_chan
  {
   channel_1 = 0,
   channel_2 = 4,
   channel_3 = 8,
   channel_4 = 12,
   channel_5 = 16,
   channel_6 = 20,
   channel_7 = 24
  }f10xxx_dma_chan_e;

typedef enum f10xxx_dma_irq
  {
   global_irq = 1,
   transfer_complete_irq = 2,
   half_transfer_irq = 4,
   transfer_error_irq = 8
  }f10xxx_dma_irq_e;

typedef enum f10xxx_dma_channel_priority
  {
   channel_priority_low = 0,
   channel_priority_medium = 1 << 12,
   channel_priority_high = 2 << 12,
   channel_priority_very_high = 3 << 12
  }f10xxx_dma_channel_priority_e;

typedef enum f10xxx_dma_data_size
  {
   data_size_8 = 0,
   data_size_16 = 1,
   data_size_32 = 2
  }f10xxx_dma_data_size_e;

typedef enum f10xxx_dma_transfer_direction
  {
   peripheral_to_memory = 0,
   memory_to_peripheral = 1 << 4
  }f10xxx_dma_transfer_direction_e;

typedef enum f10xxx_dma_memory_mode
  {
   memory_mode_disabled = 0,
   memory_mode_enabled = 1 << 14
  }f10xxx_dma_memory_mode_e;

typedef enum f10xxx_dma_memory_increment_mode
  {
   memory_increment_mode_disabled = 0,
   memory_increment_mode_enabled = 1 << 7
  }f10xxx_dma_memory_increment_mode_e;

typedef enum f10xxx_dma_peripheral_increment_mode
  {
   peripheral_increment_mode_disabled = 0,
   peripheral_increment_mode_enabled = 1 << 6
  }f10xxx_dma_peripheral_increment_mode_e;

typedef enum f10xxx_dma_circular_mode
  {
   circular_mode_disabled = 0,
   circular_mode_enabled = 1 << 5
  }f10xxx_dma_circular_mode_e;


void f10xxx_dma_set_memory_address
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 u32 address);

void f10xxx_dma_set_peripheral_address
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 u32 address);

void f10xxx_dma_set_number_of_data
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 u16 num_data);

void f10xxx_dma_enable_irq
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 f10xxx_dma_irq_e irq);

void f10xxx_dma_disable_irq
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 f10xxx_dma_irq_e irq);

void f10xxx_dma_enable
(volatile f10xxx_dma_channel_t *const dma_channel_x,
 f10xxx_dma_transfer_direction_e direction,
 f10xxx_dma_circular_mode_e circular_mode,
 f10xxx_dma_peripheral_increment_mode_e p_inc_mode,
 f10xxx_dma_memory_increment_mode_e m_inc_mode,
 f10xxx_dma_data_size_e peripheral_data_size,
 f10xxx_dma_data_size_e memory_data_size,
 f10xxx_dma_channel_priority_e channel_priority,
 f10xxx_dma_memory_mode_e memory_mode);

void f10xxx_dma_disable
(volatile f10xxx_dma_channel_t *const dma_channel_x);

u32 f10xxx_dma_irq_status(f10xxx_dma_chan_e dma_chan,
			  f10xxx_dma_irq_e irq);

void f10xxx_dma_irq_acknowledge(f10xxx_dma_chan_e dma_chan,
				f10xxx_dma_irq_e irq);


#endif
