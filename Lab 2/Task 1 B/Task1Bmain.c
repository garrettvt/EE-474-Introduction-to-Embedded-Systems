/*
* Garrett Tashiro
* SID: 2075045
* July 9, 2021
* Main file for CSE 474 Lab 2 task 1.2
* Used external LED's and switches connected to Port L.
* Made a finite state machince to create a "stop light" 
* that switches between the green LED and the red LED being on
* using a 5 second timer for the light intervals.
* There is a button for on/off that turns on and off the LED's.
* A "walk" button will have the state transition from green, 
* to yellow, then red. All buttons require a 2 second push for them
* to either turn on/off the LED's, or state transition to "warm".
*/

#include <stdint.h>
#include "stoplight.h"

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
}

//Initializes Timer 0 
//This will be used for the delay between state changes
//Has a quick start up rate, but will use a slower reset once needed. 
void Timer0_init(void){
  
  RCGCTIMER |= 0x1; //Enables timer 0
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMCFG_T0 &= ~0xFFFFFFFF;//setting pin 0 to 0 for 32-bit timer
  GPTMTAMR_T0 |= 0x2; //bit 1 to 1 for periodic
  GPTMTAMR_T0 &= ~0x10; //bit 4 to 0 for counting down
  GPTMTAILR_T0 = 16000000; //1Hz start up rate, then move to a slower blink
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
  GPTMCTL_T1 |= 0x1; //Enable the timer 
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

//Turns off all three LED's
void lightsOut(void){
  
  Red_off();
  Yellow_off();
  Green_off();
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
  GPTMTAILR_T0 = 80000000; //5 second delay
  GPTMCTL_T0 |= 0x1; //Enable the timer
}

//When all LED's are off and the on/off button is hit, 
//the red LED will light up faster than the 5 second interval
void redReset(void){
  
  GPTMCTL_T0 &= ~0x1; //Disable the timer
  GPTMICR_T0 |= 0x1;  //Reset the flag
  GPTMTAILR_T0 = 16000000; //1 second delay for start up on red
  GPTMCTL_T0 |= 0x1; //Enable the timer 
}

//This will reset the walk timer each time after the button is pressed
void buttonTimer(void){
  
  GPTMCTL_T1 &= ~0x1; //Disable the timer
  GPTMICR_T1 |= 0x1;  //Reset the flag
  GPTMTAILR_T1 = 32000000; //2 second timer reset time
  GPTMCTL_T1 |= 0x1; //Enable the timer
}

enum LED_States {O_F, LED_R, LED_Y, LED_G} LED_State;

//This is a FSM for the four different states for the stoplight.
//It implememnts two timers. One is for 5 seconds to stay in a state
//and the other checks for buttons being held for 2 seconds.
void StopLight(void){
  
  switch(LED_State){ // Transitions
    
    case O_F:  //Initial transition
      rgReset();  //Reset the 5 second timer
      
      while(((GPTMRIS_T0 & 0x1) != 0x1) && (LED_State != LED_R)){ 
        buttonTimer();
 
        while((On_Off() == 1)){ 
          if((GPTMRIS_T1 & 0x1) == 0x1){ //if walk button is held for 2 seconds
            LED_State = LED_R;           //go to LED_R state and light up red LED
            break;
          }
        }
      }
      if(LED_State == O_F){
        LED_State = O_F; // Stay in off state unless on/off is pressed
      }
      break;
    
    case LED_R:
      rgReset();  //Reset the 5 second timer
      
      while(((GPTMRIS_T0 & 0x1) != 0x1) && (LED_State != O_F)){
        buttonTimer();
        
        while((On_Off() == 1)){ 
          if((GPTMRIS_T1 & 0x1) == 0x1){ //if walk button is held for 2 seconds
            LED_State = O_F;           //go to LED_R state and light up red LED
            break;
          }
        }
      }
      if(LED_State == LED_R){
        LED_State = LED_G;  
      }
      break;
    
    case LED_Y:
      rgReset(); //Reset the 5 second timer
      
      while(((GPTMRIS_T0 & 0x1) != 0x1) && (LED_State != O_F)){
        buttonTimer();
        
        while((On_Off() == 1)){ 
          if((GPTMRIS_T1 & 0x1) == 0x1){ //if walk button is held for 2 seconds
            LED_State = O_F;           //go to LED_R state and light up red LED
            break;
          }
        }
      }
      if(LED_State == LED_Y){
        LED_State = LED_R;
      }
      break;
    
    case LED_G:
      rgReset();  //Reset the 5 second timer
      
      while(((GPTMRIS_T0 & 0x1) != 0x1) && ((LED_State != LED_Y) || (LED_State != O_F))){ //While the 5 second timer hasn't timed out
        buttonTimer();                  //Enable the 2 second timer
        
        if(Walk() == 1){
          while(Walk() == 1){
            if((GPTMRIS_T1 & 0x1) == 0x1){ //if walk button is held for 2 seconds
              LED_State = LED_Y;           //set the state to warm
              break;
            }
          }
          buttonTimer();
        }
        
        else if(On_Off() == 1){
          while((On_Off() == 1)){ 
            if((GPTMRIS_T1 & 0x1) == 0x1){ //if walk button is held for 2 seconds
              LED_State = O_F;           //go to LED_R state and light up red LED
              break;
            }
          }
          buttonTimer();
        }
      }
      if(LED_State == LED_G){ //If state didn't change from button press
        LED_State = LED_R;    //Go to the next state in the sequence
      }
      break;
  }
      
  switch(LED_State){      //State actions 
    
    case O_F:           //In the off state, have all the LED's off
      lightsOut();
      break;
    
    case LED_R:         //State with red LED (stop), have green and yellow turn off and red on
      Green_off();
      Yellow_off();
      Red_on();     
      break;
      
    case LED_Y:
      Green_off();
      Red_off();
      Yellow_on();
      break;
      
    case LED_G:
      Yellow_off();
      Red_off();
      Green_on();
      break;   
  }
}

//Initializes LED's, SW's, and both timers.
//Sets the LED_State to the on/off state so LED's are off at the start
//The while loop for the state machine to run is in this function as well
void run(void){
  
  extern_switch_init(); //initialize the switches
  LED_init();           //initialize the LED's
  Timer0_init();        //initialize Timer 0
  Timer1_init();        //initialize Timer 1
  
  LED_State = O_F;      //LED's will start in the on/off state
  
  while(1){
    
    StopLight();
  }
}

//The main calls run()
int main()
{
  run();
  return 0;
}
