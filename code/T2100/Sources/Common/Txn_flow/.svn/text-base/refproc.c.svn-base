
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             refproc.c
//      Voice referral Processing
//
//=============================================================================

#include "clib.h"

#include "fixdata.h"

#include "i8583utl.h"
#include "comdata.h"
#include "msg.h"
#include "tables.h"
#include "comm.h"
#include "hstmsg.h"

#include "refproc.h"

/* Extern Variables and Structures */


/* -- Local Routines -- */
static void ReferralUncomplete( struct trandata_rec * pTranBuf );
static void ReferralApproved( struct trandata_rec * pTranBuf );
static void ReferralDeclined( struct trandata_rec * pTranBuf );
static void FormatRefTelNo( char * dest, struct trandata_rec * pTranBuf );

//-----------Local Variables -------------------------------------------

//-----------Local Constants -------------------------------------------
static const ENTRY_STR VoiceReferral =
{
	N_Referral,			/* Title */
	CustomMsg,          /* First line */
	CustomMsg2,			/* Second line */
};

/*------------------------------------------------------------------
 *  Name:           Referral
 *  Description:    Process Voice Referral
 *                  
 *  Parameters:     pTranBuf -> transaction record
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/
void Referral( struct trandata_rec * pTranBuf )
{
	UBYTE keyval;
	Bool bPinCardStatus;

    // Check if voice referral allowed for this Response, Transaction and Issuer
	if ( (CurrentEvent.TRNOPT2 & T2_REFER) &&
		 ((pTranBuf->TRISPTR->ISOPT1) & IS1_REFER ) &&
         (pTranBuf->TRRSPOPT & R_REFERRAL)
       )
	{
        // Drop phone line
        Hang_Up_Soon ();

        // Stop Read_IO in FlagHost()
        FlagHostCntl = 0;
        SDK_Wait( ONESECOND * 2 );

        // Close all request to IO module
		read_io( 0, 0, &bPinCardStatus, &DevFlag );

        // Prepare data for screen
		GetMsg( CallAuthCentre, CSTMSG );
		FormatRefTelNo( CSTMSG2, pTranBuf );
            
        // Beep attention
		SDK_BeepIt( 4 );

		// Prompt "Call auth center/OK" without timeot (only beep it every 30 sec)
		do
			keyval = ContinueEntryTimeout(&VoiceReferral);
        while ( (0 != keyval) && (1 != keyval) );

        // 'CANCEL' pressed - finish referral with "uncomplete" state
        if ( 0 == keyval )
        {
			ReferralUncomplete( pTranBuf );
            return;
        }

        // 'OK' pressed:

        // Prompt "Auth Code? Yes/No"
		GetMsg( AuthCode, CSTMSG );
		CSTMSG2[0] = '\0';
		keyval = YesNoEntry(&VoiceReferral);
				
		// 'CANCEL' pressed or Timed Out - finish referral with "uncomplete" state
        if ( (0 == keyval) || (CANCEL_KY == keyval) )
		{
    		ReferralUncomplete( pTranBuf );
			return;
		}

		// 'NO' pressed - finish referral with "declined" state
        if ( CLEAR_KY == keyval )
        {
            ReferralDeclined( pTranBuf );
    		return;
		}

		// Prompt for Auth Code entry (up to 6 alphanumeric)
		GetMsg( N_EnterAuth, CSTMSG );
		CSTMSG2[0] = '\0';

		if ( 1 == AlphaNumEntry( 1, 6, 1, &VoiceReferral ) )
		{
            // Auth Code entered -  - finish referral with "approved" state
            ReferralApproved( pTranBuf );
            return;
        }
        else
        {
            // "CANCEL" pressed - finish referral with "uncomplete" state
			ReferralUncomplete( pTranBuf );
            return;
        }
    }
}

/*------------------------------------------------------------------
 *  Name:           ReferralUncomplete
 *  Description:    
 *                  
 *  Parameters:     None.
 *  Return Value:   None.
 *					
 *  Notes:          
 *					
 *-----------------------------------------------------------------*/
static void ReferralUncomplete(  struct trandata_rec * pTranBuf  )
{
	// Clear any Host Message
	memset( pTranBuf->TRHOSTTEXT, 0x00, S_TRHOSTTEXT );

	// 'TRANSACTION VOID' Response Code
	memcpy( pTranBuf->TRRSPC, RSP_XC, S_TRRSPC );

	// Set up response Text & Options
	RspLookUp( pTranBuf );

	// Move the 'TRANSACTION VOID' text into the buffer for printing
	memcpy( HSTPRDATA, pTranBuf->TRRSPTXT, S_TRRSPTXT );
}

/*------------------------------------------------------------------
 *  Name:           ReferralApproved
 *  Description:    
 *                  
 *  Parameters:     None.
 *  Return Value:   None.
 *					
 *  Notes:          
 *					
 *-----------------------------------------------------------------*/
static void ReferralApproved(  struct trandata_rec * pTranBuf  )
{
	// Set Auth Code
	memcpy( pTranBuf->TRAUTH, Dspbuf, 6 );

    // Set 'APPROVED' Response Code
	memcpy( pTranBuf->TRRSPC, RSP_APP, S_TRRSPC );

    // Set up response Text & Options
    RspLookUp( pTranBuf );

	// Set advice request
	pTranBuf->TRSTATUS[0] |= TS1_ADV;
}

/*------------------------------------------------------------------
 *  Name:           ReferralDeclined
 *  Description:    
 *                  
 *  Parameters:     None.
 *  Return Value:   None.
 *					
 *  Notes:          
 *					
 *-----------------------------------------------------------------*/
static void ReferralDeclined(  struct trandata_rec * pTranBuf  )
{
	// Clear any Host Message
	memset( pTranBuf->TRHOSTTEXT, 0x00, S_TRHOSTTEXT );
	memset( HSTPRDATA, 0x00, S_HSTPRDATA );

	// Set 'DECLINED' Response Code
	memcpy( pTranBuf->TRRSPC, RSP_DECLINED, S_TRRSPC );

    // Set up response Text & Options
    RspLookUp( pTranBuf );
}

/*------------------------------------------------------------------
 *  Name:           FormatRefTelNo
 *  Description:    
 *                  
 *  Parameters:     UBYTE * dest.
 *  Return Value:   None.
 *					
 *  Notes:          
 *-----------------------------------------------------------------*/
static void FormatRefTelNo( char *pDest,  struct trandata_rec * pTranBuf  )
{
    static UBYTE TmpTel[S_ISRTEL];

	// Format and move the phone number into destination */
	MoveItIss( TmpTel, pTranBuf->TRISPTR->ISRTEL, S_ISRTEL );

    // Convert from BCD to ASCII
    BfAscii( pDest, TmpTel, S_ISRTEL );

    // Find the end of telephone data
	while ( *pDest++ != 'F' );

    // Set Null Terminator
	*(pDest-1) = '\0';
}
