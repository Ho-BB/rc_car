
#include "inc/stdinclude.h"
#include "inc/f10xxx_gpio.h"


volatile f10xxx_gpio_t *gpio_a = (f10xxx_gpio_t *) 0x40010800;
volatile f10xxx_gpio_t *gpio_b = (f10xxx_gpio_t *) 0x40010C00;
volatile f10xxx_gpio_t *gpio_c = (f10xxx_gpio_t *) 0x40011000;


void f10xxx_gpio_configure_port(volatile f10xxx_gpio_t *gpio_x,
				f10xxx_gpio_port_e port,
				f10xxx_gpio_mode_e mode,
				f10xxx_gpio_conf_e conf){

  if((u32) port < 8){
    gpio_x->CRL =
      (gpio_x->CRL & ~(((u32) 0x0F) << ((u32) port << 2))) |
      (((u32) conf | (u32) mode) << ((u32) port << 2));
  }
  else{
    gpio_x->CRH =
      (gpio_x->CRH & ~(((u32) 0x0F) << (((u32) port - 8) << 2))) |
      (((u32) conf | (u32) mode) << (((u32) port - 8) << 2));
  }
}


f10xxx_gpio_val_e f10xxx_gpio_read_port(volatile f10xxx_gpio_t *gpio_x,
					f10xxx_gpio_port_e port){

  return (f10xxx_gpio_val_e) (gpio_x->IDR >> (u32) port) & 1;
}


void f10xxx_gpio_write_port(volatile f10xxx_gpio_t *gpio_x,
			    f10xxx_gpio_port_e port,
			    f10xxx_gpio_val_e val){

  if(val == set){
    gpio_x->BSRR = 1 << (u32) port;
  }
  else{
    gpio_x->BSRR = 1 << ((u32) port + 16);
  }
}

