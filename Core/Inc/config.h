#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "main.h"                  // Device header

// Define running mode.
// NOTE: You can change the wand into different mode by the following defines.
//      When you change into another mode,you should comment the define of previous mode. 
// CYBERRY_POTTER_MODE_NORMAL: Normal mode.
// CYBERRY_POTTER_MODE_DATA_COLLECTOR: Running at data collecting mode to collect 
//      data from IMU and then send those data to serial port.Those data are used
//      to training motion identify model.

//Define system configuration

#define SERIAL_DEBUG                  //通过输出调试信息与运行结果(Xshell或者其他上位机平台显示)
#define PC_SHOW                       //输出运行结果至上位机脚本显示对应画面(CNN/PC_Action/action.py)
#define SYSTEM_MODE_DATA_COLLECT      //采集动作数据(CNN/Serial_Read.py)
#define VOFA_SHOW                       //输出至Vofa+上位机显示波形图和输入至matlab进行时域频域分析(Matlab_File/Lowpass_Show.m)

//Define ADC Module Detect
#define MODULE0_LOWER_LIM 186
#define MODULE0_UPPER_LIM 555
#define MODULE1_LOWER_LIM 555
#define MODULE1_UPPER_LIM 865
#define MODULE2_LOWER_LIM 865
#define MODULE2_UPPER_LIM 1166
#define MODULE3_LOWER_LIM 1166
#define MODULE3_UPPER_LIM 1460
#define MODULE4_LOWER_LIM 1460
#define MODULE4_UPPER_LIM 1815
#define MODULE5_LOWER_LIM 1815
#define MODULE5_UPPER_LIM 2281
#define MODULE6_LOWER_LIM 2281
#define MODULE6_UPPER_LIM 2636
#define MODULE7_LOWER_LIM 2636
#define MODULE7_UPPER_LIM 2930
#define MODULE8_LOWER_LIM 2930
#define MODULE8_UPPER_LIM 3231
#define MODULE9_LOWER_LIM 3231
#define MODULE9_UPPER_LIM 3541
#define MODULE10_LOWER_LIM 3541
#define MODULE10_UPPER_LIM 3910

//Define External ROM
#define W25Q64_PAGE_SIZE 256
#define W25Q64_SECTOR_ADDRESS_INCREMENT 0x0001000
#define W25Q64_SIGNAL_TYPE_INCREMENT 0x000C000

//Define some useful constant

//Define CNN
//The out put of model must bigger than this value unless it will give a unrecognized message

#endif  //_CONFIG_H_
