//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             reqhyp.c
//      Hypercom Request Building Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"
#include "fixdata.h"

#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "transdef.h"
#include "appdata.h"
#include "bit8583.h"
#include "tables.h"
#include "findtran.h"
#include "pinpad.h"
#include "comm.h"
#include "msgfunc.h"
#include "reqhyp.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static struct bitmap_rec *FindBitMapHyp( enum transactions Trans );
static Bool MovProcHyp( void );
static Bool MovB60Hyp( void );
static void B60_OrigHyp( void );
static Bool MovInv( void );
static Bool MovB63Hyp( void );
static Bool MovMAC( void );

//=============================================================================
// Public data definitions
//=============================================================================

//! Table of Move Entries.                     
//! Each entry respresents a single move item. 
//! All items are saved in the Send Buffer.    
const struct move_rec Move_Tab[] = {
    //! Primary Account Number           
	{2, MovPan},
    //! Processing Code                  
	{3, MovProcHyp},
    //! Amount, Transaction              
	{4, MovTotAm},
    //! Systems Trace Audit Number       
	{11, MovStan},
    //! Time, Transaction                
	{12, MovTime},
    //! Date, Transaction                
	{13, MovDate},
    //! Date, Expiry                     
	{14, MovExpdYYMM},
    //! POS Entry Mode                   
	{22, MovPose},
    //! Network International Identifier 
	{24, MovNII},
    //! POS Condition Code               
	{25, MovPocc},
    //! Track 2 Data                     
	{35, MovTrk2},
    //! Retrieval Reference Number       
	{37, MovRRN},
    //! Auth Id Response code  (Approval code)           
	{38, MovAuth},
    //! Response code            
	{39, MovResp},
    //! Terminal Id                      
	{41, MovTID},
    //! Acceptor Id                      
	{42, MovAccID},
    //! PIN number                       
	{52, MovPin},
    //! Tip Amount                       
	{54, MovTip},
#ifdef MAKE_EMV
    //! ICC data
	{55, MovICC},
#endif
    //! Private Field 60                 
	{60, MovB60Hyp},
    //! Descriptors                      
	{61, MovDesc},
    //! Invoice Number                   
	{62, MovInv},
    //! Private Field 63                 
	{63, MovB63Hyp},
    //! MAC
	{64, MovMAC},
    //! Table End                        
	{0, NULL}
};


//=============================================================================
// Private data definitions
//=============================================================================

static struct bitmap_rec *pBitMap_rec;
static struct bitmap_rec *pBitMap_org;

