

#include "inc/stm32f10xxx.h"
#include "inc/nrf24l01.h"
#include "inc/pwm.h"
#include "inc/l298n.h"
#include "inc/utils.h"

extern u32 __start_ram[];

#define PWM_MOTOR ((pwm_t *) __start_ram[0])
#define PWM_SERVO ((pwm_t *) __start_ram[1])
#define RIGHT_MOTOR ((l298n_t *) __start_ram[2])
#define LEFT_MOTOR ((l298n_t *) __start_ram[3])

__attribute__((noreturn))
void f10xxx_reset_isr(){

  pwm_t pwm_motor;
  pwm_t pwm_servo;
  l298n_t right_motor;
  l298n_t left_motor;
  nrf24l01_t rf;
  u8 buf[3];
  u8 servo_byte;
  u8 motor_byte;
  u8 led_status;

  __start_ram[0] = (u32) &pwm_motor;
  __start_ram[1] = (u32) &pwm_servo;
  __start_ram[2] = (u32) &right_motor;
  __start_ram[3] = (u32) &left_motor;
  
  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  
  f10xxx_rcc_enable_clock_apb2(spi1);
  f10xxx_rcc_enable_clock_apb1(tim2);
  f10xxx_rcc_enable_clock_apb1(tim3);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(iopb);
  f10xxx_rcc_enable_clock_apb2(iopc);

  usart_setup_usart_1();

  f10xxx_afio_disable_jtag();
  
  f10xxx_rcc_reset_apb2(spi1);
  f10xxx_rcc_reset_apb1(tim2);
  f10xxx_rcc_reset_apb1(tim3);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER2);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_TIMER3);

  f10xxx_timer_enable_update_interrupt(timer_2);
  f10xxx_timer_enable_update_interrupt(timer_3);

  //led conf
  f10xxx_gpio_configure_port(gpio_c,
			     port_13,
			     output_10mhz,
			     gp_output_push_pull);

  f10xxx_gpio_write_port(gpio_c, port_13, reset);
  led_status = 0;

  //Servo ports conf
  f10xxx_gpio_configure_port(gpio_b,
			     port_9,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_configure_port(gpio_b,
			     port_8,
			     output_10mhz,
			     gp_output_push_pull);

  pwm_init(&pwm_motor, timer_3, 4000, 1000);
  pwm_init(&pwm_servo, timer_2, 20000, 1500);

  l298n_init(&left_motor,
	     gpio_b,
	     port_0,
	     gpio_b,
	     port_3,
	     gpio_b,
	     port_4,
	     l298n_way2_e);
  
  l298n_init(&right_motor,
	     gpio_b,
	     port_7,
	     gpio_b,
	     port_5,
	     gpio_b,
	     port_6,
	     l298n_way1_e);

  nrf24l01_init(&rf,
		spi_1,
		gpio_a,
		port_4,
		gpio_a,
		port_5,
		gpio_a,
		port_6,
		gpio_a,
		port_7,
		gpio_a,
		port_3);

  nrf24l01_configure_rx(&rf, 3);

  pwm_step(&pwm_motor);
  pwm_step(&pwm_servo);
  
  while(1){

    nrf24l01_ce_high(&rf);

    do{
      nrf24l01_write(&rf, NRF24L01_NOP, NULL, 0);
    }while(NRF24L01_RX_FIFO_EMPTY(rf.status));

    if(led_status == 1){
      led_status = 0;
      f10xxx_gpio_write_port(gpio_c, port_13, reset);
    }
    else{
      led_status = 1;
      f10xxx_gpio_write_port(gpio_c, port_13, set);
    }
    
    nrf24l01_ce_low(&rf);

    nrf24l01_read(&rf, NRF24L01_READ_RX, buf, 3);

    usart_write_str(usart_1, (u8 *) "x : ");
    usart_write_byte(usart_1, buf[0]);
    usart_write_str(usart_1, (u8 *) "  |  ");

    usart_write_str(usart_1, (u8 *) "y : ");
    usart_write_byte(usart_1, buf[1]);
    usart_write_str(usart_1, (u8 *) "\n");

    servo_byte = buf[0];
    motor_byte = buf[1];

    if(servo_byte <= 100){
      pwm_set_pulse(&pwm_servo, 1500 - ((100 - servo_byte) * 3));
    }
    else if(servo_byte >= 155){
      pwm_set_pulse(&pwm_servo, 1500 + ((servo_byte - 155) * 3));
    }
    else{
      pwm_set_pulse(&pwm_servo, 1500);
    }

    if(motor_byte <= 100){
      l298n_way1(&right_motor);
      l298n_way2(&left_motor);
      pwm_set_pulse(&pwm_motor, 1000 + ((100 - motor_byte) * 25));
    }
    else if(motor_byte >= 155){
      l298n_way2(&right_motor);
      l298n_way1(&left_motor);
      pwm_set_pulse(&pwm_motor, 1000 + ((motor_byte - 155) * 25));
    }
    else{
      l298n_stop(&right_motor);
      l298n_stop(&left_motor);
    }
  }
}

void f10xxx_tim2_isr(){

  pwm_step(PWM_SERVO);

  switch(PWM_SERVO->state){
  case pwm_state_high :
    f10xxx_gpio_write_port(gpio_b, port_9, set);
    f10xxx_gpio_write_port(gpio_b, port_8, set);
    break;
  case pwm_state_always_high :
    f10xxx_gpio_write_port(gpio_b, port_9, set);
    f10xxx_gpio_write_port(gpio_b, port_8, set);
    break;
  case pwm_state_low :
    f10xxx_gpio_write_port(gpio_b, port_9, reset);
    f10xxx_gpio_write_port(gpio_b, port_8, reset);
    break;
  case pwm_state_always_low :
    f10xxx_gpio_write_port(gpio_b, port_9, reset);
    f10xxx_gpio_write_port(gpio_b, port_8, reset);
    break;
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_2);
  cortex_m3_dsb();
}

void f10xxx_tim3_isr(){

  pwm_step(PWM_MOTOR);

  switch(PWM_MOTOR->state){
  case pwm_state_high :
    l298n_en_high(RIGHT_MOTOR);
    l298n_en_high(LEFT_MOTOR);
    break;
  case pwm_state_always_high :
    l298n_en_high(RIGHT_MOTOR);
    l298n_en_high(LEFT_MOTOR);
    break;
  case pwm_state_low :
    l298n_en_low(RIGHT_MOTOR);
    l298n_en_low(LEFT_MOTOR);
    break;
  case pwm_state_always_low :
    l298n_en_low(RIGHT_MOTOR);
    l298n_en_low(LEFT_MOTOR);
    break;
  }

  f10xxx_timer_acknowledge_update_interrupt(timer_3);
  cortex_m3_dsb();
}
