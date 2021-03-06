

#ifndef __F10XXX_RCC__
#define __F10XXX_RCC__

#include "inc/stdinclude.h"


//__attribute__((packed))
typedef struct f10xxx_rcc{
  u32 CR;
  u32 CFGR;
  u32 CIR;
  u32 APB2RSTR;
  u32 APB1RSTR;
  u32 AHBENR;
  u32 APB2ENR;
  u32 APB1ENR;
  u32 BDCR;
  u32 CSR;
} f10xxx_rcc_t;

typedef enum f10xxx_rcc_apb2
  {
   afio   = 1,
   iopa   = 1 << 2,
   iopb   = 1 << 3,
   iopc   = 1 << 4,
   iopd   = 1 << 5,
   iope   = 1 << 6,
   iopf   = 1 << 7,
   iopg   = 1 << 8,
   adc1   = 1 << 9,
   adc2   = 1 << 10,
   tim1   = 1 << 11,
   spi1   = 1 << 12,
   tim8   = 1 << 13,
   usart1 = 1 << 14,
   adc3   = 1 << 15,
   tim9   = 1 << 19,
   tim10  = 1 << 20,
   tim11  = 1 << 21
  }f10xxx_rcc_apb2_e;

typedef enum f10xxx_rcc_apb1
  {
   tim2 = 1,
   tim3 = 1 << 1,
   tim4 = 1 << 2,
   tim5 = 1 << 3,
   tim6 = 1 << 4,
   tim7 = 1 << 5,
   tim12 = 1 << 6,
   tim13 = 1 << 7,
   tim14 = 1 << 8,
   wwdg = 1 << 11,
   spi2 = 1 << 14,
   spi3 = 1 << 15,
   usart2 = 1 << 17,
   usart3 = 1 << 18,
   usart4 = 1 << 19,
   usart5 = 1 << 20,
   i2c1 = 1 << 21,
   i2c2 = 1 << 22,
   usb = 1 << 23,
   can = 1 << 25,
   bkp = 1 << 27,
   pwr = 1 << 28,
   dac = 1 << 29
  }f10xxx_rcc_apb1_e;

typedef enum f10xxx_rcc_ahb
  {
   dma1 = 1,
   dma2 = 1 << 1,
   sram = 1 << 2,
   flitf = 1 << 4,
   crc = 1 << 6,
   fsmc = 1 << 8,
   sdio = 1 << 10
  }f10xxx_rcc_ahb_e;

extern volatile f10xxx_rcc_t *rcc;

void f10xxx_rcc_reset_apb2(f10xxx_rcc_apb2_e peripheral);
void f10xxx_rcc_reset_apb1(f10xxx_rcc_apb1_e peripheral);

void f10xxx_rcc_enable_clock_apb2(f10xxx_rcc_apb2_e peripheral);
void f10xxx_rcc_enable_clock_apb1(f10xxx_rcc_apb1_e peripheral);
void f10xxx_rcc_enable_clock_ahb(f10xxx_rcc_ahb_e peripheral);

void f10xxx_rcc_hse_on();
u32 f10xxx_rcc_hse_ready();

void f10xxx_rcc_select_hse();
u32 f10xxx_rcc_hse_selected();

#endif
