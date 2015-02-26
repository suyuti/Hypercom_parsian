///////////////////////////////////////////////////////////////////////////////////////
//
//      OBJECT DESCRIPTION:
//              Wrapper functions for OS calls
//
//      CREATED ON:
//              23/08/2006 21:31:10 PM
//
//      COPYRIGHT NOTICE:
//              Copyright (c) 2006, by Hypercom, Inc., Phoenix, Arizona.
//
//      PROPRIETARY RIGHTS NOTICE:
//              All rights reserved.  This document and program contains proprietary
//              information of Hypercom, Inc., of Phoenix, Arizona, U.S.A., embodying
//              confidential information, ideas, and expressions, no part of which may
//              be reproduced or transmitted in any form or by any means, electronic,
//              mechanical, or otherwise, without the expressed, written permission of
//              Hypercom, Inc.
//
//              Please see http://www.hypercom.com for more information.
//
//      COMMENTS / NOTES:
//				
//
//
///////////////////////////////////////////////////////////////////////////////////////

#include "system_calls.h"
#define _INCLUDE_HYPERCOM_APPMGR_
#define _INCLUDE_HYPERCOM_SECPROV_
#include <HycDDL.h>

// Common functions
extern SEC_SPHANDLE CS_Open(const char* provName, short provVersion);

// The registered name of the default security provider
//! Security Provider names
const char* const SecurityProviderNames[] = 
{
	"DefSecProv",
	"StdSecProv"
};

//! Security Provider handle
static  SEC_SPHANDLE nHandle;

//const char SecProvName[] = "StdSecProv";

//-----------------------------------------------------------------------------
//!
//!      Get host terminal ID 
//!
//! \param
//!		dstBuffer	destination buffer
//!	\param
//!		sizeBuffer	buffer size
//!
//! \return
//!      buffer size in bytes
//!
//! \note
//!
extern unsigned char GetHostTID( char* dstBuffer, int sizeBuffer )
{
    return MgrGetHostTermID( dstBuffer, sizeBuffer );
}

//-----------------------------------------------------------------------------
//!
//!      Initialise the Security Provider
//!
//! \param
//!		dstBuffer	None
//!
//! \return
//!      True if succesful, otherwise false
//!
//! \note
//!
extern int InitSecurityProvider( void )
{
     // Open the cryptographic service
     nHandle = CS_Open( SecurityProviderNames[1], 0 );
     if ( nHandle.Handle ) return 1;
       else return 0;
}

//-----------------------------------------------------------------------------
//!
//!      Initialise the Security Provider
//!
//! \param
//!		dstBuffer	None
//!
//! \return
//!      True if succesful, otherwise false
//!
//! \note
//!
extern int GetSecurityProviderVersion( void )
{
        return Sec_GetProviderVersion( SecurityProviderNames[1] );
}
