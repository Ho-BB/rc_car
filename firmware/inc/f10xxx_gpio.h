
#ifndef __F10XXX_GPIO__
#define __F10XXX_GPIO__

#include "inc/stdinclude.h"

//__attribute__((packed))
typedef struct f10xxx_gpio{
  u32 CRL;
  u32 CRH;
  u32 IDR;
  u32 ODR;
  u32 BSRR;
  u32 BRR;
  u32 LCKR;
} f10xxx_gpio_t;


typedef enum f10xxx_gpio_port
  {
   port_0,
   port_1,
   port_2,
   port_3,
   port_4,
   port_5,
   port_6,
   port_7,
   port_8,
   port_9,
   port_10,
   port_11,
   port_12,
   port_13,
   port_14,
   port_15
  }f10xxx_gpio_port_e;


typedef enum f10xxx_gpio_mode
  {
   input,
   output_10mhz,
   output_2mhz,
   output_50mhz
  }f10xxx_gpio_mode_e;


typedef enum f10xxx_gpio_conf
  {
   input_analog = 0,
   gp_output_push_pull = 0,
   
   input_floating = 1 << 2,
   gp_output_open_drain = 1 << 2,
   
   input_pull_up_down = 2 << 2,
   af_output_push_pull = 2 << 2,

   af_output_open_drain = 3 << 2
  }f10xxx_gpio_conf_e;


typedef enum f10xxx_gpio_val
  {
   reset,
   set
  }f10xxx_gpio_val_e;


extern volatile f10xxx_gpio_t *gpio_a;
extern volatile f10xxx_gpio_t *gpio_b;
extern volatile f10xxx_gpio_t *gpio_c;


void f10xxx_gpio_configure_port(volatile f10xxx_gpio_t *gpio_x,
				f10xxx_gpio_port_e port,
				f10xxx_gpio_mode_e mode,
				f10xxx_gpio_conf_e conf);


f10xxx_gpio_val_e f10xxx_gpio_read_port(volatile f10xxx_gpio_t *gpio_x,
					f10xxx_gpio_port_e port);


void f10xxx_gpio_write_port(volatile f10xxx_gpio_t *gpio_x,
			    f10xxx_gpio_port_e port,
			    f10xxx_gpio_val_e val);

#endif
