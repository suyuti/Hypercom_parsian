
///////////////////////////////////////////////////////////////////////////
//
// Module   : Definitions
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file holding common definitions of SSW
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
// 030509  BW      Original version (based on EFTOS8 version 030411)
// 030606  BW      Removed definitions of SECTION_ZIDATA_*
// 030617  BW      Added definition of PARAM_PROCPTR and changed
//                 definition of INT_TAB.
//                 IIC interrupts changed to IRQ level 0.
// 040526  AE      New typedef for 64 bits variables, DDWORD.
// 040629  ASA     Changed for GNU C support
// 050112  RZ      Added hardware type definitions
//
///////////////////////////////////////////////////////////////////////////

#ifndef SSWDEF_H
#define SSWDEF_H

#include "44b.h"


// Defines of memory addresses

// Max Size of Loader (256kB)
#define LOADERSIZE          0x40000               

// Start address of Loader in RAM
#define LOADER_START_RAM    0x0C010000            



// Defines of interrupt vectors
#define EINT0Vector         0x0C000004      // External Interrupt 0
#define EINT1Vector         0x0C000008      // External Interrupt 1
#define EINT2Vector         0x0C00000C      // External Interrupt 2
#define EINT3Vector         0x0C000010      // External Interrupt 3
#define EINT4Vector         0x0C000014      // External Interrupt 4567
#define TICKVector          0x0C000018      // RTC TICK Interrupt
#define ZDMA0Vector         0x0C00001C      // ZDMA0
#define ZDMA1Vector         0x0C000020      // ZDMA1
#define BMA0Vector          0x0C000024      // BDMA0
#define BMA1Vector          0x0C000028      // BDMA1
#define WDTVector           0x0C00002C      // Watchdog timer
#define UERRVector          0x0C000030      // User Error  
#define TIM0Vector          0x0C000034      // Timer 0  
#define TIM1Vector          0x0C000038      // Timer 1  
#define TIM2Vector          0x0C00003C      // Timer 2  
#define TIM3Vector          0x0C000040      // Timer 3  
#define TIM4Vector          0x0C000044      // Timer 4  
#define TIM5Vector          0x0C000048      // Timer 5  
#define RXD0Vector          0x0C00004C      // Uart 0 RX
#define RXD1Vector          0x0C000050      // Uart 1 RX
#define IICVector           0x0C000054      // I^2C
#define SIOVector           0x0C000058      // SPI RX/TX
#define TXD0Vector          0x0C00005C      // Uart 0 TX
#define TXD1Vector          0x0C000060      // Uart 1 TX
#define RTCVector           0x0C000064      // RTC 
#define ADCVector           0x0C000068      // AD Converter
#define FIQVector           0x0C00006C      // FIQ 


// Define which interrupt mask for different IRQ level
#define IRQ_INTMSK_LEVEL0 (\
    BIT_ADC		| \
    BIT_RTC		| \
    BIT_SIO		| \
    BIT_TIMER5	| \
    BIT_TIMER4	| \
    BIT_TIMER3	| \
    BIT_TIMER2	| \
    BIT_TIMER1	| \
    BIT_TIMER0	| \
    BIT_WDT		| \
    BIT_BDMA1	| \
    BIT_BDMA0	| \
    BIT_ZDMA1	| \
    BIT_ZDMA0	| \
    BIT_TICK	| \
    BIT_IIC		| \
    0)


#define IRQ_INTMSK_LEVEL1 (\
    BIT_UTXD1	| \
    BIT_UTXD0	| \
    BIT_URXD1	| \
    BIT_URXD0	| \
    BIT_UERR01	| \
    BIT_EINT4567| \
    BIT_EINT3	| \
    BIT_EINT2	| \
    BIT_EINT1	| \
    0)



#ifndef FALSE
#define FALSE 0
#endif  

#ifndef TRUE
#define TRUE 1
#endif  

