
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rsphyp.c
//      Hypercom Response Parsing Routines.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "fixdata.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "tables.h"
#include "pinpad.h"
#include "comm.h"
#include "msgfunc.h"
#include "schtab.h"
#include "hstmsg.h"
#include "rsphyp.h"
#include "hstresp.h"
#include "rspunp.h"
#include "updttime.h"
#include "appdata.h"
//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module

// Public from other modules

extern UBYTE *pRecvData;
extern UBYTE *p62Data;
extern UBYTE *p63Data;

#ifdef MAKE_ISOMAC
extern UBYTE *p64Data;
#endif	// MAKE_ISOMAC


//=============================================================================
// Private defines and typedefs
//=============================================================================

struct MovFldRec
{
	UBYTE *pDest;				// Destination 
	UBYTE *pSrc;				// Source      
	UBYTE dlen;					// Data length 
};

#define N_MOVTABINQ ( sizeof( MOVTABINQ ) / sizeof( struct MovFldRec ) )


//=============================================================================
// Private function declarations
//=============================================================================

static void ResponseBad( void );
static void RespInit( void );
static void PrivateFields( void );
static void ProcB61( void );
static void ProcB62( void );
static void ProcB63( void );
static void ProcB63HstPrData( void );
static void ProcB63HstDispData( void );
static void ProcB63OldText( void );
static void ProcB63PS2000( void );
static void ProcB63RefData( void );
static void ProcB63Schedule( void );
static void ProcB63Surc( void );
static void ProcB63AVS( void );
static void ProcB63New( void );
static void ProcB63Old( void );
static void ProcB63Len( void );
static void ProcB63HostText( void );
static void ProcB63SvPtr( UBYTE * pData );
static void RspAct( void );
static void MoveFields( void );
static Bool ValRsp( void );
static void ProcB63LVL2ID( void );
static void ProcB63CVV2( void );
#ifdef MAKE_ISOMAC
static void ProcB64( void );
#endif	// MAKE_ISOMAC
static void ProcB63Inst(void);			
static void ProcB63Settle(void);                
static void ProcB63CardHolderID( void );        
static void MoveIt_NoSpace(UBYTE *dest, char *source, UBYTE len);
static void AscHex_NoSpace(UBYTE *dest, char *source, UBYTE len);
static void ProcB63AvailReward( void );
static void ProcB63UsedReward( void );
static void ProcIPUpdate( void );
static void ProcDiscountData( void );
static UDWORD convert_addr(const char *s);
//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static UWORD FldLen62;			// Bit 62 field length 
static UWORD FldLen63;			// Bit 63 field length 
static UWORD FldLen;			// Field length 
static UBYTE *pTempData;		// Temporary Data pointer 

