/*
* Garrett Tashiro
* SID: 2075045
* July 11, 2021
* Header file for CSE 474 Lab 2 Task 1 Part A: Light Sequence
* This header allows for LED's on the Tiva to be used along
* with a timer and an interrupt for the timer.
*/

#ifndef __HEADER1_H__
#define __HEADER1_H__

//Only need one RCGCGPIO for the whole program
#define RCGCGPIO     (*((volatile uint32_t *)0x400FE608)) 

//Registers for Port F in order to have LED's 3 and 4 light up
#define GPIODIR_F    (*((volatile uint32_t *)0x4005D400)) // 0x4005.D000
#define GPIODEN_F    (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F   (*((volatile uint32_t *)0x4005D3FC)) 

//Registers for Port N in order to have LED's 1 and 2 light up
#define GPIODIR_N    (*((volatile uint32_t *)0x40064400)) //0x4006.4000
#define GPIODEN_N    (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N   (*((volatile uint32_t *)0x400643FC))


// General-Purpose Time Run Mode Clock Gating Control for timers. Use for Timer 0 and Timer 1.
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))


// 1. GPTM Control: fine tune timer config
// 2. GPTM Configuration for Timer 0
// 3. GPTM Time A Mode. For periodic timer mode, and counting down
// 4. GPTM Timer A Interval Load. Used to load the starting count value for counting down
// 5. GPTM Interrupt Clear. Clears the flag for the timer. 
// 6. GPTM Interrupt Mask. Enable/disable GPTM controller-level interrupt.
// 7. NVIC enables/disables interrupts
#define GPTMCTL_T0   (*((volatile uint32_t *)0x4003000C))  // 1 
#define GPTMCFG_T0   (*((volatile uint32_t *)0x40030000))  // 2
#define GPTMTAMR_T0  (*((volatile uint32_t *)0x40030004))  // 3
#define GPTMTAILR_T0 (*((volatile uint32_t *)0x40030028))  // 4
#define GPTMICR_T0   (*((volatile uint32_t *)0x40030024))  // 5
#define GPTMIMR_T0   (*((volatile uint32_t *)0x40030018))  // 6
#define NVIC_EN0     (*((volatile uint32_t *)0xE000E100))  // 7


#endif //__HEADER1_H__