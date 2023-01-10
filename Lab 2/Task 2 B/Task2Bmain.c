/*
* Garrett Tashiro
* SID: 2075045
* July 13, 2021
* Main file for CSE lab 2 Task 2 B. Using a timer interrupts
* associated to SW1 and SW2. LED1 will blink at a 1 Hz rate.
* SW1 stops the countdown for for Timer 0 and LED2 will turn on.
* Press Sw2 and the timer counts down again while LED1 blinks
* and LED2 turns off. 
*/

#include <stdint.h>
#include "Task2B.h"

//global variable to turn on/off LED1
int count = 0;

//This function initializes LED Ports/Pins
//PN0 and PN1. 
void LED_init(void){
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x1000; // Enable Port N GPIO.
  
  delay++;// Delay 2 more cycles before access Timer registers
  delay++;// Refer to Page. 756 of Datasheet for info
 
  GPIODIR_N |= 0x3; // Set PN0 and PN1 to output.
  GPIODEN_N |= 0x3; // Set PN0 and PN1 to digital port.
}

//Initialization for the two onboard switches
//for Port J. 
void SW_init(void){
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x100; // Enable Port J and Port N GPIO.
  
  delay++;// Delay 2 more cycles before access Timer registers
  delay++;// Refer to Page. 756 of Datasheet for info
  
  GPIODEN_J |= 0x3;  //Set digital ports for PJ1 and PJ0
  GPIODATA_J = 0x0; //Set PJ1 and PJ0 data to 0
  GPIOPUR_J |= 0x3; //Pull-Up Select for PJ1 and PJ0
  
  GPIOIS_J &= ~0x3; //Make bit 1 edge sensitive
  GPIOIBE_J &= ~0x3; //Trigger for  single edge
  GPIOIEV_J &= ~0x3; //Make the trigger on a falling edge
  GPIOIM_J |= 0x3; //Unmask the interrupt
  NVIC_EN1 |= 0x80000; //Enable interrupt for GPTM Port J #51
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

//Clear the flag for Port J
void flagJ(void){
  
  GPIOICR_J |= 0x3;
}
//This function is for increasing the count
//and reseting the timer
void timer_reset(void){
  
  count++;             //Increase count to do all transitions
  GPTMICR_T0 |= 0x1;  //Reset the flag
}

//Stops the timer if SW2 is pressed
//Resets the flag
void stopTime(void){
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMICR_T0 |= 0x1;  //Reset the flag
}

//Start the timer again
//Resets the flag
void startTime(void){
  
  GPTMCTL_T0 |= 0x1; //Enable the timer
  GPTMICR_T0 |= 0x1;  //Reset the flag
}

//This function will return a 1 if SW1 is pressed
//and a 0 if it is not pressed
unsigned long SW1(void){
  
  return (GPIODATA_J & 0x1);// 0x1 (pressed) or 0 (not pressed)
}

//This function will return a 1 if SW2 is pressed
//and a 0 if it is not pressed
unsigned long SW2(void){
  
  return ((GPIODATA_J & 0x2) == 0x2);// 0x1 (pressed) or 0 (not pressed)
}

//Turn on LED labeled D0 on Tiva
//Increase count by 1
//Reset the flag for Timer 0
void LED1_On(void){
  
  GPIODATA_N |= 0x2;
  timer_reset();
}
//Turn off LED labeled D0 on Tiva
//Count to 0 and reset Timer 0 flag
void LED1_Off(void){
  
  GPIODATA_N &= ~0x2;
  timer_reset();
  count = 0;
}

//Turn on LED labeled D1 on Tiva
void LED2_On(void){
  
  GPIODATA_N |= 0x1;
}

//Turn off LED labeled D1 on Tiva
void LED2_Off(void){
  
  GPIODATA_N &= ~0x1;
}

//Function for the SW_Handler() when SW1 is pressed.
//The Timer wil stop for LED1 and LED2 will turn on.
void switch1(void){
  
  stopTime();
  LED2_On();
}

//Function for the SW_Handler() when SW2 is pressed.
//The timer will start and LED2 will turn off.
void switch2(void){
  
  startTime();
  LED2_Off();
}
//Calls functions to turn LED1 on/off
//depending on what count is
void light(void){
  
  if(count == 0){
    LED1_On();
  }
  else if(count == 1){
    LED1_Off();
  }
}

//Timer0A handler to call the lights function and 
void Timer0A_Handler(void){
  
  light();
}

//The switch handler clears the flag for port J.
//Then it tests to see which switch was pressed
//and calls functions for either switch press.
void SW_Handler(void){
  
  flagJ();
  
  if(SW1() == 0x1){
    switch1();
  }
  else if(SW2() == 0x1){
    switch2();
  }
}

//This calls the functions that initialize
//the LED's, switches, and timer.
void Registers(void){
  
  LED_init();
  SW_init();
  Timer_init();
}

//Function calls the initialization for LED's and the timer
void run(void){
  
  Registers();
  while(1){}
}

//The main calls run() with does all the initialization
//and has the infinite while loop in it.
int main(){
  
  run();
  return 0;
}