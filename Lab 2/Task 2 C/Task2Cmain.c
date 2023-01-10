/*
* Garrett Tashiro
* SID: 2075045
* July 14, 2021
* Main file for CSE 474 Lab 2 task 2 C
* Used external LED's and switches connected to Port L.
* Had a timer control what LED's were on, a second timer 
* for the the button being pressed/delay. Two timer interrupts,
* one strictly for moving from LED's at a 5 second delay. The
* other for if a button was pressed to check if it was still 
* pressed once the timer was done. SW interrupt to see which
* button was pressed. Made a stop light sequence, and the buttons
* are on 2 second delay/interrupts, which lights will stay
* on for 5 seconds. 
*/
#include <stdio.h>
#include <stdint.h>
#include "Task2C.h"

//Global variables to determine LED's that should be on/off
//as well as a variable to determine what switch was pressed
int state = 0; 
volatile unsigned int button = 0;

//Initializing the LED's for Port L
void LED_init(void){
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x400;// activate clock for Port L
  delay++;    
  delay++;    
  GPIOAMSEL_L &= ~0x1C;// disable analog function of PL2, PL3, and PL4
  GPIODIR_L |= 0x1C;// set PL2, PL3, and PL4 to output
  GPIOAFSEL_L &= ~0x1C;// set PL2, PL3, and PL4 regular port function
  GPIODEN_L |= 0x1C;// enable digital output on PL2, PL3, and PL4
}

//Initialize switches (or buttons) associated to Port L
void extern_switch_init(void){
  
  volatile unsigned short delay = 0;
  RCGCGPIO |= 0x400;// Enable Port L Gating Clock
  delay++;    
  delay++;    
  GPIOAMSEL_L &= ~0x3;// Disable PL0 and PL1 analog function
  GPIOAFSEL_L &= ~0x3;// Select PL0 and PL1 regular port function
  GPIODIR_L &= ~0x3;// Set PL0 and PL1 to input direction
  GPIODEN_L |= 0x3;// Enable PL0 and PL1 digital function
  GPIOIS_L &= ~0x3; //Make bit 1 edge sensitive
  GPIOIBE_L &= ~0x3; //Trigger for  single edge
  GPIOIEV_L |= 0x3; //Make the trigger on a rising edge
  GPIOIM_L |= 0x3; //Unmask the interrupt
  NVIC_EN1 |= 0x200000; //Enable interrupt for GPTM Port L #53
}

//Initializes Timer 0 
//This is the timer that last for 5 seconds and
//is in between light transitions
void Timer0_init(void){
  
  RCGCTIMER |= 0x1; //Enables timer 0
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMCFG_T0 &= ~0xFFFFFFFF;//setting pin 0 to 0 for 32-bit timer
  GPTMTAMR_T0 |= 0x2; //bit 1 to 1 for periodic
  GPTMTAMR_T0 &= ~0x10; //bit 4 to 0 for counting down
  GPTMTAILR_T0 = 80000000; //1Hz start up rate, then move to a slower blink
  NVIC_EN0 |= 0x80000; //Enable interrupt for GPTM Timer0A #19
  GPTMIMR_T0 |= 0x1; //Enable time-out interrup mask
  GPTMCTL_T0 |= 0x1; //Enable the timer
}

//Initializes Timer 1
//This timer is used for walk button pressed for 2 seconds
void Timer1_init(void){
  
  RCGCTIMER |= 0x2; //Enables timer 1
  
  GPTMCTL_T1 &= ~0x1; //Disable the timer
  GPTMCFG_T1 &= ~0xFFFFFFFF;//setting pin 0 to 0 for 32-bit timer
  GPTMTAMR_T1 |= 0x2; //bit 1 to 1 for periodic
  GPTMTAMR_T1 &= ~0x10; //bit 4 to 0 for counting down
  GPTMTAILR_T1 = 32000000; //1Hz blink rate
  NVIC_EN0 |= 0x200000; //Enable interrupt for GPTM Timer1A #21
  GPTMIMR_T1 |= 0x1; //Enable time-out interrup mask
  //GPTMCTL_T1 |= 0x1; //Enable the timer 
}

// turn on LED connected to PL4
void Green_on(void){
  
  GPIODATA_L |= 0x10;
}

// turn off LED connected to PL4
void Green_off(void){
  
  GPIODATA_L &= ~0x10;
}

// turn on LED connected to PL3
void Yellow_on(void){
  
  GPIODATA_L |= 0x8;
}

// turn off LED connected to PL3
void Yellow_off(void){
  
  GPIODATA_L &= ~0x8;
}

// turn on LED connected to PL2
void Red_on(void){
  
  GPIODATA_L |= 0x4;
}

