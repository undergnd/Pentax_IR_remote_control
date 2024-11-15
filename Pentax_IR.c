/*
 * Pentax_IR.c
 *
 *  Created on: 11 lis 2024
 *      Author: gbednarski
 */


#include "Pentax_IR.h"
#include "rf_driver_hal_power_manager.h"
#include "rf_driver_ll_gpio.h"
#include "rf_driver_ll_exti.h"
#include "rf_driver_ll_bus.h"
#include "system_BlueNRG_LP.h"
#include "rf_driver_ll_utils.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "rf_driver_ll_tim.h"
#include "bluenrg_lp_com.h"
#include "main.h"

#define TIM_DUTY_CYCLES_NB 11
#define TIMx                       TIM1
#define LL_EnableClock_TIMx()      LL_APB0_EnableClock(LL_APB0_PERIPH_TIM1);
void pulse_on(void);
void pulse_off(void);
void Pentax_38_kHz_pulse(uint16_t period);
void Pentax_38_kHz_delay(uint16_t period);

/* TIMx Clock */
uint32_t timxPrescaler = 0;
uint32_t timxPeriod = 0;

/* Measured duty cycle */
__IO uint32_t uwMeasuredDutyCycle = 0;

void out_pins_config(void);
static void MX_TIM1_Init(void);


/*
 *@brief Pulses sequence for camera shutter
 *@param none
 *@retval none
 */
void pentax_shutter(void)
{
	uint8_t i;
	Pentax_38_kHz_pulse(13); // 13200 us
	Pentax_38_kHz_delay(3); // 3200 us
	for (i = 0;i < 7;i++) {
		Pentax_38_kHz_pulse(1);
		Pentax_38_kHz_delay(1);
	}
}


/*
 *@brief Pulses sequence for camera focus
 *@param none
 *@retval none
 */
void pentax_focus(void)
{
	uint8_t i;
	Pentax_38_kHz_pulse(13); // 13200 us
	Pentax_38_kHz_delay(3); // 3200 us
	for (i = 0;i < 5; i++) {
		Pentax_38_kHz_pulse(1);
		Pentax_38_kHz_delay(1);
	}
	Pentax_38_kHz_pulse(1);
	Pentax_38_kHz_delay(3); //3200 us
	Pentax_38_kHz_pulse(1);
	Pentax_38_kHz_delay(1);
}

void Pentax_38_kHz_pulse(uint16_t period)
{
	pulse_on();
	LL_mDelay(period - 1);
}

void Pentax_38_kHz_delay(uint16_t period)
{
	pulse_off();
	LL_mDelay(period - 1);

}

void pulse_on(void)
{
	  /* Enable counter */
	LL_TIM_EnableCounter(TIMx);

}

void pulse_off(void)
{
  /* Disable counter */
  LL_TIM_DisableCounter(TIMx);
}

/*
 *@brief Test GPIO pin setting - just LED blinking
 *@param none
 *@retval none
 */
void led_blink(void)
{
	uint16_t t_on = 1;


	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_mDelay(t_on);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);

/*
	for(uint8_t k = 0; k < 10; k++)
	{
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
		LL_mDelay(t_on);
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
		LL_mDelay(t_off);
	}
*/
}


/*
 *@brief Test GPIO pin setting
 *@param none
 *@retval none
 */
void out_pins_config(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);

	/* Configure GPIO for INT */
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/*
 *@brief Test GPIO pin setting
 *@param none
 *@retval none
 */
void in_pins_config(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);

	/* Configure GPIO for INT */
	//LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);
	//LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_14);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void IOS_off(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOA);

	/* Configure GPIO for INT */
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	BSP_IO_Init();

	LL_AHB_DisableClock(LL_AHB_PERIPH_GPIOA);
	LL_GPIO_DeInit(GPIOA);

	/*Disabling Timer*/
	LL_TIM_DisableCounter(TIMx);
	LL_TIM_DisableAllOutputs(TIMx);
	LL_TIM_CC_DisableChannel(TIMx, LL_TIM_CHANNEL_CH1);
	LL_TIM_DeInit(TIMx);
    NVIC_DisableIRQ(TIMx_IRQn);


}



void gen_38_khz(void)
{
  /* - Set the pre-scaler value to have TIMx counter clock equal to ... kHz  */
  /* - Set the auto-reload value to have a counter frequency of 38 kHz        */
  timxPrescaler = __LL_TIM_CALC_PSC(LL_TIM_GetPeriphClock(TIMx), 76000);
  timxPeriod = __LL_TIM_CALC_ARR(LL_TIM_GetPeriphClock(TIMx), timxPrescaler, 38000);

  MX_TIM1_Init();
}


