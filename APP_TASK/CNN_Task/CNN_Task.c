//
// Created by The Fool on 2025/1/15.
//

#include "CNN_Task.h"
#include "weights.h"
#include "IMU_Task.h"

#ifdef NNOM_USING_STATIC_MEMORY
uint8_t static_buf[1024 * 6];
#endif //NNOM_USING_STATIC_MEMORY
static nnom_model_t* model;
/*
 * @brief Quantirize IMU data and feed to CNN model
 * @param None
 * @return None
 */
void model_feed_data(void)
{
    const double scale = QUANTIFICATION_SCALE;
    uint16_t i = 0;
    for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
        nnom_input_data[i*3] = (int8_t)round(IMU_Handle->attribute.gyro[i][Roll] * scale);
        nnom_input_data[i*3+1] = (int8_t)round(IMU_Handle->attribute.gyro[i][Pitch] * scale);
        nnom_input_data[i*3+2] = (int8_t)round(IMU_Handle->attribute.gyro[i][Yaw] * scale);
    }
}

//void model_feed_data(void)
//{
//	const double scale = QUANTIFICATION_SCALE;
//	uint16_t i = 0;
//	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
//		nnom_input_data[i*6] = 	 (int8_t)round(IMU.acc[i][AccX] * scale);
//		nnom_input_data[i*6+1] = (int8_t)round(IMU.acc[i][AccY] * scale);
//		nnom_input_data[i*6+2] = (int8_t)round(IMU.acc[i][AccZ] * scale);
//		nnom_input_data[i*6+3] = (int8_t)round(IMU.gyro[i][Roll] * scale);
//		nnom_input_data[i*6+4] = (int8_t)round(IMU.gyro[i][Pitch] * scale);
//		nnom_input_data[i*6+5] = (int8_t)round(IMU.gyro[i][Yaw] * scale);
//	}
//}

/*
 * @brief get output of CNN model
 * @param  None
 * @return Model_Output_t: type of recognized motion
 */
Model_Output_t model_get_output(void)
{
    volatile uint8_t i = 0;
    volatile Model_Output_t max_output = -128;
    Model_Output_t ret = 0;
    model_feed_data();
    model_run(model);
    for(i = 0; i < 13; i++){
        if(nnom_output_data[i] >= max_output){
            max_output = nnom_output_data[i] ;
            ret = i;
        }
#ifdef SERIAL_DEBUG
            log_info("Output[%d] = %.2f %%\r\n",i,(nnom_output_data[i] / 127.0)*100);
#endif //SERIAL_DEBUG
    }
    if(max_output < OUPUT_THRESHOLD || ret == NoMotion){
        ret = Unrecognized;
    }

#ifdef SERIAL_DEBUG
//    printf("\033[31;40;1m it's red \033[0m");
    log_info("CNN recognize result:\033[40;32;1m");
    switch(ret){
        case Unrecognized:
            printf("Unrecognized");
            break;
        case RightAngle:
            printf("RightAngle");
            break;
        case SharpAngle:
            printf("SharpAngle");
            break;
        case Lightning:
            printf("Lightning");
            break;
        case Triangle:
            printf("Triangle");
            break;
        case Letter_h:
            printf("Letter_h");
            break;
        case letter_R:
            printf("Letter_R");
            break;
        case letter_W:
            printf("Letter_W");
            break;
        case letter_phi:
            printf("Letter_phi");
            break;
        case Circle:
            printf("Circle");
            break;
        case UpAndDown:
            printf("UpAndDown");
            break;
        case Horn:
            printf("Horn");
            break;
        case Wave:
            printf("Wave");
            break;
        case NoMotion:
            printf("NoMotion");
            break;
    }
    printf("\033[0m\r\n");
    printf("\n");

#endif //SERIAL_DEBUG
    return ret;
}
/**
* @CreateTime 
* @Author 
* @brief 
* @param val1 
* @param val2
* @return 
*/
void CNN_Create(void)
{
#ifdef NNOM_USING_STATIC_MEMORY
    nnom_set_static_buf(static_buf, sizeof(static_buf));
#endif //NNOM_USING_STATIC_MEMORY
    model = nnom_model_create();
}