#ifndef OFF
#define OFF 0
#endif  

#ifndef ON
#define ON 1
#endif  

// Null pointer
#ifndef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL  ((void*)0)
#endif
#endif


#ifndef BYTE
typedef unsigned char BYTE;
#endif  

#ifndef BOOL
typedef unsigned int BOOL;
#endif  

#ifndef WORD
typedef unsigned short WORD;
#endif  

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef DDWORD
#ifdef WIN32
typedef unsigned __int64 DDWORD;
#else
typedef unsigned long long DDWORD;
#endif
#endif

#ifdef WIN32
#define FAR
#define FAR_FUNC
#define NAKED
#define INTERRUPT
#define FIQ_INTERRUPT
#elif defined(__ARMCC_VERSION)
#define FAR
#define FAR_FUNC
#if __ARMCC_VERSION < 200000
#define NAKED
#define ASM_START       __asm {
#define ASM_END         }
#else
#define NAKED           __asm
#define ASM_START
#define ASM_END
#endif
#define INTERRUPT       __irq
#define FIQ_INTERRUPT   __irq
#ifndef __attribute__
#define __attribute__(IGNORED)
#endif
#define ALIGN(A)        __align(A)
#else
#define FAR
#define FAR_FUNC
#define NAKED           __attribute__((naked))
#define INTERRUPT       __attribute__((isr))
#define FIQ_INTERRUPT   __attribute__((isr ("FIQ")))
#define ALIGN(A)        __attribute__((aligned(A)))
#endif

#ifdef __GNUC__
#define __init__(A) A
#else
#define __init__(A)
#endif

typedef void  FAR*      LPVOID;
typedef DWORD FAR*      LPDWORD;
typedef WORD  FAR*      LPWORD;
typedef BYTE  FAR*      LPBYTE;
typedef BOOL  FAR*      LPBOOL;
typedef unsigned char FAR*      LPSTR;
typedef const unsigned char FAR* LPCSTR;

typedef DWORD (FAR_FUNC *FUNCPTR)();
typedef void (FAR_FUNC *PROCPTR)();
typedef void (FAR_FUNC *PARAM_PROCPTR)(DWORD);
typedef PARAM_PROCPTR INT_PROC_TAB[4];

typedef void*       PVOID;
typedef DWORD*      PDWORD;
typedef WORD*       PWORD;
typedef BYTE*       PBYTE;
typedef BOOL*       PBOOL;
typedef unsigned char*       PSTR;
typedef const unsigned char* PCSTR;

// Static variables which are constant (primarily strings) are stored
// in the CONST segment
typedef const unsigned char  CONSTCHAR;
typedef const BYTE  CONSTBYTE;
typedef const WORD  CONSTWORD;
typedef const DWORD CONSTDWORD;

// Pointer to constant string
typedef const unsigned char* PCONSTCHAR;

// Far Pointer to constant string
typedef const unsigned char FAR* LPCONSTCHAR;


// Indicate that basic types have been defined as the IAR compiler
// can not detect typedef's in #ifdef statements
#ifndef BASIC_TYPE_DEFINED 
#define BASIC_TYPE_DEFINED
#endif


// During debugging make all static visible
#if defined(ALL_PUBLICS)
#define STATIC
#else
#define STATIC static
#endif





