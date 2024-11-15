/*
 * Pentax_IR.h
 *
 *  Created on: 11 lis 2024
 *      Author: gbednarski
 */

#ifndef INC_PENTAX_IR_H_
#define INC_PENTAX_IR_H_

#include "stdint.h"

#include "rf_driver_ll_rcc.h"
#include "rf_driver_ll_bus.h"
#include "rf_driver_ll_system.h"
#include "rf_driver_ll_exti.h"
#include "rf_driver_ll_cortex.h"
#include "rf_driver_ll_utils.h"
#include "rf_driver_ll_pwr.h"
#include "rf_driver_ll_dma.h"
#include "rf_driver_ll_tim.h"
#include "rf_driver_ll_gpio.h"
#if defined(CONFIG_DEVICE_BLUENRG_LP) || defined(CONFIG_DEVICE_BLUENRG_LPS)
#include "bluenrg_lpx.h"
#endif


void led_blink(void);
void gen_38_khz(void);
void pentax_shutter(void);
void pentax_focus(void);
void in_pins_config(void);
void IOS_off(void);
void out_pins_config(void);
void BSP_IO_Init(void);

/* TIMx interrupt processing */
void TimerCaptureCompare_Callback(void);

#define TIMx                                      TIM1
#define LL_EnableClock_TIMx()                     LL_APB0_EnableClock(LL_APB0_PERIPH_TIM1)
#define LL_EnableClock_TIMx_CH1()                 LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA)
#define LL_DisableClock_TIMx()                    LL_APB0_DisableClock(LL_APB0_PERIPH_TIM1)
#define LL_DisableClock_TIMx_CH1()                LL_AHB_DisableClock(LL_AHB_PERIPH_GPIOA)
#define TIMx_IRQHandler                           TIM1_IRQHandler
#define TIMx_IRQn                                 TIM1_IRQn
#define LL_GPIO_SetAFPin_TIMx_CH1()               LL_GPIO_SetAFPin_0_7(TIMx_CH1_PORT, TIMx_CH1_PIN, TIMx_CH1_AF);

  /**TIM1 GPIO Configuration
  PA4 / AF4   ------> TIM1_CH1
  */
#define TIMx_CH1_PIN                              LL_GPIO_PIN_4
#define TIMx_CH1_AF                               LL_GPIO_AF_4
#define TIMx_CH1_PORT                             GPIOA


#endif /* INC_PENTAX_IR_H_ */
