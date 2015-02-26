
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rsptextinit.c
//      Response table Init for Dyn Mutlilingual support.
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

#include "rsptextinit.h"


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
//!      Add Response messages tables for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspText_Init( void )
{
	// Initialize dynamic language tables for event table 

	// Get id for event tables  
	rsptext_table_id = TextIndexNew(  );

#ifdef MAKE_ENGLISH
	RspTextEn_Init(  );
#endif

#ifdef MAKE_GERMAN
	RspTextGr_Init(  );
#endif

#ifdef MAKE_SWEDISH
	RspTextSw_Init(  );
#endif

#ifdef MAKE_PORTUGUESE
	RspTextPt_Init(  );
#endif

#ifdef MAKE_SPANISH
	RspTextEs_Init(  );
#endif

#ifdef MAKE_FARSI
	RspTextFa_Init(  );
#endif


}


//=============================================================================
// Private function definitions
//=============================================================================
