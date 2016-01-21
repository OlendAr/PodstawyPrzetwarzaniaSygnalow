/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  
  * @version 
  * @date    06-March-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dac.h" 

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
uint16_t Sygnal1;
uint16_t Sygnal2;
uint16_t Wynik; //tablica wynikowa do obliczen 

void _GPIO_Init(void) {
GPIO_InitTypeDef	GPIO_InitStructure;

/*Enabling GPIO clock*/	
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

/*GPIO PB1 configuration ADC_1*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

/*GPIO PA5 configuration ADC_2*/		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*GPIO PA5, PA4 configuration DAC_1, DAC_2*/	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, DAC_Channel_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, DAC_Channel_2);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5/ GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void _NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

 //Configure and enable ADC interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void _ADC_Init(void) {
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
		
/*Enabling ADC clock*/
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
/*ADC1 configuration*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //Timer
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_Init(ADC1,&ADC_InitStructure);
	
/*ADC2 configuration*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //Timer
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_Init(ADC2,&ADC_InitStructure);

/*ADC common init configuration for Multi mode ADC*/
	ADC_CommonInitStruct.ADC_Mode = ADC_DualMode_RegSimult;
	ADC_CommonInitStruct.ADC_DMAAccessMode = //ADC_DMAAccessMode_Disabled; ADC_DMAAccessMode_1; ADC_DMAAccessMode_2; ADC_DMAAccessMode_3 
	ADC_CommonInitStruct.ADC_Prescaler = //ADC_Prescaler_Div2; ADC_Prescaler_Div4; ADC_Prescaler_Div6; ADC_Prescaler_Div8
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //ADC_TwoSamplingDelay_5Cycles - i tak dalej po 1 do 20 cykli	

/*Enabling ADC*/	
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);

/*Regular channels config*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_144Cycles);

/*Activating continuous mode*/
	ADC_ContinuousModeCmd(ADC1, ENABLE);
	ADC_ContinuousModeCmd(ADC2, ENABLE);

/*DMA for Multi mode ADC*/
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
}
void _DAC_Init(void) {
	DAC_InitTypeDef  DAC_InitStructure; 

/*Enabling DAC clock*/	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	
	
/*DAC Configuration*/	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	/*Enabling DAC*/  
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);
}
/* JAK Z DMA TO NVIC OK KONCA BUFFORA, JAK NIE TO OD KONCA KONWERSJI

void _DMA_Init(void) {
	DMA_InitTypeDef  DMA_InitStructure;
	
	Enabling DMA clock
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA Configuration
	DMA_InitStructure.DMA_Channel = DMA_Channel_1; // wybor kanalu 
  DMA_InitStructure.DMA_PeripheralBaseAddr = ????;	 // urzadzenie 
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint16_t)&Wynik; // pamiec 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // z urzadzenia przez DMA do pamieci 
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // inkrementacja wylaczona 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; // inkrementacja wylaczona 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // wielkosc danych 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // wielkosc danych 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // praca w petli 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; // okreslenie pierwszenstwa dla strumienia 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; // okreslenie metody przekazywania danych dla strumienia       
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
	Inicjalizacja DMA 	
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);

}
*/
/*Starting conversion*/
/*Reading the ADCs converted values*/	
	int ADC1_Convert(){
		ADC_SoftwareStartConv(ADC1);
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		return Sygnal1 = ADC_GetConversionValue(ADC1);
	}
	int ADC2_Convert(){
		ADC_SoftwareStartConv(ADC2);
		while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));
		return Sygnal2 = ADC_GetConversionValue(ADC2);
	}
/*Reading the DAC values*/
int DAC_Convert(){
	DAC_SetChannel1Data(DAC_Align_12b_R, Sygnal1);
	DAC_SetChannel2Data(DAC_Align_12b_R, Sygnal2);
}
int main(void) {
	_GPIO_Init();
	_ADC_Init();
	_DAC_Init();

/* Infinite loop */
  while (1)
  ADC1_Convert();
	ADC2_Convert();
	DAC_Convert();
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line) { 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
