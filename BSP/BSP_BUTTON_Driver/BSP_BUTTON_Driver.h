/**
 * File Name: button.h
 * 
 * Copyright © 2023 Civic_Crab. All rights reserved.
 * 
 * Author: Civic_Crab
 * Creation Date: Oct 1, 2020
 * Version: 1.0.0
 * 
 * Description: This file prodides button functions implementation
 */


#ifndef	_BUTTON_H_
#define	_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//typedef struct Button_t
//{
//    struct {
//
//    }attitude;
//	eButton_Status status;
//
//	void (*status_clear)(void);
//}Button_t;

//Define Button
//#define RCC_TIM_FOR_BUTTON RCC_APB1Periph_TIM4
//#define RCC_GPIO_FOR_BUTTON RCC_APB2Periph_GPIOA
//#define BUTTON_GPIO GPIOA
//#define BUTTON_GPIO_PIN GPIO_Pin_0
//#define TIM_FOR_BUTTON TIM4
//#define TIM_FOR_BUTTON_IRQN TIM4_IRQn
//#define TIM4_PER_INTERRUPT_MS (1)
//#define BUTTON_IDLE_SHORT_THRESHOLD_MS (10)
//#define BUTTON_SHORT_LONG_THRESHOLD_MS (500)


/* Functions -----------------------------------------------------------------*/
void Button_Init(void);
uint8_t Bsp_Button_Read(void);
#endif	//_BUTTON_H_