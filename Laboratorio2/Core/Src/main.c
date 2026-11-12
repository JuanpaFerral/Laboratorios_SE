/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define T_REBOTE_MS 50U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
volatile bool cambio = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
//void Display_SeleccionarDigito(uint8_t digito);
void Display_EscribirPatron(uint8_t);
void Display_Refrescar(uint8_t*);
void cronometrar(uint8_t*, uint8_t*, uint8_t*, uint8_t*);
bool HayNuevaPulsacion(GPIO_TypeDef* PULSADOR,uint16_t PIN, GPIO_PinState* estado_valido);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t cronometro = 0;
	uint8_t centesima = 0;
	uint8_t segundos = 0;
	uint8_t display[4] = {
			0,
			0,
			0,
			0
	};
	GPIO_PinState estado_valido1 = GPIO_PIN_SET;
	GPIO_PinState estado_valido2 = GPIO_PIN_SET;
	bool runtime = false;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(cambio == true)
	  {
		  Display_Refrescar(display);
		  if(HayNuevaPulsacion(SW1_GPIO_Port, SW1_Pin, &estado_valido1))
		  {
			  runtime = !runtime;
		  }
		  if(runtime)
		  {
			  cronometrar(&cronometro, &centesima, &segundos, display);
		  }
		  else if(HayNuevaPulsacion(SW2_GPIO_Port, SW2_Pin, &estado_valido2))
		  {
			  cronometro = 0;
			  centesima = 0;
			  segundos = 0;
			  for(int i = 0 ; i<4 ; i++)
			  {
				  display[i] = 0;
			  }
		  }
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 31999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Bit_0_Pin|Bit_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Seg_b_Pin|Seg_d_Pin|P_decimal_Pin|Seg_c_Pin
                          |Seg_g_Pin|Seg_e_Pin|Seg_a_Pin|Seg_f_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Bit_0_Pin Bit_1_Pin */
  GPIO_InitStruct.Pin = Bit_0_Pin|Bit_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin SW2_Pin */
  GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Seg_b_Pin Seg_d_Pin P_decimal_Pin Seg_c_Pin
                           Seg_g_Pin Seg_e_Pin Seg_a_Pin Seg_f_Pin */
  GPIO_InitStruct.Pin = Seg_b_Pin|Seg_d_Pin|P_decimal_Pin|Seg_c_Pin
                          |Seg_g_Pin|Seg_e_Pin|Seg_a_Pin|Seg_f_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*void Display_SeleccionarDigito(uint8_t digito)
{

}*/
void Display_EscribirPatron(uint8_t patron)
{
	uint8_t valores[10]={
			0x3f, // 0
			0x06, // 1
			0x5b, // 2
			0x4f, // 3
			0x66, // 4
			0x6d, // 5
			0x7d, // 6
			0x07, // 7
			0x7f, // 8
			0x6f  // 9
	};

	if(patron < 10)
	{
		HAL_GPIO_WritePin(Seg_a_GPIO_Port,Seg_a_Pin, (valores[patron] & (1 << 0) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento A
		HAL_GPIO_WritePin(Seg_b_GPIO_Port,Seg_b_Pin, (valores[patron] & (1 << 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento B
		HAL_GPIO_WritePin(Seg_c_GPIO_Port,Seg_c_Pin, (valores[patron] & (1 << 2) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento C
		HAL_GPIO_WritePin(Seg_d_GPIO_Port,Seg_d_Pin, (valores[patron] & (1 << 3) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento D
		HAL_GPIO_WritePin(Seg_e_GPIO_Port,Seg_e_Pin, (valores[patron] & (1 << 4) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento E
		HAL_GPIO_WritePin(Seg_f_GPIO_Port,Seg_f_Pin, (valores[patron] & (1 << 5) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento F
		HAL_GPIO_WritePin(Seg_g_GPIO_Port,Seg_g_Pin, (valores[patron] & (1 << 6) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento G
	}
}
void Display_Refrescar(uint8_t* numeros)
{
	static uint8_t contador = 0;
	Display_EscribirPatron(numeros[contador]);
	if (contador == 2)
	{
		HAL_GPIO_WritePin(P_decimal_GPIO_Port,P_decimal_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(P_decimal_GPIO_Port,P_decimal_Pin,GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(Bit_0_GPIO_Port,Bit_0_Pin, (contador & (1 << 0) ? GPIO_PIN_SET : GPIO_PIN_RESET));
	HAL_GPIO_WritePin(Bit_1_GPIO_Port,Bit_1_Pin, (contador & (1 << 1) ? GPIO_PIN_SET : GPIO_PIN_RESET));
	cambio = false;
	contador++;
	if (contador >= 4)
	{
		contador = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		cambio = true;
	}
}

void cronometrar(uint8_t* cronometro, uint8_t* centesima, uint8_t* segundos, uint8_t* numeros)
{
	(*cronometro)++;
	if((*cronometro) == 5)
	{
		(*centesima)++;
		(*cronometro) = 0;
	}
	if((*centesima) == 100)
	{
		(*segundos)++;
		(*centesima) = 0;
		if((*segundos) == 100)
		{
			(*segundos) = 0;
		}
	}
	numeros[0] = (*centesima) % 10;
	numeros[1] = (*centesima) /10;
	numeros[2] = (*segundos) % 10;
	numeros[3] = (*segundos) /10;
}

bool HayNuevaPulsacion(GPIO_TypeDef* PULSADOR,uint16_t PIN, GPIO_PinState* estado_valido)
{
	GPIO_PinState lectura1;
	GPIO_PinState lectura2;
	bool nueva_pulsacion = false;
	lectura1 = HAL_GPIO_ReadPin(PULSADOR, PIN);
	if(lectura1 != (*estado_valido))
	{
		HAL_Delay(T_REBOTE_MS);
		lectura2 = HAL_GPIO_ReadPin(PULSADOR, PIN);
		if(lectura2 == lectura1)
		{
			GPIO_PinState estado_anterior = *estado_valido;
			*estado_valido = lectura2;
			if((estado_anterior == GPIO_PIN_SET) && (*estado_valido == GPIO_PIN_RESET))
			{
				nueva_pulsacion = true;
			}
		}
	}
	return nueva_pulsacion;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
