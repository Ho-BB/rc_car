

#include "inc/stm32f10xxx.h"


extern u32 __start_ram[];


/*
  B3 = IN1 H-bridge
  B4 = IN2 H-bridge
  B5 = IN3 H-bridge
  B6 = IN4 H-bridge
  
  B7 = Motor PWM

  B9 = servo gauche
  B8 = servo droit

  A9  = TX
  A10 = RX
*/

void f10xxx_usart1_isr();
void f10xxx_tim2_isr();
void f10xxx_tim3_isr();

#define SERVO_CONTROL_STEP 10
#define MOTOR_CONTROL_STEP 200

#define PWM_PERIOD 4000

__attribute__((noreturn))
void f10xxx_reset_isr(){

  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  __start_ram[0] = 1;
  __start_ram[1] = 1500;
  __start_ram[2] = 1;
  __start_ram[3] = 2000;

  f10xxx_rcc_enable_clock_apb1(tim2);
  f10xxx_rcc_enable_clock_apb1(tim3);
  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(iopb);
  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopc);
  
  f10xxx_rcc_reset_apb1(tim2);
  f10xxx_rcc_reset_apb1(tim3);
  f10xxx_rcc_reset_apb2(usart1);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER3);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_USART1);

  f10xxx_usart_enable_irq(usart_1, rx_not_empty_irq);
  f10xxx_timer_enable_update_interrupt(timer_2);
  f10xxx_timer_enable_update_interrupt(timer_3);

  //led conf
  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_c, port_13, set);
  
  //Servo ports conf
  f10xxx_gpio_configure_port(gpio_b,
			     port_9,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(gpio_b,
			     port_8,
			     output_10mhz,
			     gp_output_push_pull);

  //H-Bridge IN3-4 conf
  f10xxx_gpio_configure_port(gpio_b,
			     port_5,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(gpio_b,
			     port_6,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_b, port_5, reset);
  f10xxx_gpio_write_port(gpio_b, port_6, reset);

  //motor PWM conf
  f10xxx_gpio_configure_port(gpio_b,
			     port_7,
			     output_10mhz,
			     gp_output_push_pull);

  //usart ports conf
  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);
  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  //usart conf
  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);
  
  //timer conf
  f10xxx_timer_set_prescaler(timer_2, 7);
  f10xxx_timer_set_prescaler(timer_3, 7);

  f10xxx_tim2_isr();
  f10xxx_tim3_isr();

  f10xxx_gpio_write_port(gpio_c, port_13, reset);
  
  while(1);
}


void f10xxx_usart1_isr(){

  u32 c = f10xxx_usart_read(usart_1);

  if(c == 'a'){
    __start_ram[1] += SERVO_CONTROL_STEP;
  }
  else if(c == 'e'){
    __start_ram[1] -= SERVO_CONTROL_STEP;
  }
  else if(c == 'q'){
    if(__start_ram[3] < PWM_PERIOD)
      __start_ram[3] += MOTOR_CONTROL_STEP;
  }
  else if(c == 'd'){
    if(__start_ram[3] > 200)
      __start_ram[3] -= MOTOR_CONTROL_STEP;
  }
  else if(c == 'o'){
    f10xxx_gpio_write_port(gpio_b, port_5, set);
    f10xxx_gpio_write_port(gpio_b, port_6, reset);
  }
  else if(c == 'p'){
    f10xxx_gpio_write_port(gpio_b, port_5, reset);
    f10xxx_gpio_write_port(gpio_b, port_6, set);
  }
}

void f10xxx_tim2_isr(){
  
  if(__start_ram[0] == 1){
    __start_ram[0] = 0;
    f10xxx_gpio_write_port(gpio_b, port_9, reset);
    f10xxx_gpio_write_port(gpio_b, port_8, reset);
    f10xxx_timer_set_autoreload(timer_2, 18500);
    f10xxx_timer_start(timer_2, one_pulse, upcounter);
  }
  else{
    __start_ram[0] = 1;
    f10xxx_gpio_write_port(gpio_b, port_9, set);
    f10xxx_gpio_write_port(gpio_b, port_8, set);
    f10xxx_timer_set_autoreload(timer_2, __start_ram[1]);
    f10xxx_timer_start(timer_2, one_pulse, upcounter);
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_2);
  cortex_m3_dsb();
}

void f10xxx_tim3_isr(){

  f10xxx_gpio_write_port(gpio_c, port_13, set);

  if(__start_ram[2] == 1){
    __start_ram[2] = 0;
    f10xxx_gpio_write_port(gpio_b, port_7, reset);
    f10xxx_timer_set_autoreload(timer_3, PWM_PERIOD - __start_ram[3]);
    f10xxx_timer_start(timer_3, one_pulse, upcounter);
  }
  else{
    __start_ram[2] = 1;
    f10xxx_gpio_write_port(gpio_b, port_7, set);
    f10xxx_timer_set_autoreload(timer_3, __start_ram[3]);
    f10xxx_timer_start(timer_3, one_pulse, upcounter);
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_3);
  cortex_m3_dsb();
}
