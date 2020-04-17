

#ifndef __PWM__
#define __PWM__

#include "inc/f10xxx_timer.h"

typedef enum pwm_state
  {
   pwm_state_high,
   pwm_state_low,
   pwm_state_always_high,
   pwm_state_always_low
  }pwm_state_e;

typedef struct pwm{
  u16 high;
  u16 low;
  u16 period;

  volatile f10xxx_timer_t *timer_x;
  pwm_state_e state;
} pwm_t;

void pwm_init(pwm_t *pwm,
	      volatile f10xxx_timer_t *timer_x,
	      u16 period,
	      u16 pulse);

void pwm_set_pulse(pwm_t *pwm, u16 pulse);

void pwm_set_period(pwm_t *pwm, u16 period);

void pwm_step(pwm_t *pwm);

#endif
