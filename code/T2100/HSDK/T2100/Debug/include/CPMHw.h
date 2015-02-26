///////////////////////////////////////////////////////////////////////////
//
// Module   : CPMHW
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module CPMHW (Clock & Power Management)
//
//         Copyright (C) 2004 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 040922  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef CPMHW_H
#define CPMHW_H

// CPU block clock control bits
#define CPM_STOP_BIT		0x00000001
#define CPM_SLOW_BIT		0x00000002
#define CPM_IDLE_BIT		0x00000004
#define CPM_PWMTIMER_BIT	0x00000008
#define CPM_ZDMA_BIT		0x00000010
#define CPM_SIO_BIT			0x00000020
#define CPM_LCDC_BIT		0x00000040
#define CPM_BDMA_BIT		0x00000080
#define CPM_UART0_BIT		0x00000100
#define CPM_UART1_BIT		0x00000200
#define CPM_GPIO_BIT		0x00000400
#define CPM_RTC_BIT			0x00000800
#define CPM_ADC_BIT			0x00001000
#define CPM_IIC_BIT			0x00002000
#define CPM_IIS_BIT			0x00004000

// CPU blocks
typedef enum
{
	CPM_STOP_CPU,
	CPM_SLOW_CPU,
	CPM_IDLE_CPU,
	CPM_PWMTIMER_BLK,
	CPM_ZDMA_BLK,
	CPM_SIO_BLK,
	CPM_LCDC_BLK,
	CPM_BDMA_BLK,
	CPM_UART0_BLK,
	CPM_UART1_BLK,
	CPM_GPIO_BLK,
	CPM_RTC_BLK,
	CPM_ADC_BLK,
	CPM_IIC_BLK,
	CPM_IIS_BLK
} CPM_CPU_BLOCK;

// Function declarations
void  S_EnableClk (CPM_CPU_BLOCK eBlock);
void  S_DisableClk(CPM_CPU_BLOCK eBlock);
BOOL  S_ActivateWDT(DWORD nDuration);
void  S_StartRTC(PROCPTR pFunc);
void  S_AutoClkOpen(void);
void  S_AutoClkClose(void);
void  S_AdjustClk( BOOL bDir);
DWORD S_GetMCLK(void);
void  S_IDLE(FUNCPTR pFunc, DWORD nPeriod);
void  S_BatteryChargerON(void);
void  S_BatteryChargerOFF(void);
DWORD S_BatteryChargerState(void);
void  S_SetPowerFailHandler(const PROCPTR pFunc);
void  S_RunPowerFailHandler(void);

#endif

