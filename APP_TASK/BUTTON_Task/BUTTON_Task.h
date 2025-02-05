/**
  ******************************************************************************
  * @file           : BUTTON_Task.h
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/18
  ******************************************************************************
  */

#ifndef MZ_CNN_PROJ_BUTTON_TASK_H
#define MZ_CNN_PROJ_BUTTON_TASK_H
#include "main.h"

typedef enum eButton_Status{
    BUTTON_IDLE = 0,
    BUTTON_RELEASE = 1,
    BUTTON_HOLD = 2,
    BUTTON_HOLD_LONG = 3
}eButton_Status;


typedef struct Button_t{
    struct {
        eButton_Status status;
    }attribute;

    struct {
        void (*Init) (void);
        void (*Read_Button) (void);
        void (*Button_Status_Clear) (void);
    }function;
}Button_t;

extern Button_t *Button_Handle;
void Button_Create(Button_t **pButton);
#endif //MZ_CNN_PROJ_BUTTON_TASK_H
