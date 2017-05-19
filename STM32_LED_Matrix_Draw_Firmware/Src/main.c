/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//LEDMOD
uint8_t ledmod_image[64] = {
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111
};
uint8_t ledmod_framebuffer[64] = {
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111
};
void LEDMOD_onoff(int mode){
	if(mode == 1){
		HAL_GPIO_WritePin(GPIOB, LEDMOD_OE_Pin, GPIO_PIN_SET);//Rows ON
	}
	else{
		HAL_GPIO_WritePin(GPIOB, LEDMOD_OE_Pin, GPIO_PIN_RESET);//Rows OFF
	}
}
void LEDMOD_shift_byte(int data){
	int i = 0;
	while (i!=8){
		HAL_GPIO_WritePin(GPIOA, LEDMOD_DATA_Pin, ((data >> (7-i)) & 1));
		HAL_GPIO_WritePin(GPIOA, LEDMOD_CLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, LEDMOD_CLK_Pin, GPIO_PIN_RESET);
		i++;
	}
}
void LEDMOD_shift_rows(int address){
	int i = 0;
	while(i != 16){
		LEDMOD_shift_byte(ledmod_framebuffer[address]);
		address++;
		i++;
	}
	HAL_GPIO_WritePin(GPIOA, LEDMOD_SHCLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, LEDMOD_SHCLK_Pin, GPIO_PIN_RESET);
}
void LEDMOD_switch_lines(int num){
	if(num == 1){
		HAL_GPIO_WritePin(GPIOB, LEDMOD_B_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, LEDMOD_A_Pin, GPIO_PIN_RESET);
	}
	else if(num == 2){
		HAL_GPIO_WritePin(GPIOB, LEDMOD_B_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, LEDMOD_A_Pin, GPIO_PIN_SET);
	}
	else if(num == 3){
		HAL_GPIO_WritePin(GPIOB, LEDMOD_B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LEDMOD_A_Pin, GPIO_PIN_RESET);
	}
	else if(num == 4){
		HAL_GPIO_WritePin(GPIOB, LEDMOD_B_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LEDMOD_A_Pin, GPIO_PIN_SET);
	}
}
void LEDMOD_delay(){
	uint32_t p = 0xFF;
	uint32_t s = 0;
	while(s != p){
		asm volatile("nop");
		s++;
	}
}
void LEDMOD_redraw(){
	LEDMOD_onoff(0);
	LEDMOD_shift_rows(0);
	LEDMOD_switch_lines(1);
	LEDMOD_onoff(1);
	LEDMOD_delay();

	LEDMOD_onoff(0);
	LEDMOD_shift_rows(16);
	LEDMOD_switch_lines(2);
	LEDMOD_onoff(1);
	LEDMOD_delay();

	LEDMOD_onoff(0);
	LEDMOD_shift_rows(32);
	LEDMOD_switch_lines(3);
	LEDMOD_onoff(1);
	LEDMOD_delay();

	LEDMOD_onoff(0);
	LEDMOD_shift_rows(48);
	LEDMOD_switch_lines(4);
	LEDMOD_onoff(1);
	LEDMOD_delay();
}
void LEDMOD_redraw_framebuffer(){
	int i = 0;
	int index = 3;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i];
		index += 4;
		i++;
	}
	i = 0;
	index = 19;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+4];
		index += 4;
		i++;
	}
	i = 0;
	index = 35;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+8];
		index += 4;
		i++;
	}
	i = 0;
	index = 51;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+12];
		index += 4;
		i++;
	}

/////////////////

	i = 0;
	index = 2;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16];
		index += 4;
		i++;
	}
	i = 0;
	index = 18;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+4];
		index += 4;
		i++;
	}
	i = 0;
	index = 34;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+8];
		index += 4;
		i++;
	}
	i = 0;
	index = 50;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+12];
		index += 4;
		i++;
	}

