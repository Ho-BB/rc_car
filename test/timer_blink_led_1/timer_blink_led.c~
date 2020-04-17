

//GCC already adds 1 to symbols defined in C if they are thumb code
//I have to do the + 1 in the linker script for default values and not in the
//ivt.s

//The whole IVT table was discarded during linking (because it's a lib?) I have
//to KEEP everything

#include "cortex_m3.h"
#include "f10xxx_gpio.h"
#include "f10xxx_nvic.h"
#include "f10xxx_timer.h"
#include "f10xxx_rcc.h"
#include "stdinclude.h"

#define TIMER_CLOCK_GUESS 8000000

extern u32 __start_ram[];

__attribute__((noreturn))
void f10xxx_reset_isr(){

  *__start_ram = 0;

  f10xxx_rcc_enable_clock_apb2(iopc);
  f10xxx_rcc_enable_clock_apb1(tim2);

  f10xxx_rcc_reset_apb1(tim2);

  f10xxx_gpio_configure_port(gpio_c,
			     port_14,
			     output_10mhz,
			     gp_output_push_pull);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);
  
  f10xxx_timer_enable_update_interrupt(timer_2);
  f10xxx_timer_set_autoreload(timer_2, 0xFFFF);
  f10xxx_timer_set_prescaler(timer_2, 1 << 5);
  f10xxx_timer_start(timer_2, auto_reload, upcounter);
  
  while(1);
}

void f10xxx_tim2_isr(){

  if(*__start_ram == 1){
    *__start_ram = 0;
    f10xxx_gpio_write_port(gpio_c, port_14, reset);
  }
  else{
    *__start_ram = 1;
    f10xxx_gpio_write_port(gpio_c, port_14, set);
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_2);
  cortex_m3_dsb();
}
