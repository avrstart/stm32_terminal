#include "usart_hw.h"
#include "stm32f1xx_hal.h"
#include <ctype.h>

extern UART_HandleTypeDef huart2;

int SendChar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, sizeof(ch), 1000);    
    return 0;
}

int GetKey (void)  {

    uint8_t ch;
    
    while(HAL_UART_Receive(&huart2, &ch, 1, 2000) != HAL_OK);   
    if(isprint(ch) || (ch == '\r') || (ch == '\n')) {
        SendChar(ch); 
        if(ch == '\r') {
            ch = '\n';
        }
    }
    return ch;
}
