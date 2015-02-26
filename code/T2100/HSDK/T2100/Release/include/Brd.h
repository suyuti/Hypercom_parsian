
///////////////////////////////////////////////////////////////////////////
//
// Module   : Definitions
// Part of  : BOOTER, EFTOS
// Created  : Bengt Westman 
// Abstract : Header file for definitions for Main Board
//
//         Copyright (C) 2003 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 030429  BW      Original version (based on EFTOS8 version 030404)
// 030613  BW      Renamed SEC_RTS and SEC_CTS to RS0_RTS and RS0_CTS
// 030916  BW      Changed location of DSP signals to port B.
//                 Added new signal COM-RST/
// 040415  JB      Added new signal PDATC_DSP_LIGHT_BIT
// 040416  JB      Added new external UART defines
// 041103  RZ      Changed Master Clock settings
// 050112  RZ      Added rUART_RS2_TMP register definition
//
///////////////////////////////////////////////////////////////////////////

#ifndef BRD_H
#define BRD_H

// Bits in Programmable I/O Data registers (GPIO Port A-F)
// Some of the port pins are not available for all terminals
#define PDATB_DSP_WR_BIT                0x0010  // Out: DSP-WR/
#define PDATB_DSP_RD_BIT                0x0020  // Out: DSP-RD/
#define PDATB_DSP_RS_BIT                0x0400  // Out: DSP-RS

#define PDATC_SEC_RESET_BIT             0x0001  // Out: SEC-RESET
#define PDATC_COM_RESET_BIT             0x0002  // Out: COM-RST/
#define PDATC_DSP_RESET_BIT             0x0004  // Out: DSP-RST/
#define PDATC_DSP_LIGHT_BIT             0x0008  // Out: DSP-RST/
#define PDATC_PRN_RST_BIT               0x0040  // Out: PRN-RST/
#define PDATC_RS1_DCD_BIT               0x0080  // Out: RS1-DCD/
#define PDATC_RS1_DTR_BIT               0x0100  // Out: RS1-DTR/
#define PDATC_RS1_DSR_BIT               0x0200  // Out: RS1-DSR/
#define PDATC_RS1_RTS_BIT               0x0400  // Out: RS1-RTS/
#define PDATC_RS1_CTS_BIT               0x0800  // In:  RS1-CTS/
#define PDATC_RS0_RTS_BIT               0x4000  // Out: RS0-RTS/
#define PDATC_RS0_CTS_BIT               0x8000  // In:  RS0-CTS/

#define PDATE_MSR_DATA1_BIT             0x0020  // In: MSR-DATA1
#define PDATE_MSR_DATA2_BIT             0x0040  // In: MSR-DATA2
#define PDATE_MSR_DATA3_BIT             0x0080  // In: MSR-DATA3

#define PDATF_HARDWARE_BIT				0x0004	// In:  ACS/ (Advanced Charge Station)
#define PDATF_TEST_BIT                  0x0008  // In:  TEST/
#define PDATF_BOOTER_BIT                0x0010  // In:  BOOTER/ (Forced Boot)
#define PDATF_PRN_RDY_BIT               0x0040  // In:  PRN-READY/


#define ADC_CH_BAT_VOLT                 0x0  // Battery Voltage (AIN0)
#define ADC_CH_CONF_MODE                0x1  // Configuration Mode (AIN1)

#define ADC_CH_FRONT_SENSOR             0x2  // FRONT-SENSOR (AIN2)
#define ADC_CH_MID_SENSOR               0x3  // MID-SENSOR (AIN3)
#define ADC_CH_REAR_SENSOR              0x4  // REAR-SENSOR (AIN4)

#define ADC_MAX_CHANNELS                8

// ADC channels mask
#define ADC_BAT_VOLT_MASK				0x01	// Battery Voltage    (AIN0)
#define ADC_CFG_MODE_MASK				0x02	// Configuration Mode (AIN1)
#define ADC_FRONT_SENSOR_MASK			0x04	// FRONT-SENSOR       (AIN2)
#define ADC_MID_SENSOR_MASK				0x08	// MID-SENSOR         (AIN3)
#define ADC_REAR_SENSOR_MASK			0x10	// REAR-SENSOR        (AIN4)
#define ADC_AIN5_RESERVED_MASK			0x20	// Reserved           (AIN5)
#define ADC_AIN6_RESERVED_MASK			0x40	// Reserved           (AIN6)
#define ADC_AIN7_RESERVED_MASK			0x80	// Reserved           (AIN7)

//#define ADCPRESCALER                    0x14
//// Battery Voltage level for low level (approx. 2.6 V)
//#define ADC_CH_BAT_VOLT_LOW_LEVEL       822  

#define ADCPRESCALER                    0xFF
// Battery Voltage level for low level (approx. 1.9 V)
#define ADC_CH_BAT_VOLT_LOW_LEVEL       778  


//// 66MHz internal clock
//#define MCLK 66000000
                             
// 66355200 Hz internal clock
#define MCLK 66355200


// Crystal frequency used by External Uart
#define UART_EXT_CLK_FREQ   11059200L	//11.0592MHz

#define UART_EXT_BASE_ADDR			 0x08000000
#define UART_EXT_OFFSET_DATA         0x0  // Read RHR, Write THR
#define UART_EXT_OFFSET_IER          0x1  // R/W
#define UART_EXT_OFFSET_IIR          0x2  // Read
#define UART_EXT_OFFSET_FCR          0x2  // Write
#define UART_EXT_OFFSET_LCR          0x3  // R/W
#define UART_EXT_OFFSET_MCR          0x4  // R/W
#define UART_EXT_OFFSET_LSR          0x5  // Read only
#define UART_EXT_OFFSET_MSR          0x6  // Read only
#define UART_EXT_OFFSET_TMP          0x7  // R/W (scratch register)
#define UART_EXT_OFFSET_DLL          0x0  // If LCR bit-7 is 1
#define UART_EXT_OFFSET_DLH          0x1  // If LCR bit-7 is 1

#define UART_EXT_DATA   (UART_EXT_BASE_ADDR+UART_EXT_OFFSET_DATA)

// Alternative names
#define UART_RS2_BASE           UART_EXT_BASE_ADDR
#define UART_RS2_DATA           UART_EXT_DATA

#define rUART_RS2_DATA	(*(volatile unsigned char *)UART_RS2_DATA)
#define rUART_RS2_IER   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_IER))
#define rUART_RS2_IIR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_IIR))
#define rUART_RS2_FCR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_FCR))
#define rUART_RS2_LCR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_LCR))
#define rUART_RS2_MCR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_MCR))
#define rUART_RS2_LSR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_LSR))
#define rUART_RS2_MSR   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_MSR))
#define rUART_RS2_TMP   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_TMP))

// Next two registers are only accessed if LCR bit-7 is 1
#define rUART_RS2_DLL   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_DLL))
#define rUART_RS2_DLH   (*(volatile unsigned char *)(UART_RS2_BASE+UART_EXT_OFFSET_DLH))


#endif
