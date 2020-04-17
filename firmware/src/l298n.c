

#include "inc/l298n.h"


void l298n_init(l298n_t *ln,
		volatile f10xxx_gpio_t *gpio_en,
		f10xxx_gpio_port_e port_en,
		volatile f10xxx_gpio_t *gpio_in1,
		f10xxx_gpio_port_e port_in1,
		volatile f10xxx_gpio_t *gpio_in2,
		f10xxx_gpio_port_e port_in2,
		l298n_way_e default_way){

  ln->gpio_en = gpio_en;
  ln->port_en = port_en;
  ln->port_en_status = reset;

  ln->gpio_in1 = gpio_in1;
  ln->port_in1 = port_in1;

  ln->gpio_in2 = gpio_in2;
  ln->port_in2 = port_in2;

  ln->way = l298n_stop_e;
  ln->default_way = default_way;

  f10xxx_gpio_configure_port(gpio_en,
			     port_en,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_write_port(gpio_en, port_en, reset);

  f10xxx_gpio_configure_port(gpio_in1,
			     port_in1,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_write_port(gpio_in1, port_in1, reset);

  f10xxx_gpio_configure_port(gpio_in2,
			     port_in2,
			     output_10mhz,
			     gp_output_push_pull);
  f10xxx_gpio_write_port(gpio_in2, port_in2, reset);
}

void l298n_stop(l298n_t *ln){

  ln->way = l298n_stop_e;
  f10xxx_gpio_write_port(ln->gpio_in1, ln->port_in1, reset);
  f10xxx_gpio_write_port(ln->gpio_in2, ln->port_in2, reset);
}

void l298n_start(l298n_t *ln){

  switch(ln->default_way){
  case l298n_way1_e :
    l298n_way1(ln);
    break;
  case l298n_way2_e :
    l298n_way2(ln);
    break;
  case l298n_stop_e :
    l298n_stop(ln);
    break;
  }
}

void l298n_way1(l298n_t *ln){

  ln->way = l298n_way1_e;
  f10xxx_gpio_write_port(ln->gpio_in1, ln->port_in1, set);
  f10xxx_gpio_write_port(ln->gpio_in2, ln->port_in2, reset);
}

void l298n_way2(l298n_t *ln){

  ln->way = l298n_way2_e;
  f10xxx_gpio_write_port(ln->gpio_in1, ln->port_in1, reset);
  f10xxx_gpio_write_port(ln->gpio_in2, ln->port_in2, set);
}

void l298n_change_way(l298n_t *ln){
  
  switch(ln->way){
  case l298n_stop_e :
    l298n_start(ln);
    break;
  case l298n_way1_e :
    l298n_way2(ln);
    break;
  case l298n_way2_e :
    l298n_way1(ln);
    break;
  }
}

void l298n_en_high(l298n_t *ln){

  ln->port_en_status = set;
  f10xxx_gpio_write_port(ln->gpio_en, ln->port_en, set);
}

void l298n_en_low(l298n_t *ln){

  ln->port_en_status = reset;
  f10xxx_gpio_write_port(ln->gpio_en, ln->port_en, reset);
}

void l298n_change_en(l298n_t *ln){

  switch(ln->port_en_status){
  case set :
    l298n_en_low(ln);
    break;
  case reset :
    l298n_en_high(ln);
    break;
  }
}
