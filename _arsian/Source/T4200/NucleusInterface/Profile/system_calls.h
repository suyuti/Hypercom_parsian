///////////////////////////////////////////////////////////////////////////////////////
//
//      OBJECT DESCRIPTION:
//              Wrapper functions for OS calls
//
//      ORIGINAL AUTHOR:
//              Michael Kulakov <mkulakov@hypercom.com>
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

#ifndef _SYSTEM_CALLS_H_
#define _SYSTEM_CALLS_H_

//=============================================================================
// Public function declarations
//=============================================================================
//! Get host terminal ID 
unsigned char GetHostTID( char * dstBuffer, int sizeBuffer );

//! Initialise the Security Module  
extern int InitSecurityProvider( void );
//! Get the security provider version
extern int GetSecurityProviderVersion( void );
#endif // _SYSTEM_CALLS_H_

