
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     acctproc.c
//      Functions used to implement the Account Process functionality
//
//=============================================================================

#include "string.h"
#include "clib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "struct.h"
#include "appdata.h"
#include "utllib.h"

#include "defines.h"
#include "msg.h"
#include "util.h"
#include "util2.h"
#include "pinpad.h"
#include "tables.h"
#include "crdrange.h"
#include "chkbatch.h"
#include "app2.h"
#include "bit8583.h"

#include "acctproc.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

//=============================================================================
// Private function declarations
//=============================================================================
static void StoreAcct( void );
static void SavTrk1Name( void );
static UBYTE SetServCode(void);
static void SetValDate(void);
static UBYTE SetIssueNum(void);
static int isdigitstr(UBYTE *p, UBYTE len);

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
//!  \brief     Do acct validation & card range check
//!
//!  \param
//!     None
//!
//!  \return
//!       None
//!
//!  \note
//!     Global Inputs:  chRetval.   0 - invalid account was entered or card invalid
//!                                 1 - Card validated
//!     Puts "ISSUER    TRAN" in DefaultTitle[]
//-----------------------------------------------------------------------------

extern UBYTE AcctProc( void )
{
	UBYTE chRetval;

	// May have to retry if track 1 had an error, depending on issuer option 
	while ( True )
	{
#ifdef MAKE_EMV
		// We have ICC card, but not start it processing yet
		// Exit from function, we not ready to analyze PAN
		if (!( 0x50 == (TRINP.TRPOSE[1] & 0xF0)) && ChipCardInserted )
		{
			chRetval = 1;
			return chRetval;
		}
#endif

		/* We already have PAN if ICC Transaction */
		if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
		{
		    /* Store the Account Number (PAN) in TRINP.TRPAN */
		    StoreAcct();
		}

		chRetval = CheckCardRange(  );

		if ( !chRetval )
		{
			ShowErrMsg( UnsuppCard );
			return chRetval;
		}

		// Don't test mod-10 or manual entry if entry from idle.            
		// Must determine proper issuer, debit, etc. before checking.  
		if ( SALE != TRINP.TRKEY )
		{
			// Check if we need to perform MOD-10 verification 
			if ( TRINP.TRISPTR->ISOPT2 & IS2_MOD10 )
			{
				// Yes; validate the PAN using MOD-10 check
				if ( !ValidateAcct () )
				{
					ShowErrMsg( InvalidCard );
					return chRetval;
				}
			}

			// If Acct# was entered manually, we should check ISSUER option byte #1
			// to see if it is allowed 
			if ( INJECTKEY )
			{
                if(!((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN))
				{
					ShowErrMsg( NoManEntry );
					chRetval = 0;
					return chRetval;
				}
				break;			// exit the loop
			}
		}

		if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
		{
		    /* See if Track 1 error */
		    if (( !TRINP.TRTRK1[0] ) && (!INJECTKEY))
		    {
			    /* Yes; see if Issuer does not ignore Track 1 errors */
			    if ( !((TRINP.TRISPTR->ISOPT4) & IS4_TRK1ERR) )
			    {
				    /* Yes; notify the user including pinpad if in use. */
	    		    if( PINSTAT & PinCard_Read )
				    {
				 	    /* Display ** CARD ERROR ** on the pinpad */
					    PinMessage ( Msg_PinCardErr );
				    }

				    ShowErrMsg( CardErr );

	                /* Allow user to re-enter the card */
	                if( !ReadData())
	                {
	                    chRetval = 0;
	                    return chRetval;
	                }

	                /* Start AcctProc over again */
	                continue;
			    }
	            else
	            {
	                /* Ignore track 1 error, continue account processing */
	                break;
	            }
		    }
	        else
	        {
	            /* No track 1 error, continue account processing */
	            break;
	        }
		}	// if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
		else
		{
			break;
		}	// if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
    }

	/* Skip these checkings for ICC */
	if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )
	{
	// Set Service Code if available
		SetServCode();

	// Set Validity Date if available
		SetValDate();

	// Set Issue Number for SWITH SOLO cards	
		if(!SetIssueNum( ))
		{
	   	    ShowErrMsg(InvalidCard);
	   	    return chRetval;
		}

		/* Account data OK, save the card holder's name  */
		SavTrk1Name();

		/* See if Offline allowed */
		if ( SALOFFL == TRINP.TRKEY )
		{
			/* Yes; see if Offline allowed */
			if ( !((TRINP.TRISPTR->ISOPT1) & IS1_OFFL) )
			{
				/* No; notify the user */
				ShowErrMsg(UnsuppCard);
				chRetval = 0;
				return chRetval;
			}
		}

	    /* Don't test for settle required if entry from idle.				*/
		/* Must determine proper issuer, debit, ebt, etc. before checking.	*/
	    if ( TRINP.TRKEY != SALE )
	    {
			/* See if Batch Pending for this Acquirer */
			if ( ChkBatPend( ) )
			{
				/* Yes; notify the user of Batch Pending */
				ShowErrMsg(RetryBatchTrans);
				chRetval = 0;
				return chRetval;
			}
	   }
	}	// if ( !( 0x50 == (TRINP.TRPOSE[1] & 0xF0) ) )

	/* Show issuer and transaction on title line */
	SetDefaultTitle( );

	return chRetval;
}



