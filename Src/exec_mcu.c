#include "exec_mcu.h"

#define GPIOA_PINMASK   (0x1F03)
#define GPIOB_PINMASK   (0x8400)
#define GPIOC_PINMASK   (0xEFCF)
#define GPIOD_PINMASK   (0xFF03)
#define GPIOE_PINMASK   (0x707C)

int Char_ToGPIOx(GPIO_TypeDef **gpio_tport, char *port_str)
{
    uint8_t port;
    
    if(strlen(port_str) == 1) {
        port = port_str[0];
    }
    else {
        return EXIT_FAILURE;
    }

    if(!isalpha(port)) {
        return EXIT_FAILURE;
    }
                
    if(islower(port)) {
        port = toupper(port);
    }
                
    switch(port) {
        case 'A': *gpio_tport = GPIOA;   break;
        case 'B': *gpio_tport = GPIOB;   break;
        
        #ifdef GPIOC
        case 'C': *gpio_tport = GPIOC;   break;
        #endif
        
        #ifdef GPIOD
        case 'D': *gpio_tport = GPIOD;   break;
        #endif
        
        #ifdef GPIOE
        case 'E': *gpio_tport = GPIOE;   break;
        #endif
        
        #ifdef GPIOF
        case 'F': *gpio_tport = GPIOF;   break;
        #endif
        
        #ifdef GPIOG
        case 'G': *gpio_tport = GPIOG;   break;
        #endif
        
        #ifdef GPIOH
        case 'H': *gpio_tport = GPIOH;   break;
        #endif
        
        default: return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;   
}

int Str_ToPin(char *gpio_str, uint16_t *gpio_hwpin) {
    
    uint16_t gpio_pin;
    uint16_t gpio_strlen;
    
    gpio_strlen = strlen(gpio_str);
    
    for(int i = 0; i < gpio_strlen; i++) {
        gpio_pin = gpio_str[i];
        
        if(!isdigit(gpio_pin)) {
            return EXIT_FAILURE;
        }
    }
    gpio_pin = 0;
    
    switch(gpio_strlen) {
        case 1: 
            gpio_pin |= (1 << (gpio_str[0] - 0x30));
            break;
        case 2:
            gpio_pin |=  (1 << ((gpio_str[0] - 0x30) *10 + (gpio_str[1] - 0x30)));
            break;
        default:
            return EXIT_FAILURE;
    }
    *gpio_hwpin = gpio_pin;
    return EXIT_SUCCESS;       
}


#ifdef STM32L4
uint32_t GPIO_GetPinMode(GPIO_TypeDef *gpio_tport, uint16_t pin) {
    return (uint32_t)(READ_BIT(gpio_tport->MODER, (GPIO_MODER_MODE0 << (POSITION_VAL(pin) * 2U))) >> (POSITION_VAL(pin) * 2U));  
}
#endif

//#ifdef STM32F1
uint32_t GPIO_GetPinMode(GPIO_TypeDef *gpio_tport, uint16_t pin) {
    
    uint32_t ioposition = POSITION_VAL(pin);
    uint32_t gpio_mode;
    
    uint32_t configregister = (pin < GPIO_PIN_8) ? gpio_tport->CRL : gpio_tport->CRH;  
    gpio_mode = (pin < GPIO_PIN_8) ?  (configregister >> (ioposition * 4U)) : (configregister >> ((ioposition - 8) * 4U));
    gpio_mode &= 0x03;
    
    return gpio_mode;
}
//#endif

int execlp(int argc, char *argv[]){
    
    GPIO_TypeDef *gpio_port;
    uint16_t gpio_pin;
    
    if (!strcmp (argv[0], "gpio_set")) {
        if(argc == 3) {
            if((Char_ToGPIOx(&gpio_port, argv[1]) == EXIT_SUCCESS) && (Str_ToPin(argv[2], &gpio_pin) == EXIT_SUCCESS)) {
                HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
                return EXIT_SUCCESS;
            }                                                          
        }
    }

    else if(!strcmp (argv[0], "gpio_reset")) {
       if(argc == 3) {
            if((Char_ToGPIOx(&gpio_port, argv[1]) == EXIT_SUCCESS) && (Str_ToPin(argv[2], &gpio_pin) == EXIT_SUCCESS)) {
                HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
                return EXIT_SUCCESS;
            }                                                          
       }
    }
    else if(!strcmp (argv[0], "gpio_read")) {
       if(argc == 3) {
           if((Char_ToGPIOx(&gpio_port, argv[1]) == EXIT_SUCCESS) && (Str_ToPin(argv[2], &gpio_pin) == EXIT_SUCCESS)) {
                printf("\n\rpin = %d", HAL_GPIO_ReadPin(gpio_port, gpio_pin));
                return EXIT_SUCCESS;
           }  
       }
    }
    else if(!strcmp (argv[0], "gpio_mode")) {
       if(argc == 3) {
           if((Char_ToGPIOx(&gpio_port, argv[1]) == EXIT_SUCCESS) && (Str_ToPin(argv[2], &gpio_pin) == EXIT_SUCCESS)) {
                uint32_t pin_mode = GPIO_GetPinMode(gpio_port, gpio_pin);
                printf("\n\rmode = ");
                if(pin_mode  == GPIO_MODE_INPUT) {
                    printf("in");
                }
                else {
                    printf("out");
                }
                
                return EXIT_SUCCESS;
           }  
       }
    }
    else if(!strcmp (argv[0], "port_read")) {
       if(argc == 2) {
           if(Char_ToGPIOx(&gpio_port, argv[1]) == EXIT_SUCCESS)  {
                printf("\n\rIDR=0x%x", gpio_port->IDR);
                return EXIT_SUCCESS;
           }  
       }
    }
    
    else if(!strcmp (argv[0], "gpio_readAll")) {
       if(argc == 1) {
           printf("\n\rA=0x%08x ", GPIOA->IDR);
           printf("B=0x%08x ", GPIOA->IDR);
           #ifdef GPIOC 
               printf("C=0x%08x ", GPIOC->IDR); 
           #endif
           #ifdef GPIOD 
               printf("D=0x%08x ", GPIOD->IDR); 
           #endif
           #ifdef GPIOE 
               printf("E=0x%08x ", GPIOE->IDR); 
           #endif 
           #ifdef GPIOF 
               printf("F=0x%08x ", GPIOF->IDR); 
           #endif
           #ifdef GPIOG 
               printf("G=0x%08x ", GPIOG->IDR); 
           #endif
           #ifdef GPIOH 
               printf("H=0x%08x ", GPIOH->IDR); 
           #endif

           return EXIT_SUCCESS;
       }
       else if(argc == 2) {
           if(!strcmp (argv[1], "\n\rmasked")) {
               printf("A=0x%08x,B=0x%08x,C=0x%08x,D=0x%08x,E=0x%08x", 
                       (GPIOA->IDR & GPIOA_PINMASK), (GPIOB->IDR & GPIOB_PINMASK), 
                       (GPIOC->IDR & GPIOC_PINMASK), (GPIOD->IDR & GPIOD_PINMASK), (GPIOE->IDR & GPIOE_PINMASK));
               return EXIT_SUCCESS;
           }
       }
    } 
    
     else if(!strcmp (argv[0], "port_write")) {
       if(argc == 3) {
           char *port_str = argv[2];
           
           if(Char_ToGPIOx(&gpio_port, argv[1]) != EXIT_SUCCESS)  {
               return EXIT_FAILURE;
           } 
  
           if(strlen(port_str) == 6) {
               if(!memcmp(port_str, "0x", 2)) {
                   for(int i = 2; i < 6; i++) {
                       if(!isxdigit(port_str[i])) {
                           return EXIT_FAILURE;
                       }
                   }
                   
                   long int port_val = strtol(port_str, NULL, 0);                  
                   gpio_port->ODR = (port_val & 0x0fff);
                   
                   return EXIT_SUCCESS;
               }
           } 
       }
    }
   
    return EXIT_FAILURE;
}



