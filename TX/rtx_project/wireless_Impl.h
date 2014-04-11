
#ifndef _WIRELESS_IMPL_H_
#define _WIRELESS_IMPL_H_

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define BURST_ACCESS_CMD           ((uint8_t)0x40) // 1 -> multiple, 0 -> single access
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00) // sent to generate clk


/* Configuration values ------------------------------------------------------*/
#define SMARTRF_SETTING_FSCTRL1    0x0C
#define SMARTRF_SETTING_FSCTRL0    0x00
#define SMARTRF_SETTING_FREQ2      0x5D 
#define SMARTRF_SETTING_FREQ1      0x93
#define SMARTRF_SETTING_FREQ0      0xB1
#define SMARTRF_SETTING_MDMCFG4    0x0E 
#define SMARTRF_SETTING_MDMCFG3    0x3B 
#define SMARTRF_SETTING_MDMCFG2    0x73 
#define SMARTRF_SETTING_MDMCFG1    0x42 
#define SMARTRF_SETTING_MDMCFG0    0xF8 
#define SMARTRF_SETTING_CHANNR     0x40 
#define SMARTRF_SETTING_DEVIATN    0x00 
#define SMARTRF_SETTING_FREND1     0xB6
#define SMARTRF_SETTING_FREND0     0x10
#define SMARTRF_SETTING_MCSM0      0x18 
#define SMARTRF_SETTING_MCSM1			 0x30 //
#define SMARTRF_SETTING_FOCCFG     0x1D 
#define SMARTRF_SETTING_BSCFG      0x1C
#define SMARTRF_SETTING_AGCCTRL2   0xC7
#define SMARTRF_SETTING_AGCCTRL1   0x40 
#define SMARTRF_SETTING_AGCCTRL0   0xB0
#define SMARTRF_SETTING_FSCAL3     0xEA
#define SMARTRF_SETTING_FSCAL2     0x0A
#define SMARTRF_SETTING_FSCAL1     0x00
#define SMARTRF_SETTING_FSCAL0     0x19 
#define SMARTRF_SETTING_FSTEST     0x59
#define SMARTRF_SETTING_TEST2      0x88
#define SMARTRF_SETTING_TEST1      0x31
#define SMARTRF_SETTING_TEST0      0x0B
#define SMARTRF_SETTING_FIFOTHR    0x07
#define SMARTRF_SETTING_IOCFG2     0x29
#define SMARTRF_SETTING_IOCFG0     0x06
#define SMARTRF_SETTING_PKTCTRL1   0x04
#define SMARTRF_SETTING_PKTCTRL0   0x44 
#define SMARTRF_SETTING_ADDR       0x00 
#define SMARTRF_SETTING_PKTLEN     0x02 // PACKET LENGTH
#define SMARTRF_SETTING_SYNCWORD   0xAF // 


/* Strobe command addresses --------------------------------------------------*/
#define WIRELESS_STROBE_SRES         0x30        // Reset chip.
#define WIRELESS_STROBE_SFSTXON      0x31        // Enable/calibrate freq synthesizer
#define WIRELESS_STROBE_SXOFF        0x32        // Turn off crystal oscillator.
#define WIRELESS_STROBE_SCAL         0x33        // Calibrate freq synthesizer & disable
#define WIRELESS_STROBE_SRX          0x34        // Enable RX.
#define WIRELESS_STROBE_STX          0x35        // Enable TX.
#define WIRELESS_STROBE_SIDLE        0x36        // Strobe IDLE
#define WIRELESS_STROBE_SAFC         0x37        // AFC adjustment of freq synthesizer
#define WIRELESS_STROBE_SWOR         0x38        // Start automatic RX polling sequence
#define WIRELESS_STROBE_SPWD         0x39        // Enter pwr down mode when CSn goes hi
#define WIRELESS_STROBE_SFRX         0x3A        // Flush the RX FIFO buffer.
#define WIRELESS_STROBE_SFTX         0x3B        // Flush the TX FIFO buffer.
#define WIRELESS_STROBE_SWORRST      0x3C        // Reset real time clock.
#define WIRELESS_STROBE_SNOP         0x3D        // No operation.

/* Status registers ----------------------------------------------------------*/
#define WIRELESS_STATUS_PARTNUM      0x30 // Part number
#define WIRELESS_STATUS_VERSION      0x31 // Current version number
#define WIRELESS_STATUS_FREQEST      0x32 // Frequency offset estimate
#define WIRELESS_STATUS_LQI          0x33 // Demodulator estimate for link quality
#define WIRELESS_STATUS_RSSI         0x34 // Received signal strength indication
#define WIRELESS_STATUS_MARCSTATE    0x35 // Control state machine state
#define WIRELESS_STATUS_WORTIME1     0x36 // High byte of WOR timer
#define WIRELESS_STATUS_WORTIME0     0x37 // Low byte of WOR timer
#define WIRELESS_STATUS_PKTSTATUS    0x38 // Current GDOx status and packet status
#define WIRELESS_STATUS_VCO_VC_DAC   0x39 // Current setting from PLL cal module
#define WIRELESS_STATUS_TXBYTES      0x3A // Underflow and # of bytes in TXFIFO
#define WIRELESS_STATUS_RXBYTES      0x3B // Overflow and # of bytes in RXFIFO


/* MultipleByte register ----------------------------------------------------------*/
//#define WIRELESS_NUM_RXBYTES  0x7F // Mask "# of bytes" field in _RXBYTES
#define WIRELESS_RXTX_FIFO    0x3F // Mask "# of bytes" field in _RXBYTES



void wireless_spi_Init(void);
void wireless_config_settings(void);

static uint8_t WIRELESS_SPI_SendByte(uint8_t byte);

void WIRELESS_SPI_Read(uint8_t* pBuffer, uint8_t headerByte, uint16_t NumByteToRead);
uint8_t WIRELESS_SPI_Write(uint8_t* pBuffer, uint8_t writeHeaderByte, uint16_t NumByteToWrite);

uint8_t read_Status_Register(uint8_t regAddr);
void send_command_strobe(uint8_t commandStrobeRegAddr);

void wirelessTransmit_TX(int8_t* angles);
void wirelessReceive_RX(void);

uint32_t WIRELESS_SPI_TIMEOUT_UserCallback(void);

#endif
