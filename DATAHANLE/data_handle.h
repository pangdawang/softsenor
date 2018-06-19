/******************** (C) COPYRIGHT 2018 SOFT_SENSE PROJECT **********************************
 * File Name          : Data_handle.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/03/28
 * Description        : This file is sample data handle
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
  * 2018/03/28 | v1.0  | panda_wang     | AD采样数据处理函数头文件
 *******************************************************************************/
 #ifndef DATA_HANDLE_H_
#define DATA_HANDLE_H_

/*---------------include------------*/
#include "sys.h" 
#include "math.h"
// #include "stm32f10x.h"
#include "stdint.h"
#include "malloc.h"
#define RETURN_IS_NOERR 0
#define RETURN_IS_ERR  1


/*---------declartion------*/
void  data_peak_handle(uint16_t (*data_ptr)[8],uint8_t row_num,uint8_t column_num,uint16_t * ppeak_value_ptr,uint8_t (*subcript)[2]);
uint16_t data_average_handle(uint16_t * data_ptr,uint8_t data_num);
uint16_t data_recur_average_handle(uint16_t * data_ptr,uint8_t data_num);

#endif

