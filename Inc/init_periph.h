#ifndef __INIT_PERIPH_H
#define __INIT_PERIPH_H

void init_all(void);
    
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);

#endif
