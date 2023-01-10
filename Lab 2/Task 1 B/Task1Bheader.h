/*
* Garrett Tashiro
* SID: 2075045
* July 9, 2021
* Header file for CSE 474 Lab 2 task 1 B
* Used external LED's and switches connected to Port L
* along with two timers. 
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


// General-Purpose Time Run Mode Clock Gating Control for timers. Use for Timer 0 and Timer 1.
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))


// 1. GPTM Control: fine tune timer config
// 2. GPTM Configuration for Timer 0
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Raw Interrupt Status. Flags when timer has hit 0.
// 6. GPTM Interrupt Clear. Clears the flag for the timer. 
#define GPTMCTL_T0   (*((volatile uint32_t *)0x4003000C))  // 1 
#define GPTMCFG_T0   (*((volatile uint32_t *)0x40030000))  // 2
#define GPTMTAMR_T0  (*((volatile uint32_t *)0x40030004))  // 3
#define GPTMTAILR_T0 (*((volatile uint32_t *)0x40030028))  // 4
#define GPTMRIS_T0   (*((volatile uint32_t *)0x4003001C))  // 5
#define GPTMICR_T0   (*((volatile uint32_t *)0x40030024))  // 6


// 1. GPTM Control: fine tune timer config
// 2. GPTM Configuration for Timer 1
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Raw Interrupt Status. Flags when timer has hit 0.
// 6. GPTM Interrupt Clear. Clears the flag for the timer.
#define GPTMCTL_T1   (*((volatile uint32_t *)0x4003100C))  // 1 
#define GPTMCFG_T1   (*((volatile uint32_t *)0x40031000))  // 2
#define GPTMTAMR_T1  (*((volatile uint32_t *)0x40031004))  // 3
#define GPTMTAILR_T1 (*((volatile uint32_t *)0x40031028))  // 4
#define GPTMRIS_T1   (*((volatile uint32_t *)0x4003101C))  // 5
#define GPTMICR_T1   (*((volatile uint32_t *)0x40031024))  // 6
#endif //__HEADER1_H__