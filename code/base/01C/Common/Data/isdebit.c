//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             isdebit.c
//      Checks if transaction is a debit type

//=============================================================================
#include "string.h"
#include "basictyp.h"
#include "transdef.h"

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
//! \brief Checks if Transaction is a Debit Type (DEBIT, DBCASH, REFDEBIT or SALCASH)
//!
//! \param[in] Tran             Transaction to check
//!                             
//! \return
//!     True if Transaction is a Debit Type else False.
//!                             
//! \note
//!     Upon booting up, if checksum is wrong, application will report
//!     error and will not run.
//-----------------------------------------------------------------------------
extern Bool IsDebitTrans( enum transactions Tran )
{
	// See if Transaction is a Debit Type
	if ( ( DEBIT == Tran ) || ( DBCASH == Tran ) || ( REFDEBIT == Tran ) ||
		 ( SALCASH == Tran ) )
	{
		// Yes; return true
		return ( True );
	}
	else
	{
		// No; return false
		return ( False );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

