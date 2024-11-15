/* Includes ------------------------------------------------------------------*/
#include "bluenrg_lp_com.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "main.h"
#include "OTA_btl.h"

#include "rf_device_it.h"
#include "Beacon_config.h"
#include "rf_driver_hal_vtimer.h"
#include "bleplat.h"
#include "nvm_db.h"
#include "pka_manager.h"
#include "rng_manager.h"
#include "aes_manager.h"
#include "ble_controller.h"

#include "rf_driver_ll_utils.h"
#include "rf_driver_ll_pwr.h"
#include "rf_driver_ll_radio_2g4.h"
#include "rf_driver_ll_rtc.h"
#include "rf_driver_ll_tim.h"


#include "rf_driver_ll_adc.h"
#include "i2c_config.h"
#include "custom_utils.h"
#include "Pentax_IR.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BLE_BEACON_VERSION_STRING "2.1"
#define DISCOVERY_TIMEOUT 3000 /* at least 3 seconds */

/* Private macro -------------------------------------------------------------*/
#ifdef DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
/* Private variables ---------------------------------------------------------*/

extern volatile uint32_t min_period_value;

NO_INIT(uint32_t dyn_alloc_a[DYNAMIC_MEMORY_SIZE>>2]);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ModulesInit(void)
{
  uint8_t ret;
  BLE_STACK_InitTypeDef BLE_STACK_InitParams = BLE_STACK_INIT_PARAMETERS;
  
  LL_AHB_EnableClock(LL_AHB_PERIPH_PKA|LL_AHB_PERIPH_RNG);

  BLECNTR_InitGlobal();
  
  HAL_VTIMER_InitType VTIMER_InitStruct = {780, INITIAL_CALIBRATION, CALIBRATION_INTERVAL};
  HAL_VTIMER_Init(&VTIMER_InitStruct);
  
  BLEPLAT_Init();
  if (PKAMGR_Init() == PKAMGR_ERROR)
  {
      while(1);
  }
  if (RNGMGR_Init() != RNGMGR_SUCCESS)
  {
      while(1);
  }

  /* BlueNRG-LP stack init */
  ret = BLE_STACK_Init(&BLE_STACK_InitParams);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BLE_STACK_Init() 0x%02x\r\n", ret);
    while(1);
  }

}

void ModulesTick(void)
{
  /* Timer tick */
  HAL_VTIMER_Tick();
  
  /* Bluetooth stack tick */
  BLE_STACK_Tick();
  
  /* NVM manager tick */
  NVMDB_Tick();
}

int main(void) 
{
  WakeupSourceConfig_TypeDef wakeupIO;
  PowerSaveLevels stopLevel;

  /* System initialization function */
  //if (SystemInit(SYSCLK_DIRECT_HSE, BLE_SYSCLK_32M) != SUCCESS)
  if (SystemInit(SYSCLK_DIRECT_HSE, BLE_SYSCLK_32M) != SUCCESS)
  {
    /* Error during system clock configuration take appropriate action */
    while(1);
  }

  /* Set systick to 1ms using system clock frequency */
  //LL_Init1msTick(SystemCoreClock);
  LL_Init1msTick( 32000000);


  ModulesInit();


  in_pins_config();
  out_pins_config();
  LL_mDelay(5);

  if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_15))
  {
	  gen_38_khz();
	  pentax_focus();
	  led_blink();
  }
  else if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_14))
  {
	  gen_38_khz();
	  pentax_shutter();
	  led_blink();
	  LL_mDelay(50);
	  led_blink();
	  LL_mDelay(50);
	  led_blink();
  }

  IOS_off();

  //COM_Init(NULL);

 HAL_PWR_MNGR_ShutdownRequest(FALSE);

#if DEBUG_ON == 1
  uint32_t UID_0 = LL_GetUID_Word0();
  uint32_t UID_1 = LL_GetUID_Word1();


  printf("HELLO from Bluenrg-LP ");
  printf("UID_1: %x, UID_0: %x \r\n", UID_1, UID_0);
  LL_mDelay(10);
#endif



  while(1)
  {
  }

}

  /****************** BlueNRG-LP Power Management Callback ********************************/



/* Event used to notify the Host that a hardware failure has occurred in the Controller. 
   See bluenrg_lp_events.h. */
void hci_hardware_error_event(uint8_t Hardware_Code)
{
  if (Hardware_Code <= 0x03)
  {
    NVIC_SystemReset();
  }
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
/** \endcond
 */
