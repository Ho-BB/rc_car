

#include "inc/stm32f10xxx.h"

//TX = PA9 | RX = PA10

extern u8 __start_ram[];

void f10xxx_usart1_isr();
void f10xxx_dma1_channel4_isr();//TX
void f10xxx_dma1_channel5_isr();//RX

__attribute__((noreturn))
void f10xxx_reset_isr(){

  *((u32 *) (__start_ram + 4)) = 0;
  
  f10xxx_rcc_hse_on();
  while(!f10xxx_rcc_hse_ready());

  f10xxx_rcc_select_hse();
  while(!f10xxx_rcc_hse_selected());

  f10xxx_rcc_enable_clock_apb2(iopa);
  f10xxx_rcc_enable_clock_apb2(afio);
  f10xxx_rcc_enable_clock_apb2(usart1);
  f10xxx_rcc_enable_clock_ahb(dma1);

  f10xxx_rcc_reset_apb2(usart1);

  cortex_m3_enable_all_interrupts();

  f10xxx_nvic_enable_irq(F10XXX_IRQ_USART1);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_DMA1_CHANNEL4);
  f10xxx_nvic_enable_irq(F10XXX_IRQ_DMA1_CHANNEL5);

  f10xxx_usart_enable_irq(usart_1, rx_not_empty_irq);

  f10xxx_gpio_configure_port(gpio_a,
			     port_9,
			     output_50mhz,
			     af_output_push_pull);

  f10xxx_gpio_configure_port(gpio_a,
			     port_10,
			     input,
			     input_floating);

  f10xxx_usart_start(usart_1,
		     B9600,
		     data8_parity0,
		     even,
		     stop1);

  while(1);
}

void f10xxx_usart1_isr(){

  __start_ram[*((u32 *) (__start_ram + 4))] = f10xxx_usart_read(usart_1);

  if(*((u32 *) (__start_ram + 4)) == 1){
    *((u32 *) (__start_ram + 4)) = 0;

    f10xxx_usart_disable_irq(usart_1, rx_not_empty_irq);
    f10xxx_usart_enable_dma(usart_1, reception);
    
    f10xxx_dma_set_memory_address(dma_channel_5, (u32) (__start_ram + 8));
    f10xxx_dma_set_peripheral_address(dma_channel_5, (u32) &usart_1->DR);
    f10xxx_dma_set_number_of_data(dma_channel_5, *((u16 *) __start_ram));
    f10xxx_dma_enable_irq(dma_channel_5, transfer_complete_irq);
    f10xxx_dma_enable(dma_channel_5,
		      peripheral_to_memory,
		      circular_mode_disabled,
		      peripheral_increment_mode_disabled,
		      memory_increment_mode_enabled,
		      data_size_8,
		      data_size_8,
		      channel_priority_very_high,
		      memory_mode_disabled);
  }
  else{
    *((u32 *) (__start_ram + 4)) += 1;
  }
}

void f10xxx_dma1_channel4_isr(){//TX

  
}
void f10xxx_dma1_channel5_isr(){//RX

  f10xxx_dma_disable(dma_channel_5);
  
  u16 len = *((u16 *) __start_ram);
  u16 i;
  for(i = 0; i < len; i++){
    f10xxx_usart_write(usart_1, __start_ram[8 + i]);
    while(!f10xxx_usart_tx_empty(usart_1));
  }

  f10xxx_usart_disable_dma(usart_1, reception);
  f10xxx_usart_enable_irq(usart_1, rx_not_empty_irq);

  f10xxx_dma_irq_acknowledge(channel_5, transfer_complete_irq);
  cortex_m3_dsb();
}
