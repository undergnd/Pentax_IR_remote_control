/*
 * main.h
 *
 *  Created on: 18 lut 2024
 *      Author: gbednarski
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stdint.h"
#include "stdio.h"
#include "timers.h"


//#define DEBUG
#define DEBUG_ON 0
#define POWER_SAVE 1
#define MAX_BITS_NO 	12
#define INT_UART_RX_EXTI_LINE              		LL_EXTI_LINE_PB0

typedef enum
{
	BLE_RAW = 0,
	BLE_STACK  = 1
}ble_stack_raw;


typedef enum
{
	ADV_DEBUG = 0,
	ADV_FMB  = 1
}ble_adv_type_message;

/* Private macro -------------------------------------------------------------*/

#define PRINT_INT(x)    ((int)(x))
#define PRINT_FLOAT_1000(x)  (x>0)? ((int) (((x) - PRINT_INT(x)) * 1000)) : (-1*(((int) (((x) - PRINT_INT(x)) * 1000)))) // trzy miejsca po przecinku
#define PRINT_FLOAT(x)  (x>0)? ((int) (((x) - PRINT_INT(x)) * 100)) : (-1*(((int) (((x) - PRINT_INT(x)) * 100)))) // dwa miejsca po przecinku

#define POWER_HIGH_LOW   1 //1
#define POWER_LEVEL_ACI  31 //24


void ModulesTick(void);
void BlueNRG_reset(void );

#endif /* INC_MAIN_H_ */
