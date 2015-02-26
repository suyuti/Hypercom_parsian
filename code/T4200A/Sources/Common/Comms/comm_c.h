//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
#ifndef _COMM_C_H_
#define _COMM_C_H_

//=============================================================================
//
//
// Module overview:
//  Data type and function declarations related to the COMM process based 
//	on the HPOSFC library
// The interface to the comms process is handled with COM_* functions and 
// one global variable - HPOSFC_COMERR
//=============================================================================

#include "basictyp.h"

#include "HypCConfig.hpp"
#include "HypEErrors.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Public defines and typedefs
//=============================================================================

extern int COM_HPOSFCERR;
extern void COM_ConnectReq();
extern int  COM_ConnectCompleted();
extern void COM_WriteReq(const void* buf , unsigned int len);
extern int  COM_WriteCompleted();
extern void COM_ReadReq(void* buf , unsigned short* len);
extern int  COM_ReadCompleted();
extern void COM_CancelRead();
extern void COM_HangUpReq();
extern void COM_HangUpSoonReq();
extern int  COM_HangUpCompleted();
extern int  COM_GetStatus();
extern void COM_ResetReq();
extern int  COM_ResetCompleted();
extern void COM_AddCfgParam(enum HypEConfigKeys k, int val);
extern void COM_AddCfgParamStr(enum HypEConfigKeys k, const char * s);
extern int  COM_GetLineStatus( );
extern int  COM_GetSignalLevel( );
extern void COM_CloseSessionReq();
extern int  COM_CloseSessionCompleted();
extern int COM_GetGateStatus( );
extern Bool IsStateSocketIdle( void );

extern void DBSwitchToAnalog(); //@AAMELIN
extern void DBSwitchToGSM(); //@AAMELIN


#ifdef __cplusplus
}
#endif



#endif // _COMM_C_H_