// *INDENT-OFF*
// Table of Fields to Move 
static const struct MovFldRec MOVTABINQ[] = {
	// Balance Inquiries also move in the Balance Fields 
	{ TRREQ.TRTOTAM,	TRRSP.TRTOTAM,	S_TRTOTAM	},	// Total Amount 
	{ TRREQ.TRTIP,		TRRSP.TRTIP,	S_TRTIP		},	// Additional ("Today") Amount 

	// Non-Balance Fields 
	{ TRREQ.TRTIME,		TRRSP.TRTIME,	S_TRTIME	},	// Transaction Time  
	{ TRREQ.TRDATE,		TRRSP.TRDATE,	S_TRDATE	},	// Transaction Date  
	{ TRREQ.TRRRN,		TRRSP.TRRRN,	S_TRRRN		},	// Retrieval Ref Num 
	{ TRREQ.TRAUTH,		TRRSP.TRAUTH,	S_TRAUTH	},	// Auth ID Resp Code 
	{ TRREQ.TRRSPC,		TRRSP.TRRSPC,	S_TRRSPC	},	// Response Code     
	{ TRREQ.TRRSPA,		TRRSP.TRRSPA,	S_TRRSPA	},	// Additnl Resp Data 
	{ TRREQ.TRSURC,		TRRSP.TRSURC,	S_TRBASE	},	// Surcharge         
	{ TRREQ.TRTOTF,		TRRSP.TRTOTF,	S_TRBASE	},	// Total with Fee           
};
// *INDENT-ON*


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//	RspHyper		Handle Hypercom Response.
//
//	Parameters: 	OS_RETURN_VALS ReqCommStatus	status Req Communication.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void RspHyper( OS_RETURN_VALS ReqCommStatus )
{
	OS_RETURN_VALS CommStatus;

	// Loop till Response Done 
	while ( True )
	{
		// Clear the Buffer 
		memset( (UBYTE*)&TRRSP, 0, TRSZE );

		// Set Initial Responses 
		RespInit(  );

		// Check if Request Packet was sent okay 
		if ( ReqCommStatus == orvOK )
		{
			// Yes; get Response Message from COM 
			RecvMessage ();
			CommStatus = HostOSRetval;
		}
		else
		{
			// No; set Comm status to reflect the send status 
			CommStatus = ReqCommStatus;
		}

		// Check if Normal Data Was Received 
		if ( HostResp( CommStatus ) )
		{
			// Yes; set Receive Buffer Pointer to TPDU 
			pRecvData = RECVBUF;

			// Store the TPDU 
			memcpy( TRRSP.TRTPDU, pRecvData, S_TRTPDU );

			// Check if an NMS Message 
			if ( TRRSP.TRTPDU[0] == 0x68 )
			{
				// Yes; return Host Not Available Response 
				memcpy( TRRSP.TRRSPC, RSP_NA, S_TRRSPC );

				// Response is Done 
				break;
			}

			// Check if ISO Format Message 
			if ( TRRSP.TRTPDU[0] != 0x60 )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next Message 
				continue;
			}

			// Point to the Response Message Type 
			pRecvData += S_TRTPDU;

			// Save Response Message Type 
			memcpy( TRRSP.TRMTYPE, pRecvData, S_TRMTYPE );

			// Make sure Message Type matches and low byte is Req+10 
			if (   ( TRRSP.TRMTYPE[0] != TRREQ.TRMTYPE[0] ) 
				|| ( TRRSP.TRMTYPE[1] != ( TRREQ.TRMTYPE[1] + 0x10 ) ) )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next message 
				continue;
			}

			// Point to the Bitmap 
			pRecvData += S_TRMTYPE;

			// Unpack the Message and check if completed okay 
			if ( !RspUnPackMsg(  ) )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next Message 
				continue;
			}

			// The Message is unbuilt 

			// Calculate Base again 
			SubAmt( TRRSP.TRBASE, TRRSP.TRTOTAM, TRRSP.TRTIP );

			// See if Response is for us 
			if ( ValRsp(  ) )
			{
				// Yes; handle data of Fields 60-64 
				PrivateFields(  );

				// Get out of the loop 
				break;
			}
			else
			{
				// No; so ignore it 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;
			}
		}
		else
		{
			// No; error.  Get out of the loop 
			break;
		}
	}

	// Move fields from TRRSP to TRREQ 
	MoveFields(  );

	// Look up Response in TRREQ 
	RspLookUp( &TRREQ );

	// Take specific actions from Request 
	RspAct(  );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//	ResponseBad		If an 0830 or 0820 then display Please Wait.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ResponseBad( void )
{
	// Check for an 0830 or an 0820 
	if (   ( TRRSP.TRMTYPE[0] == 0x08 ) 
		&& ( ( TRRSP.TRMTYPE[1] == 0x20 ) || ( TRRSP.TRMTYPE[1] == 0x30 ) ) )
	{
		// Yes; display Please Wait 
		ShowInfoMsg( PleaseWait, N_Comms );

		// Signal user 
		SDK_BeepIt( 1 );
	}
}


//-----------------------------------------------------------------------------
//	RespInit		Set Initial Values for Response.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			Any values validated in ValRsp must be set up in this 
//					routine for timed out responses, and other errors.
//
//-----------------------------------------------------------------------------

static void RespInit( void )
{
	// Approved Response 
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );

	// Terminal ID 
	memcpy( TRRSP.TRTID, TRREQ.TRTID, S_TRTID );

	// Trace Number 
	memcpy( TRRSP.TRSTAN, TRREQ.TRSTAN, S_TRSTAN );

	// Transaction Amount 
	memcpy( TRRSP.TRTOTAM, TRREQ.TRTOTAM, S_TRTOTAM );

	// Set a null value for TRTIP for balance inquire -- difference
	// between zero and nothing sent from Host 
	memset( TRRSP.TRTIP, 0xFF, S_TRTIP );

	// Put 0's in all PS2000 fields 
	memset( ( char * ) &TRREQ.TRPSI, '0', sizeof( TRREQ.TRPSI ) + S_TRTRANID
			+ S_TRVALID );

	// Authorization Number 
	memcpy( TRRSP.TRAUTH, TRREQ.TRAUTH, S_TRAUTH );
}


//-----------------------------------------------------------------------------
//	PrivateFields	Process Data for 60-63.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void PrivateFields( void )
{
	// Process Bit 61 
	ProcB61( );

	// Process Bit 62 
	ProcB62( );

	// Process Bit 63 
	ProcB63( );

#ifdef MAKE_ISOMAC
	// Process Bit 64
	ProcB64( );
#endif	// MAKE_ISOMAC
}


//-----------------------------------------------------------------------------
//	ProcB61			Process Bit 61 of Response
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB61( void )
{
}


