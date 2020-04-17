

#include "inc/stdinclude.h"
#include "inc/f10xxx_timer.h"


volatile f10xxx_timer_t *timer_2 = (f10xxx_timer_t *) 0x40000000;
volatile f10xxx_timer_t *timer_3 = (f10xxx_timer_t *) 0x40000400;
volatile f10xxx_timer_t *timer_4 = (f10xxx_timer_t *) 0x40000800;


void f10xxx_timer_start(volatile f10xxx_timer_t *timer_x,
			f10xxx_timer_mode_e mode,
			f10xxx_timer_dir_e dir){

  timer_x->CR1 = dir | mode | 1;
}

void f10xxx_timer_stop(volatile f10xxx_timer_t *timer_x){

  timer_x->CR1 = 0;
}

u32 f10xxx_timer_is_running(volatile f10xxx_timer_t *timer_x){

  return timer_x->CR1 & 1;
}

void f10xxx_timer_set_counter(volatile f10xxx_timer_t *timer_x, u16 counter){

  timer_x->CNT = counter;
}

u16 f10xxx_timer_get_counter(volatile f10xxx_timer_t *timer_x){

  return (u16) timer_x->CNT;
}

void f10xxx_timer_set_prescaler(volatile f10xxx_timer_t *timer_x,
				u16 prescaler){

  timer_x->PSC = prescaler;
}

u16 f10xxx_timer_get_prescaler(volatile f10xxx_timer_t *timer_x){

  return (u16) timer_x->PSC;
}

void f10xxx_timer_set_autoreload(volatile f10xxx_timer_t *timer_x,
				 u16 autoreload){

  timer_x->ARR = autoreload;
}

u16 f10xxx_timer_get_autoreload(volatile f10xxx_timer_t *timer_x){

  return timer_x->ARR;
}

void f10xxx_timer_generate_update(volatile f10xxx_timer_t *timer_x){

  timer_x->EGR = 1;
}

void f10xxx_timer_enable_update_interrupt(volatile f10xxx_timer_t *timer_x){
  
  timer_x->DIER = 1;
}

void f10xxx_timer_disable_update_interrupt(volatile f10xxx_timer_t *timer_x){

  timer_x->DIER = 0;
}

void
f10xxx_timer_acknowledge_update_interrupt(volatile f10xxx_timer_t *timer_x){

  timer_x->SR = 0;
}

u32 f10xxx_timer_update_interrupt_pending(volatile f10xxx_timer_t *timer_x){

  return timer_x->SR & 1;
}

//conversion utils

u32 f10xxx_timer_us_to_ticks(u32 clk_mhz, u32 us){

  return clk_mhz * us;
}

u32 f10xxx_timer_ticks_to_us(u32 clk_mhz, u32 ticks){

  return ticks / clk_mhz;
}

u32 f10xxx_timer_split_ticks(u32 ticks){

  u32 prescaler = 1;
  while(ticks >= (1 << 16)){
    ticks >>= 1;
    prescaler <<= 1;
  }

  return (ticks << 16) | (prescaler - 1);
}