// turn off LED connected to PL2
void Red_off(void){
  
  GPIODATA_L &= ~0x4;
}

//On/Off button
unsigned long On_Off(void){
  return (GPIODATA_L & 0x1);// 0x1 (pressed) or 0 (not pressed)
}

//Walk button to be able to turn on yellow LED
unsigned long Walk(void){
  return ((GPIODATA_L & 0x2) == 0x2);// 1 (pressed) or 0 (not pressed)
}

//This is for 5 second delay between the red and green LED
void rgReset(void){
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMICR_T0 |= 0x1;  //Reset the flag
}

//Flag reset for Timer0 A
//Also reset the time to the full 5 sec
//for when there is a transition from 
//no LED's to red, or from yellow to red
void resetT0(void){
  
  GPTMICR_T0 |= 0x1;  //Reset the flag
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMTAILR_T0 = 80000000; //reset the time back to 5 seconds
  GPTMCTL_T0 |= 0x1; //Enable the timer
}

//Stops the timer if SW2 is pressed
//Resets the flag
void stopTime(void){
  
  GPTMCTL_T1 &= ~0x1; //Disable the timer
  GPTMICR_T1 |= 0x1;  //Reset the flag
}

//Start Timer 1 again
//Resets the flag
void startTime(void){
  
  GPTMCTL_T1 |= 0x1; //Enable the timer
  GPTMICR_T1 |= 0x1;  //Reset the flag
}

//This function resets the flag for
//the external switches
void resetSwFlag(void){
  
  GPIOICR_L |= 0x3;
}

//Updates state and turns off the red
//and yellow LED. Turns on the green LED.
void LED_G(void){
  
  state = 1;
  Yellow_off();
  Red_off();
  Green_on();
}

//Updates state and turns off the green
//and red LED. Turns on the yellow LED.
void LED_Y(void){
  
  state = 1;
  Green_off();
  Red_off();
  Yellow_on();
}

//Updates state and turns off the green
//and yellow LED. Turns on the red LED.
void LED_R(void){
  
  state = 3;
  Green_off();
  Yellow_off();
  Red_on();
}

//Turns off all three LED's
//This is the starting state since
//state is a global variable set to 0
void lightsOut(void){
  
  Red_off();
  Yellow_off();
  Green_off();
}

//This function changes what LED's are on/off
//based on the global variable state.
void ledStates(void){
  
  if(state == 0){  
    lightsOut();  
  }
  else if(state == 3){
    LED_G();
  }
  else if(state == 2){
    LED_Y();
  }
  else if(state == 1){
    LED_R();
  }
}

//This function  if called from Timer1A_Handler. It checks
//to see which button was pushed to start the SW_Handler
//AND if that button is still being pushed at that time.
//If the variable button matches with the SW being pressed
//a tranition to another LED on/off will happen.
//Timer0A is reset to it's starting value if and LED is 
//turned on/off because of this.
void switchLED(void){
  
  if((button == 0) && (On_Off() == 0x1)){
    if(state == 0){
      state = 1;
      resetT0();
      ledStates();
    }
    else{
      state = 0;
      ledStates();
    }
  }
  else if((button == 1) && (Walk() == 0x1)){
    if(state == 1){
      state = 2;
      resetT0();
      ledStates();
    }
  }
}

//Timer0A is for the 5 second timer interrupt.
//Resets the flag and calls lights0A() to transition
//between the LED's.
void Timer0A_Handler(void){
  
  resetT0();
  ledStates();
}

//Calls stopTime() to disable the 2 second timer and reset 
//switchLED() to possibly turn on/off LED's
void Timer1A_Handler(void){
  
  stopTime();
  switchLED();
}

//Reset the swtich flag. Start Timer1A. 
//Test to see which SW is pressed, and update a 
//variable according to that.
void SW_Handler(void){
  
  resetSwFlag();
  startTime(); //2 second timer is enabled and flag is cleared
  if(On_Off() == 0x1){ //the on/off button
    button = 0;
  }
  else if(Walk() == 0x1){ //the walk button
    button = 1;
  }
}
//Calls the functions to initialize switches,
//the LED's, and both timers.
void Registers(void){
  
  extern_switch_init(); //initialize the switches
  LED_init();           //initialize the LED's
  Timer0_init();        //initialize Timer 0
  Timer1_init();        //initialize Timer 1
}

//Calls Registers() which does initialization
//Sets the LED_State to the on/off state so LED's are off at the start
//The while loop for the state machine to run is in this function as well
void run(void){
  
  Registers();
  
  while(1){}
}

//The main() calls run() 
int main(){
  
  run();
  return 0;
}