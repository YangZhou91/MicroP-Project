

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _WIRELESS_SPI_CONFIG
#define _WIRELESS_SPI_CONFIG

/* Defines -------------------------------------------------------------------*/
#define WIRELESS_SPI_CS_LOW()       			  	 GPIO_ResetBits(WIRELESS_SPI_CONFIG_CS_GPIO_PORT, WIRELESS_SPI_CONFIG_CS_PIN)
#define WIRELESS_SPI_CS_HIGH()      					 GPIO_SetBits(WIRELESS_SPI_CONFIG_CS_GPIO_PORT, WIRELESS_SPI_CONFIG_CS_PIN)

#define WIRELESS_SPI                              SPI2
#define WIRELESS_SPI_CONFIG_CLK                   RCC_APB1Periph_SPI2

#define WIRELESS_SPI_CONFIG_SCK_PIN               GPIO_Pin_13                  /* PA.05 */
#define WIRELESS_SPI_CONFIG_SCK_GPIO_PORT         GPIOB                       /* GPIOA */
#define WIRELESS_SPI_CONFIG_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define WIRELESS_SPI_CONFIG_SCK_SOURCE            GPIO_PinSource13
#define WIRELESS_SPI_CONFIG_SCK_AF                GPIO_AF_SPI2

#define WIRELESS_SPI_CONFIG_GDO0_PIN									 GPIO_Pin_3									/* PA.03 */
#define WIRELESS_SPI_CONFIG_GDO0_GPIO_PORT						 GPIOA											/* GPIOA */
#define WIRELESS_SPI_CONFIG_GDO0_GPIO_CLK             RCC_AHB1Periph_GPIOA

#define WIRELESS_SPI_CONFIG_MISO_PIN              GPIO_Pin_14                  /* PA.6 */
#define WIRELESS_SPI_CONFIG_MISO_GPIO_PORT        GPIOB                       /* GPIOA */
#define WIRELESS_SPI_CONFIG_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define WIRELESS_SPI_CONFIG_MISO_SOURCE           GPIO_PinSource14
#define WIRELESS_SPI_CONFIG_MISO_AF               GPIO_AF_SPI2

#define WIRELESS_SPI_CONFIG_MOSI_PIN              GPIO_Pin_15                  /* PA.7 */
#define WIRELESS_SPI_CONFIG_MOSI_GPIO_PORT        GPIOB                       /* GPIOA */
#define WIRELESS_SPI_CONFIG_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define WIRELESS_SPI_CONFIG_MOSI_SOURCE           GPIO_PinSource15
#define WIRELESS_SPI_CONFIG_MOSI_AF               GPIO_AF_SPI2

//#define WIRELESS_SPI_CONFIG_CS_PIN                GPIO_Pin_0                  /* PB.0 */
//#define WIRELESS_SPI_CONFIG_CS_GPIO_PORT          GPIOB                       /* GPIOB */
//#define WIRELESS_SPI_CONFIG_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define WIRELESS_SPI_CONFIG_CS_PIN                GPIO_Pin_8                  /* PB.0 */
#define WIRELESS_SPI_CONFIG_CS_GPIO_PORT          GPIOA                       /* GPIOB */
#define WIRELESS_SPI_CONFIG_CS_GPIO_CLK           RCC_AHB1Periph_GPIOA

#define WIRELESS_SPI_FLAG_TIMEOUT         ((uint32_t)0x1000)

void wireless_spi_config(void);
//uint8_t WIRELESS_SPI_SendByte(uint8_t byte);
//uint8_t WIRELESS_SPI_SendByte(uint8_t byte);
//uint32_t WIRELESS_SPI_TIMEOUT_UserCallback(void);

#endif
