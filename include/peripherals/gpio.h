#ifndef __P_GPIO_H
#define __P_GPIO_H

#include "peripherals/base.h"

// Address mappings for GPIO registers
#define GPFSEL1   (PBASE + 0x00200004)
#define GPSET0    (PBASE + 0x0020001C)
#define GPCLR0    (PBASE + 0x00200028)
#define GPPUD     (PBASE + 0x00200094)
#define GPPUDCLK0 (PBASE + 0x00200098)

#endif /* __P_GPIO_H */
