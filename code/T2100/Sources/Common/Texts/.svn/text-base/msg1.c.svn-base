
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             msg1.c
//      Message table functions
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
//  TextGetString   Retrieve a message from an specific table, and 
//                  store the message in an user buffer.
//
//  Parameters:     char *pBuf          buffer to return the message.
//
//                  table_index Tid     table id to get the message.
//
//                  text_msg_id         Mid message index in the table.
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Use MLSemaphore.
//
//-----------------------------------------------------------------------------

extern void TextGetString( char *pBuf, table_index Tid, text_msg_id Mid )
{
	struct MLGetMsg_str MLGetMessage;

	// Get MultiLingual Semaphore
	GetSem( &MLSemaphore );

	// Set Table and Index of message to retrieve.
	MLGetMessage.tblid = Tid;
	MLGetMessage.msgid = Mid;

	// Get the message from the tables.
	TEXTGETSTRING ( &MLGetMessage );

	// Save message in application buffer.
	memcpy( pBuf, MLBuffer,
			( UWORD ) ( StrLn( MLBuffer, sizeof( MLBuffer ) ) + 1 ) );

	// Release MultiLingual Semaphore
	RelSem( &MLSemaphore );
}

//=============================================================================
// Private function definitions
//=============================================================================
