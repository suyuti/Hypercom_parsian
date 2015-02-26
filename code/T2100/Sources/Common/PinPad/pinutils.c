
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
// Public defines and typedefs
//=============================================================================
extern Bool IsPinRequired( void );

extern void FormatPin( void );
extern Bool piniAlSifrele();
extern UBYTE binToBcd_( UBYTE *deger );
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
//  GetPIN          Gets the PIN from the PINPAD, if required
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        chRetval =  0 to abort the transaction.
//                              1 to continue the transaction, pin not required.
//                              2 to continue, pinpad was required and OK.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UBYTE GetPIN( void )
{
	UBYTE chRetval;

	chRetval = 1;				// Continue the transaction
	
        if ( ! IsPinRequired( ) )
            return chRetval;
	
	//if ( !PinTranEnabled(  ) ) // Check if PIN PAD enabled.
	//	return chRetval;
        
        // Tell host we have PIN entry capability
        TRINP.TRPOSE[1] = ( TRINP.TRPOSE[1] & 0xF0 ) | 0x01;
        
        // TODO !!!!

        piniAlSifrele();

        return 2;


        // TODO !!!!

	if ( !PinGetPin ( False ) )
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
//  PinTranEnabled  Is use of the PIN Pad enabled for this transaction?
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          
//                  True if PIN pad is enabled for this transaction,
//                  False otherwise
//
//                  ISPINTRAN bit on indicates no pin required.
//
//-----------------------------------------------------------------------------
/*
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
              case EMV_INSTALLMENT:
              case EMV_FALLBACK:
                  // CVM list & term capabilities should be enough 
                  // to determine that we should ask for PIN
                  break;
#endif	//MAKE_EMV

              case SALE:   
              case DEBIT:
                      if (ispintran & ISPIN_SALE)		// No PIN? 
                              bRetVal = False;                        
                      break;                                        
                              

              case REFUND:
                      if (( ispintran & ISPIN_RETURN ) )	// No PIN?
                              bRetVal = False;
                      break;		

              case BALINQ:
                      if ( ispintran & ISPIN_BALINQ )	// No PIN?
                              bRetVal = False;
                      break;				

              case VOIDTRAN:
                      if ( ( ispintran & ISPIN_VOID ) )// No PIN?
                              bRetVal = False;
                      break;

              default:
                      bRetVal = False;
                      break;
	}

	return bRetVal;
}
*/


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  IsPinRequired   Transaction pin istiyor mu
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Transaction PIN istiyorsa   True
//                                  istemiyorsa False döndürür
//
//                  
//
//-----------------------------------------------------------------------------
extern Bool IsPinRequired( void )
{

    // Does the issuer want PIN entry?
    if ( ((TRINP.TRISPTR->ISOPT1) & IS1_PIN) )
    {
        switch( TRINP.TRKEY )
        {
            case SALE:
              if ( TRINP.TRISPTR->ISPINTRAN & ISPIN_SALE )
                  return True;
              break;              
            
            case REFUND:
              if ( TRINP.TRISPTR->ISPINTRAN & ISPIN_RETURN )
                 return True;
              break;

            case VOIDTRAN:
            case PREAUTHCANCEL:
            case CLOSEDPAUTHCANCEL:
              if ( TRINP.TRISPTR->ISPINTRAN & ISPIN_VOID )
                  return True;
              break;
    
            case BALINQ:
              if ( TRREQ.TRISPTR->ISPINTRAN & ISPIN_BALINQ )
                  return True;
              break;
    
            case PREAUTH:
              if ( TRINP.TRISPTR->ISPINTRAN & ISPIN_PREAUTH ) 
                  return True;
              break;
    
            case CLOSED_PREAUTH:
              if ( TRINP.TRISPTR->ISPINTRAN & ISPIN_CLOSEDPREAUTH )
                  return True;
              break;
           
            default:
                // Pin gerekmiyor
                return False;
        }
    }

    return False;
}

extern Bool piniAlSifrele()
{
    static const ENTRY_STR PinEntry = {
	N_Pinpad,					/* Title */
	EnterPswd,					/* Prompt */
	N_NullStr,					/* Default value */
    };

    UBYTE status, mkid;
    Bool bRetVal = False;
    UBYTE *WorkingKeyAddr;
    short WorkingKeyLen;							
    UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];	
    UBYTE pinblock[13];

    UBYTE wk1[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    UBYTE wk2[] = { 'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0' };
  
    int i;
    
    UBYTE Result[20];
    UBYTE Key[20];

    UBYTE *pWD;
    UBYTE WorkData[20];
    UBYTE WD[20];
    UWORD pinlen;


    // Tell host we have PIN entry capability
    TRINP.TRPOSE[1] = ( TRINP.TRPOSE[1] & 0xF0 ) | 0x01;
  
    CustomAmount( AmountTotals, TRINP.TRTOTAM );

    memset( pinblock, 0, sizeof( pinblock ) );

    // Do until pin entered or cancel
    do
    {
	// Read pin from the keyboard.
	status = NumEntry( 2, 12, 4, &PinEntry );

	// Check if pin number  has been entered
	if ( 1 == status )
	{
		// Yes; save the Pin entered
		MoveTillZero( ( char * ) pinblock, ( char * ) Dspbuf, sizeof( pinblock ) );
		memset( Dspbuf, 0, sizeof(Dspbuf) );
		break;
	}
	else
	{
		// Exit the function
		memset( Dspbuf, 0, sizeof(Dspbuf) );
		return bRetVal;
	}
    }while ( True );

    pinlen = strlen( ( char * ) pinblock );

    pWD = pinBlockFormat( pinblock, pinlen );

    //TODO
    //BfAscii( ( char * ) Dspbuf, TRINP.TRPAN, sizeof( FormatPanBuf ) );
    
    BfAscii( ( char * ) WD, pWD, 16 );
    
    TRINP.TRSTATUS[1] |= TS2_PINBLOCK;

    Encrypt( wk1, WD, Result);

    memcpy( WD, Result, 16 );

    Decrypt( wk2, WD, Result);

    memcpy( WD, Result, 16);
    
    Encrypt( wk1, WD, Result);

    for( i = 0 ; i < 8 ; ++i )
    {
        TRINP.TRPINBL[i] = binToBcd_( & (Result[i*2]) );
    }

    return True;
    
}

UBYTE binToBcd_( UBYTE *deger )
{
    UBYTE tmp;
    
    if ( deger[0] >= '0' && deger[0] <= '9' ) 
      tmp = deger[0] - '0' ;
    else
      tmp = deger[0] - 'A' + 10;
  
    tmp <<= 4;

    if ( deger[1] >= '0' && deger[1] <= '9' )
      tmp += deger[1] - '0';
    else
      tmp += deger[1] - 'A' + 10;

    
    
}