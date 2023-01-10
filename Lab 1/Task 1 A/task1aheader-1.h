/*
* Garrett Tashiro
* SID: 2075045
* June 24, 2021
* Header file for CSE 474 Lab 1 task 1 a
* This header allows for LED's on the Tiva to be used.
*/
#ifndef __HEADER1_H__
#define __HEADER1_H__

#define RCGCGPIO     (*((volatile uint32_t *)0x400FE608)) //Only need one for the whole of these for the whole program

//Registers for Port F in order to have LED's 3 and 4 light up
#define GPIODIR_F    (*((volatile uint32_t *)0x4005D400)) // 0x4005.D000
#define GPIODEN_F    (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F   (*((volatile uint32_t *)0x4005D3FC)) 

//Registers for Port N in order to have LED's 1 and 2 light up
#define GPIODIR_N    (*((volatile uint32_t *)0x40064400)) //0x4006.4000
#define GPIODEN_N    (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N   (*((volatile uint32_t *)0x400643FC))

#endif //__HEADER1_H__