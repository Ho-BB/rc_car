

#ifndef __L298N__
#define __L298N__

#include "inc/f10xxx_gpio.h"

typedef enum l298n_way
  {
   l298n_stop_e,
   l298n_way1_e,
   l298n_way2_e
  }l298n_way_e;

typedef struct l298n{
  volatile f10xxx_gpio_t *gpio_en;
  f10xxx_gpio_port_e port_en;
  f10xxx_gpio_val_e port_en_status;
  
  volatile f10xxx_gpio_t *gpio_in1;
  f10xxx_gpio_port_e port_in1;
  
  volatile f10xxx_gpio_t *gpio_in2;
  f10xxx_gpio_port_e port_in2;

  l298n_way_e way;
  l298n_way_e default_way;
} l298n_t;

void l298n_init(l298n_t *ln,
		volatile f10xxx_gpio_t *gpio_en,
		f10xxx_gpio_port_e port_en,
		volatile f10xxx_gpio_t *gpio_in1,
		f10xxx_gpio_port_e port_in1,
		volatile f10xxx_gpio_t *gpio_in2,
		f10xxx_gpio_port_e port_in2,
		l298n_way_e default_way);

void l298n_stop(l298n_t *ln);
void l298n_start(l298n_t *ln);
void l298n_way1(l298n_t *ln);
void l298n_way2(l298n_t *ln);
void l298n_change_way(l298n_t *ln);

void l298n_en_high(l298n_t *ln);
void l298n_en_low(l298n_t *ln);
void l298n_change_en(l298n_t *ln);

#endif
