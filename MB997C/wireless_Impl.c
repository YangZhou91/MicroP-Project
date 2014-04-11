#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "wireless_spi_config.h"
#include "wireless_Impl.h"
#include "stm32f4xx_rcc.h"

/** @defgroup STM32F4_DISCOVERY_LIS302DL_Private_Defines
  * @{
  */
__IO uint32_t  WIRELESS_SPI_Timeout = WIRELESS_SPI_FLAG_TIMEOUT;   

#define USE_DEFAULT_TIMEOUT_CALLBACK

void wireless_spi_Init(void)
{
	
	wireless_spi_config();
	wireless_config_settings();
	
	return;
}


/**
  * @brief  Reads a block of data from the address specifies by headerByte, NOT applicable for command strosbes
  * @param  uint8_t* pBuffer : pointer to the buffer that receives the data read from the wireless chipset.
  * @param  uint8_t headerByte : wireless' internal address to read from.
  * @param  uint16_t NumByteToRead : number of bytes to read from the specified register.
  * @retval None
  */
void WIRELESS_SPI_Read(uint8_t* pBuffer, uint8_t readHeaderByte, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    readHeaderByte |= (uint8_t)(READWRITE_CMD | BURST_ACCESS_CMD); //(R/W)=1 B=1 for burst read operations
  }
  else
  {
    readHeaderByte |= (uint8_t)READWRITE_CMD; //(R/W)=1 for read operations
  }
  /* Set CS Low at the start of the transmission, wakes up from SPWD(SLEEP) and goes to IDLE*/
  WIRELESS_SPI_CS_LOW();
 
	WIRELESS_SPI_SendByte(readHeaderByte);
	
	/* Receive the data that will be read from the device (MSB First) */
	while(NumByteToRead > 0x00)
	{	
		/* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
		*pBuffer = WIRELESS_SPI_SendByte(DUMMY_BYTE);
		NumByteToRead--;
		pBuffer++;
	}
	
  /* Set chip select High at the end of the transmission */ 
 WIRELESS_SPI_CS_HIGH();
}

/**
  * @brief  Writes a block of data to the specified address in wireless chipset
  * @param  uint8_t* pBuffer : pointer to the buffer  containing the data to be written to the wireless.
  * @param  uint8_t writeHeaderByte : wireless' internal address to write to.
  * @param  uint16_t NumByteToWrite: Number of bytes to write
  * @retval None
  */
