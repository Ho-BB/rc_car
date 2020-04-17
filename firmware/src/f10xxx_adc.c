

#include "inc/f10xxx_adc.h"
#include "inc/f10xxx_rcc.h"


volatile f10xxx_adc_t *const adc_1 = (volatile f10xxx_adc_t *const) 0x40012400;
volatile f10xxx_adc_t *const adc_2 = (volatile f10xxx_adc_t *const) 0x40012800;


void f10xxx_adc_simple_init(volatile f10xxx_adc_t *const adc_x){

  u32 i;
  
  if(adc_x == adc_1)
    f10xxx_rcc_reset_apb2(adc1);
  else
    f10xxx_rcc_reset_apb2(adc2);

  adc_x->CR2 = 1;

  i = 256;
  while(i--);

  adc_x->CR2 = (1 << 2) | 1;
  while(adc_x->CR2 & (1 << 2));
}

void f10xxx_adc_enable_eoc_irq(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR1 |= (1 << 5);
}

void f10xxx_adc_disable_eoc_irq(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR1 &= ~(1 << 5);
}

void f10xxx_adc_regular_sequence_length(volatile f10xxx_adc_t *const adc_x,
					u8 len){

  adc_x->SQR[0] = (adc_x->SQR[0] & ~(0x0F << 20)) | (((len - 1) & 0x0F) << 20);
}

void f10xxx_adc_set_channel_position(volatile f10xxx_adc_t *const adc_x,
				     u8 pos,
				     f10xxx_adc_channel_e channel){

  u32 x;
  
  pos = pos & 0x0F;
  if(pos >= 12){
    pos = pos - 12;
    x = 0;
  }
  else if(pos >= 6){
    pos = pos - 6;
    x = 1;
  }
  else{
    x = 2;
  }
  
  adc_x->SQR[x] =
    (adc_x->SQR[x] & ~(0x1F << (5 * pos)))
    | ((u32) channel << (5 * pos));
}

void f10xxx_adc_enable_continuous(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR2 |= 2;
}

void f10xxx_adc_disable_continuous(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR2 &= ~2;
}

void f10xxx_adc_enable_discontinuous(volatile f10xxx_adc_t *const adc_x,
				     u8 disc_len){

  adc_x->CR1 =
    (adc_x->CR1 & ~(0x07 << 13))
    | (1 << 11)
    | (((disc_len - 1) & 0x07) << 13);
}

void f10xxx_adc_disable_discontinuous(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR1 &= ~(1 << 11);
}

void f10xxx_adc_enable_software_trigger(volatile f10xxx_adc_t *const adc_x){

  //if not modified then it'll start a single regular conversion, potential
  //problem
  adc_x->CR2 |= 0x0F << 17;
}

void f10xxx_adc_trigger(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR2 |= 1 << 22;
}

void f10xxx_adc_start(volatile f10xxx_adc_t *const adc_x){

  adc_x->CR2 = adc_x->CR2;
}

u32 f10xxx_adc_eoc(volatile f10xxx_adc_t *const adc_x){

  return adc_x->SR & 2;
}

u16 f10xxx_adc_get_regular_value(volatile f10xxx_adc_t *const adc_x){

  return *((u16 *) &adc_x->DR);
}
