/*
* Garrett Tashiro
* SID: 2075045
* July 13, 2021
* Header file for CSE 474 Lab 2 Task 2 Part B: Light Sequence
* This header allows for LED's on the Tiva to be used with a
* Timer and interrupts. Port J registers for the onboard switches.
*/

#ifndef __HEADER1_H__
#define __HEADER1_H__

//Only need one RCGCGPIO for the whole program
#define RCGCGPIO     (*((volatile uint32_t *)0x400FE608)) 

//Registers for Port N in order to have LED's 1 and 2 light up
#define GPIODIR_N    (*((volatile uint32_t *)0x40064400)) //0x4006.4000
#define GPIODEN_N    (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N   (*((volatile uint32_t *)0x400643FC))

//Port J registers for onboard switches 
#define GPIODIR_J    (*((volatile uint32_t *)0x40060400)) //Base: 0x4006.0000
#define GPIODEN_J    (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J   (*((volatile uint32_t *)0x400603FC))
#define GPIOPUR_J    (*((volatile uint32_t *)0x40060510))


// 1. GPIOIS: Interrupt Sence. 0 for falling edge.
// 2. GPIOIBE: Interrup both edges. 0 for single edge.
// 3. GPIOIEV: Interrupt event. 0 for falling edge, 1 for rising. 
// 4. GPIOIM: Interrupt mask. Allows interrupts to be sent to the interrupt controller.
// 5. GPIOICR: Interrupt clear. Clears prior interrupt by setting to 1.
// 6. NVIC enables/disables interrupts for Port J 
#define GPIOIS_J     (*((volatile uint32_t *)0x40060404)) // 1
#define GPIOIBE_J    (*((volatile uint32_t *)0x40060408)) // 2
#define GPIOIEV_J    (*((volatile uint32_t *)0x4006040C)) // 3
#define GPIOIM_J     (*((volatile uint32_t *)0x40060410)) // 4
#define GPIOICR_J    (*((volatile uint32_t *)0x4006041C)) // 5
#define NVIC_EN1     (*((volatile uint32_t *)0xE000E104)) // 6


// General-Purpose Time Run Mode Clock Gating Control for timers. Use for Timer 0 and Timer 1.
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))


// 1. GPTM Control: fine tune timer config
// 2. GPTM Configuration for Timer 0
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Raw Interrupt Status. Flags when timer has hit 0.
// 6. GPTM Interrupt Clear. Clears the flag for the timer. 
// 7. GPTM Interrupt Mask. Enable/disable GPTM controller-level interrupt.
// 8. NVIC enables/disables interrupts 
#define GPTMCTL_T0   (*((volatile uint32_t *)0x4003000C))  // 1 
#define GPTMCFG_T0   (*((volatile uint32_t *)0x40030000))  // 2
#define GPTMTAMR_T0  (*((volatile uint32_t *)0x40030004))  // 3
#define GPTMTAILR_T0 (*((volatile uint32_t *)0x40030028))  // 4
#define GPTMRIS_T0   (*((volatile uint32_t *)0x4003001C))  // 5
#define GPTMICR_T0   (*((volatile uint32_t *)0x40030024))  // 6
#define GPTMIMR_T0   (*((volatile uint32_t *)0x40030018))  // 7
#define NVIC_EN0     (*((volatile uint32_t *)0xE000E100))  // 8

#endif //__HEADER1_H__