// Return codes
#define OK                  1      // Return status = OK
#define PROGRESS            2      // Program is running
#define TIMEOUT             3      // Timeout
#define TO_MANY_RETRIES     4      // Number of tries exceeded
#define NO_CONNECTION       5      // No connection
#define ILLEGAL             6      // Illegal function
#define NOT_PRESENT         7      // Not present
#define CONTINUE            8      // Continue
#define NO_READ             9      // No read done
#define NO_WRITE            10     // No write done
#define ERRONEOUS_READ      11     // Read with error
#define ERRONEOUS_WRITE     12     // Write with error
#define OUT_OF_TASKS        15     // Number of buffers exceeded
#define CANCELED            16     // Canceled
#define NOT_ALLOWED         17     // Not allowed
#define MSG_RECEIVED        19     // Message received
#define URGENT              20     // Urgent end of the formatter
#define SYN_ERROR           21     // Syntax error in the format string
#define NO_CARRIER          29     // No carrier telephone line
#define LRC_ERROR           50     // LRC error
#define PARITY_ERROR        51     // Parity error
#define FORMAT_ERROR        52     // Format error in code
#define NUM_CHR_ERROR       54     // To many characters
#define NOT_AVAILABLE       56     // The track is not available
#define BUFFER_OVERFLOW     58     // Overflow in the user buffer
#define RECEIVER_BUSY       59     // Receiver busy
#define RESIDUE             70     // Residue at receive
#define BUFFER_FULL         71     // Buffer full


#define MHWRONG_LU          80     // Wrong LU to update AHTABLE
#define MHWRONG_CMD         81     // Wrong command code to LU18
#define MHLONG_CMD          82     // To long command to LU18
#define MHBUFFER_TO_SMALL   83     // To small buffer at read
#define MHLU_CLOSED         84     // LU is closed
#define MHWAIT_TIMEOUT      85     // LU has wait to long time

#define AHWRONG_LU          90     // Wrong LU
#define AHWRONG_ADDR        91     // Logical addr is not defined
#define AHBUFFER_BUSY       92     // Output buffer is busy
#define AHLU_NOT_ACTIVE     93     // LU is not active

#define LIFETIME_TIMEOUT    100    // From network level
#define ILLEGAL_ADDR        101    // From network level

// Return codes 128-255 are not allowed
#define BUSY                0xFF   // Busy


// Key codes 
#define ENTER               0x0D
#define CE                  0x0B
#define CANCEL              0x80
#define KEY_UP              0x81
#define KEY_SELECT          0x82
#define KEY_DOWN            0x83
#define SCROLL_UP           0xA5
#define SCROLL_DOWN         0xA6
#define SHIFT_SH1           0xF1
#define SHIFT_SH2           0xF2
#define SHIFT_SH3           0xF3
#define SHIFT_N1            0xF5
#define SHIFT_N2            0xF6
#define SHIFT_N3            0xF7
#define INVALID             0xFF


// ASCII character codes
#define ASCII_SOH           0x01
#define ASCII_STX           0x02
#define ASCII_ETX           0x03
#define ASCII_EOT           0x04
#define ASCII_ENQ           0x05
#define ASCII_ACK           0x06
#define ASCII_BEL           0x07
#define ASCII_BS            0x08
#define ASCII_TAB           0x09
#define ASCII_LF            0x0a
#define ASCII_FF            0x0c
#define ASCII_CR            0x0d
#define ASCII_DLE           0x10
#define ASCII_XON           0x11
#define ASCII_XOFF          0x13
#define ASCII_NAK           0x15
#define ASCII_DEL           0x18
#define ASCII_EM            0x19
#define ASCII_GS            0x1d

#define ASCII_FILLER        0xff


// Hardware platform definitions
#define HARDWARE_UNKNOWN	0	// Not possible to detect hardware
#define HARDWARE_ACS		1	// Advansed Charge Station
#define HARDWARE_D2100		HARDWARE_ACS
#define HARDWARE_M2100		2	// M2100 terminal
#define HARDWARE_T2100		3	// T2100 terminal
#define HARDWARE_P2100		4	// P2100 terminal


//  Extract the low order byte of UINT 'x'
#define LOWBYTE(word)       ((word) & 0xff)

//  Extract the high order byte of UINT 'x'
#define HIGHBYTE(word)      LOWBYTE((word) >> 8)

//  Computes the dimension of an array 'x'
#define DIM(x)              (sizeof(x) / sizeof(x[0]))

// Macros for min and max value selection
#if !defined(MIN)
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#endif
