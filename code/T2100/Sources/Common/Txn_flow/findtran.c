
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             findtran.c
//      Find information related to a specific transaction type
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "eventbl.h"
#include "findtran.h"
#include "loyalty.h"

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
//  FindTran        Find information related to a specific transaction type
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          The caller will store the desired transaction type in
//                  the TRNKEY field of the global variable SecondEvent.
//                  The retrieved info is also stored in SecondEvent.
//                  If SecondEvent is completely empty on return, the
//                  desired transaction was not found in event_table.
//
//-----------------------------------------------------------------------------

extern void FindTran( void )
{
	if ( EVENT_TABLE[SecondEvent.TRNKEY].TRNKEY == SecondEvent.TRNKEY )
	{
		memcpy( ( struct event_entry * ) &SecondEvent,
				( struct event_entry * ) &EVENT_TABLE[SecondEvent.TRNKEY],
				sizeof( struct event_entry ) );

		// Updates Event message from Multilingual table
		TextGetString( ( char * ) &SecondEvent.TRNTEXT,
					   evttext_table_id, SecondEvent.TRNKEY );
                SetKIBTranName(SecondEvent.TRNTEXT, SecondEvent.TRNKEY);
	}
	else
	{
		memset( ( UBYTE * ) & SecondEvent, 0, sizeof( struct event_entry ) );
	}
}


//-----------------------------------------------------------------------------
//  HostFindTran    Find information related to a specific transaction type for
//                  the Host Process
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          The caller will store the desired transaction type in
//                  the TRNKEY field of the global variable HostEvent.
//                  The retrieved info is also stored in HostEvent.
//                  If HostEvent is completely empty on return, the
//                  desired transaction was not found in event_table.
//
//-----------------------------------------------------------------------------

extern void HostFindTran( void )
{
	if ( EVENT_TABLE[HostEvent.TRNKEY].TRNKEY == HostEvent.TRNKEY )
	{
		memcpy( ( struct event_entry * ) &HostEvent,
				( struct event_entry * ) &EVENT_TABLE[HostEvent.TRNKEY],
				sizeof( struct event_entry ) );

		// Updates Event message from Multilingual table
		TextGetString( ( char * ) &HostEvent.TRNTEXT,
					   evttext_table_id, HostEvent.TRNKEY );
	}
	else
	{
		memset( ( UBYTE * ) & HostEvent, 0, sizeof( struct event_entry ) );
	}
}



//=============================================================================
// Private function definitions
//=============================================================================
