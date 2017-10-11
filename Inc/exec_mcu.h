#ifndef __EXEC_MCU_H
#define __EXEC_MCU_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <ctype.h>

int execlp(int argc, char *argv[]);

uint32_t GPIO_GetPinMode(GPIO_TypeDef *gpio_tport, uint16_t pin);

#endif
