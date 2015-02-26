
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
	{FFIX + ATTBIN, 64},		//  1-BITMAP, EXTENDED
	{FLLVAR + ATTN, 19},		//  2-PRIMARY ACCOUNT NUMBER
	{FFIX + ATTN, 6},			//  3-PROCESSING CODE
	{FFIX + ATTN, 12},			//  4-AMOUNT, TRANSACTION
	{FFIX + ATTN, 12},			//  5-AMOUNT, SETTLEMENT
	{FFIX + ATTN, 12},			//  6-AMOUNT, CARDHOLDER BILLING
	{FFIX + ATTN, 10},			//  7-TRANSMISSION DATE/TIME MMDDHHMMSS
	{FFIX + ATTN, 8},			//  8-AMOUNT, CARDHOLDER BILLING FEE
	{FFIX + ATTN, 8},			//  9-CONVERSION RATE, SETTLEMENT
	{FFIX + ATTN, 8},			// 10-CONVERSION RATE, CARDHOLDER BILLING
	{FFIX + ATTN, 6},			// 11-SYSTEMS TRACE AUDIT NUMBER
	{FFIX + ATTN, 6},			// 12-TIME, LOCAL TRANSACTION HHMMSS
	{FFIX + ATTN, 4},			// 13-DATE, LOCAL TRANSACTION MMDD
	{FFIX + ATTN, 4},			// 14-DATE, EXPIRATION YYMM
	{FFIX + ATTN, 4},			// 15-DATE, SETTLEMENT MMDD
	{FFIX + ATTN, 4},			// 16-DATE, CONVERSION MMDD
	{FFIX + ATTN, 4},			// 17-DATE, CAPTURE
	{FFIX + ATTN, 4},			// 18-MERCHANT'S TYPE
	{FFIX + ATTN, 3},			// 19-ACQUIRING INSTITUTION COUNTRY CODE
	{FFIX + ATTN, 3},			// 20-PRIMARY ACCOUNT NUMBER EXTENDED,
	//    COUNTRY CODE
	{FFIX + ATTN, 3},			// 21-FORWARDING INSTITUTION COUNTRY CODE
	{FFIX + ATTN, 3},			// 22-POINT OF SERVICE ENTRY MODE
	{FFIX + ATTN, 3},			// 23-CARD SEQUENCE NUMBER
	{FFIX + ATTN, 3},			// 24-NETWORK INTERNATIONAL IDENTIFIER
	{FFIX + ATTN, 2},			// 25-POINT OF SERVICE CONDITION CODE
	{FFIX + ATTN, 2},			// 26-POINT OF SERVICE PIN CAPTURE CODE
	{FFIX + ATTN, 1},			// 27-AUTHORIZATION IDENTIFICATION
	//    RESPONSE LENGTH
	{FFIX + ATTN, 8},			// 28-AMOUNT, TRANSACTION FEE  X+N
	{FFIX + ATTN, 8},			// 29-AMOUNT, SETTLEMENT FEE   X+N
	{FFIX + ATTN, 8},			// 30-AMOUNT, TRANSACTION PROCESSING FEE  X+N
	{FFIX + ATTN, 8},			// 31-AMOUNT, SETTLEMENT PROCESSING FEE
	{FLLVAR + ATTN, 11},		// 32-ACQUIRING INSTITUTION
	//    IDENTIFICATION CODE
	{FLLVAR + ATTN, 11},		// 33-FORWARDING INSTITUTION
	//    IDENTIFICATION CODE
	{FLLVAR + ATTN, 28},		// 34-PRIMARY ACCOUNT NUMBER, EXTENDED
	{FLLVAR + ATTN, 37},		// 35-TRACK 2 DATA
	{FLLLVAR + ATTN, 104},		// 36-TRACK 3 DATA
	{FFIX + ATTAN, 12},			// 37-RETRIEVAL REFERENCE NUMBER
	{FFIX + ATTAN, 6},			// 38-AUTHORIZATION IDENTIFICATION RESPONSE
	{FFIX + ATTAN, 2},			// 39-RESPONSE CODE
	{FFIX + ATTAN, 3},			// 40-SERVICE RESTRICTION CODE
	{FFIX + ATTANS, 8},			// 41-CARD ACCEPTOR TERMINAL  IDENTIFICATION
	{FFIX + ATTANS, 15},		// 42-CARD ACCEPTOR IDENTIFICATION CODE
	{FFIX + ATTANS, 40},		// 43-CARD ACCEPTOR NAME LOCATION
	{FLLVAR + ATTANS, 25},		// 44-ADDITIONAL RESPONSE DATA
	{FLLVAR + ATTANS, 76},		// 45-TRACK 1 DATA
	{FLLLVAR + ATTANS, 999},	// 46-ADDITIONAL DATA - ISO
	{FLLLVAR + ATTANS, 999},	// 47-ADDITIONAL DATA - NATIONAL
	{FLLLVAR + ATTANS, 999},	// 48-ADDITIONAL DATA - PRIVATE
        //{FFIX + ATTN, 3},			// 49-CURRENCY CODE, TRANSACTION // YTL fix 
	{FFIX + ATTAN, 2},			// 49-CURRENCY CODE, TRANSACTION
	{FFIX + ATTAN, 3},			// 50-CURRENCY CODE, SETTLEMENT
	{FFIX + ATTAN, 3},			// 51-CURRENCY CODE, CARDHOLDER BILLING
	{FFIX + ATTBIN, 64},		// 52-PERSONAL IDENTIFICATION NUMBER
	//    (PIN) DATA
	{FFIX + ATTN, 16},			// 53-SECURITY RELATED CONTROL INFORMATION
	{FLLLVAR + ATTAN, 120},		// 54-ADDITIONAL AMOUNTS
	{FLLLVAR + ATTANS, 999},	// 55-RESERVED ISO
	{FLLLVAR + ATTANS, 999},	// 56-RESERVED ISO
	{FLLLVAR + ATTANS, 999},	// 57-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},	// 58-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},	// 59-RESERVED NATIONAL
	{FLLLVAR + ATTANS, 999},	// 60-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},	// 61-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},	// 62-RESERVED PRIVATE
	{FLLLVAR + ATTANS, 999},	// 63-RESERVED PRIVATE
	{FFIX + ATTBIN, 64}			// 64-MESSAGE AUTHENTICATION CODE FIELD
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  FindBit         Return ptr to Bit Field Desciption
//
//  Parameters:     UBYTE BitNum            bit number
//
//  Global Inputs:
//
//  Returns:        struct is8583_rec *     Pointer to struct is8583_rec
//
//  Notes:
//
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
