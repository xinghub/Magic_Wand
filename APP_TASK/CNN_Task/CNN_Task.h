//
// Created by The Fool on 2025/1/15.
//

#ifndef MZ_CNN_PROJ_CNN_TASK_H
#define MZ_CNN_PROJ_CNN_TASK_H
#include "main.h"
#include "nnom.h"
typedef int8_t Model_Output_t;
typedef uint32_t ROM_Address_t;
#define QUANTIFICATION_SCALE (pow(2,INPUT_1_OUTPUT_DEC))
#define OUPUT_THRESHOLD 63 //The out put of model must bigger than this value unless the out put would be unrecognized.

typedef enum eModel_Output{
    Unrecognized = -1,
    RightAngle = 0,
    SharpAngle = 1,
    Lightning = 2,
    Triangle = 3,
    Letter_h = 4,
    letter_R = 5,
    letter_W = 6,
    letter_phi = 7,
    Circle = 8,
    UpAndDown = 9,
    Horn = 10,
    Wave = 11,
    NoMotion = 12
}eModel_Output;

void CNN_Create(void);
Model_Output_t model_get_output(void);
//extern nnom_model_t* model;
#endif //MZ_CNN_PROJ_CNN_TASK_H
