//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdttextinit.c
//
// File Description:
//      HDT table Init for Dyn Mutlilingual support
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

#include "hdttextinit.h"


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
//!      Initialize dynamic language tables, for general purpose, and PIN use.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtText_Init( void )
{
	// Get id for event tables
	hdttext_table_id = TextIndexNew(  );

	// Get id for pin tables
	pintext_table_id = TextIndexNew(  );

#ifdef MAKE_ENGLISH
	HdtTextEn_Init(  );
#endif

#ifdef MAKE_GERMAN
	HdtTextGr_Init ();
#endif

#ifdef MAKE_SWEDISH
	HdtTextSw_Init ();
#endif

#ifdef MAKE_SPANISH
	HdtTextEs_Init ();
#endif

#ifdef MAKE_PORTUGUESE
	HdtTextPt_Init ();
#endif
#ifdef MAKE_FARSI
	HdtTextFa_Init ();
#endif
}


//=============================================================================
// Private function definitions
//=============================================================================
