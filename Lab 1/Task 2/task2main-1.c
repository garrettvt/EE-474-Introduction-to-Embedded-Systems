/*
* Garrett Tashiro
* SID: 2075045
* July 6, 2021
* Main file for CSE 474 Lab 1 task 2
* Used external LED's and switches connected to Port L.
* Made a finite state machince to create a "stop light" 
* that switches between the green LED and the red LED being on.
* There is a button for on/off that turns on and off the LED's.
* A "walk" button will have the state transition from green, 
* to yellow, then red. 
*/

#include <stdint.h>
#include "lab1.h"

//Initializing the LED's
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

//Initialize switches (or buttons) 
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

//On/Off button
unsigned long On_Off(void){
  return (GPIODATA_L & 0x1);// 0x1 (pressed) or 0 (not pressed)
}

//Walk button to be able to turn on yellow LED
unsigned long Walk(void){
  return ((GPIODATA_L & 0x2) == 0x2);// 1 (pressed) or 0 (not pressed)
}

//Delay for if either button is pressed
void Delay(void){
  
  for(int i = 0; i < 1700000; i++) {
    
    if(Walk() == 1 || On_Off() == 1){
      break;
    }
  }
}

void SysDelay(void){
  
  for(int i = 0; i < 1700000; i++) {
    
    if(On_Off() == 1){
      
      break;
    }
  }
}

//O_F for on/off
//LED_R for the red LED
//LED_Y for the yellow LED
//LED_G for the green LED
enum LED_States {O_F, LED_R, LED_Y, LED_G} LED_State;

void StopLight(void){
  
  switch(LED_State){ // Transitions
    
    case O_F:  //Initial transition
      if(On_Off() == 1){
        while(On_Off() == 1){} //Needed for the button press to register correctly
        LED_State = LED_R; //If on/off button is pressed in the off state, go to LED_R
      }
      else{
        LED_State = O_F; // Stay in off state unless on/off is pressed
      }
      break;
    
    case LED_R:
      if(On_Off() == 1){    //if the On/Off button is pressed
        while(On_Off() == 1){}  //Needed for the button press to register correctly
        LED_State = O_F;
      }
      else{                //Transition to the go state if no button is pressed
        LED_State = LED_G;
      } 
      break;
    
    case LED_Y:
      if(On_Off() == 1){    //if the On/Off button is pressed
        while(On_Off() == 1){}  //Needed for the button press to register correctly
        LED_State = O_F;
      }
      else{
        LED_State = LED_R;
      }
      break;
      
    case LED_G:
        if(On_Off() == 1){          //If on/off is pressed, go to O_F state and turn off LED's
          while(On_Off() == 1){}
          LED_State = O_F; 
          break;
        }
        else if((Walk() == 1)){        //If the button for walk is pushed, go to yellow.
          while(Walk() == 1){}
          LED_State = LED_Y; 
          break;
        }
        else{
          LED_State = LED_R;
        }
      break;

    default:
      LED_State = O_F;
  } //End of transitions
  
  switch(LED_State){      //State actions 
    
    case O_F:           //In the off state, have all the LED's off
      Green_off();
      Yellow_off();
      Red_off();
      break;
    
    case LED_R:         //State with red LED (stop), have green and yellow turn off and red on
      Green_off();
      Yellow_off();
      Red_on();
      SysDelay();       
      Red_off();
      break;
      
    case LED_Y:
      Green_off();
      Yellow_on();
      Red_off();
      SysDelay();
      Yellow_off();
      break;
      
    case LED_G:
      Green_on();
      Yellow_off();
      Red_off();
      SysDelay();
      Green_off();
      break;
      
  }
}

//This main will run a state machine to flash between red and green
//and whne you press the button for walk it will go from green, yellow, then to red.
//There is an on/off button that will turn the LED's on and off. 
int main()
{
  
  extern_switch_init(); //initialize the switches
  LED_init();           //initialize the LED's
  
  LED_State = O_F;      //LED's will start in the on/off state
  
  while(1){             //infinite loop
    
    StopLight();        //Call the state machine
  }
  return 0;
}
