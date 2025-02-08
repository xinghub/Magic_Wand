/**
  ******************************************************************************
  * @file           : BUTTON_Task.c
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/18
  ******************************************************************************
  */

#include "BUTTON_Task.h"
#include "BSP_BUTTON_Driver.h"

Button_t *Button_Handle;
#define HOLD_LONG_TIME 2000
#define HOLD_SHORT_TIME 100

/**
* @CreateTime
* @Author
* @brief led init
* @param val1
* @param val2
* @return
*/
void Button_Create(Button_t **pButton)
{
    *pButton = (Button_t*)malloc(sizeof(Button_t));
    if(NULL == *pButton){
        log_info("Button_Handle malloc failed\r\n");
        return;
    }

    (*pButton)->attribute.status = BUTTON_IDLE;

    //(*pButton)->function.Init = LED_Init;
    log_info("Button_Handle malloc succeed\r\n");
}
/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
void Button_IT_Callback(void)
{
    static uint32_t hold_time, release_time;
    if(Bsp_Button_Read() == 0){     //按下
        //Button_Handle->attribute.status = BUTTON_HOLD;
        hold_time = HAL_GetTick();
        log_info("hold_time:%ld\r\n", hold_time);
    }
    if(Bsp_Button_Read() == 1){     //释放
        release_time = HAL_GetTick();
        log_info("release_time:%ld\r\n", release_time);
    }
    if(release_time - hold_time > HOLD_LONG_TIME){
        Button_Handle->attribute.status = BUTTON_HOLD_LONG;
        log_info("long\r\n");
    }
    else if(release_time - hold_time > HOLD_SHORT_TIME){
        Button_Handle->attribute.status = BUTTON_HOLD;
        log_info("short\r\n");
    }
}
/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if(GPIO_Pin == Button_Pin){
//        log_info("Button IT start\r\n");
        //Button_IT_Callback();
//        static uint32_t hold_time, release_time;
//        if(Bsp_Button_Read() == 0){     //按下
//            //Button_Handle->attribute.status = BUTTON_HOLD;
//            hold_time = HAL_GetTick();
//            log_info("hold_time:%ld\r\n", hold_time);
//        }
//        if(Bsp_Button_Read() == 1){     //释放
//            release_time = HAL_GetTick();
//            log_info("release_time:%ld\r\n", release_time);
//        }
//        if(release_time - hold_time > HOLD_LONG_TIME){
//            Button_Handle->attribute.status = BUTTON_HOLD_LONG;
//            log_info("long\r\n");
//        }
//        else if(release_time - hold_time > HOLD_SHORT_TIME){
//            Button_Handle->attribute.status = BUTTON_HOLD;
//            log_info("short\r\n");
//        }
//    }
//
//    if(GPIO_Pin == IMU_EXTI_Pin){
//
//    }
//}