//! Table of Transaction Bitmap Entries.     
//! Each entry respresents a single bit map. 
static const struct bitmap_rec BitMap_Tab[] = {
	{
	 SALE,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
      }},

	{
	 CRDVER,
     //! MESSAGE TYPE
	 {0x01, 0x00},
     //! PROCESSING CODE
	 0x38,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 DEBIT,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 REFDEBIT,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x20,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 SALCASH,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x09,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 DBCASH,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x01,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 VOIDTRAN,					// Defined as VOID_ONLINE. VOID WAS NEVER USED 
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x02,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B55_RRN + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 TESTTRAN,
     //! MESSAGE TYPE
	 {0x08, 0x00},
     //! PROCESSING CODE
	 0x99,
	 {B13_PROC,
	 B23_STAN,
	 B38_NII,
	 0,
	 0,
	 B61_TID,
	 0,
	 B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 AUTH,
     //! MESSAGE TYPE
	 {0x01, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 REFUND,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x20,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 ADJSALE,
     //! MESSAGE TYPE
	 {0x02, 0x20},
     //! PROCESSING CODE
	 0x02,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B55_RRN + B56_AUTH + B57_RESP,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 ADJREFUND,
     //! MESSAGE TYPE
	 {0x02, 0x20},
     //! PROCESSING CODE
	 0x22,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B55_RRN + B56_AUTH + B57_RESP,
	 B61_TID + B62_ACCID,
	 B74_PIN,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 SALOFFL,
     //! MESSAGE TYPE
	 {0x02, 0x20},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 CLBATCH,
     //! MESSAGE TYPE
	 {0x05, 0x00},
     //! PROCESSING CODE
	 0x92,
	 {B13_PROC,
	 B23_STAN,
	 B38_NII,
	 0,
	 0,
	 B61_TID + B62_ACCID,
	 0,
	 B84_BIT60 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 CLBATCHA,
     //! MESSAGE TYPE
	 {0x05, 0x00},
     //! PROCESSING CODE
	 0x92,
	 {B13_PROC,
	 B23_STAN,
	 B38_NII,
	 0,
	 0,
	 B61_TID + B62_ACCID,
	 0,
	 B84_BIT60 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 CLBATCHE,
     //! MESSAGE TYPE
	 {0x05, 0x00},
     //! PROCESSING CODE
	 0x96,
	 {B13_PROC,
	 B23_STAN,
	 B38_NII,
	 0,
	 0,
	 B61_TID + B62_ACCID,
	 0,
	 B84_BIT60 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 RUPLOAD,
     //! MESSAGE TYPE
	 {0x03, 0x20},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B55_RRN + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 OPENTAB,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 BALINQ,
     //! MESSAGE TYPE
	 {0x01, 0x00},
     //! PROCESSING CODE
	 0x31,
	 {B12_PAN + B13_PROC,
	 B23_STAN + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2,
	 B61_TID + B62_ACCID,
	 B74_PIN,
	 0
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 LOGON,
     //! MESSAGE TYPE
	 {0x08, 0x00},
     //! PROCESSING CODE
	 0x92,
	 {B13_PROC,
	 B23_STAN,
	 B38_NII,
	 0,
	 0,
	 B61_TID + B62_ACCID,
	 0,
	 0
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 MAIL_ORDER,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B76_TIP,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

#ifdef MAKE_EMV
	{
	 EMV_SALE,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x00,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP + B77_ICC,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 EMV_CASH,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x01,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP + B77_ICC,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},

	{
	 EMV_SALCASH,
     //! MESSAGE TYPE
	 {0x02, 0x00},
     //! PROCESSING CODE
	 0x09,
	 {B12_PAN + B13_PROC + B14_TOTAM,
	 B23_STAN + B24_TIME + B25_DATE + B26_EXPD,
	 B36_POSE + B38_NII,
	 B41_POCC,
	 B53_TRK2 + B56_AUTH,
	 B61_TID + B62_ACCID,
	 B74_PIN + B76_TIP + B77_ICC,
	 B84_BIT60 + B85_BIT61 + B86_BIT62 + B87_BIT63
#ifdef MAKE_ISOMAC
     + B88_MAC
#endif // MAKE_ISOMAC
     }},
#endif

};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Formats Hypercom Request packet
//!
//! \param
//!		[in]Amount			Pointer to Destination Buffer
//!
//! \return
//!     Comm Status of Request send if Request Packet was built orvINVDATA
//!     if Request Packet not built
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS ReqHyper( void )
{
	OS_RETURN_VALS CommStatus;	// Comm Status of Request send  

	Move_Table = ( struct move_rec * ) Move_Tab;	// Set Hypercom Bit table 

	// Reset Comm status 
	CommStatus = orvINVDATA;

	// Reset Request Packet built okay flag 
	MsgComFlag &= ~REQUEST_BUILT;

	// Find the transaction in the Bitmap Table 
	pBitMap_rec = FindBitMapHyp( TRREQ.TRKEY );

	// See if transaction in the Bitmap Table 
	if ( NULL != pBitMap_rec )
	{
		// Yes; clear out Transmit Buffer 
		memset( SENDBUF, 0,sizeof( SENDBUF ) );

		// Set for Application Messages 
		SENDBUF[0] = TPDUID_APP;

		// See if a Reversal Advice 
		if ( ( TRREQ.TRSTATUS[0] & ( TS1_REV + TS1_ADV ) ) ==
			 ( TS1_REV + TS1_ADV ) )
		{
			// Yes; make it a Reversal Advice 
			SENDBUF[TPDU_SZE] = 0x04;
			SENDBUF[TPDU_SZE + 1] = 0x20;
		}
		else
		{
			// No; see if a Reversal 
			if ( TRREQ.TRSTATUS[0] & TS1_REV )
			{
				// Yes; make it a Reversal 
				SENDBUF[TPDU_SZE] = 0x04;
				SENDBUF[TPDU_SZE + 1] = 0x00;
			}
			else
			{
				// No; see if an Advice 
				if (TRREQ.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ ))
				{
					// Yes; make it an Advice 
					SENDBUF[TPDU_SZE] = pBitMap_rec->TRNMSG[0];
					SENDBUF[TPDU_SZE + 1] = 0x20;
				}
				else
				{
					// No; make it a normal request 
					SENDBUF[TPDU_SZE] = pBitMap_rec->TRNMSG[0];
					SENDBUF[TPDU_SZE + 1] = pBitMap_rec->TRNMSG[1];
				}
			}
		}

		// Move it into Request Area 
		memcpy( TRREQ.TRMTYPE, SENDBUF + TPDU_SZE, S_TRMTYPE );

		// Set up the data pointer 
		pSendData = SENDBUF + TPDU_SZE + 10;

		// Pack the Message Request into the Send Buffer 
		if ( True == ReqPackMsg( pBitMap_rec->bitmap ) )
		{
			// Okay; indicate that the Request was built okay 
			MsgComFlag |= REQUEST_BUILT;

			// Calculate the length 
			SendMsgLen = pSendData - SENDBUF;


			// Send the Message Request to the Host 
			SendMessage ();

			// Get the Comm status 
			CommStatus = HostOSRetval;
		}
	}

	// Return the Comm status 
	return ( CommStatus );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Finds the Bitmap Table Entry for the transaction
//!
//! \param
//!		[in]Trans			Transaction ID
//!
//! \return
//!     NULL if Transaction not found otherwise Pointer to Transaction Bitmap
//!     record
//-----------------------------------------------------------------------------
static struct bitmap_rec *FindBitMapHyp( enum transactions Trans )
{
	struct bitmap_rec *Found;
	UBYTE i;

	// Reset pointer to record 
	Found = NULL;

	// Search the Bitmap Table for the transaction 
	for ( i = 0; i < ( sizeof( BitMap_Tab ) / sizeof( struct bitmap_rec ) );
		  i++ )
	{
		// Check if transaction found 
		if ( BitMap_Tab[i].trans == Trans )
		{
			// Yes; save pointer to record 
			Found = ( struct bitmap_rec * ) &BitMap_Tab[i];

			// Break out of the loop 
			break;
		}
	}

	// Return pointer to record 
	return ( Found );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Processing Code
//!
//! \param
//!		[in]Trans			Transaction ID
//!
//! \return
//!     True if Data has been moved else False
//-----------------------------------------------------------------------------
static Bool MovProcHyp( void )
{
	// Clear the Processing Code 
	memset( TRREQ.TRPROC, 0,S_TRPROC );

	if ( (RUPLOAD == TRREQ.TRKEY) || (VOIDTRAN == TRREQ.TRKEY) )
	{
		pBitMap_org = FindBitMapHyp( TRREQ.TRORIGKEY );
		TRREQ.TRPROC[0] = pBitMap_org->TRNPROC;
	}
	else
	{
		// Set first byte 
		TRREQ.TRPROC[0] = pBitMap_rec->TRNPROC;
	}
	// Check for void
	if ( (TRREQ.TRSTATUS[0] & TS1_VOID) && ((TRREQ.TRPROC[0] & 0x0f) == 0) && (RUPLOAD != TRREQ.TRKEY) )
	{
		// Add voided status to processing code
		TRREQ.TRPROC[0] += 0x02;
	}

	// Modification to get TRNOPT1 that goes with the   
	// transaction being processed.                     
	// Get the Transaction from the Request 
	if ( RUPLOAD == TRREQ.TRKEY )
	{
		HostEvent.TRNKEY = TRREQ.TRORIGKEY;
	}
	else
	{
		HostEvent.TRNKEY = TRREQ.TRKEY;
	}

	// Locate the Transaction - will be in HostEvent 
	HostFindTran ();

    // Check if account required
    if (  !( HostEvent.TRNOPT1 & T1_NOACCT )  &&  // no T1_NOACCT transaction attribute 
           ( 0x31 != TRREQ.TRPROC[0] ) )          // not Balance Inquiry transaction 
    {
        // Set default account from issuer table
        TRREQ.TRPROC[1] = TRREQ.TRISPTR->ISDEFAC;
    }

	// Is this a Re-Request 
	if ( TRREQ.TRAGAIN )
	{
		// Yes; set bit 0 in the last byte of the processing code 
		TRREQ.TRPROC[2] |= 0x01;
	}

	// Call the Generic Move routine 
	MovGen( TRREQ.TRPROC, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves bit 60 data
//!
//! \param
//!		[in]Trans			Transaction ID
//!
//! \return
//!     True if Data has been moved else False
//!
//! \note
//!     This function needs updating whenever a new transaction is added to
//!     the application.
//-----------------------------------------------------------------------------
static Bool MovB60Hyp( void )
{
	Bool MoveFlag;

	// Reset move data flag 
	MoveFlag = False;

	// Check the type of Transaction 
	switch ( TRREQ.TRKEY )
	{
		case SALE:
		case DEBIT:
		case VOIDTRAN:
		case REFUND:
		case OPENTAB:
		case REFDEBIT:
		case SALCASH:
		case DBCASH:			// See if REVERSAL ADVICE 
			if ( ( TRREQ.TRSTATUS[0] & ( TS1_REV + TS1_ADV ) )
				 == ( TS1_REV + TS1_ADV ) )
			{
				// Yes; move original data from Transaction 
				B60_OrigHyp(  );

				// Data moved 
				MoveFlag = True;
			}

			break;

		case RUPLOAD:			// Unconditionaly send bit 60 
			B60_OrigHyp(  );

			// Data moved 
			MoveFlag = True;

			break;

		case ADJSALE:			// ADJUST SALE   
		case ADJREFUND:		// ADJUST REFUND; see if Original Amount exists 
			if ( IfThere( TRREQ.TRORIGAMT, S_TRBASE ) )
			{
				// Yes; convert the Original Amount 
				BfAscii( ( char * ) TmpBuf, TRREQ.TRORIGAMT, S_TRBASE );

				// Call the Generic Move routine 
				MovGen( TmpBuf, 12 );

				// Data moved 
				MoveFlag = True;
			}

			break;

		case CLBATCH:			// CLOSE BATCH   
		case CLBATCHE:			// CLOSE BATCH END 
			// Move with length field for batch number. 
			MovGen( TRREQ.TRBATNO, S_TRBATNO );

			// Data moved 
			MoveFlag = True;

			break;

		default:				// Default 
			break;
	}

	// Return move data flag 
	return ( MoveFlag );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves original data from a Transaction
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void B60_OrigHyp( void )
{
	// Get the original message type using the original key 
	pBitMap_org = FindBitMapHyp( TRREQ.TRORIGKEY );

	// Hardcode size of 22  
	memset( ( char * ) TmpBuf, ' ',22 );
	BfAscii( ( char * ) &TmpBuf[0], pBitMap_org->TRNMSG, 2 );
	BfAscii( ( char * ) &TmpBuf[4], TRREQ.TROSTAN, 3 );

	// Call the Generic Move routine 
	MovGen( TmpBuf, 22 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Invoice Number
//!
//! \return
//!     True if Data has been moved else False
//-----------------------------------------------------------------------------
static Bool MovInv( void )
{
	// Check if Invoice Number has data 
	if ( IfThere( TRREQ.TRINV, S_TRINV ) )
	{
		// Call the Generic Move routine 
		MovGen( TRREQ.TRINV, S_TRINV );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats Bit 63 Hypercom data
//!
//! \return
//!     True if Data exists else False (no data in buffer)
//!
//! \note
//!     Destination buffer pointed by pSendData.
//-----------------------------------------------------------------------------
static Bool MovB63Hyp( void )
{
	static Bool retval;

	retval = False;
	pB63Len = pSendData;		// init pointer to Bit 63 length bytes  
	B63_Len = 0;				// init bit 63 total length  

	if ( ( TRREQ.TRKEY == CLBATCH )
		 || ( TRREQ.TRKEY == CLBATCHA ) || ( TRREQ.TRKEY == CLBATCHE ) )
	{
		retval = B63_MoveStdTotals(  );
		return ( retval );
	}

	if ( TRREQ.TRKEY == TESTTRAN )
	{
		// Note:  This will need to chek if in AutoTest mode when TESTTRAN 
		return ( False );
	}

	pSendData += 2;				// Skip the two-byte total length field  
	if ( TCONF.TCOPT1 & TC1_CASHIER )
		retval |= B63_Cashier(  );

#ifdef	MAKE_EMV
	/* Move Issuer Script Results */
	retval |= B63_IssScriptRes( );
#endif	// MAKE_EMV

	retval |= B63_CVV2(  );

	retval |= B63_PS2000(  );

	retval |= B63_Hostref(  );

	retval |= B63_DUKPT(  );

	retval |= B63_Addprmpt(  );

	retval |= B63_BatNum(  );

	retval |= B63_TaxAmt(  );

	retval |= B63_CashBK(  );

	retval |= B63_BarAmt(  );

	if ( B63_Len )
	{
		if ( 100 > B63_Len )
		{
			*pB63Len = 0;
			*( pB63Len + 1 ) = BinBcdB( ( UBYTE ) B63_Len );
		}
		else
		{
			*( pB63Len ) = BinBcdB( ( UBYTE ) ( B63_Len / 100 ) );
			*( pB63Len + 1 ) = BinBcdB( ( UBYTE ) ( B63_Len % 100 ) );
		}
	}
	else
	{
		// Set the pointer back since no Bit 63 data 
		pSendData -= 2;
	}

	return retval;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Formats Bit 64 Hypercom data
//!
//! \return
//!     True if Data exists else False (no data in buffer)
//!
//! \note
//!     Destination buffer pointed by pSendData.
//-----------------------------------------------------------------------------
static Bool MovMAC( void )
{
#ifdef MAKE_ISOMAC
	int i;

	UBYTE PinMacId;					// Mac index.
	UBYTE *pMacKeyPtr;				// Pointer to MAC key.
	UBYTE *pMacDataPtr;				// Pointer to data to be MAC'ed.
	UWORD MacDataLength;			// Length of data to be MAC'ed.
	UBYTE PinData[8];				// Used for passing data to/from pinpad.

    // Check if MAC calculation enabled
    if ( !( ( TRREQ.TRAQPTR->AQOPT4 ) & AQ4_MAC ) )
        return False;

	// Set data pointer
	pMacDataPtr = SENDBUF + TPDU_SZE;

	// Data length
	MacDataLength = pSendData - SENDBUF - TPDU_SZE;

	// update bitmap before mac calculation, or it will be WRONG!
	*( pMacDataPtr + 9 ) |= B88_MAC;

	// Set Master Key ID
	PinMacId = TRREQ.TRAQPTR -> AQMACID;

    // Pointer to encrypted MAC key
	pMacKeyPtr = TRREQ.TRAQPTR -> AQMACKEY;
	
    // Display info message.
	ShowInfoMsg ( PleaseWait, N_Pinpad );
	
    // Calculate MAC
	if( !PinGenMac( PinMacId, pMacKeyPtr, pMacDataPtr, MacDataLength, PinData ) )
	{
        // calculation was unsuccessful
        memset(pSendData, 'E', 8);
        pSendData += 8;
	} 
    else 
    {
        for (i = 0; i < 4; i++) 
        {
            unsigned char n = (PinData[i] >> 4) & 15;
            *pSendData++ = n + (n < 10 ? '0' : 'A' - 10);
            n = PinData[i] & 15;
            *pSendData++ = n + (n < 10 ? '0' : 'A' - 10);
        }
	}

    // Restore Cancel allowed 
//    FlagHostCntl = tmpFlagHostCntl;

	return True;

#else // MAKE_ISOMAC

    // no MAC in any conditions
	return False;

#endif	// MAKE_ISOMAC
}

