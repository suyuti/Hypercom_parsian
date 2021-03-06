
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             totals.c
//      Calculate Totals routines
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "tlib.h"

#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "findtran.h"
#include "totals.h"
#include "journal.h"

//=============================================================================
// External variables / declarations
//=============================================================================



//=============================================================================
// Private defines and typedefs
//=============================================================================

#define BDF_YES     0x01		// Business Date Selection Process
#define BDF_FIRST   0x02		// First Time Through Flag


//=============================================================================
// Private function declarations
//=============================================================================

static void CalcTotals_Common( void );
static void CalcTotals_AllKinds( void );
static void CalcTotals_Credit( void );
static void CalcTotals_Debit( void );
static Bool CalcTotals_IsRestKey( void );
static void CalcTotals_TipPct( void );
static void StdCalcTotals_Common( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static UWORD Ident;
static UBYTE BusDTFlg;

static UBYTE BUSDT[S_TRBUSDT];			// Business Date

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals based on JSREC structure.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void CalcTotals_Jsrec( void )
{
	// Reset Business Date Processing flag 
	BusDTFlg = 0x00;

	// Reset the Identifier to zero 
	Ident = 0x0000;

	// Calculate Totals 
	CalcTotals_Common(  );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals based on default JSREC.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void CalcTotals( void )
{
	// Reset Business Date Processing flag 
	BusDTFlg = 0x00;

	// Reset the Identifier to zero 
	Ident = 0x0000;

	// Lock the Journal 
	JournalLock ();

	// Set up selection data in common record JSREC         
	SetDefaultSelection ();

	// Don't look at reversals or settled records.  
	JSREC.JSSTATOFF = TS1_REV + TS1_SETL;

	// Look only for the acquirer defined in TRINP.  
	JSREC.JSAQID = TRINP.TRAQID;

	// Look only for the issuer defined in TRINP.  
	JSREC.JSISID = TRINP.TRISID;

	// Calculate Totals 
	CalcTotals_Common(  );

	// Release Journal 
	JournalRelease ();
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals for one or all acquirers.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void StdCalcTotals( void )
{
	// Reset the Identifier to zero 
	Ident = 0x0000;

	// Lock the Journal 
	JournalLock ();

	// Set up selection data in common record JSREC         
	SetDefaultSelection ();

	// Don't look at reversals or settled records or at voids.  
	JSREC.JSSTATOFF = TS1_REV + TS1_SETL + TS1_VOID;

	// Look only for the acquirer defined in TRINP.  
	JSREC.JSAQID = TRINP.TRAQID;

	// Look only for the issuer defined in TRINP.  
	JSREC.JSISID = TRINP.TRISID;

	// Calculate Totals 
	StdCalcTotals_Common(  );

	// Release Journal 
	JournalRelease ();
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals based on JSREC.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas: JSREC, Ident, BusDTFlg
//!     
//!
//-----------------------------------------------------------------------------
static void CalcTotals_Common( void )
{
	Bool FirstRead;
    static UWORD JrnIdent;

	// Display "SCANNING BATCH/PLEASE WAIT" 
	ShowInfoMsg( ScanningBatch, PleaseWait );

	// Clear out the Totals area 
	memset( ( UBYTE * ) & TERMTOTALS, 0, TOTSZE );

	// Set the first read flag 
	FirstRead = True;

	// Is this Business Date Processing 
	if ( BusDTFlg & BDF_YES )
	{
		// Yes; is there an Identifier already using it 
		if ( Ident )
		{
			// Yes; reset the first read flag 
			FirstRead = False;
		}
	}

	// See if the first record needs to be read 
	if ( FirstRead )
	{
		// Yes; select the first record 
		JrnIdent = JournalSelFirst ();

		// See if any entries in the Journal 
		if ( 0x0000 == JrnIdent )
		{
			// Calculate the Total Tip Percentage of Total-Tip 
			CalcTotals_TipPct(  );

			// Return to caller 
			return;
		}

		// Yes; update the Identifier 
		Ident = JrnIdent;
	}

	// Loop till done 
	while ( True )
	{
		// Read the record that was found  
		JournalRead ( Ident, &JRREC );

		// Is this Business Date Processing 
		if ( BusDTFlg & BDF_YES )
		{
			// Yes; see if first time through 
			if ( BusDTFlg & BDF_FIRST )
			{
				// Yes; reset first time through flag 
				BusDTFlg &= ~BDF_FIRST;

				// Save the Business Date 
				memcpy( BUSDT, JRREC.TRBUSDT, S_TRBUSDT );
			}
			else
			{
				// No; memcmpe the dates 
				if ( memcmp( JRREC.TRBUSDT, BUSDT, S_TRBUSDT ) )
				{
					// Not the same; exit the loop 
					break;
				}
			}
		}

		// Get the transaction from the journal record read in 
		SecondEvent.TRNKEY = JRREC.TRKEY;

		// Look for the transaction 
		FindTran ();

		// Are we doing REPORTS transaction? 
		if ( REPORTS == TRINP.TRKEY )
		{
			// Yes; UNADJ Report? 
			if ( RPT_UNADJ == RPTOPTS )
			{
				// Yes; is transaction a not a Credit? 
				if ( !( SecondEvent.TRNOPT1 & T1_CREDIT ) )
				{
					// Yes; see if Tip Amount is zero 
					if ( NullComp( ( char * ) JRREC.TRTIP, S_TRTIP ) )
					{
						// Yes; perform common processing 
						CalcTotals_AllKinds(  );
					}
				}
			}
			else
			{
				// No; perform common processing 
				CalcTotals_AllKinds(  );
			}
		}
		else
		{
			// No; printing Till Report? 
			if ( PRTTILL == TRINP.TRKEY )
			{
				// Yes; is Till Number the same? 
				if ( !memcmp( JRREC.TRTILLNO, TRINP.TRTILLNO, S_TRTILLNO ) )
				{
					// Yes; perform common processing 
					CalcTotals_AllKinds(  );
				}
			}
			else
			{
				// No; perform common processing 
				CalcTotals_AllKinds(  );
			}
		}

		// Select the next record 
		JrnIdent = JournalSelNext ( JrnIdent );

		// See if found in the Journal 
		if ( 0x0000 == JrnIdent )
		{
			// No; reset the Identifier 
			Ident = 0x0000;

			// Exit the loop 
			break;
		}
		else
		{
			// Yes; save the Identifier 
			Ident = JrnIdent;
		}
	}

	// Calculate the Total Tip Percentage of Total-Tip 
	CalcTotals_TipPct(  );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate all kinds of totals 
//!
//!  \param
//!     None.
//!
//!  \return
//!     Nothing.
//!
//!  \note
//!     Global Inputs: JSREC.
//!
//-----------------------------------------------------------------------------
static void CalcTotals_AllKinds( void )
{

	// See if a Credit 
	if ( SecondEvent.TRNOPT1 & T1_CREDIT )
	{
		// Calculate Credit 
		CalcTotals_Credit(  );
		return;
	}

	// calculate Debit 
	CalcTotals_Debit(  );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals Credit.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globals: JRREC. Updates TERMTOTALS.TOTBASIC.
//!
//-----------------------------------------------------------------------------
static void CalcTotals_Credit( void )
{
	// See if VOID Credit 
	if ( JRREC.TRSTATUS[0] & TS1_VOID )
	{
		// Yes; increment Voided Credits Count 
		Bcd2( TERMTOTALS.TOTBASIC.TOTCRVCNT );

		// Accumulate Totals 
		AddAmt( TERMTOTALS.TOTBASIC.TOTCRVAMT,
				TERMTOTALS.TOTBASIC.TOTCRVAMT, JRREC.TRTOTAM );
	}
	else
	{
		// No; increment Credits Count 
		Bcd2( TERMTOTALS.TOTBASIC.TOTCRCNT );

		// Accumulate Totals 
		AddAmt( TERMTOTALS.TOTBASIC.TOTCRAMT,
				TERMTOTALS.TOTBASIC.TOTCRAMT, JRREC.TRTOTAM );
	}
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals Debit.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas: JRREC. Updates TERMTOTALS.TOTBASIC.
//!
//-----------------------------------------------------------------------------
static void CalcTotals_Debit( void )
{
	// See if VOID Debit 
	if ( JRREC.TRSTATUS[0] & TS1_VOID )
	{
		// Yes; increment Voided Debits Count 
		Bcd2( TERMTOTALS.TOTBASIC.TOTDBVCNT );

		// Accumulate Totals 
		AddAmt( TERMTOTALS.TOTBASIC.TOTDBVAMT,
				TERMTOTALS.TOTBASIC.TOTDBVAMT, JRREC.TRTOTAM );
	}
	else
	{
		// No; increment Debits Count 
		Bcd2( TERMTOTALS.TOTBASIC.TOTDBCNT );

		// Accumulate Totals 
		AddAmt( TERMTOTALS.TOTBASIC.TOTDBAMT,
				TERMTOTALS.TOTBASIC.TOTDBAMT, JRREC.TRTOTAM );

		// Accumulate Cashback Totals 
		AddAmt( TERMTOTALS.TOTCBAMT, TERMTOTALS.TOTCBAMT, JRREC.TRCASHBK );

		// See if Restaurant 
		if ( CalcTotals_IsRestKey(  ) )
		{
			// Yes; accumulate Base Totals 
			AddAmt( TERMTOTALS.TOTBASEAMT, TERMTOTALS.TOTBASEAMT,
					JRREC.TRBASE );

			// Accumulate Tax Totals 
			AddAmt( TERMTOTALS.TOTTAXAMT, TERMTOTALS.TOTTAXAMT, JRREC.TRTAX );

			// Accumulate Bar Totals 
			AddAmt( TERMTOTALS.TOTBARAMT, TERMTOTALS.TOTBARAMT, JRREC.TRBAR );

			// Accumulate Tip Totals 
			AddAmt( TERMTOTALS.TOTTIPAMT, TERMTOTALS.TOTTIPAMT, JRREC.TRTIP );
		}
	}
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Does this Transaction require Base+Tip
//!
//!  \param
//!     None.
//!
//!  \return
//!     Bool        True  - Transaction requires Base+Tip,
//!                 False - otherwise.
//!
//!  \note
//!     Globals: SecondEvent and TCONF.
//!
//-----------------------------------------------------------------------------
static Bool CalcTotals_IsRestKey( void )
{
	// See if Tips allowed 
	if ( !( SecondEvent.TRNOPT1 & T1_NOTIP ) )
	{
		// Yes; see if Restaurant Mode Selected 
		if ( TCONF.TCOPT1 & TC1_REST )
		{
			// Yes; Transaction requires Base+Tip 
			return ( True );
		}
	}

	// Transaction does not require Base+Tip 
	return ( False );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate the Total Tip Percentage of (Total-Tip)
//!
//!  \param
//!     None.
//!
//!  \return
//!     None
//!
//!  \note
//!     Value returned in TERMTOTALS.
//!
//-----------------------------------------------------------------------------
static void CalcTotals_TipPct( void )
{
	UBYTE HoldAmt[6];

	// First compute Base-Tip 
	SubAmt( HoldAmt, TERMTOTALS.TOTBASIC.TOTDBAMT, TERMTOTALS.TOTTIPAMT );

	// Compute the Percentage X/Y 
	Percent0( TERMTOTALS.TOTTIPPCT, TERMTOTALS.TOTTIPAMT, HoldAmt );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Calculate Totals in 'standard format'.
//!
//!  \param
//!     None.
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void StdCalcTotals_Common( void )
{
    UWORD Ident;

	// Display "SCANNING BATCH/PLEASE WAIT" 
	ShowInfoMsg( ScanningBatch, PleaseWait );

	// Clear out the Totals area 
	memset( ( UBYTE * ) & STDTOTALS, 0,STDTOTSZE );

	// select the first record if it exists.  
	Ident = JournalSelFirst ();

	// See if any entries in the Journal 
	if ( 0x0000 == Ident )
	{
		// Display scanning msg for 1 sec then return to caller 
		SDK_Wait( ONESECOND * 1 );
		return;
	}

	// Loop till done 
	while ( True )
	{
		// Read the record that was found  
		JournalRead ( Ident, &JRREC );

		// Get the transaction from the journal record read in 
		SecondEvent.TRNKEY = JRREC.TRKEY;

		// Look for the transaction 
		FindTran ();

		// Update standard totals structure based on transaction type. 
		// Check if Debit card transactions. 
		if ( ( DEBIT == JRREC.TRKEY ) || ( SALCASH == JRREC.TRKEY )
			 || ( REFDEBIT == JRREC.TRKEY ) || ( DBCASH == JRREC.TRKEY ) )
		{
			if ( SecondEvent.TRNOPT1 & T1_CREDIT )
			{
				// Increment debit refund count and amount.  
				Bcd2( STDTOTALS.TOTDBREFCNT );
				AddAmt( STDTOTALS.TOTDBREFAMT, STDTOTALS.TOTDBREFAMT,
						JRREC.TRTOTAM );
			}
			else
			{
				// Increment debit sales count and amount.  
				Bcd2( STDTOTALS.TOTDBSALCNT );
				AddAmt( STDTOTALS.TOTDBSALAMT, STDTOTALS.TOTDBSALAMT,
						JRREC.TRTOTAM );
			}

		}
		else
		{
			if ( SecondEvent.TRNOPT1 & T1_CREDIT )
			{
				// Increment refund count and amount.  
				Bcd2( STDTOTALS.TOTCPREFCNT );
				AddAmt( STDTOTALS.TOTCPREFAMT, STDTOTALS.TOTCPREFAMT,
						JRREC.TRTOTAM );
			}
			else
			{
				// Increment sales count and amount.  
				Bcd2( STDTOTALS.TOTCPSALCNT );
				AddAmt( STDTOTALS.TOTCPSALAMT, STDTOTALS.TOTCPSALAMT,
						JRREC.TRTOTAM );
			}
		}

		// Select the next record 
		Ident = JournalSelNext ( Ident );

		// See if found in the Journal 
		if ( 0x0000 == Ident )
		{
			// Exit the loop 
			break;
		}

    }

}


