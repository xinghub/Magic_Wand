/**
  ******************************************************************************
  * @file           : LED_Task.c
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/17
  ******************************************************************************
  */

#include "LED_Task.h"
#include "BSP_LED_Driver.h"

LED_t *Led_Handle;

/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
void LED_Init(){
    Led_Handle->function.Led_OFF();
    Led_Handle->attribute.status = OFF;
}
/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
static void LED_ON()
{
    Bsp_Led_ON();
    Led_Handle->attribute.status = ON;
    //log("led ON\r\n");
}
/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
static void LED_OFF()
{
    Bsp_Led_OFF();
    Led_Handle->attribute.status = OFF;
    //log("led OFF\r\n");
}
/**
* @CreateTime
* @Author
* @brief LED闪烁
* @param Hz
* @return
*/
static void Blink(eLED_STATUS Hz)
{
    uint8_t i = 0;
    switch(Hz)
	{
        case OFF:
			LED_OFF();
			break;
		case ON:
            LED_ON();
			break;
		case BLINK_5HZ:
            //log("5Hz\r\n");
			for(i = 0; i < 5; i++){
                Led_Handle->function.Led_ON();
			HAL_Delay(100);
                Led_Handle->function.Led_OFF();
            HAL_Delay(100);
			}
			break;
		case BLINK_10HZ:
			for(i = 0; i < 10; i++){
                Led_Handle->function.Led_ON();
            HAL_Delay(50);
                Led_Handle->function.Led_OFF();
            HAL_Delay(50);
			}
			break;
		case BLINK_2HZ:
			for(i = 0; i < 4; i++){
                Led_Handle->function.Led_ON();
            HAL_Delay(250);
                Led_Handle->function.Led_OFF();
            HAL_Delay(250);
			}
			break;
	}
    Led_Handle->attribute.status = OFF;
}
/**
* @CreateTime
* @Author
* @brief led init
* @param val1
* @param val2
* @return
*/
void LED_Create(LED_t **led)
{
    *led = (LED_t*)malloc(sizeof(LED_t));
    if(NULL == *led){
        log_info("Led_Handle malloc failed %d\r\n", sizeof(LED_t));
        return;
    }

    (*led)->attribute.status = OFF;

    (*led)->function.Init = LED_Init;
    (*led)->function.Led_ON = LED_ON;
    (*led)->function.Led_OFF = LED_OFF;
    (*led)->function.BLINK = Blink;

    log_info("Led_Handle malloc succeed %d bytes\r\n", sizeof(LED_t));
    (*led)->function.Init();
}