/**
  ******************************************************************************
  * @file           : IMU_Task.h
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/17
  ******************************************************************************
  */

#ifndef __IMU_TASK_H
#define __IMU_TASK_H

#include "config.h"
#include "main.h"

//#define SYSTEM_MODE_DATA_COLLECT

//Define IMU
#define IMU_IS_MPU6050
#define IMU_DATA_PRINT_HEADER "IMU\n"
#define IMU_SAMPLING_TIME_MS (1500)
#define IMU_SEQUENCE_LENGTH_MAX (150)
#define IMU_ACC_TRANS_CONSTANT (8192.0)  //+-4g
//#define IMU_ACC_TRANS_CONSTANT (1.0)  //+-4g
//+-500 degrees raw value to degree constant is divided by 16.384
//+-500 to radian is divided by (73.537*180/PI) = 4213.359738
#define IMU_GYRO_TRANS_RADIAN_CONSTANT (4213.359738)
//#define IMU_GYRO_TRANS_RADIAN_CONSTANT (1.0)

typedef enum eIMU_STATUS{
    IMU_Idle = 0,
    IMU_Sampling = 1,
    IMU_Sampled = 2,
}eIMU_STATUS;

typedef enum IMU_ACC_Index{
    AccX = 0,
    AccY = 1,
    AccZ = 2,

}IMU_ACC_Index;

typedef enum IMU_GYRO_Index{
    Roll = 0,
    Pitch = 1,
    Yaw = 2

}IMU_GYRO_Index;

typedef struct IMU_t
{
    struct {
        eIMU_STATUS status;
        float INS_Angle[3];
        float INS_Acc[3];
        float INS_Gyro[3];
        float acc[IMU_SEQUENCE_LENGTH_MAX][3];
        float gyro[IMU_SEQUENCE_LENGTH_MAX][3];
    }attribute;

    struct {
        void (*IMU_Init)(void);
        void (*IMU_Get_Angle) (void);
        void (*Sample_Start)(void);
        void (*Sample_Stop)(void);
#ifdef SYSTEM_MODE_DATA_COLLECT
        void (*IMU_Data_Print)(void);
#endif //SYSTEM_MODE_DATA_COLLECT

    }function;
    eIMU_STATUS status;
    void (*Sample_Start)(void);
    void (*Sample_Stop)(void);
    float acc[IMU_SEQUENCE_LENGTH_MAX][3];
    float gyro[IMU_SEQUENCE_LENGTH_MAX][3];
}IMU_t;

//void IMU_Get_Data(uint8_t index);
//void IMU_Init(void);
//void IMU_Data_Print();
//extern struct IMU_t IMU;
extern IMU_t *IMU_Handle;
void IMU_create(IMU_t **imu);

#endif //MZ_CNN_PROJ_IMU_TASK_H
