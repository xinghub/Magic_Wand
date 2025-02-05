/**
  ******************************************************************************
  * @file           : IMU_Task.c
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/17
  ******************************************************************************
  */

#include <math.h>
#include "IMU_Task.h"
#include "MPU6050.h"
#include "IIC.h"
#include "MPU6050_Reg.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "vofa.h"
#include "MahonyAHRS.h"

IMU_t IMU;
//加速度计低通滤波
static float accel_fliter_1[3] = {0.0f, 0.0f, 0.0f};
static float accel_fliter_2[3] = {0.0f, 0.0f, 0.0f};
static float accel_fliter_3[3] = {0.0f, 0.0f, 0.0f};
static const float fliter_num[3] = {1.929454039488895f, -0.93178349823448126f, 0.002329458745586203f};

static const double Epsilon = 0.0009765625f;
static const double Threshold = 0.5f - Epsilon;

//static float INS_quat[4] = {1.0f, 0.0f, 0.0f, 0.0f};			//四元数
//measured data beginning with m, d means derivative ,mdAngle is measured angular velovity in this case.
int16_t IMU_bias[6] = {0,0,0,0,0,0};

//This function is used to print the Acc data to your computer
//IMU_DATA_PRINT_HEADER is the header that the script use to identify which the following messages is IMU data or not.

#ifdef SYSTEM_MODE_DATA_COLLECT
void IMU_Data_Print(void){
	int16_t i = 0;
	printf(IMU_DATA_PRINT_HEADER);
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
	printf("%f %f %f %f %f %f\n",
		IMU.acc[i][AccX], IMU.acc[i][AccY], IMU.acc[i][AccZ],
		IMU.gyro[i][Roll], IMU.gyro[i][Pitch], IMU.gyro[i][Yaw]);
        HAL_Delay(10);
	}
}
#endif //SYSTEM_MODE_DATA_COLLECT

void IMU_Sample_Start(void)
{
    //HAL_NVIC_DisableIRQ(GPIO_PIN_0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    IMU.status = IMU_Sampling;
}

void IMU_Sample_Stop(void)
{
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    //HAL_NVIC_EnableIRQ(GPIO_PIN_0);
    IMU.status = IMU_Sampled;
}
/**
* @CreateTime
* @Author
* @brief 四元数转换为欧拉角
* @param val1
* @param val2
* @return
*/
void Quaternion_To_Euler(float *q)
{
    double TEST = q[0] * q[2] - q[1] * q[3];

    if (TEST < -Threshold || TEST > Threshold) // 奇异姿态,俯仰角为±90°
    {
        int sign = __signbitd(TEST);

        IMU.attribute.INS_Angle[0] = -2 * sign * atan2(q[1], q[0]); // yaw
        IMU.attribute.INS_Angle[1] = sign * (PI / 2.0); // pitch
        IMU.attribute.INS_Angle[2] = 0; // roll
    }
    else
    {
        IMU.attribute.INS_Angle[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2]* q[2] + 1);	// roll
        IMU.attribute.INS_Angle[1] =  asin(-2 * q[1] * q[3] + 2 * q[0] * q[2]);	// pitch
        IMU.attribute.INS_Angle[0] = atan2(2*(q[1] * q[2]+ q[0]*q[3]),q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3]);	//yaw
    }
}
/**
* @CreateTime
* @Author
* @brief 获取欧拉角
* @param val1
* @param val2
* @return
*/
void IMU_Get_Angle(void)
{
    //加速度计低通滤波
    // accel low-pass filter
    accel_fliter_1[0] = accel_fliter_2[0];
    accel_fliter_2[0] = accel_fliter_3[0];

    accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + IMU.attribute.INS_Acc[0] * fliter_num[2];
    accel_fliter_1[1] = accel_fliter_2[1];
    accel_fliter_2[1] = accel_fliter_3[1];
    accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] + IMU.attribute.INS_Acc[1] * fliter_num[2];

    accel_fliter_1[2] = accel_fliter_2[2];
    accel_fliter_2[2] = accel_fliter_3[2];
    accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + IMU.attribute.INS_Acc[2] * fliter_num[2];
