

#include "inc/stm32f10xxx.h"


extern u32 __start_ram[];


__attribute__((noreturn))
void f10xxx_reset_isr(){

  __start_ram[0] = 1;

  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  f10xxx_rcc_enable_clock_apb1(tim2);
  f10xxx_rcc_enable_clock_apb2(iopb);
  f10xxx_rcc_enable_clock_apb2(iopc);

  f10xxx_rcc_reset_apb1(tim2);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);

  f10xxx_gpio_configure_port(gpio_b,
			     port_9,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(gpio_b,
			     port_8,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_b, port_8, set);
  f10xxx_gpio_write_port(gpio_b, port_9, set);
  
  while(1);
}

