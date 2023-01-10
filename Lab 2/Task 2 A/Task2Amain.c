/*
* Garrett Tashiro
* SID: 2075045
* July 11, 2021
* Main file for CSE 474 Lab 2 Task 2 A: Light Sequence with Timer
* and an interrupt. The LED's will light up when the timer hits 0
* and the interrupt happens. The 
*/

#include <stdint.h>
#include "LightSequence.h"

//Global variable for counting to know what LED to turn on and off
int count = 0;


//This function initializes LED Ports and Pins
//PF0, PF4, PN0, PN1
void LED_init(void){
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1020; // Enable Port F and Port N GPIO.
  
  delay++;// Delay 2 more cycles before access Timer registers
  delay++;// Refer to Page. 756 of Datasheet for info
  
  GPIODIR_F |= 0x11;// Set PF0 and PF4 to output.
  GPIODIR_N |= 0x3; // Set PN0 and PN1 to output.
  
  GPIODEN_F |= 0x11;// Set PF0 and PF4 to digital port.
  GPIODEN_N |= 0x3; // Set PN0 and PN1 to digital port.
}

//Initialize timer 0 and have it be periodic
//and enables the NVIC and GPTMIMR for the interrupt
void Timer_init(void){
  
  RCGCTIMER |= 0x1; //Enables timer 0
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMCFG_T0 &= ~0xFFFFFFFF; //setting pin 0 to 0 for 32-bit timer
  GPTMTAMR_T0 |= 0x2; //bit 1 to 1 for periodic
  GPTMTAMR_T0 &= ~0x10; //bit 4 to 0 for counting down
  GPTMTAILR_T0 = 16000000;
  NVIC_EN0 |= 0x80000; //Enable interrupt for GPTM Timer0A #19
  GPTMIMR_T0 |= 0x1; //Enable time-out interrup mask
  GPTMCTL_T0 |= 0x1; //Enable the timer
}

//This function is for increasing the count
//and reseting the timer
void timer_reset(void){
  
  count++;             //Increase count to do all transitions
  GPTMICR_T0 |= 0x1;  //Reset the flag
}

//Turn on LED labeled D0 on Tiva
//Increase count by 1
//Reset the flag
void LED1_On(void){
  
  GPIODATA_N |= 0x2;
  timer_reset();
}
//Turn off LED labeled D0 on Tiva
void LED1_Off(void){
  
  GPIODATA_N &= ~0x2;
  timer_reset();
}

//Turn on LED labeled D1 on Tiva
void LED2_On(void){
  
  GPIODATA_N |= 0x1;
  timer_reset();
}
//Turn off LED labeled D1 on Tiva
void LED2_Off(void){
  
  GPIODATA_N &= ~0x1;
  timer_reset();
}

//Turn on LED labeled D2 on Tiva
void LED3_On(void){
  
  GPIODATA_F |= 0x10;
  timer_reset();
}
//Turn off LED labeled D2 on Tiva
void LED3_Off(void){
  
  GPIODATA_F &= ~0x10;
  timer_reset();
}

//Turn on LED labeled D3 on Tiva
void LED4_On(void){
  
  GPIODATA_F |= 0x1;
  timer_reset();
}

//Turn off LED labeled D3 on Tiva
void LED4_Off(void){
  
  GPIODATA_F &= ~0x1;
  timer_reset();
  count = 0;
}

//Function calls the initialization for LED's and the timer
void Registers(void){
  
  LED_init();
  Timer_init();
}

//lights() tests to see if global variable count is at a certain
//number then turns on/off the LED's that related certain numbers
//of count. 
void lights(void){
  
  if(count == 0){
    LED1_On();
  }
  else if(count == 1){   
    LED2_On();
  }
  else if(count == 2){   
    LED3_On();
  }
  else if(count == 3){   
    LED4_On();
  }
  else if(count == 4){   
    LED1_Off();
  }
  else if(count == 5){   
    LED2_Off();
  }
  else if(count == 6){   
    LED3_Off();
  }
  else if(count == 7){   
    LED4_Off();
  }
}

//Timer0A handler to call the lights function and 
void Timer0A_Handler(void){
  
  lights();
}

//run() calls registers which does initialization for LED's and timer
//Empty inifinte loop. The interrupt and handlers will run code
//to have the LED's turn on/off at given times
void run(void){
  
  Registers();

  while(1){}
}

//The main calls run() 
int main(){

  run();
  return 0;
}