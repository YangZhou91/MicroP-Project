#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "wireless_spi_config.h"
#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_it.h"




void wireless_spi_config(void) 
{
	GPIO_InitTypeDef 	SPI_GPIO_InitStruct;
  SPI_InitTypeDef  	SPI_InitStructure;
	
	// Enable SYSCFG clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SYSCFG, ENABLE);
	
	// Enable the SPI periph 
  RCC_APB1PeriphClockCmd(WIRELESS_SPI_CONFIG_CLK, ENABLE);
	
	// Enable CS  GPIO clock
  RCC_AHB1PeriphClockCmd(WIRELESS_SPI_CONFIG_CS_GPIO_CLK, ENABLE);

  // Enable SCK, MOSI and MISO GPIO clocks 
  RCC_AHB1PeriphClockCmd(WIRELESS_SPI_CONFIG_SCK_GPIO_CLK | WIRELESS_SPI_CONFIG_MISO_GPIO_CLK | WIRELESS_SPI_CONFIG_MOSI_GPIO_CLK, ENABLE);

	// Configure the Alternate Function pins on the Discovery board
  GPIO_PinAFConfig(WIRELESS_SPI_CONFIG_SCK_GPIO_PORT, WIRELESS_SPI_CONFIG_SCK_SOURCE, WIRELESS_SPI_CONFIG_SCK_AF);
  GPIO_PinAFConfig(WIRELESS_SPI_CONFIG_MISO_GPIO_PORT, WIRELESS_SPI_CONFIG_MISO_SOURCE, WIRELESS_SPI_CONFIG_MISO_AF);
  GPIO_PinAFConfig(WIRELESS_SPI_CONFIG_MOSI_GPIO_PORT, WIRELESS_SPI_CONFIG_MOSI_SOURCE, WIRELESS_SPI_CONFIG_MOSI_AF);

	// Configure the GPIO
  SPI_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  SPI_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  SPI_GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
  SPI_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  // SPI SCK pin configuration
  SPI_GPIO_InitStruct.GPIO_Pin = WIRELESS_SPI_CONFIG_SCK_PIN;
  GPIO_Init(WIRELESS_SPI_CONFIG_SCK_GPIO_PORT, &SPI_GPIO_InitStruct);

  // SPI  MOSI pin configuration 
  SPI_GPIO_InitStruct.GPIO_Pin =  WIRELESS_SPI_CONFIG_MOSI_PIN;
  GPIO_Init(WIRELESS_SPI_CONFIG_MOSI_GPIO_PORT, &SPI_GPIO_InitStruct);

  // SPI MISO pin configuration 
  SPI_GPIO_InitStruct.GPIO_Pin = WIRELESS_SPI_CONFIG_MISO_PIN;
  GPIO_Init(WIRELESS_SPI_CONFIG_MISO_GPIO_PORT, &SPI_GPIO_InitStruct);

  // SPI configuration
  SPI_I2S_DeInit(WIRELESS_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(WIRELESS_SPI, &SPI_InitStructure);

  // Enable SPI1 
  SPI_Cmd(WIRELESS_SPI, ENABLE);	
	
	// Configure GPIO PIN for Lis Chip select 
  SPI_GPIO_InitStruct.GPIO_Pin = WIRELESS_SPI_CONFIG_CS_PIN;
  SPI_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  SPI_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  SPI_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(WIRELESS_SPI_CONFIG_CS_GPIO_PORT, &SPI_GPIO_InitStruct);

  // Deselect : Chip Select high 
  //GPIO_SetBits(WIRELESS_SPI_CONFIG_CS_GPIO_PORT, WIRELESS_SPI_CONFIG_CS_PIN);
  WIRELESS_SPI_CS_HIGH();	
}





