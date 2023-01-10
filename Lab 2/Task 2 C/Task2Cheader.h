/*
* Garrett Tashiro
* SID: 2075045
* July 14, 2021
* Header file for CSE 474 Lab 2 Task 2 C
* Used external LED's and switches connected to Port L
* Registers for Timer 0 and Timer 1 for 2 and 5 second timers.
* This header file also has the registers that are associated with
* Timer 1 and 0 as well as interrupts for the switches and both timers.
*/

#ifndef __HEADER1_H__
#define __HEADER1_H__

#define RCGCGPIO     (*((volatile uint32_t *)0x400FE608)) //Only need one for the whole of these for the whole program

// All the registers for Port L, which is being used for the external LED's and switches. 
#define GPIOAMSEL_L  (*((volatile uint32_t *)0x40062528))
#define GPIODIR_L    (*((volatile uint32_t *)0x40062400))
#define GPIODEN_L    (*((volatile uint32_t *)0x4006251C))
#define GPIOAFSEL_L  (*((volatile uint32_t *)0x40062420))
#define GPIODATA_L   (*((volatile uint32_t *)0x400623FC))


// 1. GPIOIS: Interrupt Sence. 0 for falling edge.
// 2. GPIOIBE: Interrup both edges. 0 for single edge.
// 3. GPIOIEV: Interrupt event. 0 for falling edge, 1 for rising. 
// 4. GPIOIM: Interrupt mask. Allows interrupts to be sent to the interrupt controller.
// 5. GPIOICR: Interrupt clear. Clears prior interrupt by setting to 1.
// 6. NVIC enables/disables interrupts for Port L. Bit 53.
#define GPIOIS_L     (*((volatile uint32_t *)0x40062404)) // 1
#define GPIOIBE_L    (*((volatile uint32_t *)0x40062408)) // 2
#define GPIOIEV_L    (*((volatile uint32_t *)0x4006240C)) // 3
#define GPIOIM_L     (*((volatile uint32_t *)0x40062410)) // 4
#define GPIOICR_L    (*((volatile uint32_t *)0x4006241C)) // 5
#define NVIC_EN1     (*((volatile uint32_t *)0xE000E104)) // 6

// 1. General-Purpose Time Run Mode Clock Gating Control for timers. Use for Timer 0 and Timer 1.
// 2. NVIC enables/disables interrupts. EN0 works for both timers of bit 19 and 21 
#define RCGCTIMER    (*((volatile uint32_t *)0x400FE604)) // 1
#define NVIC_EN0     (*((volatile uint32_t *)0xE000E100)) // 2 


// 1. GPTM Control: fine tune timer config for Timer 0
// 2. GPTM Configuration for Timer 0
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Interrupt Clear. Clears the flag for the timer.
// 6. GPTM Interrupt Mask. Enable/disable GPTM controller-level interrupt.
#define GPTMCTL_T0   (*((volatile uint32_t *)0x4003000C))  // 1 
#define GPTMCFG_T0   (*((volatile uint32_t *)0x40030000))  // 2
#define GPTMTAMR_T0  (*((volatile uint32_t *)0x40030004))  // 3
#define GPTMTAILR_T0 (*((volatile uint32_t *)0x40030028))  // 4
#define GPTMICR_T0   (*((volatile uint32_t *)0x40030024))  // 5
#define GPTMIMR_T0   (*((volatile uint32_t *)0x40030018))  // 6


// 1. GPTM Control: fine tune timer config for Timer 1
// 2. GPTM Configuration for Timer 1
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Interrupt Clear. Clears the flag for the timer.
// 6. GPTM Interrupt Mask. Enable/disable GPTM controller-level interrupt.
#define GPTMCTL_T1   (*((volatile uint32_t *)0x4003100C))  // 1 
#define GPTMCFG_T1   (*((volatile uint32_t *)0x40031000))  // 2
#define GPTMTAMR_T1  (*((volatile uint32_t *)0x40031004))  // 3
#define GPTMTAILR_T1 (*((volatile uint32_t *)0x40031028))  // 4
#define GPTMICR_T1   (*((volatile uint32_t *)0x40031024))  // 5
#define GPTMIMR_T1   (*((volatile uint32_t *)0x40031018))  // 6

#endif //__HEADER1_H__