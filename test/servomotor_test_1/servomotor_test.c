

#include "stm32f10xxx.h"


extern u32 __start_ram[];


void f10xxx_tim2_isr();


__attribute__((noreturn))
void f10xxx_reset_isr(){

  __start_ram[0] = 1;

  f10xxx_rcc_hse_on();//using HSE to see if it's more stable
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());
  
  f10xxx_rcc_enable_clock_apb2(iopb);
  f10xxx_rcc_enable_clock_apb1(tim2);

  f10xxx_rcc_reset_apb1(tim2);

  f10xxx_gpio_configure_port(gpio_b,
			     port_9,
			     output_10mhz,
			     gp_output_push_pull);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);
  
  f10xxx_timer_enable_update_interrupt(timer_2);
  f10xxx_timer_set_prescaler(timer_2, 7);

  f10xxx_tim2_isr();
  
  while(1);
}


void f10xxx_tim2_isr(){
  
  if(__start_ram[0] == 1){
    __start_ram[0] = 0;
    f10xxx_gpio_write_port(gpio_b, port_9, reset);
    f10xxx_timer_set_autoreload(timer_2, 18500);
    f10xxx_timer_start(timer_2, one_pulse, upcounter);
  }
  else{
    __start_ram[0] = 1;
    f10xxx_gpio_write_port(gpio_b, port_9, set);
    f10xxx_timer_set_autoreload(timer_2, 1500);
    f10xxx_timer_start(timer_2, one_pulse, upcounter);
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_2);
  cortex_m3_dsb();
}
