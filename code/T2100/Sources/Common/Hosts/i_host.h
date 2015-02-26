
#ifndef _I_HOST_H_
#define _I_HOST_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             i_host.h
//      Host entry header file
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

// Host Table Entry Structure
typedef struct
{
	void ( *pHostFunc ) ( void );	// Address of Host Function
	    OS_RETURN_VALS( *pHostReq ) ( void );	// Address of Host Request Function
	void ( *pHostRsp ) ( OS_RETURN_VALS ReqCommStatus );	// Address of Host Response Function
	char HostName[11];			// Name of Host Function
}
HOSTENTRY;


//=============================================================================
// Public data declarations
//=============================================================================
extern HOSTENTRY const VHDT_HOST_TBL1[];
extern const int HOST_MARKER1;


extern HOSTENTRY *Host;

//=============================================================================
// Public function declarations
//=============================================================================


#endif // _I_HOST_H_