//-----------------------------------------------------------------------------
//	ProcB62			Process Bit 62 of Response
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB62( void )
{
/*
      // PARSIAN OZEL KISMI
      if ( p62Data != NULL )
      {
          // Yes; convert the length to binary and save it 
          FldLen62 = BcdBin( p62Data );

          // Save pointer to the start of data 
          pRecvData = p62Data + 2;

          pTempData = pRecvData;

          if ( TRRSP.TRTOTAM == LOGON ) 
          {
              // Because of Len field 2 bayt
              pRecvData += 2; 

              // Working Keyi ilk kýsma 2. bayttan baþlayarak kopyala 
              memcpy ( TRRSP.TRAQPTR->AQWORKKEY + 1 , pTempData , FldLen62 ) ;

              // Working Keyi ikinci kýsma 2. bayttan baþlayarak kopyala
              memcpy ( TRRSP.TRAQPTR->AQWORKKEY + S_AQWORKKEY + 1 , pTempData , FldLen62 ) ;

              pRecvData += FldLen62;

              //ProcB62SvPtr( pRecvData );

              return;
          }

      }
*/
}


//-----------------------------------------------------------------------------
//	ProcB63			Parse data in Bit 63 of Response from Hyper, 
//					CDC or Alpine acquirers
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			Layout of alternate Host Response and/or 
//					PS2000 Data in Bit 63.
//
//					(Ignore spacing, only to clarify description).
//
//					AA L1 22 ZZZZZZZZZZZZZZZZZZZZ L2 20 B CCCCCCCCCCCCCCC 
//					DDDD EE FF
//
//						AA		Total length data in Bit 63				BCD
//						L1		First field length						BCD
//						22		First field type						ASCII
//								('22' for alternative Host Response)
//						Z...Z	20(?) Digit Host Response Text			ASCII
//						L2		Second field length						BCD
//						20		Second field type						ASCII
//								('20' for PS2000 Data)
//						B		1 Digit Payment Services Indicator		ASCII
//						C...C	15 Digit Transaction Identifier			ASCII
//						DDDD	4 Digit Validation Code					ASCII
//						EE		2 Character Visa II Response Code		ASCII
//						FF		2 Character Visa II POS Entry Mode		ASCII
//
//					AA ZZZZZZZZZZZZZZZZZZZZ
//
//						AA		Total length of data in Bit 63			BCD
//						Z...Z	20(?) Digit Host Response Text			ASCII
//
//					AA L2 20 B CCCCCCCCCCCCCCC DDDD EE FF
//
//						L2		Second field length						BCD
//						20		Second field type						ASCII
//								('20' for PS2000 Data)
//						B		1 Digit Payment Services Indicator		ASCII
//						C...C	15 Digit Transaction Identifier			ASCII
//						DDDD	4 Digit Validation Code					ASCII
//						EE		2 Character Visa II Response Code		ASCII
//						FF		2 Character Visa II POS Entry Mode		ASCII
//
//-----------------------------------------------------------------------------

