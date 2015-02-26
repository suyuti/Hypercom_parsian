
#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             initcryp.c
//      initialize crypto slot
//
//=============================================================================

// System includes
#include "string.h"

// HSDK includes
#include "basictyp.h"
#include "scard.h"

// Application includes
#include "struct.h"
#include "appdata.h"

// This module external declaration
#include "initcryp.h"


/*------------------------------------------------------------------
 *  Name:           InitCryptoProc
 *  Description:    InitCryptoProc function
 *  Parameters:     none
 *  Return Value:   
 *  Notes:          Writes to global variable CryptSlot
 *-----------------------------------------------------------------*/
void InitCryptoProc ( void )
{
    UBYTE   SCBytes[50];   // 50 bytes is enough for an ATR !!
    UWORD   SCLength;
    UBYTE   SCStatus;
    UBYTE   HistBytesOffset;
    UBYTE   HistLen;
    Bool    Found = False;

	memset( Atmel_ATR, 0, sizeof(Atmel_ATR) );

    CryptSlot = scrd_SC1;
     
    do
    {
         ScrdGetATR( CryptSlot, 1, SCBytes, 
			        &SCLength, &HistBytesOffset,
			        &HistLen, &SCStatus );

		 // close the session 
		 ScrdEndSession (CryptSlot) ;

         if (ATR_GOOD == SCStatus &&
             memcmp(&SCBytes[HistBytesOffset], (UBYTE*) "HYPEMV", 6) == 0)
       
         {
              Found = True;
              // save the ATR information
              memcpy ( Atmel_ATR, &SCBytes[HistBytesOffset], 11 ) ;
              Atmel_ATR[11] = 0 ; // null terminate
         }
         else
         {
             CryptSlot++;
         } 

    } while (!Found && CryptSlot < scrd_SCR);

    if (!Found) CryptSlot = 0;
}
                  	
#endif // MAKE_EMV
