//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evttextinit.c
//
// File Description:
//      Event table Init for Dyn Mutlilingual support
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"

#include "struct.h"
#include "msg.h"
#include "appdata.h"
#include "rsptext.h"

#include "evttextinit.h"


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
//!      Initialize dynamic language tables for EvtText tables.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
void EvtText_Init( void )
{
	// Initialize dynamic language tables for event table

	// Get id for event tables
	evttext_table_id = TextIndexNew(  );

#ifdef MAKE_ENGLISH
	EvtTextEn_Init(  );
#endif

#ifdef MAKE_GERMAN
	EvtTextGr_Init(  );
#endif

#ifdef MAKE_SWEDISH
	EvtTextSw_Init(  );
#endif

#ifdef MAKE_SPANISH
	EvtTextEs_Init(  );
#endif

#ifdef MAKE_PORTUGUESE
	EvtTextPt_Init(  );
#endif

#ifdef MAKE_FARSI
	EvtTextFa_Init(  );
#endif

}


//=============================================================================
// Private function definitions
//=============================================================================
