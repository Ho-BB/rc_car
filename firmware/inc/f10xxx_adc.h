

#ifndef __F10XXX_ADC__
#define __F10XXX_ADC__

#include "stdinclude.h"

//__attribute__((packed))
typedef struct f10xxx_adc{
  u32 SR;
  u32 CR1;
  u32 CR2;
  u32 SMPR1;
  u32 SMPR2;
  u32 JOFR[4];
  u32 HTR;
  u32 LTR;
  u32 SQR[3];
  u32 JSQR;
  u32 JDR[4];
  u32 DR;
} f10xxx_adc_t;


typedef enum f10xxx_adc_channel
  {
   adc_channel_0,
   adc_channel_1,
   adc_channel_2,
   adc_channel_3,
   adc_channel_4,
   adc_channel_5,
   adc_channel_6,
   adc_channel_7,
   adc_channel_8,
   adc_channel_9,
   adc_channel_10,
   adc_channel_11,
   adc_channel_12,
   adc_channel_13,
   adc_channel_14,
   adc_channel_15,
   adc_channel_16,
   adc_channel_17
  }f10xxx_adc_channel_e;


extern volatile f10xxx_adc_t *const adc_1;
extern volatile f10xxx_adc_t *const adc_2;


void f10xxx_adc_simple_init(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_enable_eoc_irq(volatile f10xxx_adc_t *const adc_x);
void f10xxx_adc_disable_eoc_irq(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_regular_sequence_length(volatile f10xxx_adc_t *const adc_x,
					u8 len);

void f10xxx_adc_set_channel_position(volatile f10xxx_adc_t *const adc_x,
				     u8 pos,
				     f10xxx_adc_channel_e channel);

void f10xxx_adc_enable_continuous(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_disable_continuous(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_enable_discontinuous(volatile f10xxx_adc_t *const adc_x,
				     u8 disc_len);

void f10xxx_adc_disable_discontinuous(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_enable_software_trigger(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_trigger(volatile f10xxx_adc_t *const adc_x);

void f10xxx_adc_start(volatile f10xxx_adc_t *const adc_x);

u32 f10xxx_adc_eoc(volatile f10xxx_adc_t *const adc_x);

u16 f10xxx_adc_get_regular_value(volatile f10xxx_adc_t *const adc_x);


#endif
