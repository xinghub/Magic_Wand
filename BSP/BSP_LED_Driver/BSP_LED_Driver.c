/**
 * File Name: SPI.c
 * 
 * Copyright © 2023 Civic_Crab. All rights reserved.
 * 
 * Author: Civic_Crab
 * Creation Date: Oct 1, 2020
 * Version: 1.0.0
 * 
 * Description: This file prodides SPI functions implementation
 */


/* Includes ------------------------------------------------------------------*/
#include "BSP_LED_Driver.h"

/**
* @CreateTime
* @Author
* @brief led on
* @param null
* @return
*/
void Bsp_Led_ON()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    //log("bsp led on\r\n");
}

/**
* @CreateTime
* @Author
* @brief led off
* @param null
* @return
*/
void Bsp_Led_OFF(){
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    //log("bsp led off\r\n");
}