/**
  * @brief TIMx Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

	  LL_TIM_InitTypeDef TIM_InitStruct = {0};
	  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* Peripheral clock enable */
	  LL_EnableClock_TIMx();
	  LL_EnableClock_TIMx_CH1();

	  GPIO_InitStruct.Pin = TIMx_CH1_PIN;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	  GPIO_InitStruct.Alternate = TIMx_CH1_AF;
	  LL_GPIO_Init(TIMx_CH1_PORT, &GPIO_InitStruct);

	  /* TIMx interrupt Init */
	  NVIC_SetPriority(TIMx_IRQn, IRQ_MED_PRIORITY);
	  NVIC_EnableIRQ(TIMx_IRQn);

	  TIM_InitStruct.Prescaler = timxPrescaler;
	  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	  TIM_InitStruct.Autoreload = timxPeriod;
	  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	  TIM_InitStruct.RepetitionCounter = 0;
	  LL_TIM_Init(TIMx, &TIM_InitStruct);
	  LL_TIM_EnableARRPreload(TIMx);
	  LL_TIM_OC_EnablePreload(TIMx, LL_TIM_CHANNEL_CH1);
	  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	  TIM_OC_InitStruct.CompareValue = ((timxPeriod + 1 ) / 2);
	  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
	  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_HIGH;
	  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	  LL_TIM_OC_Init(TIMx, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
	  LL_TIM_OC_DisableFast(TIMx, LL_TIM_CHANNEL_CH1);


	  /* Enable output channel 1 */
	  LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH1);

	  /* Enable the outputs (set the MOE bit in TIMx_BDTR register). */
	  LL_TIM_EnableAllOutputs(TIMx);

	  /* Enable counter */
	  //LL_TIM_EnableCounter(TIMx);

	  /* Force update generation */
	  LL_TIM_GenerateEvent_UPDATE(TIMx);

#if DEBUG_ON == 1
	  printf("TIM1 seems to be configured \n\r");
#endif

}

/**
  * @brief  Timer capture/compare interrupt processing
  * @param  None
  * @retval None
  */
void TimerCaptureCompare_Callback(void)
{
  uint32_t CNT, ARR;
  CNT = LL_TIM_GetCounter(TIMx);
  ARR = LL_TIM_GetAutoReload(TIMx);

  if (LL_TIM_OC_GetCompareCH1(TIMx) > ARR )
  {
    /* If capture/compare setting is greater than autoreload, there is a counter overflow and counter restarts from 0.
       Need to add full period to counter value (ARR+1)  */
    CNT = CNT + ARR + 1;
  }
  uwMeasuredDutyCycle = (CNT * 100) / ( ARR + 1 );
}


void BSP_IO_Init(void)
{

	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO0);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO1);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO2);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO3);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO4);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO5);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO6);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO7);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO8);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO9);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO10);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO11);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO12);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO13);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO14);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO15);

	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO0);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO1);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO2);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO3);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO4);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO5);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO6);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO7);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO8);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO9);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO10);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO11);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO12);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO13);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO14);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO15);

	//  LL_PWR_EnablePUPDCfg();

/*
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO0);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO1);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO2);
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO3);

	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO4);  //INT1 from acc
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO5);  //SPI2_SCK
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO6);  //SPI2_MOSI
	  LL_PWR_EnablePUA(LL_PWR_PUPD_IO7);  //SPI2_MISO
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO8);  //ACC CS
	  LL_PWR_EnablePUA(LL_PWR_PUPD_IO9);  //INT1 from acc
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO10); //ACC power
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO11); //sensors GND

	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO12); //TH INT
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO13); //I2C SCL
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO14); //I2C SDA
	  LL_PWR_EnablePDA(LL_PWR_PUPD_IO15); //TH VCC

	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO0);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO1);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO2);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO3);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO4);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO5);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO6);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO7);
	  LL_PWR_EnablePUB(LL_PWR_PUPD_IO8);
	  LL_PWR_EnablePUB(LL_PWR_PUPD_IO9);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO10);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO11);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO14);
	  LL_PWR_EnablePDB(LL_PWR_PUPD_IO15);

	  LL_PWR_EnablePUPDCfg();
*/
}

