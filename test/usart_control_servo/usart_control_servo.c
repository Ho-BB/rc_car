

#include "inc/stm32f10xxx.h"

//TX = PA9 | RX = PA10

extern u32 __start_ram[];

void f10xxx_usart1_isr();
void f10xxx_tim2_isr();

#define SERVO_CONTROL_STEP 10

__attribute__((noreturn))
void f10xxx_reset_isr(){

  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  __start_ram[0] = 1;
  __start_ram[1] = 1500;

  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopb);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_apb1(tim2);

  f10xxx_rcc_reset_apb2(usart1);
  f10xxx_rcc_reset_apb1(tim2);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_USART1);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);

  f10xxx_usart_enable_irq(usart_1, rx_not_empty_irq);
  f10xxx_timer_enable_update_interrupt(timer_2);

  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  f10xxx_gpio_configure_port(gpio_b,
			     port_9,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_timer_set_prescaler(timer_2, 7);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  f10xxx_tim2_isr();

  while(1);
}

void f10xxx_usart1_isr(){

  if(f10xxx_usart_read(usart_1) == 'a'){
    __start_ram[1] += SERVO_CONTROL_STEP;
  }
  else{
    __start_ram[1] -= SERVO_CONTROL_STEP;
  }
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
    f10xxx_timer_set_autoreload(timer_2, __start_ram[1]);
    f10xxx_timer_start(timer_2, one_pulse, upcounter);
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_2);
  cortex_m3_dsb();
}
