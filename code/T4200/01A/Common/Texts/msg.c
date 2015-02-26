
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             msg.c
//      Message functions
//
//=============================================================================

#include "string.h"

#include "defines.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "msg.h"
#include "appdata.h"
#include "util.h"
#include "msgfunc.h"

#include "icepak.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Retrieve a message and store it in a user buffer
//!
//! \param
//!		Mid			message id, language id
//! \param
//!		pBuf	   	where message is put
//!				  		
//! \return
//!     None
//!
//! \note
//!
extern void GetMsg( enum msg_id Mid, char *pBuf )
{
	// Get MultiLingual Semaphore
	GetSem( &MLSemaphore );

	// Use GETMSG instead of TEXTGETSTRING to handle special cases.
	GETMSG ( Mid );

	// Save message in application buffer
	memcpy( pBuf, MLBuffer,
			( UWORD ) ( StrLn( MLBuffer, sizeof( MLBuffer ) ) + 1 ) );

	// Release MultiLingual Semaphore
	RelSem( &MLSemaphore );
}

//-----------------------------------------------------------------------------
//!
//!      Display an informational message or IcePak EOT image
//!
//! \param
//!		Mid			nformational Message id
//! \param
//!		Tid			Title Message
//!				  		
//! \return
//!     None
//!
//! \note
//!
extern void ShowEOTMsg( enum msg_id Mid, enum msg_id Tid )
{
	ShowIcePak( Mid, Tid, IMAGE_EOT );
}

//-----------------------------------------------------------------------------
//!
//!      Display an informational message or IcePak interval image
//!
//! \param
//!		Mid			nformational Message id
//! \param
//!		Tid			Title Message
//!				  		
//! \return
//!     None
//!
//! \note
//!
extern void ShowIntervalMsg( enum msg_id Mid, enum msg_id Tid )
{
	ShowIcePak( Mid, Tid, IMAGE_INTERVAL );
}

//=============================================================================
// Private function definitions
//=============================================================================
