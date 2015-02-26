
///////////////////////////////////////////////////////////////////////////
//
// Module   : SswHelp
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module SswHelp
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
// 030422  BW      Original version (based on EFTOS8 version 030409)
// 050112  RZ      Changed ADC staff, deleted useless functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef SSWHELP_H
#define SSWHELP_H

#include "44b.h"

// Define a prescaler value giving input clock frequency of 400 kHz.
// A divider value of 4 will then give a timer tick period of 10 us
// Timer 0 is used for Buzzer (at 2 kHz)
#define HW_TIMER01_PRESCALER ((MCLK/400000)-1)

// Define a prescaler value giving input clock frequency of 2 MHz.
// A divider value of 2 will then give a timer tick period of 1 us
#define HW_TIMER23_PRESCALER ((MCLK/2000000)-1)

// Define a prescaler value giving input clock frequency of 2 MHz.
// A divider value of 2 will then give a timer tick period of 1 us
#define HW_TIMER45_PRESCALER ((MCLK/2000000)-1)


// Define delay values used when calling H_Delay
#define H_DELAY_100_US  (100/100)
#define H_DELAY_1_MS    (1000/100)
#define H_DELAY_1_S     (1000000/100)


typedef enum {
    HW_TIMER0=0,
    HW_TIMER1=1,
    HW_TIMER2=2,
    HW_TIMER3=3,
    HW_TIMER4=4,
    HW_TIMER5=5
} HW_TIMER_E;

typedef enum {
    DIV2=0,
    DIV4=1,
    DIV8=2,
    DIV16=3,
    DIV32=4     // Timer 4:EXTCLK and Timer 5:TCLK
} HW_TIMER_DIV_E;

typedef enum {
    HW_TIMER_ONE_SHOT=0,
    HW_TIMER_RELOAD=1
} HW_TIMER_MODE_E;



// Global Function Prototypes
#ifndef WIN32
// Use these macros instead of functions for speed optimization
#define H_RdB(Reg)         (Reg)
#define H_RdDW(Reg)        (Reg)
#define H_RdW(Reg)         (Reg)
#define H_WrB(Reg, nVal)   ((Reg)=(nVal))
#define H_WrDW(Reg, nVal)  ((Reg)=(nVal))
#define H_WrW(Reg, nVal)   ((Reg)=(nVal))

#else 
// Always use functions for simulation in PC and for better
// type checking
#define H_RdB(Reg)         H_RdB_Func((volatile LPBYTE) (&Reg))
#define H_RdDW(Reg)        H_RdDW_Func((volatile LPDWORD)(&Reg))
#define H_RdW(Reg)         H_RdW_Func((volatile LPWORD) (&Reg))
#define H_WrB(Reg, nVal)   H_WrB_Func((volatile LPBYTE) (&Reg), nVal)
#define H_WrDW(Reg, nVal)  H_WrDW_Func((volatile LPDWORD)(&Reg), nVal)
#define H_WrW(Reg, nVal)   H_WrW_Func((volatile LPWORD) (&Reg), nVal)
#endif


// This staff removed from Th.c file. Names kept for compatibility.
// Battery low event flag updated if battery low is detected
// Only cleared at startup by SSW
extern BOOL volatile bTH_BatLow_Evt;
DWORD TH_RetBatAdcVal(void);


// Disable and enable interrupts
void    DisInt(void);
void    EnInt(void);

// Other public functions
void	H_AdcSetup(DWORD nMask);
DWORD   H_CalcChecksum(DWORD nStartAddr, DWORD nEndAddr);
void    H_Delay(DWORD Time);
void    H_IntInstall(DWORD nIntVecAddr, PROCPTR pIntHand, DWORD IntMaskBit);
BOOL    H_IsIntEnable(void);
void    H_SetISR_Vec(DWORD nIntVecAddr, PROCPTR pIntHand);
DWORD   H_SpbCom(DWORD nSlave, LPBYTE pSend, LPBYTE pRec,
                 DWORD nSendLen, DWORD nMaxLen, DWORD* nRecLen, DWORD Timer);
DWORD   H_SpbRead(DWORD nSlave, LPBYTE pRec, DWORD nMaxLen, DWORD* nRecLen,
                  DWORD Timer);
DWORD   H_SpbWrite(DWORD nSlave, LPBYTE pSend, DWORD nLen, DWORD Timer);
void    H_TimerInitCommon(void);
void    H_TimerStart(HW_TIMER_E Timer, HW_TIMER_DIV_E Divider, DWORD Count,
            DWORD Compare, HW_TIMER_MODE_E Mode, BOOL bInverterOn);
void    H_TimerStop(HW_TIMER_E Timer, BOOL bStopAutoReload);


BYTE    H_RdB_Func(volatile LPBYTE pReg);
DWORD   H_RdDW_Func(volatile LPDWORD pReg);
void    H_WrB_Func(volatile LPBYTE pReg, BYTE nVal);
void    H_WrDW_Func(volatile LPDWORD pReg, DWORD nVal);




// Macro to mask interrupt in Interrupt Controller (mask register
// is read/modify/write with interrupts disabled in core)
#define INTMSK_MASK_PROTECTED(MASK_BITS)    \
    if (H_IsIntEnable()) \
    { \
        DisInt(); \
        H_WrDW(rINTMSK, H_RdDW(rINTMSK)|MASK_BITS); \
        EnInt(); \
    } \
    else \
        H_WrDW(rINTMSK, H_RdDW(rINTMSK)|MASK_BITS);


// Macro to unmask interrupt in Interrupt Controller (mask register
// is read/modify/write with interrupts disabled in core)
#define INTMSK_UNMASK_PROTECTED(MASK_BITS)    \
    if (H_IsIntEnable()) \
    { \
        DisInt(); \
        H_WrDW(rINTMSK, H_RdDW(rINTMSK)&(~MASK_BITS)); \
        EnInt(); \
    } \
    else \
        H_WrDW(rINTMSK, H_RdDW(rINTMSK)&(~MASK_BITS));



// Interrupt mask bit of selected timer
#define TH_INTMSK_BIT   BIT_TIMER2

// Disable task handler interrupts by mask of timer interrupt
#define TH_Disable() \
    INTMSK_MASK_PROTECTED(TH_INTMSK_BIT);


// Enable task handler interrupts by unmask of timer interrupt
#define TH_Enable() \
    INTMSK_UNMASK_PROTECTED(TH_INTMSK_BIT);




// Workaround to handle INTMSK problem (used in beginning of ISR)
#ifndef WIN32
#define IRQ_INTMSK_WORKAROUND()    \
    if (H_RdDW(rI_ISPR)==0) \
    { \
        H_WrDW(rI_PMST, H_RdDW(rI_PMST)); \
        return; \
    } 
#else
#define IRQ_INTMSK_WORKAROUND()
#endif


// Special handling for ARM processor before enabling interrupts in IRQ.
void IRQ_ENTER_SVC(void);

// Special handling for ARM processor is before returning from interrupts.
void IRQ_EXIT_SVC(void);

#endif
