/*
* Garrett Tashiro
* SID: 2075045
* June 24, 2021
* Header file for CSE 474 Lab 1 task 1 b
* This header allows for LED's and switches on the Tiva to be used.
*/

#ifndef __HEADER1_H__
#define __HEADER1_H__

//May not need more than 1 RCGCGPIO. COuld use if/else statements to change it
//with a counter

#define RCGCGPIO     (*((volatile uint32_t *)0x400FE608)) //Only need one for the whole of these for the whole program

//Port N registers for onboard LED's 1 and 2
#define GPIODIR_N    (*((volatile uint32_t *)0x40064400)) //0x4006.4000
#define GPIODEN_N    (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N   (*((volatile uint32_t *)0x400643FC))

//Port J registers for onboard switches 
#define GPIODIR_J    (*((volatile uint32_t *)0x40060400)) //Base: 0x4006.0000
#define GPIODEN_J    (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J   (*((volatile uint32_t *)0x400603FC))
#define GPIOPUR_J    (*((volatile uint32_t *)0x40060510))
#endif //__HEADER1_H__