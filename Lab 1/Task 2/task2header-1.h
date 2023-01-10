/*
* Garrett Tashiro
* SID: 2075045
* July 6, 2021
* Header file for CSE 474 Lab 1 task 2
* Used external LED's and switches connected to Port L
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
#endif //__HEADER1_H__