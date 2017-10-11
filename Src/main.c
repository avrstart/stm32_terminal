#include "main.h"
#include "stm32f1xx_hal.h"
#include "init_periph.h"
#include "exec_mcu.h"

uint16_t str_size;
char rcv_buf[30];

int main(void)
{

  init_all();
  printf("\r\n/************************/ \r\nTest terminal v0.1\r\n>");
    
  while (1)
  {
      /* get newline terminated string from stdin */
      if(fgets(rcv_buf, sizeof(rcv_buf), stdin) != 0)  {
          
          /* get string size*/
          str_size = strlen(rcv_buf);

          if(str_size > 0) {
              /* replace newline symbol with null */
              if (rcv_buf[str_size - 1] == '\n'){
                  rcv_buf[str_size - 1] = 0;                        
              }

              /* get args from command */
              char *args[10];
              char *ptr = strtok (rcv_buf," ,.");
                  
              int i = 0;
              
              /* terminate strings with delimiters */
              while (ptr != NULL)  {
                  args[i++] = ptr;
                  ptr = strtok (NULL, " ,.");
              }
              
              /* try execute command */
              if(rcv_buf[0] != 0) {
                  if(execlp(i, args) == EXIT_FAILURE) {
                      printf("\n\rcommand not found: %s\n\r", rcv_buf);
                  }
                  else {
                      printf("\n\rOK\n\r");
                  }
              }
              else {
                  printf("\n\r");
              }
          }
                
          /* wait next command */          
          printf(">");
      }
  }
}