//-----------------------------------------------------------------------------
//!  \brief     Perform MOD10 on account number
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Global Inputs chRetval: 0 - if Check digit does not match
//!                             1 - if Account number is valid.
//!     The source is in TRINP.TRPAN in BCD form.
//!
//-----------------------------------------------------------------------------
extern UBYTE ValidateAcct( void )
{
	static int AcctLen, i;
	static UBYTE Chksum, tmp;
	UBYTE chRetval;

	// Convert PAN to ASCII for quick check
	AcctLen =
		( S_TRPAN * 2 ) - MovPanAsc( Dspbuf, TRINP.TRPAN, ( S_TRPAN * 2 ) );

	// perform a MOD-10 check
	Chksum = 0;
	for ( i = ( AcctLen - 2 ); i >= 0; i -= 2 )
	{
		tmp = ( Dspbuf[i] - '0' ) * 2;
		if ( tmp >= 10 )
			Chksum += ( tmp % 10 ) + 1;
		else
			Chksum += tmp;

		if ( ( i - 1 ) < 0 )
			break;
		else
			Chksum += ( Dspbuf[i - 1] - '0' );
	}

	if ( Chksum % 10 )
		Chksum = ( ( Chksum / 10 ) + 1 ) * 10 - Chksum;
	else
		Chksum = 0;

	if ( ( Dspbuf[AcctLen - 1] - '0' ) == Chksum )
		chRetval = 1;
	else
		chRetval = 0;

	return chRetval;
}






//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Stores Account Number in TRINP.TRPAN in BCD form
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     The source is in TRINP.TRTRK2 or Dspbuf in ASCII form.
//!
//-----------------------------------------------------------------------------
static void StoreAcct( void )
{
	static int AcctLen, idx, i;
	static UBYTE *pAcctNum;

	for ( AcctLen = 0;
		  TRINP.TRTRK2[AcctLen] && ( TRINP.TRTRK2[AcctLen] != 0x3d ); )
		AcctLen++;
	if ( AcctLen )
	{
		// Track 2 data from card swipe 
		pAcctNum = TRINP.TRTRK2;
	}
	else
	{
		// Manual input, Must be in Dspbuf 
		pAcctNum = ( UBYTE * ) Dspbuf;
		AcctLen = StrLn( Dspbuf, 20 );
	}

	memset( TRINP.TRPAN, 0xFF, S_TRPAN );
	idx = 0;
	for ( i = 0; i < AcctLen; i += 2 )
	{
		TRINP.TRPAN[idx] = pAcctNum[i] - '0';
		TRINP.TRPAN[idx] <<= 4;

		if ( ( i + 1 ) < AcctLen )
			TRINP.TRPAN[idx++] |= pAcctNum[i + 1] - '0';
		else
			TRINP.TRPAN[idx++] |= 0x0f;
	}
}


//-----------------------------------------------------------------------------
//!  \brief     Save the Card holder's name into TRINP.TRNAME
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void SavTrk1Name( void )
{
	UBYTE Name_Max;
	UBYTE Name_Start, Name_End;

	// check if Trk1 data available 
	if ( TRINP.TRTRK1[0] )
	{
		// Save the card holder's name into TRNAME field 
		Name_Max =
			StrLnChr( ( char * ) TRINP.TRTRK1, ( UBYTE ) 0xff, S_TRTRK1 );
		Name_Start = StrLnChr( ( char * ) TRINP.TRTRK1, TRK1SEP, Name_Max );

		// If there is not a track one seperator don't continue with the 
		// name processing.  Don't move to TRNAME for later printing.    
		if ( Name_Max == Name_Start )
			return;

		++Name_Start;
		Name_End = StrLnChr( ( char * ) &TRINP.TRTRK1[Name_Start], TRK1SEP,
							 ( UBYTE ) ( Name_Max - Name_Start ) );

		if ( Name_End > S_TRNAME )
			Name_End = S_TRNAME;
		memcpy( TRINP.TRNAME, &TRINP.TRTRK1[Name_Start], Name_End );
	}
}

