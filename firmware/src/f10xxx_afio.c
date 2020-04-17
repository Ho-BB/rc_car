

#include "inc/f10xxx_afio.h"

#define AFIO_MAPR ((u32 *) (0x40010000 + 0x04))

void f10xxx_afio_disable_jtag(){

  *AFIO_MAPR = *AFIO_MAPR | (2 << 24);
}