static void ProcB63( void )
{
	UBYTE DataType;

	// Any Bit 63 Data to process 
	if ( p63Data != NULL )
	{
		// Yes; convert the length to binary and save it 
              FldLen63 = BcdBin( p63Data );

		// Save pointer to the start of data 
		pRecvData = p63Data + 2;

		// Increment to point at the possible data type number and
		// save field type address 
		pTempData = pRecvData + 2;

		// Loop through the Bit 63 Data and process it 
		while ( FldLen63 )
		{
			// Convert the possible data type number to binary 
			AscHex( &DataType, ( char * ) pTempData, ( UBYTE ) 1 );

			// Call proper routine based on data type number 
			switch ( DataType )
			{                                                                                              
                                // Host Display Data 
				case 0x22:		
					ProcB63HstDispData(  );
					break;

                                // Host Print Data  
				case 0x29:		
					ProcB63HstPrData(  );
					break;

				// Schedule Event 
				case 0x42:		
					ProcB63Schedule(  );
					break;

                                // Settle
                                case 0x80:		
					//ProcB63Settle();
					break;
                                // Installment
                                case 0x87:
                                        ProcB63Inst( );
                                        break;

                                 // CardHolder ID 
                                 case 0x88:		
					ProcB63CardHolderID( );
					break;

                                  // Kalan Puan 
                                  case 0x89:		
					ProcB63AvailReward(  );
					break;

                                  // Kullanýlan Puan  
                                  case 0x90:		
					ProcB63UsedReward(  );
					break;

                                  // Ýndirimli Tutar ve indirim oraný 
                                  case 0x95:
                                       ProcDiscountData();
                                       break;

                                  case 0x96:
                                        ProcIPUpdate();
                                        break; 

				default:		// Old Host Text 
					ProcB63OldText(  );
					break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	ProcB63HstPrData	Process Host Print Data.
//
//	Parameters: 		None.
//
//	Global Inputs:		None.
//
//	Returns:			Nothing.
//
//	Notes:				None.
//
//-----------------------------------------------------------------------------

static void ProcB63HstPrData( void )
{
	UWORD Len;

	// Get the length and point to the data 
	ProcB63New(  );

	// Clear Additional Host Print Data Buffer 
	memset( (UBYTE*)&HSTPRDATA, 0, S_HSTPRDATA );

	// Set the move length equal to the smallest of the buffer size
	// and the actual data length 
	if ( FldLen < S_HSTPRDATA )
	{
		// Actual data length is the smaller than the buffer size 
		Len = FldLen;
	}
	else
	{
		// Buffer size is equal to or larger than the actual data length 
		Len = S_HSTPRDATA;
	}

	// Copy the Host Print Data 
	memcpy( HSTPRDATA, pRecvData, Len );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//	ProcB63NewText	Process New Text Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			@ydeniz -- BC 
//
//-----------------------------------------------------------------------------

static void ProcB63HstDispData( void )
{	
        UBYTE Length;

	ProcB63New(  );
        ClrIt((UBYTE *)&HSTDISPDATA, sizeof(HSTDISPDATA));

        if (FldLen < sizeof(HSTDISPDATA))		// Yes; use this length 
            Length = FldLen;
	else						// No; user maximum Host Text length 
            Length = sizeof(HSTDISPDATA);

        memcpy(HSTDISPDATA, pRecvData, Length);	// Save the Host Text 

	// Process the Host Text 
	ProcB63HostText(  );
}


//-----------------------------------------------------------------------------
//	ProcB63OldText	Process Old Text Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63OldText( void )
{
	// Get the length and point to the data 
	ProcB63Old(  );

	// Process the Host Text 
	ProcB63HostText(  );
}


//-----------------------------------------------------------------------------
//	ProcB63PS2000	Process PS2000 fields.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			This is a Sale/Refund/Auth Only that may have
//					PS2000 fields in Bit 63.
//
//-----------------------------------------------------------------------------

static void ProcB63PS2000( void )
{
	UWORD Length;

	// Get the length and point to the data 
	ProcB63New(  );

	// Calculate length to copy 
	Length = sizeof( TRREQ.TRPSI ) + S_TRTRANID + S_TRVALID;

	// Copy PS2000 data 
	memcpy( &TRREQ.TRPSI, pRecvData, Length );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + Length );

	// If a value already exists in the Authorized Amount field,
	// don't overwrite it 
	if ( NullComp( ( char * ) TRREQ.TRAUTHAMT, S_TRAUTHAMT ) )
	{
		// No; copy Total Amount into PS2000 Authorized Amount 
		memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAM, S_TRAUTHAMT );
	}
}


//-----------------------------------------------------------------------------
//	ProcB63RefData	Process Host Reference Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63RefData( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Move in the binary length 
	memcpy( TRREQ.TRREFDATA, &FldLen, 2 );

	// Copy the Host Reference Data 
	memcpy( TRREQ.TRREFDATA + 2, pRecvData, FldLen );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//	ProcB63Schedule	Process a Schedule Event Request.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63Schedule( void )
{
	struct schedule_rec *pSchdPtr;	// Pointer to SCHTAB entry 
	UBYTE TmpMsg[15];
        UBYTE TmpMsg2[30];
	UBYTE i = 0, len;

	// Get the length and point to the data 
	ProcB63New();

	// Find a free schedule table entry 
	for (pSchdPtr = SCHTAB; pSchdPtr < &SCHTAB[SCHMAX]; pSchdPtr++) {
		// Look at every entry to see if this entry is not used. 
		if (!(pSchdPtr->SCHFLAG & SCH_USED))	// Found one 
			break;
		i++;
	}

	if (i >= SCHMAX)	// If not found unused entry, point to first schedule table entry 
          pSchdPtr = SCHTAB;

	// Clear this unused entry first 
	ClrIt((UBYTE *)pSchdPtr, (UWORD)SCHSZE);

	// Set entry flag to used 
	pSchdPtr->SCHFLAG |= SCH_USED;

	// Get and convert scheduled information to BCD 
	AscHex((UBYTE *)&TmpMsg[0], (char *)pRecvData, (UBYTE)15);
        memcpy( TmpMsg2, pRecvData, 30);

	// Schedule Type	
        // ‘00’ = No Action
        // ‘01’ = Schedule Program Downline Load
        // ‘02’ = Schedule Initialization
	pSchdPtr->SMTYPE = TmpMsg[0];

	memcpy(pSchdPtr->SMWHEN, &TmpMsg[1], S_SMWHEN);

        pSchdPtr->SMTELTYPE = TmpMsg[S_SMWHEN + 1];
	
        for (  len = 0; len < 14 && TmpMsg2[16 + len] != 0x20 ; ++len ) ;
        memset(pSchdPtr->SMTELNO, 0xFF, 7 );
        AscHex((UBYTE *)pSchdPtr->SMTELNO, &TmpMsg2[16], (len/2+len%2));	
        if ( len % 2 )
        {
          pSchdPtr->SMTELNO[len/2+len%2 - 1] = pSchdPtr->SMTELNO[len/2+len%2 - 1] | 0x0F ;
        }
        
        // ‘00’ = Do not use telephone number information that follows
        // ‘01’ = Use the following temporary telephone number
        // ‘02’ = Use primary transaction telephone number from first acquirer table as a temporary telephone number
        // ‘03’ = Permanent change ( Not Implemented)

        switch( pSchdPtr->SMTELTYPE )
        {
          case 0x00: 
                 memcpy( TempInitNo, TERM.TERMInitTel, S_SMTELNO);
                 break;

          case 0x01:
                memcpy( TempInitNo, pSchdPtr->SMTELNO, S_SMTELNO);
                break;

          case 0x02:
                memcpy( TempInitNo, AQTAB[0].conn.dial.trans.primary.phoneNum, S_SMTELNO);  
                break;

          case 0x03:
                memcpy( TERM.TERMInitTel, pSchdPtr->SMTELNO, S_SMTELNO);
                memcpy( TempInitNo, TERM.TERMInitTel, S_SMTELNO);
                SetTERMTabLRC();
                break;

          default:
              memcpy( TempInitNo, TERM.TERMInitTel, S_SMTELNO);
              break;           
        }

        
        SetSCHTabLRC();	
	ProcB63SvPtr(pRecvData + FldLen);	// Save the pointers
}


//-----------------------------------------------------------------------------
//	ProcB63Surc		Process Surcharge Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63Surc( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Get Surcharge Amount 
	AscHex( TRRSP.TRSURC, ( char * ) pRecvData, S_TRBASE );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + S_TRBASE );

	// Build Total with Network Fee 
	AddAmt( TRRSP.TRTOTF, TRRSP.TRTOTAM, TRRSP.TRSURC );
}


//-----------------------------------------------------------------------------
//	ProcB63AVS		Process Address Verification Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63AVS( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Store Address Verification Response 
	memcpy( TRREQ.TRAVSRSP, pRecvData, FldLen );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//	ProcB63New		Process New Style Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63New( void )
{
	// Get length in binary 
	FldLen = BcdBin( pRecvData );

	// Subtract from overall length 
	ProcB63Len(  );

	// Subtract 2 for data type code 
	FldLen -= 2;

	// Point past data length and table id 
	pRecvData += 4;
}


//-----------------------------------------------------------------------------
//	ProcB63Old		Process Old Style Data.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------
static void ProcB63Old( void )
{
	// Length of old style is the length of all of field 63 
	FldLen = FldLen63;

	// Clear the overall field length 
	FldLen63 = 0;
}


//-----------------------------------------------------------------------------
//	ProcB63Len		Update overall length.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63Len( void )
{
	// Subtract field length and length bytes from overall length 
	FldLen63 -= FldLen + 2;
}


//-----------------------------------------------------------------------------
//	ProcB63HostText	Process the Host Text.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------
static void ProcB63HostText( void )
{
	UWORD Length;

	// Clear the Host Text 
	memset( TRREQ.TRHOSTTEXT, 0, S_TRHOSTTEXT );

	// Check if field length smaller than maximum Host Text length 
	if ( FldLen < S_TRHOSTTEXT )
	{
		// Yes; use this length 
		Length = FldLen;
	}
	else
	{
		// No; user maximum Host Text length 
		Length = S_TRHOSTTEXT;
	}

	// Save the Host Text 
	memcpy( TRREQ.TRHOSTTEXT, pRecvData, Length );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + Length );
}


