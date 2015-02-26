
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

#ifdef MAKE_FARSI
#include "comdata.h" // LOCTAB
#endif

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

#ifdef MAKE_FARSI
    // Remap FARSI numbers 0x30->0x80...0x39->0x89
    int i;
    if( LOCTAB.LANGID == LNG_FA )
    {
        for ( i=0; i< sizeof(MLBuffer); i++ )
        {
            if( (MLBuffer[i] >= '0') && (MLBuffer[i] <= '9') )
            {
               MLBuffer[i] += 0x50;
            }
        }
    }
#endif

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
