/*
* 	I build for country life.
* 	Author : Pham Dinh Van
*	----------------%-------------------
*	
*/

#include <msp430.h>
#include "../lib/tempt/sens-temperature.h"
#include "../lib/uart/uart.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  UART_Init(); 

  float tmp = 0;

  while(1){
    UART_Write_String("Nhiet do phong = ");
    tmp = ReadTempt();
    UART_Write_Float(tmp,5);
    UART_Write_Char('\n');
    __delay_cycles(1000000);
  }
  
}
