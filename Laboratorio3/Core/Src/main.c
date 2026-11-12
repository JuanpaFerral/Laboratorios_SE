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
#include "uart.h"
#include "contador_display.h"
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
volatile bool cambio = false;
volatile bool runtime = false;
volatile bool sw1 = false;
volatile bool sw2 = false;
volatile uint32_t last_sw1 = 0;
volatile uint32_t last_sw2 = 0;
volatile uint8_t indice = 0;
volatile uint8_t instruccion[10];
volatile bool fin_instruccion = false;
volatile bool fin_cronometro = false;
int segundos_recibidos = 0;
uint8_t mensaje;
uint8_t cronometro = 0;
uint8_t centesima = 0;
uint8_t segundos = 0;
uint8_t minutos = 0;
uint8_t alarma[] = "ALARMA\n";
uint8_t display[4] = {
		0,
		0,
		0,
		0
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_UART4_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
//bool HayNuevaPulsacion(GPIO_TypeDef* PULSADOR,uint16_t PIN, GPIO_PinState* estado_valido);
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
	//GPIO_PinState estado_valido1 = GPIO_PIN_SET;
	//GPIO_PinState estado_valido2 = GPIO_PIN_SET;

	uint32_t current_time1 = 0;
	uint32_t current_time2 = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  __HAL_UART_CLEAR_OREFLAG(&huart4);
  __HAL_UART_FLUSH_DRREGISTER(&huart4);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_UART4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_UART_Receive_IT(&huart4, &mensaje, sizeof(uint8_t));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		if(fin_instruccion)
		{
			ejecutar_instruccion();
		}
		if(sw1)
		{
			current_time1 = HAL_GetTick();
			if(current_time1-last_sw1 > T_REBOTE_MS)
			{
				if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
				{
					if(runtime)
					{
						start_pause(2);
					}
					else
					{
						start_pause(1);
					}
				}
				sw1 = false;
				__HAL_GPIO_EXTI_CLEAR_IT(SW1_Pin);
				HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
				HAL_NVIC_EnableIRQ(EXTI2_IRQn);
			}
		}
		if(sw2)
		{
			current_time2 = HAL_GetTick();
			if(current_time2-last_sw2 > T_REBOTE_MS)
			{
				if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET && runtime == false)
				{
					restart();
				}
				sw2 = false;
				__HAL_GPIO_EXTI_CLEAR_IT(SW2_Pin);
				HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
			}
		}
    /* USER CODE END WHILE */

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 7999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*void Display_SeleccionarDigito(uint8_t digito)
{

}*/

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart4)
	{
		if(mensaje == '\0')
		{
			instruccion[indice] = mensaje;
			fin_instruccion = true;
			indice = 0;
		}
		else if(indice < (sizeof(instruccion)-1))
		{
			instruccion[indice++] = mensaje;
		}
		HAL_UART_Receive_IT(&huart4, &mensaje, sizeof(uint8_t));
	}
}

void ejecutar_instruccion()
{
	int8_t num_instrc = -1;
	uint8_t error[] = "ERROR\n";
	uint8_t ok[] = "OK\n";
	uint8_t run[] = "RUNNING\n";
	uint8_t pause[] = "PAUSE\n";
	uint8_t estado[7];
	if(sscanf((char*)instruccion, "SET %d", &segundos_recibidos)==1)
	{
		if(segundos_recibidos >= 0 && segundos_recibidos<= 5999)
		{
			num_instrc = 0;
		}
	}
	else
	{
		num_instrc = (strcmp((char*)instruccion, "START")==0) ? 1 :
				     (strcmp((char*)instruccion, "PAUSE")==0) ? 2 :
				     (strcmp((char*)instruccion, "RESET")==0) ? 3 :
				     (strcmp((char*)instruccion, "STATUS")==0) ? 4 : -1;
	}
	switch (num_instrc) {
		case 0:
			if(!runtime)
			{
				HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
				minutos = segundos_recibidos/60;
				segundos = segundos_recibidos%60;
				display[0] = segundos % 10;
				display[1] = segundos /10;
				display[2] = minutos % 10;
				display[3] = minutos /10;
			}
			else
			{
				HAL_UART_Transmit(&huart4, error, strlen((char*)error), 100);
			}
			fin_instruccion = false;
			break;
		case 1:
			HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
			runtime = true;
			fin_instruccion = false;
			break;
		case 2:
			HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
			runtime = false;
			fin_instruccion = false;
			break;
		case 3:
			HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
			runtime = false;
			minutos = segundos_recibidos/60;
			segundos = segundos_recibidos%60;
			display[0] = segundos % 10;
			display[1] = segundos /10;
			display[2] = minutos % 10;
			display[3] = minutos /10;
			if(fin_cronometro)
			{
				fin_cronometro = false;
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
			}
			fin_instruccion = false;
			break;
		case 4:
			display_estado(estado);
			if(runtime)
			{
				HAL_UART_Transmit(&huart4, run, strlen((char*)run), 100);
				HAL_UART_Transmit(&huart4, estado, strlen((char*)estado), 100);
			}
			else
			{
				HAL_UART_Transmit(&huart4, pause, strlen((char*)pause), 100);
				HAL_UART_Transmit(&huart4, estado, strlen((char*)estado), 100);
			}
			fin_instruccion = false;
			break;
		case -1:
			HAL_UART_Transmit(&huart4, error, strlen((char*)error), 100);
			fin_instruccion = false;
			break;
		default:
			break;
	}
}*/

/*void display_estado(uint8_t* estado)
{
	estado[0] = display[3] + '0';
	estado[1] = display[2] + '0';
	estado[2] = '.';
	estado[3] = display[1] + '0';
	estado[4] = display[0] + '0';
	estado[5] = '\n';
	estado[6] = '\0';
}*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == SW1_Pin)
	{
		HAL_NVIC_DisableIRQ(EXTI2_IRQn);
		last_sw1 = HAL_GetTick();
		sw1 = true;
	}
	if(GPIO_Pin == SW2_Pin)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		last_sw2 = HAL_GetTick();
		sw2 = true;
	}
}



/*void Display_EscribirPatron(uint8_t patron)
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
		Display_Refrescar(display);
		if(runtime)
		{
			cronometrar();
		}
	}

}

void cronometrar()
{
	cronometro++;
	if(cronometro == 5)
	{
		centesima++;
		cronometro = 0;
	}
	if(centesima == 100)
	{
		centesima = 0;
		if(segundos > 0)
		{
			segundos--;
		}
		else
		{
			if(minutos > 0)
			{
				minutos--;
				segundos = 59;
			}
			else
			{
				runtime = false;
				fin_cronometro = true;
				HAL_UART_Transmit(&huart4, alarma, strlen((char*)alarma), 100);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
			}
		}
	}
	display[0] = segundos % 10;
	display[1] = segundos /10;
	display[2] = minutos % 10;
	display[3] = minutos /10;
}*/

/*bool HayNuevaPulsacion(GPIO_TypeDef* PULSADOR,uint16_t PIN, GPIO_PinState* estado_valido)
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
}*/

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
