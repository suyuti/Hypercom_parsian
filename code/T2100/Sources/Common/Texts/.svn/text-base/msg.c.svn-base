
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
//  GetMsg          Retrieve a message and store it in a user buffer
//
//  Parameters:     enum msg_id     message id, language id
//
//                  char *pBuf      where message is put
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

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
//  ShowEOTMsg      Display an informational message or IcePak 
//                  EOT image
//
//  Parameters:     enum msg_id     Mid - Informational Message id
//
//                  enum msg_id     Tid - Title Message.
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void ShowEOTMsg( enum msg_id Mid, enum msg_id Tid )
{
	ShowIcePak( Mid, Tid, IMAGE_EOT );
}


//-----------------------------------------------------------------------------
//  ShowIntervalMsg     Display an informational message or IcePak 
//                      interval image
//
//  Parameters:         enum msg_id     Mid - Informational Message id
//
//                      enum msg_id     Tid - Title Message.
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void ShowIntervalMsg( enum msg_id Mid, enum msg_id Tid )
{
	ShowIcePak( Mid, Tid, IMAGE_INTERVAL );
}

//=============================================================================
// Private function definitions
//=============================================================================
