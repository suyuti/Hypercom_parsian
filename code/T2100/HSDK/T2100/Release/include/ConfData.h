
///////////////////////////////////////////////////////////////////////////
//
// Module   : CONF_DATA
// Part of  : Booter HFT325 Project, EFTOS 9
// Created  : Bengt Westman
// Abstract : Header file for module CONF_DATA
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
// 030416  JB	   Original version
// 030821  JB      Added USB & BLUETOOTH as possible COM. Modules
// 040416  JB      Changed to fit new HW / Format
// 040621  JB	   Fixed typos, no functional change
// 040629  ASA     Changed for GNU C and ADS 1.2 support
// 041001  JB      Added GSM with external antenna as comboard
// 041118  JB      Added parameter 6, comboard extra features
// 050512  RZ      Added LAN 485 as possible COM Module
// 051220  RZ      Added GSM/GPRS Dial Backup Module
// 060328  RZ      Added WIFI communication module (COM_WIFI)
//                 and extra definition for Bluetooth Multi-Point hardware (COM_BLUETOOTH_MP)
// 060814  RZ      Added extra definition for Bluetooth module (COM_BLUETOOTH_ABSENT)
//
///////////////////////////////////////////////////////////////////////////

#ifndef CONFDATA_H
#define CONFDATA_H

#ifndef BASIC_TYPE_DEFINED
typedef unsigned long   DWORD;
#endif  


// Define entries in Configuration Parameter table
#define NO_CONF_PAR_ENTRY        (16)
#define CONF_PAR_RS1_CONFIG            0
#define CONF_PAR_RS2_CONFIG	           1
#define CONF_PAR_RS0TYPE_CONFIG        2
#define CONF_PAR_COM_CONFIG            3
#define CONF_PAR_MSR_CONFIG            4
#define CONF_PAR_FLASH_CONFIG          5
#define CONF_PAR_COMEXTA_CONFIG        6

// Configuration for parameter table
// P1 (RS1):
#define RS1_NOTPRESENT   0	
#define RS1_PRESENT      1

// P2 (RS2):
#define RS2_NOTPRESENT   0	
#define RS2_PRESENT      1

// P3 (RS0TYPE):
#define RS232			 0
#define RS422            1

// P4 (COM):
#define COM_NOTPRESENT   0x000
#define COM_MODEM_V22    0x001
#define COM_ISDN         0x002
#define COM_GSM          0x004
#define COM_MODEM_V90    0x008
#define COM_USB		     0x010
#define COM_BLUETOOTH	 0x020
#define COM_10BASET  	 0x040
#define COM_GSM_EXT		 0x080
#define COM_LAN			 0x100
#define COM_GSM_V90		 0x200
#define COM_WIFI		 0x400

// P5 (MSR):
#define MSR_NOTPRESENT   0
#define MSR_TRACK_1      1 
#define MSR_TRACK_2      2
#define MSR_TRACK_3      4
#define MSR_TRACK_12     (1+2)
#define MSR_TRACK_13     (1+4)
#define MSR_TRACK_23     (2+4)
#define MSR_TRACK_123    (1+2+4)

// P6 (DATAFLASH):
// Size of Data Flash area given in number of 64 kbyte
#define DATA_FLASH_0MB   0
#define DATA_FLASH_1MB   16
#define DATA_FLASH_2MB   32

// P7 (COM EXTR):
//Communication board extra parameters
#define COM_EXTRA_NONE			0x00
#define COM_GSM_REVB			0x01
#define COM_BLUETOOTH_MP		0x02
#define COM_BLUETOOTH_ABSENT	0x04





#ifdef __ARMCC_VERSION
// 0x4000100
#pragma arm section zidata = "Prodconfig"
#endif
extern DWORD CONF_ParTable[NO_CONF_PAR_ENTRY];
#ifdef __ARMCC_VERSION
#pragma arm section zidata
#endif

#endif
