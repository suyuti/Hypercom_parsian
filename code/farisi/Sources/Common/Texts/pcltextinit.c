
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pcltextinit.c
//      PCL table Init for Dyn Mutlilingual support.
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

#include "pcltextinit.h"


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
//!      Add PCL messages table for Dynamic Multilingual Support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
void PclText_Init( void )
{
	// Initialize dynamic language tables for pcl tables

	// Get id for event tables
	pcltext_tableDef_id = TextIndexNew(  );
	pcltext_tableExt_id = TextIndexNew(  );

#ifdef MAKE_ENGLISH
	PclTextEn_Init(  );
#endif

#ifdef MAKE_GERMAN
	PclTextGr_Init ();
#endif

#ifdef MAKE_SWEDISH
	PclTextSw_Init ();
#endif

#ifdef MAKE_SPANISH
	PclTextEs_Init ();
#endif

#ifdef MAKE_PORTUGUESE
	PclTextPt_Init ();
#endif

#ifdef MAKE_FARSI
	PclTextFa_Init ();
#endif

}


//=============================================================================
// Private function definitions
//=============================================================================
