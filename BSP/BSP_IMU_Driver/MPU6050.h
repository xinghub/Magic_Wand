#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"                  // Device header

//#define INT_STOP EXTI->IMR &= ~(EXTI_Line5)
//#define INT_START EXTI->IMR |= EXTI_Line5

#define IMU_DEFAULT_HZ  (100)

void MPU6050_Init(void); 
void DMP_Init(void);
void Read_DMP(void);

#endif