//-----------------------------------------------------------------------------
//!  \brief     Save the Service Code into TRINP.TRSTARTD
//!
//!  \param
//!     None
//!
//!  \return
//!     UBYTE       0 - invalid number
//!                 1 - success
//!
//-----------------------------------------------------------------------------
UBYTE SetServCode(void)
{
	UBYTE *ptrend,*pTRK;
	
	if(TRINP.TRTRK2[0])
	{
		pTRK = TRINP.TRTRK2;
		ptrend = &(TRINP.TRTRK2[S_TRTRK2]);

		pTRK = memchr(TRINP.TRTRK2,0x3d,S_TRTRK2);

	   
	   	pTRK++;           /* skip the separator 0x3d, '=' */
		pTRK += 4;        /* skip the Expired Date */
   
//		Service Code for non ANSI Card  TRSERVCD
		if(!(isdigitstr( pTRK,3 )))
			{
				return 0;
			}

		TRINP.TRSERVCD[0] = 0x00;
		TRINP.TRSERVCD[0] |= *pTRK++ & 0x0f;
   		TRINP.TRSERVCD[1] = *pTRK++;
   		TRINP.TRSERVCD[1] <<= 4;
  		TRINP.TRSERVCD[1] |= *pTRK++ & 0x0f;

	/* Start date entry */
		if(((TRINP.TRISPTR->ISOPT6) & IS6_STARTD))
			{
				TRINP.TRSTARTD[0] = *pTRK++;
				TRINP.TRSTARTD[0] <<= 4;
				TRINP.TRSTARTD[0] |= *pTRK++ & 0x0f;
	    		TRINP.TRSTARTD[1] = *pTRK++;
	    		TRINP.TRSTARTD[1] <<= 4;
	    		TRINP.TRSTARTD[1] |= *pTRK++ & 0x0f;
			
			/* If month is invalid */
			if	(!((TRINP.TRSTARTD[1] >= 1) && (TRINP.TRSTARTD[1] <= 12)))
				{
					TRINP.TRSTARTD[0] = 0;
					TRINP.TRSTARTD[1] = 0;
				}
				

			}

	}
	return 1;
}

//-----------------------------------------------------------------------------
//!  \brief     Save the Validity Date into TRINP.TRSTARTD
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
void SetValDate(void)
{
	UBYTE *pTRK;
	UBYTE	tmp,ValDatPos;
	UBYTE	ValYears,ValMonths;

	if(TRINP.TRTRK2[0])
	{
		pTRK = TRINP.TRTRK2;

/* 		Move Validity Period if exists */
		if ( (ValDatPos=(TRINP.TRCRPTR->CRVPPOS)) != 0 )
		{
			pTRK += (bcdtob(ValDatPos)-2);

			/* Two digits */
			tmp = *pTRK++;
			tmp <<= 4;
			tmp |= *pTRK++ & 0x0f;

			tmp = (bcdtob(tmp));
			ValYears = 	tmp/12;
			ValMonths = tmp%12-1;
			
			TRINP.TRSTARTD[0] = (int)(TRINP.TREXPD[0]>>4) * 10 + (TRINP.TREXPD[0] & 0x0f );

			if((TRINP.TREXPD[0] < ValYears))
				{
					TRINP.TRSTARTD[0]=BinBcdB((int) (( TRINP.TRSTARTD[0] + 100) - ValYears));
				}
			else
				{
					TRINP.TRSTARTD[0]=BinBcdB((TRINP.TRSTARTD[0] - ValYears));
				}


			TRINP.TRSTARTD[1]=(int) ((TRINP.TREXPD[1]>>4) * 10 + (TRINP.TREXPD[1] & 0x0f ));

			if((TRINP.TREXPD[1] < ValMonths))
				{
					TRINP.TRSTARTD[1]=BinBcdB(( (TRINP.TRSTARTD[1] + 12) - ValMonths));
					TRINP.TRSTARTD[0]=TRINP.TRSTARTD[0]-1;
				}
			else
				{
					TRINP.TRSTARTD[1]=BinBcdB((TRINP.TRSTARTD[1] - ValMonths));
				}

		}
	}
}

//-----------------------------------------------------------------------------
//!  \brief     Save the Issue Number into TRINP.TRISSUENUM
//!
//!  \param
//!     None
//!
//!  \return
//!     UBYTE       0 - invalid number
//!                 1 - success
//!
//-----------------------------------------------------------------------------
UBYTE SetIssueNum(void)
{
	UBYTE *pTRK;
	UBYTE IssNumPos, s;

	if(TRINP.TRTRK2[0])
	{
		pTRK = TRINP.TRTRK2;

/* 		Move Issue Number if exists */
		if ( (IssNumPos=(TRINP.TRCRPTR->CRINPOS)) != 0 )
		{
			pTRK += (bcdtob(IssNumPos)-2);

			/* Check how many digits in the Issue Number */
			if ((TRINP.TRCRPTR->CROPT1) & CR1_2DIGISSN)
			{
				/* Two digits */
				TRINP.TRISSUENUM = *pTRK++;
				TRINP.TRISSUENUM <<= 4;

				s = *pTRK;

				if (!(s >= '0' && s <= '9'))
				{
					return 0;
				}

				TRINP.TRISSUENUM |= *pTRK++ & 0x0f;
			}
			else
			{
				/* One digit */
				TRINP.TRISSUENUM |= *pTRK++ & 0x0f;
			}
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------
//!  \brief     Check if string contains only digits
//!
//!  \param[in]
//!     p           string pointer
//!  \param[in]
//!     len         string length
//!
//!  \return
//!     int         0 - False
//!                 1 - True, digits only
//!
//-----------------------------------------------------------------------------
int isdigitstr(UBYTE *p, UBYTE len)
{
	UBYTE	s,i;

	for(i=0; i<len; i++)
	{
		s = *p++;
		if (!(s >= '0' && s <= '9'))
		{
			return 0;
		}
	}
	return 1;
}