/////////////////

	i = 0;
	index = 1;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16];
		index += 4;
		i++;
	}
	i = 0;
	index = 17;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+4];
		index += 4;
		i++;
	}
	i = 0;
	index = 33;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+8];
		index += 4;
		i++;
	}
	i = 0;
	index = 49;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+12];
		index += 4;
		i++;
	}

/////////////////

	i = 0;
	index = 0;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+16];
		index += 4;
		i++;
	}
	i = 0;
	index = 16;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+16+4];
		index += 4;
		i++;
	}
	i = 0;
	index = 32;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+16+8];
		index += 4;
		i++;
	}
	i = 0;
	index = 48;
	while(i != 4){
		ledmod_framebuffer[index] = ledmod_image[i+16+16+16+12];
		index += 4;
		i++;
	}
}

//USB
//#1-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//#2-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//#3-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//#4-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//#S
uint8_t usb_receive_buffer[2048];
uint8_t usb_transmit_command_1[4]= "SET\n";
uint8_t usb_transmit_command_2[7]= "READY\n";
char tmp[3] = "FF";
void USB_copy_buffer_to_ledmod_image(int addr){
	int i = 0;
	while(i!= 32){
		tmp[0] = usb_receive_buffer[i+3];
		tmp[1] = usb_receive_buffer[i+4];
		ledmod_image[(i/2)+addr] = (uint8_t)strtol(tmp, NULL, 16);
		i++;
		i++;
	}
}
void USB_parse(){
	if(usb_receive_buffer[0] == '#'){
		uint8_t ledmod_img_buff_addr = 0;
		if(usb_receive_buffer[1] == '1'){
			ledmod_img_buff_addr = 0;
			USB_copy_buffer_to_ledmod_image(ledmod_img_buff_addr);
			CDC_Transmit_FS(usb_transmit_command_1,4);
		}
		else if(usb_receive_buffer[1] == '2'){
			ledmod_img_buff_addr = 16;
			USB_copy_buffer_to_ledmod_image(ledmod_img_buff_addr);
			CDC_Transmit_FS(usb_transmit_command_1,4);
		}
		else if(usb_receive_buffer[1] == '3'){
			ledmod_img_buff_addr = 32;
			USB_copy_buffer_to_ledmod_image(ledmod_img_buff_addr);
			CDC_Transmit_FS(usb_transmit_command_1,4);
		}
		else if(usb_receive_buffer[1] == '4'){
			ledmod_img_buff_addr = 48;
			USB_copy_buffer_to_ledmod_image(ledmod_img_buff_addr);
			LEDMOD_redraw_framebuffer();
			CDC_Transmit_FS(usb_transmit_command_1,4);
		}
		else if(usb_receive_buffer[1] == 'S'){
			CDC_Transmit_FS(usb_transmit_command_2,6);
		}
	}
}
void USB_enable(int mode){
	if(mode == 1){
		HAL_GPIO_WritePin(GPIOC, USB_EN_Pin, GPIO_PIN_RESET);
	}
	else{
		HAL_GPIO_WritePin(GPIOC, USB_EN_Pin, GPIO_PIN_SET);
	}
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 2 */
  USB_enable(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  LEDMOD_redraw();
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LEDMOD_CLK_Pin|LEDMOD_SHCLK_Pin|LEDMOD_DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LEDMOD_A_Pin|LEDMOD_B_Pin|LEDMOD_OE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LEDMOD_CLK_Pin LEDMOD_SHCLK_Pin LEDMOD_DATA_Pin */
  GPIO_InitStruct.Pin = LEDMOD_CLK_Pin|LEDMOD_SHCLK_Pin|LEDMOD_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDMOD_A_Pin LEDMOD_B_Pin LEDMOD_OE_Pin */
  GPIO_InitStruct.Pin = LEDMOD_A_Pin|LEDMOD_B_Pin|LEDMOD_OE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_EN_Pin */
  GPIO_InitStruct.Pin = USB_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_EN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
