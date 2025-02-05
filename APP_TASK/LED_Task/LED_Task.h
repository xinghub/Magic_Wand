/**
  ******************************************************************************
  * @file           : LED_Task.h
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/17
  ******************************************************************************
  */

#ifndef LED_TASK_H
#define LED_TASK_H
//#include "main.h"
typedef enum eLED{
    OFF = 0,
    ON = 1,
    BLINK_2HZ = 2,
    BLINK_5HZ = 3,
    BLINK_10HZ = 4
}eLED_STATUS;

typedef struct LED_t{
    struct {
        eLED_STATUS status;
    }attribute;

    struct {
        void    (*Init) (void);
        void    (*Led_ON)   (void);
        void    (*Led_OFF)  (void);
        void    (*BLINK)    (eLED_STATUS Hz);
    }function;

}LED_t;

extern LED_t *Led_Handle;
void LED_Create(LED_t **led);

#endif //__LED_TASK_H