//-----------------------------------------------------------------------------
//	ProcB63SvPtr	Save the pointers for the next field.
//
//	Parameters: 	UBYTE * pData			pointer to a data field.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63SvPtr( UBYTE * pData )
{
	// Save the pointers for the next field 
	pRecvData = pData;
	pTempData = pData + 2;
}


//-----------------------------------------------------------------------------
//	RspAct			Take specific actions from Request
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void RspAct( void )
{
	// Update System Time/Date from Host Response 
	UpdateTime(  );
}


//-----------------------------------------------------------------------------
//	MoveFields		Move Fields From TRRSP to TRREQ.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void MoveFields( void )
{
	UBYTE index;
	UBYTE i;

	// See if Balance Inquiry Transaction 
	if ( TRREQ.TRKEY == BALINQ )
	{
		// Yes; set index to beginning of table 
		index = 0;
	}
	else
	{
		// No; set index to beginning of non-balance fields 
		index = 2;
	}

	// Move fields from TRRSP to TRREQ 
	for ( i = index; i < N_MOVTABINQ; i++ )
	{
		memcpy( MOVTABINQ[i].pDest, MOVTABINQ[i].pSrc, MOVTABINQ[i].dlen );
	}
}


//-----------------------------------------------------------------------------
//	ValRsp			Validate Response is for this Terminal
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Bool	True  - Response for this terminal,
//							False - otherwise.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static Bool ValRsp( void )
{
	// Make sure the following fields are the same in the Response as
	// in the Request 

	// Check Terminal ID 
	if ( memcmp( TRREQ.TRTID, TRRSP.TRTID, S_TRTID ) == 0 )
	{
		// The same; check if Trace Number exists 
		if ( !NullComp( ( char * ) TRRSP.TRSTAN, S_TRSTAN ) )
		{
			// Yes; check Trace Number 
			if ( memcmp( TRREQ.TRSTAN, TRRSP.TRSTAN, S_TRSTAN ) == 0 )
			{
				// The same; response for this Terminal 
				return ( True );
			}
		}
		else
		{
			// No; response for this Terminal 
			return ( True );
		}
	}

	// Response not for this Terminal 
	return ( False );
}