uint8_t WIRELESS_SPI_Write(uint8_t* pBuffer, uint8_t writeHeaderByte, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
	
	uint8_t statusByte;
  if(NumByteToWrite > 0x01)
  {
    writeHeaderByte |= (uint8_t)BURST_ACCESS_CMD;
  }
	
  /* Set CS Low at the start of the transmission, wakes up from SPWD(SLEEP) and goes to IDLE*/
  WIRELESS_SPI_CS_LOW();
  
  /* Send the Address of the specified register to write on*/
  WIRELESS_SPI_SendByte(writeHeaderByte);
	
  /* Send the block of data that will be written into the address */
  while(NumByteToWrite >= 0x01)
  {
    statusByte = WIRELESS_SPI_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  WIRELESS_SPI_CS_HIGH();
	
	return statusByte;
}

/**
  * @brief  Reads the register status
  * @param  regAddr: reads the register status 
  * @retval register status of the register specified by regAddr
  */
uint8_t read_Status_Register(uint8_t regAddr)
{
	uint8_t regStatus;
	
	regAddr |= (uint8_t)(READWRITE_CMD | BURST_ACCESS_CMD); //(R/W)=1 B=1 for burst read operations to read status registers
	
	WIRELESS_SPI_CS_LOW();
 
	WIRELESS_SPI_SendByte(regAddr);
	regStatus = WIRELESS_SPI_SendByte(DUMMY_BYTE);
	
  /* Set chip select High at the end of the transmission */ 
  WIRELESS_SPI_CS_HIGH();
	
	return regStatus;
}

/**
  * @brief  Send a command strobe
  * @param  regAddr: reads the register status 
  * @retval register status of the register specified by regAddr
  */
void send_command_strobe(uint8_t commandStrobeRegAddr)
{
	WIRELESS_SPI_CS_LOW();
	
	WIRELESS_SPI_SendByte(commandStrobeRegAddr);
	
	WIRELESS_SPI_CS_HIGH();
}

/** 
  * @brief  Send and receive a byte through SPI interface bus, 
  *         sends a byte and receives a byte over the bus
  * @param  uint8_t byte : byte to send.
  * @retval The received byte value
  */
static uint8_t WIRELESS_SPI_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  WIRELESS_SPI_Timeout = WIRELESS_SPI_FLAG_TIMEOUT;   
  while (SPI_I2S_GetFlagStatus(WIRELESS_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((WIRELESS_SPI_Timeout--) == 0) return WIRELESS_SPI_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(WIRELESS_SPI, byte);
  
  /* Wait to receive a Byte */
  WIRELESS_SPI_Timeout = WIRELESS_SPI_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(WIRELESS_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((WIRELESS_SPI_Timeout--) == 0) return WIRELESS_SPI_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(WIRELESS_SPI);
}



#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t WIRELESS_SPI_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  while (1)
 // {   
  //}
}

#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

void wirelessTransmit_TX(float* angles)
{
	uint8_t roll, pitch, maskMARCSTATE = 0x1F, maskTXBYTES = 0x7F, maskTxbytesBytesSent = SMARTRF_SETTING_PKTLEN, statusRegRet;
	uint8_t txBuffer[2];
	txBuffer[0] = (uint8_t)angles[0];
	txBuffer[1] = (uint8_t)angles[1];
	
	//Go to IDLE state
	send_command_strobe(WIRELESS_STROBE_SIDLE);
	
	//wait to reach IDLE state
	statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	statusRegRet &= maskMARCSTATE;
	while(statusRegRet != 0x01)
	{
		statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	  statusRegRet &= maskMARCSTATE;
	}
  
  //Flush the TX FIFO
	send_command_strobe(WIRELESS_STROBE_SFTX);
	
	//Wait for the TX FIFO to be flushed
	statusRegRet = read_Status_Register(WIRELESS_STATUS_TXBYTES);
	statusRegRet &= maskTXBYTES;
	while(statusRegRet != 0x00)
	{
		statusRegRet = read_Status_Register(WIRELESS_STATUS_TXBYTES);
	  statusRegRet &= maskTXBYTES;
	}
	
	//Burst write to TX buffer
	WIRELESS_SPI_Write(txBuffer, WIRELESS_RXTX_FIFO, 2);
  
  //Wait for the write to complete
  statusRegRet = read_Status_Register(WIRELESS_STATUS_TXBYTES);
	statusRegRet &= maskTXBYTES;
	while(statusRegRet != maskTxbytesBytesSent)
	{
		statusRegRet = read_Status_Register(WIRELESS_STATUS_TXBYTES);
	  statusRegRet &= maskTXBYTES;
	}
	
	//Enable the STX
	send_command_strobe(WIRELESS_STROBE_STX);
	
	//wait to reach IDLE state
	statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	statusRegRet &= maskMARCSTATE;
	while(statusRegRet != 0x01)
	{
		statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	  statusRegRet &= maskMARCSTATE;
	}
	
	//Read the number of bytes left in the TX FIFO after send operation
  statusRegRet = read_Status_Register(WIRELESS_STATUS_TXBYTES);
	statusRegRet &= maskTXBYTES;
	printf("Bytes left the TX FIFO:%d\n", statusRegRet);
	       
	return;
	
}


void wirelessReceive_RX(uint8_t* rxBufferPtr)
{
	uint8_t roll, pitch, maskMARCSTATE = 0x1F, maskTXBYTES = 0x7F, maskTxbytesBytesSent = SMARTRF_SETTING_PKTLEN, statusRegRet;
	uint8_t rxBuffer[4];
	//Go to IDLE state
	send_command_strobe(WIRELESS_STROBE_SIDLE);
	
	
	
	//wait to reach IDLE state
	statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	statusRegRet &= maskMARCSTATE;

	while(statusRegRet != 0x01)
	{
		statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	  statusRegRet &= maskMARCSTATE;
		printf("Wait for IDLE state:%s, status:%x\n", __FUNCTION__, statusRegRet);
	}
	
	//Enable the SRX
	send_command_strobe(WIRELESS_STROBE_SRX);
	
	//wait to reach RX state
	statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	statusRegRet &= maskMARCSTATE;

	while(statusRegRet != 0x01)
	{
		
		statusRegRet = read_Status_Register(WIRELESS_STATUS_MARCSTATE);
	  statusRegRet &= maskMARCSTATE;
		//printf("IDLE status:%x\n", statusRegRet);
		//printf("Wait for RX state:%s, status:%x\n", __FUNCTION__, statusRegRet);
	}
	
	//Read number of bytes in RX FIFO
	statusRegRet = read_Status_Register(WIRELESS_STATUS_RXBYTES);
	statusRegRet &= maskTXBYTES;
	
	
	if (statusRegRet == 0x04)
	{
		//printf("Num of bytes in RX FIFO:%x\n", statusRegRet);

		WIRELESS_SPI_Read(rxBufferPtr, WIRELESS_RXTX_FIFO,2); 
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	send_command_strobe(WIRELESS_STROBE_SFRX);
}


void wireless_config_settings(void)
{
	uint8_t ctrl, setting;
	
	//Do a chip reset
	ctrl = (uint8_t)WIRELESS_STROBE_SRES;
	send_command_strobe(ctrl);
	
	ctrl = (uint8_t)WIRELESS_STROBE_SIDLE;
	send_command_strobe(ctrl);
  
	ctrl = 0x00;
	setting = (uint8_t)SMARTRF_SETTING_IOCFG2;
	WIRELESS_SPI_Write(&setting, ctrl, 1);
	
	ctrl = 0x02;
 	setting = (uint8_t)SMARTRF_SETTING_IOCFG0;
	WIRELESS_SPI_Write(&setting, ctrl, 1);

	ctrl = 0x03;
	setting = (uint8_t)SMARTRF_SETTING_FIFOTHR;
	WIRELESS_SPI_Write(&setting, ctrl, 1);
	
	ctrl = 0x06;
	setting = (uint8_t)SMARTRF_SETTING_PKTLEN;
	WIRELESS_SPI_Write(&setting, ctrl, 1);
	
	ctrl = 0x07;
	setting = (uint8_t)SMARTRF_SETTING_PKTCTRL1;
	WIRELESS_SPI_Write(&setting, ctrl, 1);
	
	ctrl = 0x08;
	setting = (uint8_t)SMARTRF_SETTING_PKTCTRL0;
	WIRELESS_SPI_Write(&setting, ctrl, 1);
	
	ctrl = 0x09;
	setting = (uint8_t)SMARTRF_SETTING_ADDR;
	WIRELESS_SPI_Write(&setting, ctrl, 1);	// ctrless
	
	ctrl = 0x0A;
	setting = (uint8_t)SMARTRF_SETTING_CHANNR;
	WIRELESS_SPI_Write(&setting, ctrl, 1);	// channel
	
	ctrl = 0x0B;
	setting = (uint8_t)SMARTRF_SETTING_FSCTRL1;
	WIRELESS_SPI_Write(&setting, ctrl, 1); // freq synth control 1
	
	ctrl = 0x0C;
	setting = (uint8_t)SMARTRF_SETTING_FSCTRL0;
	WIRELESS_SPI_Write(&setting, ctrl, 1); // freq synth control 0
	
	ctrl = 0x0D;
	setting = (uint8_t)SMARTRF_SETTING_FREQ2;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x0E;
	setting = (uint8_t)SMARTRF_SETTING_FREQ1;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x0F;
	setting = (uint8_t)SMARTRF_SETTING_FREQ0;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x10;
	setting = (uint8_t)SMARTRF_SETTING_MDMCFG4;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x11;
	setting = (uint8_t)SMARTRF_SETTING_MDMCFG3;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x12;
	setting = (uint8_t)SMARTRF_SETTING_MDMCFG2;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x13;
	setting = (uint8_t)SMARTRF_SETTING_MDMCFG1;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x14;
	setting = (uint8_t)SMARTRF_SETTING_MDMCFG0;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x15;
	setting = (uint8_t)SMARTRF_SETTING_DEVIATN;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x18;
	setting = (uint8_t)SMARTRF_SETTING_MCSM0;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x19;
	setting = (uint8_t)SMARTRF_SETTING_FOCCFG;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x1A;
	setting = (uint8_t)SMARTRF_SETTING_BSCFG;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x1B;
	setting = (uint8_t)SMARTRF_SETTING_AGCCTRL2;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x1C;
	setting = (uint8_t)SMARTRF_SETTING_AGCCTRL1;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	
	
	ctrl = 0x1D;
	setting = (uint8_t)SMARTRF_SETTING_AGCCTRL0;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 

	ctrl = 0x21;
	setting = (uint8_t)SMARTRF_SETTING_FREND1;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 

	ctrl = 0x22;
	setting = (uint8_t)SMARTRF_SETTING_FREND0;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x23;	
	setting = (uint8_t)SMARTRF_SETTING_FSCAL3;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	
	
	ctrl = 0x24;
	setting = (uint8_t)SMARTRF_SETTING_FSCAL2;	
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x25;
	setting = (uint8_t)SMARTRF_SETTING_FSCAL1;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	
	
	ctrl = 0x26;
	setting = (uint8_t)SMARTRF_SETTING_FSCAL0;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x29;
	setting = (uint8_t)SMARTRF_SETTING_FSTEST;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	
	
	ctrl = 0x2C;
	setting = (uint8_t)SMARTRF_SETTING_TEST2;
	WIRELESS_SPI_Write(&setting, ctrl, 1);	
	
	ctrl = 0x2D;
	setting = (uint8_t)SMARTRF_SETTING_TEST1;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
	ctrl = 0x2E;
	setting = (uint8_t)SMARTRF_SETTING_TEST0;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	
	
	ctrl = 0x04;
	setting = (uint8_t)SMARTRF_SETTING_SYNCWORD;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 	

	ctrl = 0x05;
	setting = (uint8_t)SMARTRF_SETTING_SYNCWORD;
	WIRELESS_SPI_Write(&setting, ctrl, 1); 
	
}

