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
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dac.h" 

uint16_t Sygnal1 = 0; //ADC1 PB1
uint16_t Sygnal2 = 0; //ADC2 PC#
uint16_t Wynik = 0; //DAC PA4


void _GPIO_Init(void) {
GPIO_InitTypeDef	GPIO_InitStructure;

//Enabling GPIO clock
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

//GPIO PB1 configuration ADC_1	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//GPIO PC3 configuration ADC_2		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//GPIO PA4 configuration DAC_1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, DAC_Channel_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void _TIM_Init(void){
TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

//Enabling TIM clock	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //to moze byc zle
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//TIM1 configuration
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_Period = 49999; // 
	TIM_BaseInitStructure.TIM_Prescaler = 5599; //5600 = 15 kHZ
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0x0000;	
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse	= 49999;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);	

//Enabling TIM	
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
	TIM_Cmd(TIM1, ENABLE);	
  
  TIM_BaseInitStructure.TIM_Prescaler = 5599;;
  TIM_BaseInitStructure.TIM_Period = 1000;
  TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_BaseInitStructure);
// TIM6 TRGO selection 
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
// TIM6 enable counter 
  TIM_Cmd(TIM6, ENABLE);
}
void _ADC_Init(void) {
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
		
//Enabling ADC clock
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

//ADC common init configuration for Multi mode ADC
	ADC_CommonInitStruct.ADC_Mode = ADC_DualMode_RegSimult;
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //ADC_DMAAccessMode_Disabled; ADC_DMAAccessMode_1; ADC_DMAAccessMode_2; ADC_DMAAccessMode_3 
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2; //ADC_Prescaler_Div2; ADC_Prescaler_Div4; ADC_Prescaler_Div6; ADC_Prescaler_Div8
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //ADC_TwoSamplingDelay_5Cycles - i tak dalej po 1 do 20 cykli	
	ADC_CommonInit(&ADC_CommonInitStruct);
	
//ADC1 configuration
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //Timer
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_Init(ADC1,&ADC_InitStructure);
	
//ADC2 configuration
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //Timer
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_Init(ADC2,&ADC_InitStructure);

//Regular channels config
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_144Cycles);

//DMA for Multi mode ADC
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
//Activating continuous mode
	ADC_ContinuousModeCmd(ADC1, ENABLE);
	ADC_ContinuousModeCmd(ADC2, ENABLE);

//Enabling ADC	
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);

	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC2);
}
void _DAC_Init(void) {
	DAC_InitTypeDef  DAC_InitStructure; 

//Enabling DAC clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	
	
//DAC Configuration
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

//Enabling DAC
	DAC_Cmd(DAC_Channel_1, ENABLE);
}
//Starting conversion
//Reading the ADCs converted values
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
int Suma(){

	return Wynik = Sygnal1 + Sygnal2;
}

//Reading the DAC values
int DAC_Convert(){

	DAC_SetChannel1Data(DAC_Align_12b_R, Wynik);
}
int main(void) {
	_GPIO_Init();
	_TIM_Init();
	_ADC_Init();
	_DAC_Init();

// Infinite loop 
  while (1){
  ADC1_Convert();
	ADC2_Convert();
	Suma();
  DAC_Convert();
	}
};
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line) { 
  
  /* Infinite loop */
  while (1) {
  }
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
