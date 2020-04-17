

	.code 16

	.text

	.global cortex_m3_enable_all_interrupts
	.global cortex_m3_disable_all_interrupts
	.global cortex_m3_dsb

	@CPSIE f is probably better
cortex_m3_enable_all_interrupts:
	mov r0, #0
	msr PRIMASK, r0
	msr FAULTMASK, r0
	msr BASEPRI, r0
	
	bx lr


cortex_m3_disable_all_interrupts:
	cpsid f

	bx lr


cortex_m3_dsb:
	dsb

	bx lr
