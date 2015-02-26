//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             is8583.c
//      IS8583 Table Definitions
//
//=============================================================================
#include "string.h"

#include "basictyp.h"

#include "defines.h"
#include "struct.h"
#include "is8583.h"

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

// The following table defines the attributes associated
// with each bit of an IS8583 message.  All bits 1-64 
// are defined.

const struct is8583_rec IS8583_TAB[] = {
    //! 1-BITMAP, EXTENDED
	{FFIX + ATTBIN, 64},
    //! 2-PRIMARY ACCOUNT NUMBER
	{FLLVAR + ATTN, 19},
    //! 3-PROCESSING CODE
	{FFIX + ATTN, 6},
    //! 4-AMOUNT, TRANSACTION
	{FFIX + ATTN, 12},
    //! 5-AMOUNT, SETTLEMENT
	{FFIX + ATTN, 12},
    //! 6-AMOUNT, CARDHOLDER BILLING
	{FFIX + ATTN, 12},
    //! 7-TRANSMISSION DATE/TIME MMDDHHMMSS
	{FFIX + ATTN, 10},
    //! 8-AMOUNT, CARDHOLDER BILLING FEE
	{FFIX + ATTN, 8},
    //! 9-CONVERSION RATE, SETTLEMENT
	{FFIX + ATTN, 8},
    //! 10-CONVERSION RATE, CARDHOLDER BILLING
	{FFIX + ATTN, 8},
    //! 11-SYSTEMS TRACE AUDIT NUMBER
	{FFIX + ATTN, 6},
    //! 12-TIME, LOCAL TRANSACTION HHMMSS
	{FFIX + ATTN, 6},
    //! 13-DATE, LOCAL TRANSACTION MMDD
	{FFIX + ATTN, 4},
    //! 14-DATE, EXPIRATION YYMM
	{FFIX + ATTN, 4},
    //! 15-DATE, SETTLEMENT MMDD
	{FFIX + ATTN, 4},
    //! 16-DATE, CONVERSION MMDD
	{FFIX + ATTN, 4},
    //! 17-DATE, CAPTURE
	{FFIX + ATTN, 4},
    //! 18-MERCHANT'S TYPE
	{FFIX + ATTN, 4},
    //! 19-ACQUIRING INSTITUTION COUNTRY CODE
	{FFIX + ATTN, 3},
    //! 20-PRIMARY ACCOUNT NUMBER EXTENDED,
	{FFIX + ATTN, 3},

    //! COUNTRY CODE
    //! 21-FORWARDING INSTITUTION COUNTRY CODE
	{FFIX + ATTN, 3},
    //! 22-POINT OF SERVICE ENTRY MODE
	{FFIX + ATTN, 3},
    //! 23-CARD SEQUENCE NUMBER
	{FFIX + ATTN, 3},
    //! 24-NETWORK INTERNATIONAL IDENTIFIER
	{FFIX + ATTN, 3},
    //! 25-POINT OF SERVICE CONDITION CODE
	{FFIX + ATTN, 2},
    //! 26-POINT OF SERVICE PIN CAPTURE CODE
	{FFIX + ATTN, 2},
    //! 27-AUTHORIZATION IDENTIFICATION
	{FFIX + ATTN, 1},

    //! RESPONSE LENGTH
    //! 28-AMOUNT, TRANSACTION FEE  X+N
	{FFIX + ATTN, 8},
    //! 29-AMOUNT, SETTLEMENT FEE   X+N
	{FFIX + ATTN, 8},
    //! 30-AMOUNT, TRANSACTION PROCESSING FEE  X+N
	{FFIX + ATTN, 8},
    //! 31-AMOUNT, SETTLEMENT PROCESSING FEE
	{FFIX + ATTN, 8},
    //! 32-ACQUIRING INSTITUTION
	{FLLVAR + ATTN, 11},

    //! IDENTIFICATION CODE
    //! 33-FORWARDING INSTITUTION
	{FLLVAR + ATTN, 11},

    //! IDENTIFICATION CODE
    //! 34-PRIMARY ACCOUNT NUMBER, EXTENDED
	{FLLVAR + ATTN, 28},
    //! 35-TRACK 2 DATA
	{FLLVAR + ATTN, 37},
    //! 36-TRACK 3 DATA
	{FLLLVAR + ATTN, 104},
    //! 37-RETRIEVAL REFERENCE NUMBER
	{FFIX + ATTAN, 12},
    //! 38-AUTHORIZATION IDENTIFICATION RESPONSE
	{FFIX + ATTAN, 6},
    //! 39-RESPONSE CODE
	{FFIX + ATTAN, 2},
    //! 40-SERVICE RESTRICTION CODE
	{FFIX + ATTAN, 3},
    //! 41-CARD ACCEPTOR TERMINAL  IDENTIFICATION
	{FFIX + ATTANS, 8},
    //! 42-CARD ACCEPTOR IDENTIFICATION CODE
	{FFIX + ATTANS, 15},
    //! 43-CARD ACCEPTOR NAME LOCATION
	{FFIX + ATTANS, 40},
    //! 44-ADDITIONAL RESPONSE DATA
	{FLLVAR + ATTANS, 25},
    //! 45-TRACK 1 DATA
	{FLLVAR + ATTANS, 76},
    //! 46-ADDITIONAL DATA - ISO
	{FLLLVAR + ATTANS, 999},
    //! 47-ADDITIONAL DATA - NATIONAL
	{FLLLVAR + ATTANS, 999},
    //! 48-ADDITIONAL DATA - PRIVATE
	{FLLLVAR + ATTANS, 999},
    //! 49-CURRENCY CODE, TRANSACTION
	{FFIX + ATTAN, 3},
    //! 50-CURRENCY CODE, SETTLEMENT
	{FFIX + ATTAN, 3},
    //! 51-CURRENCY CODE, CARDHOLDER BILLING
	{FFIX + ATTAN, 3},
    //! 52-PERSONAL IDENTIFICATION NUMBER
	{FFIX + ATTBIN, 64},

    //! (PIN) DATA
    //! 53-SECURITY RELATED CONTROL INFORMATION
	{FFIX + ATTN, 16},
    //! 54-ADDITIONAL AMOUNTS
	{FLLLVAR + ATTAN, 120},
    //! 55-RESERVED ISO
	{FLLLVAR + ATTANS, 999},
    //! 56-RESERVED ISO
	{FLLLVAR + ATTANS, 999},
    //! 57-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},
    //! 58-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},
    //! 59-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},
    //! 60-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},
    //! 61-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},
    //! 62-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},
    //! 63-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},
    //! 64-MESSAGE AUTHENTICATION CODE FIELD
	{FFIX + ATTBIN, 64}
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Finds ptr to Bit Field Desciption
//!
//! \param
//!		[in]BitNum			bit number
//!
//! \return
//!     Pointer to struct is8583_rec or NULL if not found
//-----------------------------------------------------------------------------
extern struct is8583_rec *FindBit( UBYTE BitNum )
{
	struct is8583_rec *pIs8583Rec;

	// Check if valid bit number
	if ( ( BitNum < 1 ) || ( BitNum > 64 ) )
	{
		// No; error
		pIs8583Rec = NULL;
	}
	else
	{
		// Yes; bit found
		pIs8583Rec = ( struct is8583_rec * ) &IS8583_TAB[BitNum - 1];
	}

	return ( pIs8583Rec );
}

//=============================================================================
// Private function definitions
//=============================================================================