//-----------------------------------------------------------------------------
//	ProcB63LVL2ID	Process Level II ID fields.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63LVL2ID( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Store Level II ID Response 
	memcpy( &TRREQ.TRLVL2ID, pRecvData, sizeof( TRREQ.TRLVL2ID ) );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + sizeof( TRREQ.TRLVL2ID ) );
}


//-----------------------------------------------------------------------------
//	ProcB63CVV2		Process CVV2 fields.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static void ProcB63CVV2( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}

#ifdef MAKE_ISOMAC
//-----------------------------------------------------------------------------
//	ProcB64			Process Bit 64 of Response
//	Parameters:
//	Global Inputs:
//	Returns:
//	Notes:
//-----------------------------------------------------------------------------
static void ProcB64( void )
{
	UBYTE rcvMAC[ 8 ];
    int i;
	static char buf[8];
	char* ptr = buf;

	UBYTE PinMacId;					// Mac index.
	UBYTE *pMacKeyPtr;				// Pointer to MAC key.
	UBYTE *pMacDataPtr;				// Pointer to data to be MAC'ed.
	UWORD MacDataLength;			// Length of data to be MAC'ed.
	UBYTE PinData[8];				// Used for passing data to/from pinpad.

    // Check if MAC calculation enabled
    if ( !( ( TRREQ.TRAQPTR->AQOPT4 ) & AQ4_MAC ) )
        return;

	// Process bit 64 data if it exists.
	if ( p64Data == NULL )
    {
        // MAC expected but not found -- 
        // Set in a Invalid MAC Response Code
        memcpy( TRRSP.TRRSPC, RSP_IM, S_TRRSPC );
        return;
    }
    else
	{
		GetRspTxtOpts();

        // Check MAC if approved
		if( ( RSPOPT & R_APPROVED ) || ( RSPOPT & R_UPLOAD ) )
		{
			// Move received MAC to rcvMAC
			memcpy( rcvMAC, p64Data, 8 );

			// Set data pointer
			pMacDataPtr = RECVBUF + TPDU_SZE;
			
            // Set data length for MAC calulating
			MacDataLength = p64Data - RECVBUF - TPDU_SZE;
			
	
            // Set Master Key ID
            PinMacId = TRREQ.TRAQPTR -> AQMACID;

            // Pointer to encrypted MAC key
            pMacKeyPtr = TRREQ.TRAQPTR -> AQMACKEY;
			
            // Display info message.
			ShowInfoMsg ( ProcNow, N_Pinpad );
			
            // Calculate MAC
			PinGenMac( PinMacId, pMacKeyPtr, pMacDataPtr, MacDataLength, PinData );

            for (i = 0; i < 4; i++) 
            {
                unsigned char n = (PinData[i] >> 4) & 15;
                *ptr++ = n + (n < 10 ? '0' : 'A' - 10);
                n = PinData[i] & 15;
                *ptr++ = n + (n < 10 ? '0' : 'A' - 10);
			 }
			 memcpy(PinData, buf, sizeof(buf));

            // Compare MAC
			if( memcmp( rcvMAC, PinData, 4 ) != 0 ) 
            {
                // Set in a Invalid MAC Response Code
                memcpy( TRRSP.TRRSPC, RSP_IM, S_TRRSPC );
            }
        }
	}
}
#endif	// MAKE_ISOMAC

// @ydeniz DK DK --- testler aþamasýnda yazýlacak
//-----------------------------------------------------------------------------
//	ProcB63Inst		Process Installment Data.
//-----------------------------------------------------------------------------
static void ProcB63Inst(void)
{
        UBYTE *pTempPtr;
	// Get the length and point to the data 
	ProcB63New();
	
	pTempPtr= pRecvData;

	// Save installments quantity
	memcpy(instnum,(char *)pTempPtr, 2);     
        AscHex(&TRREQ.TRINSTNUM, instnum, 1);
	pTempPtr +=2;
	AscHex(ekstreamount, (char *)pTempPtr, 6);
	pTempPtr +=12;
	memcpy(billingdate,(char *)(pTempPtr+2), 8);
	pTempPtr +=8;

	// Save the pointers 
	ProcB63SvPtr(pRecvData + FldLen);
}

