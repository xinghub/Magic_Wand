/**
  ******************************************************************************
  * @file    vofa.h
  * @author  crud
  * @brief   Header file for vofa.c file
  ******************************************************************************
  * @attention
  *
  * 使用VOFA上位机打印数据
  ******************************************************************************
  */
#include "vofa.h"
#include "usart.h"

static uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

/**
* @CreateTime
* @Author
* @brief vofa发送浮点数
* @param data 浮点数组
* @param ch, 通道数量
* @return
*/
void Vofa_Fdata_Send(float *data, uint8_t ch)
{
    uint8_t i = 0;
    for(i = 0; i < ch; i++){
        HAL_UART_Transmit(&huart1, (uint8_t*)(data+i), sizeof(float), 0xFF);
    }
    HAL_UART_Transmit(&huart1, tail, 4, 0xFF);
}