

#include "inc/pwm.h"

void pwm_init(pwm_t *pwm,
	      volatile f10xxx_timer_t *timer_x,
	      u16 period,
	      u16 pulse){

  if(period == 0 || pulse == 0){
    pwm->high = 0;
    pwm->low = period;
    pwm->period = period;
    pwm->state = pwm_state_always_low;
  }
  else if(pulse >= period){//always high
    pwm->high = period;
    pwm->low = 0;
    pwm->period = period;
    pwm->state = pwm_state_always_high;
  }
  else{
    pwm->high = pulse;
    pwm->low = period - pulse;
    pwm->period = period;
    pwm->state = pwm_state_high;
  }

  f10xxx_timer_set_prescaler(timer_x, 7);//suposes clock speed
  
  pwm->timer_x = timer_x;
}

void pwm_set_pulse(pwm_t *pwm, u16 pulse){

  if(pwm->period != 0){
    if(pulse == 0){
      pwm->high = 0;
      pwm->low = pwm->period;
      pwm->state = pwm_state_always_low;
    }
    else if(pulse >= pwm->period){
      pwm->high = pwm->period;
      pwm->low = 0;
      pwm->state = pwm_state_always_high;
    }
    else{
      pwm->high = pulse;
      pwm->low = pwm->period - pulse;
    }
  }
}

void pwm_set_period(pwm_t *pwm, u16 period){

  if(period == 0){
    pwm->high = 0;
    pwm->low = 0;
    pwm->period = 0;
    pwm->state = pwm_state_always_low;
  }
  else if(period <= pwm->high){
    pwm->high = period;
    pwm->low = 0;
    pwm->period = period;
    pwm->state = pwm_state_always_high;
  }
  else{
    pwm->low = period - pwm->high;
    pwm->period = period;
  }
}

void pwm_step(pwm_t *pwm){

  switch(pwm->state){
  case pwm_state_always_high :
    f10xxx_timer_set_autoreload(pwm->timer_x, pwm->period);
    f10xxx_timer_start(pwm->timer_x, one_pulse, upcounter);
    break;
  case pwm_state_always_low :
    f10xxx_timer_set_autoreload(pwm->timer_x, 0xFFFF);
    f10xxx_timer_start(pwm->timer_x, one_pulse, upcounter);
    break;
  case pwm_state_high :
    pwm->state = pwm_state_low;
    f10xxx_timer_set_autoreload(pwm->timer_x, pwm->low);
    f10xxx_timer_start(pwm->timer_x, one_pulse, upcounter);
    break;
  case pwm_state_low :
    pwm->state = pwm_state_high;
    f10xxx_timer_set_autoreload(pwm->timer_x, pwm->high);
    f10xxx_timer_start(pwm->timer_x, one_pulse, upcounter);
    break;
  }
}