//-----------------------------------------------------------------------------
//	ProcB63Settle		Process Settlement Data. // @ydeniz -- BC
//-----------------------------------------------------------------------------
static void ProcB63Settle(void)
{
  UBYTE *pTempPtr;
  UWORD TempLen;
	
  ProcB63New();

  pTempPtr = pRecvData;
  TempLen = FldLen;

  memset( ( UBYTE * ) &SETTLETOTALS, 0,SETTOTSZE );

  if (TempLen >= 15) 
  {
    MoveIt_NoSpace(SETTLETOTALS.TOTALCNT,(char *)pTempPtr, 3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.TOTALAMT, (char *)pTempPtr, 6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.NETCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.NETAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }
    
   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.DMCRCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.DMCRAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }
    
   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.INTVISACRCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.INTVISACRAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.INTEUROCRCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.INTEUROCRAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }
  
   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.DMDBCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.DMDBAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }
    
   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.INTDBCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.INTDBAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.SALECNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.SALEAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.REFUNDCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.REFUNDAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.VOIDCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.VOIDAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }

   if (TempLen >= 15) 
   {
    MoveIt_NoSpace(SETTLETOTALS.PAYCNT,(char *)pTempPtr,3);	
    pTempPtr += 3;
    AscHex_NoSpace(SETTLETOTALS.PAYAMT, (char *)pTempPtr,6);
    pTempPtr += 12;
    TempLen -= 15;
   }
  
   // son günsonu raporu için sakla
   TEMPSETTLE = SETTLETOTALS;

   ProcB63SvPtr(pRecvData + FldLen);

}

//-----------------------------------------------------------------------------
//	MoveIt_NoSpace		
//-----------------------------------------------------------------------------
static void MoveIt_NoSpace(UBYTE *dest, char *source, UBYTE len)
{
	UBYTE i;

	for( i=0; i < len; i++)         
              if(source[i] == ' ')
                 source[i] = 0x00;
              else
                  source[i] -= 0x30;
	

	memcpy( dest, source, len );
}

//-----------------------------------------------------------------------------
//	AscHex_NoSpace	
//-----------------------------------------------------------------------------
static void AscHex_NoSpace(UBYTE *dest, char *source, UBYTE len)
{
	UBYTE i;

	if(!memcmp((UBYTE *)source, (UBYTE *)"            ", 12)) {
		ClrIt(dest, len);
		return;
	}
	for(i=0; i < (len * 2); ++i) {
		if(source[i] == ' ')
			source[i] = '0';
		else
			break;
	}
	AscHex(dest, source, len);
	return;
}


//-----------------------------------------------------------------------------
//	ProcB63CardHolderID	
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------
// table id 88 Cardholder ID Method
static void ProcB63CardHolderID( void )
{
    ProcB63New(  );
    
    if ( pRecvData[0] == 'P' )
    {	
        TRREQ.TRFLAG |= TR_PINFLAG;
    }

    if ( pRecvData[1] == 'D' )
    {
        // debit kart 
    }
    else if ( pRecvData[0] == 'C' ) 
    {
        // kredit kart 
    }

    if ( pRecvData[2] == 'I' )
    {
        // international kart 
    }
    else if ( pRecvData[0] == 'D' )
    {
        // domestik kart 
    }

    ProcB63SvPtr(pRecvData + FldLen);	// Save the pointers;
    
}

// Table Id 89
static void ProcB63AvailReward( void )
{
    ProcB63New(  );

#ifdef MAKE_KIB 
    AscHex( TRREQ.AVAIL_LOYALTY_AMT, ( char *) &pRecvData[0], 6 );        
    TRREQ.TRFLAG |= TR_HASPOINT;
#endif
    
   pRecvData += 12;				

   ProcB63SvPtr( pRecvData );		
}

// Table Id 90
static void ProcB63UsedReward( void )
{
    ProcB63New(  );

#ifdef MAKE_KIB 
    AscHex( TRREQ.TRUSEDLOYALTYAMOUNT, ( char *) &pRecvData[0], 6 );
#endif
    
    pRecvData += 12;				

    ProcB63SvPtr( pRecvData );
}

