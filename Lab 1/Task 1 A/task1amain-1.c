/*
* Garrett Tashiro
* SID: 2075045
* June 24, 2021
* Main file for CSE 474 Lab 1 task 1 a
* This programs uses four LED's on the Tiva. The LED's
* will light up from left to right and stay lit as they
* light up in a sequence. Once they are all lit up, the 
* turn off in the same sequence that they lit up in.
*/

#include <stdint.h>
#include "lab1.h"

int main(void)
{
  //Port N -> 0x1000 (LED1 and LED2)
  //Port F -> 0x20 (LED3 and LED4)
  //Port J -> 0x100 (switches)
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020; // Enable Port F and Port N GPIO.
  
  delay++;// Delay 2 more cycles before access Timer registers
  delay++;// Refer to Page. 756 of Datasheet for info
  
  GPIODIR_F = 0x11;// Set PF0 to output. Pin 0 and Pin 4. Both LED's associated with Port F.
  GPIODEN_F = 0x11;// Set PF0 and PF4 to digital port.
  
  GPIODIR_N = 0x3; //For PN0 and PN1
  GPIODEN_N = 0x3;

  while(1){
     
     for(int i = 0; i < 1000000; i++) {} //Cause a delay between LED's turning on
     GPIODATA_N = 0x2; //Turn on LED connected to PN1
       
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_N = 0x3; //Turn on LED connected to PN0 and keeping PN1 on
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_F = 0x10; //Turn on LED connected to PF0
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_F = 0x11; //Turn of LED connected to PF4 and PF0
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_N = 0x1;  //Turn off the LED connected to PN1
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_N = 0x0;  //Turn off the LED connected to PN0
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_F = 0x1; //Turn off the LED connected to PF4
     
     for(int i = 0; i < 1000000; i++) {}
     GPIODATA_F = 0x00; //Turn off the LED connected to PF0
 
  } //End of while loop
  
  return 0;
}