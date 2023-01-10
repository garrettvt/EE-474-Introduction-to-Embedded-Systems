/*
* Garrett Tashiro
* SID: 2075045
* June 25, 2021
* Main file for CSE 474 Lab 1 task 1 b
* This program uses two switches and LED's on the Tiva.
* Pressings switch 1 will light up the first LED. Switch
* 2 will light up the second LED. Both switches pressed 
* will light up both LED's at the same time.  
*/
#include <stdint.h>
#include "lab1.h"

int main(void)
{
  //Port N -> 0x1000 (LED1 and LED2)
  //Port J -> 0x100 (switches)
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1100; // Enable Port J and Port N GPIO. J is for switches.
  
  delay++;// Delay 2 more cycles before access Timer registers
  delay++;// Refer to Page. 756 of Datasheet for info
   
  GPIODIR_N = 0x3; //Set the output for PN1 and PN0
  
  GPIODEN_J = 0x3;  //Set digital ports for PJ1 and PJ0
  GPIODEN_N = 0x3;  //Set digital ports for PN1 and PN0
  
  GPIODATA_N = 0x0; //Have the data set to 0 so the LED's are off
  GPIODATA_J = 0x0; //Set PJ1 and PJ0 data to 1
  
  GPIOPUR_J |= 0x3; //Pull-Up Select for PJ1 and PJ0
  
  while(1){
     
    //If both switches are pressed output is 0 then LED's will turn on.
    if((GPIODATA_J & 0x3) == 0){
       
       GPIODATA_N = 0x3;
    }
    //If switch 1 is pressed, light up LED 1.
    //Numbering is swapped. Check diagram in lab doc.
    else if((GPIODATA_J & 0x1) == 0){
       
       GPIODATA_N = 0x2;
    }
    //If switch 2 is pressed, light up LED 2.
    //Numbering is swapped. Check diagram in lab doc.
    else if((GPIODATA_J & 0x2) == 0){
       
       GPIODATA_N = 0x1;
    }
    //If no switches are pressed then all LED's are off.
    else{
       
       GPIODATA_N = 0x0;
    }
  } 
  
  return 0;
}