// Table ID 90 Telefon numaralarý ve IP Update 
static void ProcIPUpdate( void )
{
  UBYTE index, len, i;
  UBYTE tempipaddr[15];

  ProcB63New(  );

  index = 0;
  if(pRecvData[index++] == 'Y')
  {   
      len = 0; 
      for(i = index; pRecvData[i] != 0x00 ; i++)               
            len++;   
                
      memset((UBYTE *)TRREQ.TRAQPTR->conn.dial.trans.primary.phoneNum, 0xFF, 12);
      AscHex((UBYTE *)TRREQ.TRAQPTR->conn.dial.trans.primary.phoneNum, &pRecvData[index], (len/2+len%2));	
      if ( len % 2 )
      {
        TRREQ.TRAQPTR->conn.dial.trans.primary.phoneNum[len/2+len%2 - 1] = TRREQ.TRAQPTR->conn.dial.trans.primary.phoneNum[len/2+len%2 - 1] | 0x0F ;
      }
      SetAQTabLRC ();      
  }
  index += 15;

  if(pRecvData[index++] == 'Y')
  {   
      len = 0; 
      for(i = index; pRecvData[i] != 0x00 ; i++)               
        len++;   

      memset((UBYTE *)TRREQ.TRAQPTR->conn.dial.trans.secondary.phoneNum, 0xFF, 12);
      AscHex((UBYTE *)TRREQ.TRAQPTR->conn.dial.trans.secondary.phoneNum, &pRecvData[index], (len/2+len%2));	
      if ( len % 2 )
      {
        TRREQ.TRAQPTR->conn.dial.trans.secondary.phoneNum[len/2+len%2 - 1] = TRREQ.TRAQPTR->conn.dial.trans.secondary.phoneNum[len/2+len%2 - 1] | 0x0F ;
      }
      SetAQTabLRC ();      
   }
   index += 15;
    
   if(pRecvData[index++] == 'Y')
   {  
      len = 0; 
      for(i = index; pRecvData[i] != 0x00 ; i++)               
        len++;   

      memset((UBYTE *)TERM.TERMInitTel, 0xFF, 12);
      AscHex((UBYTE *)TERM.TERMInitTel, &pRecvData[index], (len/2+len%2));	
      if ( len % 2 )
      {
        TERM.TERMInitTel[len/2+len%2 - 1] = TERM.TERMInitTel[len/2+len%2 - 1] | 0x0F ;
      }
      SetTERMTabLRC();      
   }   
   index += 15;
   
   // backup parameter download 
   if( pRecvData[index++] == 'Y' )
   {
   
   }
   index += 15;

   if( pRecvData[index++] == 'Y' )
   {  
      len = 0; 
      for(i = index; pRecvData[i] != 0x00 ; i++)               
        len++;   

      memset((UBYTE *)TERM.TERMNmsTel, 0xFF, 12);
      AscHex((UBYTE *)TERM.TERMNmsTel, &pRecvData[index], (len/2+len%2));	
      if ( len % 2 )
      {
        TERM.TERMNmsTel[len/2+len%2 - 1] = TERM.TERMNmsTel[len/2+len%2 - 1] | 0x0F ;
      }
      SetTERMTabLRC();      
   }
   index += 15;

   if( pRecvData[index++] == 'Y' )
   {
      for (i = 0; i < IPCMAX; i++ ) 
        if ( IPCTAB[i].id == TRREQ.TRAQPTR->AQ_TRANS_P_IPTAB) 
            break;
      if( i <= IPCMAX ) 
      {
        memcpy(&IPCTAB[i].ipaddr, &pRecvData[index], 15);
        SetIPCTabLRC();
      }
   }
   index += 15;  

   if( pRecvData[index++] == 'Y' )
   {
      for (i = 0; i < IPCMAX; i++ ) 
        if ( IPCTAB[i].id == TRREQ.TRAQPTR->AQ_TRANS_S_IPTAB ) 
            break;
      if( i <= IPCMAX ) 
      {
        memcpy(&IPCTAB[i].ipaddr, &pRecvData[index], 15);
        SetIPCTabLRC();
      }
   }
   index += 15;    

   if ( pRecvData[index++] == 'Y')
   {    
        memcpy(tempipaddr, &pRecvData[index], 15);
        TERM.TERMInitIP = convert_addr(tempipaddr);
   }
   index += 15; 
			

  ProcB63SvPtr( pRecvData + FldLen);
}

static void ProcDiscountData( void )
{  
  UBYTE index = 0;
  ProcB63New(  );
  
  AscHex ( TRREQ.TRDISCOUNTTOTAM, ( char *) &pRecvData[index], S_TRTOTAM );
  index += 12;

  AscHex ( TRREQ.TRDISCOUNTRATIO, ( char *) &pRecvData[index+8], S_TRTOTAM );  

  pRecvData += 24;				

  ProcB63SvPtr( pRecvData );

}



//-----------------------------------------------------------------------------
//  inet_addr		Converts an IP address in a string to a UDWORD
//  Parameters:
//  Global Inputs:
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
static UDWORD convert_addr(const char *s)
{
//	int i;
	union
	{
		UDWORD l;
		UBYTE b[4];
	} ip;
	
	const char *text = s;

	while(1) 
	{
		ip.b[3] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[2] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[1] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[0] = (UBYTE)atoi( text );

		return ip.l;
	}
	return 0;
}

