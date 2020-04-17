

#include "inc/stdinclude.h"
#include "inc/f10xxx_rcc.h"

volatile f10xxx_rcc_t *rcc = (f10xxx_rcc_t *) 0x40021000;

void f10xxx_rcc_reset_apb2(f10xxx_rcc_apb2_e peripheral){

  rcc->APB2RSTR = (u32) peripheral;
  rcc->APB2RSTR = 0;
}

void f10xxx_rcc_reset_apb1(f10xxx_rcc_apb1_e peripheral){

  rcc->APB1RSTR = (u32) peripheral;
  rcc->APB1RSTR = 0;
}

void f10xxx_rcc_enable_clock_apb2(f10xxx_rcc_apb2_e peripheral){

  rcc->APB2ENR = rcc->APB2ENR | (u32) peripheral;
}

void f10xxx_rcc_enable_clock_apb1(f10xxx_rcc_apb1_e peripheral){

  rcc->APB1ENR = rcc->APB1ENR | (u32) peripheral;
}

void f10xxx_rcc_enable_clock_ahb(f10xxx_rcc_ahb_e peripheral){

  rcc->AHBENR = rcc->AHBENR | (u32) peripheral;
}

void f10xxx_rcc_hse_on(){

  rcc->CR = rcc->CR | ((u32) 1 << 16);// | (1 << 18)
}

u32 f10xxx_rcc_hse_ready(){

  return (rcc->CR >> 17) & 1;
}

void f10xxx_rcc_select_hse(){

  rcc->CFGR = rcc->CFGR & ~((u32) 3);
  rcc->CFGR = rcc->CFGR | 1;
}

u32 f10xxx_rcc_hse_selected(){

  return (rcc->CFGR >> 2) & 1;
}
