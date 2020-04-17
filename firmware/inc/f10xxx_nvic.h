
#ifndef __F10XXX_NVIC__
#define __F10XXX_NVIC__

#include "inc/stdinclude.h"


//__attribute__((packed))
typedef struct f10xxx_nvic{
  u32 ICTR;              // Interrupt Controller Type Register
  u32 not_used_1[2];
  u32 SCSR;              // SysTick Control and Status Register
  u32 SRVR;              // SysTick Reload Value Register
  u32 SCuVR;             // SysTick Current Value Register
  u32 SCaVR;             // SysTick Calibration Value Register
  u32 not_used_2[56];
  u32 ISER[8];           // Irq Set Enable Registers
  u32 not_used_3[24];
  u32 ICER[8];           // Irq Clear Enable Registers
  u32 not_used_4[24];
  u32 ISPR[8];           // Irq Set Pending Registers
  u32 not_used_5[24];
  u32 ICPR[8];           // Irq Clear Pending Registers
  u32 not_used_6[24];
  u32 IABR[8];           // Irq Active Bit Registers
  u32 not_used_7[56];
  u32 IPR[8];            // Irq Priority Registers
  u32 not_used_8[8 * 64 + 56];
  u32 CPUIDBR;           // CPUID Base Register
  u32 ICSR;              // Interrupt Control State Register
  u32 VTOR;              // Vector Table Offset Register
  u32 AIRCR;             // Application Interrupt/Reset Control Register
  u32 SCR;               // System Control Register
  u32 CCR;               // Configuration Control Register
  u32 SHPR[3];           // System Handlers Priority Registers
  u32 SHCSR;             // System Handler Control and State Register
  u32 CFSR;              // Configurable Fault Status Register
  u32 HFSR;              // Hard Fault Status Register
  u32 DFSR;              // Debuf Fault Status Register
  u32 MMAR;              // Mem Manage Address Register
  u32 BFAR;              // Bus Fault Address Register
  u32 AFSR;              // Auxiliary Fault Status Register
  u32 PFR[2];            // Processor Feature Registers
  u32 DFR0;              // Debug Feature Register 0
  u32 AFR0;              // Auxiliary Feature Register 0
  u32 MMFR[4];           // Memory Model Feature Registers
  u32 ISAR[5];           // ISA Feature Registers
  u32 not_used_9[35];
  u32 STIR;              // Software Trigger Interrupt Register
  // ... (missing some for now)
} f10xxx_nvic_t;

extern volatile f10xxx_nvic_t *nvic;

#define F10XXX_IRQ_DMA1_CHANNEL1        11
#define F10XXX_IRQ_DMA1_CHANNEL2        12
#define F10XXX_IRQ_DMA1_CHANNEL3        13
#define F10XXX_IRQ_DMA1_CHANNEL4        14
#define F10XXX_IRQ_DMA1_CHANNEL5        15
#define F10XXX_IRQ_DMA1_CHANNEL6        16
#define F10XXX_IRQ_DMA1_CHANNEL7        17
#define F10XXX_IRQ_ADC1_2               18
#define F10XXX_IRQ_TIMER2               28
#define F10XXX_IRQ_TIMER3               29
#define F10XXX_IRQ_TIMER4               30
#define F10XXX_IRQ_USART1               37


void f10xxx_nvic_enable_irq(u8 irq);

void f10xxx_nvic_disable_irq(u8 irq);

void f10xxx_nvic_trigger_irq(u8 irq);

#endif
