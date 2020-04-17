

	@The linker script provides default adresses for isr symbols the library
	@user didn't define
	
	.code 16
	
	.section .f10xxx_ivt

	.global f10xxx_ivt
	.global f10xxx_default_reset_isr
	.global f10xxx_default_isr

	@ARM Exceptions

f10xxx_ivt:	
	.word __main_stack_pointer          @Main stack pointer loaded at reset
	.word f10xxx_reset_isr              @Reset
	.word f10xxx_nmi_isr                @Non Maskable Interrupt
	.word f10xxx_hardfault_isr          @Hard Fault
	
	.word f10xxx_memmanage_isr          @Mem Manage
	.word f10xxx_busfault_isr           @Bus Fault
	.word f10xxx_usagefault_isr         @Usage Fault
	.word 0                             @Reserved (see ARM)

	.word 0                             @Reserved (see ARM)
	.word 0                             @Reserved (see ARM)
	.word 0                             @Reserved (see ARM)
	.word f10xxx_svcall_isr             @SVCall

	.word f10xxx_debugmonitor_isr       @Debug Monitor
	.word 0                             @Reserved (see ARM)
	.word f10xxx_pendsv_isr             @PendSV
	.word f10xxx_systick_isr            @SysTick

	@External Interrupts (Hardware specific)

	.word f10xxx_wwdg_isr               @0
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_dma1_channel1_isr      @11

	.word f10xxx_dma1_channel2_isr      @12
	.word f10xxx_dma1_channel3_isr      @13
	.word f10xxx_dma1_channel4_isr      @14
	.word f10xxx_dma1_channel5_isr      @15

	.word f10xxx_dma1_channel6_isr      @16
	.word f10xxx_dma1_channel7_isr      @17
	.word f10xxx_adc1_2_isr             @18
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_tim2_isr               @28
	.word f10xxx_tim3_isr               @29
	.word f10xxx_tim4_isr               @30
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_spi1_isr               @35

	.word f10xxx_spi2_isr               @36
	.word f10xxx_usart1_isr             @37
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG
	.word f10xxx_wwdg_isr               @Window WatchDoG

	@End of External Interrupts

	.text
	
	@Default Reset ISR

f10xxx_default_reset_isr:	
	b f10xxx_default_reset_isr

	
	@Default ISR for all other Exceptions / Interrupts

f10xxx_default_isr:
	bx lr
