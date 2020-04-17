

MEMORY
{
	FLASH : ORIGIN = 0x08000000, LENGTH = 64k
	RAM   : ORIGIN = 0x20000000, LENGTH = 20k
}

/* Necessary so that LD doesn't discard the IVT */
EXTERN(f10xxx_ivt f10xxx_default_reset_isr f10xxx_default_isr);

__start_ram = ORIGIN(RAM);
__end_ram = ORIGIN(RAM) + LENGTH(RAM);

/* IVT symbols */
/* ARM Exceptions */
__main_stack_pointer = ORIGIN(RAM) + 2k;
PROVIDE(f10xxx_reset_isr = f10xxx_default_reset_isr + 1);
PROVIDE(f10xxx_nmi_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_hardfault_isr = f10xxx_default_isr + 1);

PROVIDE(f10xxx_memmanage_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_busfault_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_usagefault_isr = f10xxx_default_isr + 1);
/* Reserved */

/* Reserved */
/* Reserved */
/* Reserved */
PROVIDE(f10xxx_svcall_isr = f10xxx_default_isr + 1);

PROVIDE(f10xxx_debugmonitor_isr = f10xxx_default_isr + 1);
/* Reserved */
PROVIDE(f10xxx_pendsv_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_systick_isr = f10xxx_default_isr + 1);

/* External Interrupts */
PROVIDE(f10xxx_wwdg_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel1_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel2_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel3_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel4_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel5_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel6_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_dma1_channel7_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_adc1_2_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_tim2_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_tim3_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_tim4_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_spi1_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_spi2_isr = f10xxx_default_isr + 1);
PROVIDE(f10xxx_usart1_isr = f10xxx_default_isr + 1);


SECTIONS
{
	.text :
	{
		__f10xxx_ivt_start = .;
		KEEP( *(.f10xxx_ivt) )
		__f10xxx_ivt_end = .;
		KEEP( *(.text) )
	} > FLASH
}