//    Vofa_Fdata_Send(accel_fliter_3,3);
    float *q = MahonyAHRSupdateIMU(IMU.attribute.INS_Gyro[0],
                                   IMU.attribute.INS_Gyro[1],
                                   IMU.attribute.INS_Gyro[2],
                                   accel_fliter_3[0],
                                   accel_fliter_3[1],
                                   accel_fliter_3[2]);
//    INS_quat[0] = q[0];
//    INS_quat[0] = q[1];
//    INS_quat[0] = q[2];
//    INS_quat[0] = q[3];
    if(NULL != q){
        Quaternion_To_Euler(q);
    }

//    Vofa_Fdata_Send(IMU.attribute.INS_Angle,3);
    float s[6];
    s[0] = accel_fliter_3[0];
    s[1] = accel_fliter_3[1];
    s[2] = accel_fliter_3[2];
    s[3] = IMU.attribute.INS_Acc[0];
    s[4] = IMU.attribute.INS_Acc[1];
    s[5] = IMU.attribute.INS_Acc[2];

    //Vofa_Fdata_Send(s, 6);

}
void IMU_Get_Data(uint8_t i)
{
    uint8_t temp_acc[6];
    uint8_t temp_gyro[6];
    int16_t IMU_Received[6];
    IIC1_read(0x68,MPU6050_RA_ACCEL_XOUT_H,6,temp_acc);
    IMU_Received[AccX] = (temp_acc[0] << 8) + temp_acc[1] - IMU_bias[AccX];
    IMU_Received[AccY] = (temp_acc[2] << 8) + temp_acc[3] - IMU_bias[AccY];
    IMU_Received[AccZ] = (temp_acc[4] << 8) + temp_acc[5] - IMU_bias[AccZ];

    IMU.acc[i][AccX] = IMU_Received[AccX] / IMU_ACC_TRANS_CONSTANT;
    IMU.acc[i][AccY] = IMU_Received[AccY] / IMU_ACC_TRANS_CONSTANT;
    IMU.acc[i][AccZ] = IMU_Received[AccZ] / IMU_ACC_TRANS_CONSTANT;
    IMU.attribute.INS_Acc[0] = IMU.acc[i][AccX];
    IMU.attribute.INS_Acc[1] = IMU.acc[i][AccY];
    IMU.attribute.INS_Acc[2] = IMU.acc[i][AccZ];

    IIC1_read(0x68,MPU6050_RA_GYRO_XOUT_H,6,temp_gyro);
    IMU_Received[Roll] = (temp_gyro[0] << 8) + temp_gyro[1] - IMU_bias[Roll];
    IMU_Received[Pitch] = (temp_gyro[2] << 8) + temp_gyro[3]- IMU_bias[Pitch];
    IMU_Received[Yaw] = (temp_gyro[4] << 8) + temp_gyro[5]  - IMU_bias[Yaw];

    IMU.gyro[i][Roll] = IMU_Received[Roll] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
    IMU.gyro[i][Pitch] = IMU_Received[Pitch] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
    IMU.gyro[i][Yaw] = IMU_Received[Yaw] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
    IMU.attribute.INS_Gyro[0] = IMU.gyro[i][Roll];
    IMU.attribute.INS_Gyro[1] = IMU.gyro[i][Pitch];
    IMU.attribute.INS_Gyro[2] = IMU.gyro[i][Yaw];

//    IMU.function.IMU_Get_Angle();
}

void IMU_Init(void)
{
    MPU6050_Init();

    IMU.Sample_Start = &IMU_Sample_Start;
    IMU.Sample_Stop = &IMU_Sample_Stop;
    IMU.function.IMU_Get_Angle = IMU_Get_Angle;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Button_Pin){
        log_info("Button IT start\r\n");
    }
    static uint8_t i = 0;
    if (GPIO_Pin == GPIO_PIN_5)
    {

        IMU_Get_Data(i);
        i++;
        if (i >= IMU_SEQUENCE_LENGTH_MAX)
        {
            i = 0;
#ifdef SERIAL_DEBUG
            log_debug("IMU Sampled\r\n");
#endif
            IMU.Sample_Stop();
        }
    }
}
