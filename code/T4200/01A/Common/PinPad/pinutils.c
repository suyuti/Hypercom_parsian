//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pinutils.c
//      Pin pad utilities
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "msg.h"
#include "appdata.h"
#include "comdata.h"
#include "transdef.h"
#include "util.h"
#include "pinpad.h"
#include "tables.h"
#include "pinutils.h"

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
static const ENTRY_STR YNEntry =	// structure for YesNoEntry()
{
	N_NullStr,
	CustomMsg,
	CustomMsg2,
};


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Gets the PIN from the PINPAD, if required
//!
//! \return
//!     0 to abort the transaction,<br>
//!     1 to continue the transaction, pin not required,<br>
//!     2 to continue, pinpad was required and OK
//-----------------------------------------------------------------------------
extern UBYTE GetPIN( void )
{
	UBYTE chRetval;

	chRetval = 1;				// Continue the transaction

	// /* if not ICC Transaction check for issuer PIN entry mode */ // @AAMELIN
	if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
	{
		// Does the issuer want PIN entry?
		if ( !((TRINP.TRISPTR->ISOPT1) & IS1_PIN) )
			return chRetval;					// No
	}

	// Tell host we have PIN entry capability
	TRINP.TRPOSE[1] = ( TRINP.TRPOSE[1] & 0xF0 ) | 0x01;

	if ( !PinTranEnabled(  ) ) // Check if PIN PAD enabled.
		return chRetval;

	if ( !PinGetPin () )
	{
		SDK_Beeper( TENMS * 40 );
		chRetval = 0;			// Pinpad was required but was not successful.
	}
	else
	{
		chRetval = 2;			// Pinpad was required and successful.
	}

	return chRetval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Is use of the PIN Pad enabled for this transaction?
//!
//! \return
//!     True if PIN pad is enabled for this transaction otherwise False
//!
//! \note
//!     ISPINTRAN bit on indicates no pin required.
//-----------------------------------------------------------------------------
extern Bool PinTranEnabled( void )
{
	UBYTE	ispintran,isopt3;
	Bool bRetVal;

	ispintran = TRINP.TRISPTR->ISPINTRAN;
	isopt3 = TRINP.TRISPTR->ISOPT3;

	bRetVal = True;
	// Should we get the PIN for this transaction?

	switch ( TRINP.TRKEY )
	{
#ifdef	MAKE_EMV
        case EMV_SALE:
		case EMV_REFUND:
        case EMV_SALCASH:
        case EMV_CASH:
        case EMV_FALLBACK:
            // CVM list & term capabilities should be enough 
            // to determine that we should ask for PIN
            break;
#endif	//MAKE_EMV

	 	case SALE:                                       
 			if (ispintran & ISPIN_SALE)		/* No PIN? */ 
 				bRetVal = False;                        
	 		break;                                        

		case REFUND:
			if ( ( TCONF.TCOPT2 & TC2_REF_OFF ) ||	// Offline?
				 ( isopt3 & IS3_REF_OFF ) ||	// Offline?
				 ( ispintran & ISPIN_RETURN ) )	// No PIN?
				bRetVal = False;
			break;

		case SALCASH:
		case DBCASH:
			if ( ispintran & ISPIN_CASHBK )	// No PIN?
				bRetVal = False;
			break;

		case BALINQ:
			if ( ispintran & ISPIN_BALINQ )	// No PIN?
				bRetVal = False;
			break;

		case ADJUST:
		case ADJSALE:
		case ADJREFUND:
			if ( ispintran & ISPIN_ADJUST )	// No PIN?
				bRetVal = False;
			break;

		case VOIDTRAN:
			if ( ( TCONF.TCOPT2 & TC2_VOID_OFF ) ||	// Offline?
				 ( isopt3 & IS3_VOID_OFF ) ||	// Offline?
				 ( ispintran & ISPIN_VOID ) )	// No PIN?
				bRetVal = False;
			break;

		default:
			bRetVal = False;
			break;
	}

	return bRetVal;
}



//=============================================================================
// Private function definitions
//=============================================================================

