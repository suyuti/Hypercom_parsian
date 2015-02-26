
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             textinit.c
//      Initialization for Dynamic Mutlilingual support.
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "evttextinit.h"
#include "hdttextinit.h"
#include "rsptextinit.h"
#include "pcltextinit.h"
#include "text.h"
#include "textinit.h"

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
//!      Initializes Multilingual Semaphore.
//!      Initializes the Database tables.
//!      Initializes ALL the tables (HDT, CHK, RSP, PIN, PCL)
//!
//! \param
//!		
//! \return
//!		None
//!
//! \note
//!		LNG_DEFAULT macro MUST be defined. See description above.
//!
extern void TextTables_Init( void )
{
	// Initialize MultiLingual Semaphore
	RelSem( &MLSemaphore );

	// Get MultiLingual Semaphore
	GetSem( &MLSemaphore );

	// Initialize Database
	TextDatabaseInit ();

	//
	// Always initialize PCL text and LANGID, PCL always uses Dynamic
	//

	// Initialize PCL TEXT database files  
	PclText_Init ();

	// Initialize default TEXT database files
	HdtText_Init(  );

	// Initialize event table TEXT database files
	EvtText_Init ();

	// Initialize response table TEXT database files
	RspText_Init ();

	// Release MultiLingual Semaphore
	RelSem( &MLSemaphore );
}


//=============================================================================
// Private function definitions
//=============================================================================
