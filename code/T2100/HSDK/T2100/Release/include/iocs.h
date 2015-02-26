/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __IOCS_H
#define __IOCS_H

#include "execdef.inc"
#include "ddtm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	@defgroup GroupIOCS IOCS/PTCS Emulation Layer Public Interface
	@{
*/

#ifdef MAKE_OPTIMUM

// undefine items from the legacy .inc files that are unused or 
// incompatible with ported CPOS code

#undef	SOH
#undef	STX
#undef	ETX
#undef	EOT
#undef	ENQ
#undef	ACK
#undef	AC2
#undef	DLE
#undef	NAK
#undef	SYN
#undef	ETB
#undef	IFS
#undef	US
#undef	DC2
#undef	ABORT

typedef struct
{
	void (*entry_point)();
	char Name[6];
	char IOPendingFlag;
	NU_TASK *NuAppTask;	
} PCBDATA; 

extern PCBDATA pcb[];

#endif

/**
	@brief Access to input/output control system.

	This function is the link from the application to the IOCS control
	system in the terminal operating system. All IO requests made through
	the SDK functions invoke this function.

	@param pIOCSBlock Pointer to requested command block.
*/
UBYTE iocs( IOCS_BLK* pIOCSBlock );

/**
	@brief Initialize IOCS emulation layer.

	This function initializes all the devices in the system.
*/
void InitDevices( void );

/**
	@internal

	@brief Called by hardware specific code when input is received from a
	       device.

	@param devnum Device number (PRT, PIN, etc.).
	@param data Pointer to the data block.
	@param length Lenght of data block received.
*/
void InputReceived( unsigned char devnum, unsigned char errCode, UBYTE *data, unsigned int length );

/**
	@internal

	@brief Called by hardware specific code when a pending request is
	       complete for a given device.

	@param devnum Device number (PRT, PIN, etc.).
	@param data Pointer to the data block.
	@param length Lenght of data block.
*/
void ConfigRequestComplete( unsigned char devnum, UBYTE *data, unsigned int length );

/**
	@internal

	@brief Called by hardware specific code when a pending request is
	       complete for a given device.

	@param devnum Device number (PRT, PIN, etc.).
	@param status Status of write request completion.
*/
void WriteRequestComplete( unsigned char devnum, int status );

/**
	@internal

	@brief Process device responce and complete IOCS request.

	@param pIOCSBlock Pointer to processed command block.
*/
void ProcessDeviceResponse( IOCS_BLK *pIOCSBlock );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __IOCS_H
