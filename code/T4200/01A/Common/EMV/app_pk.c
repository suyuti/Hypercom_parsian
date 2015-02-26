#ifdef MAKE_EMV
//*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 1998                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  Hypercom Base Application in C                  *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV Application Page 2                          *        */
/*  *                                                               *        */
/*  *  FILENAME  :  app_pk.c                                     	*        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  11/02/00  Alexei Toumantsev 1st Draft                     	*        */
/*  *  08/01/02  A.Shuttleworth    Removed emvtabs1.h reference  	*        */
/*  *                                                               *        */
/*  *****************************************************************        */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"

#include "defines.h"
#include "utllib.h"

#include "emv2_pk.h"
#include "emv_l2.h"
#include "scard.h"

/***************** External Function prototypes **************/
extern void InitTable( UBYTE *, UBYTE *, UWORD, UBYTE * );
extern void Clr_TermInit( void );

/***************** Local Function prototypes **************/
void InitEMVTabs1 ( void );

void CAPKTabPre ( void );
void CAPKTabPost ( void );

void REVKTabPre ( void );
void REVKTabPost ( void );

void KeyExpiryTabPre ( void );
void KeyExpiryTabPost ( void );

void P60_LDIR_EMVTabs1 ( void );
void Val_EMVTabs1 ( void );
static void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec );
static UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC );
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void PrintCorruptKeys( void );
void PrintKeyTable( void );
#endif // !(MAKE_P2100) && !(MAKE_H2200)

//
/*--------------- EXTERNAL VARIABLE DATA ---------------------*/
#ifdef MAKE_EMV
/* Parameters passed by RSPINIT to EMV initialisation routines */
//! Subfield data start pointer
extern UBYTE *pSFData;
//! Pointer to destination buffer
extern UBYTE *pB60DAddress;
//! Number of bytes to move
extern UWORD MoveSize;
//! B60DBytesLeft is passed from EMV Pre-processing routines to RSPINIT
//! Bytes left in destination buffer
extern UWORD B60DBytesLeft;
#endif /* MAKE_EMV */
//

//! Used for locating the Atmel crypto processing chip
extern SCRDDEVICE CryptSlot;

//! Public Keys Table formatter
extern UBYTE PKFMT;
//! Public Keys Table
extern struct CAPublicKeyEntry PublicKeyStore[];
//! Public Keys Table LRC
extern UBYTE PKLRC;

//! Public Keys Lookup Table
extern struct PublicKeyLookupEntry KeyLookupTable[KEYMAX];

//! Revocation Table formatter
extern UBYTE RKFMT;
//! Revocation Table
extern struct RevocationEntry RevocationTable;
//! Revocation Table LRC
extern UBYTE RKLRC;

//! Key Expiry Table formatter
extern UBYTE KEFMT;
//! Key Expiry Table
extern struct KeyExpiryEntry KeyExpiryTable[KEYMAX];
//! Key Expiry Table LRC
extern UBYTE KELRC;


//-----------------------------------------------------------------------------
//! \brief
//!     Initialises CAPK and REVK keys
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitEMVTabs1 ( void )
{
// Clear the CA Public Keys table 
	InitTable( &PKFMT, (UBYTE *)&PublicKeyStore,  KEYMAX*KEYSZE, &PKLRC );  
// Clear the CA Public Key lookup table 
	memset(KeyLookupTable, 0, KEYMAX*KEYLOOKUPSZE);  
// Clear the Revoked Keys table 
	InitTable( &RKFMT, (UBYTE *)&RevocationTable,  REVSZE, &RKLRC );  
// Clear the Key Expiry table 
	InitTable( &KEFMT, (UBYTE *)&KeyExpiryTable,  KEYMAX*KEYEXPSZE, &KELRC );  
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup CAPK destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void CAPKTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *)&PublicKeyStore, KEYSZE, KEYMAX );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup REVK destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void REVKTabPre ( void )
{
	pB60DAddress = (UBYTE *)&RevocationTable;
    B60DBytesLeft = REVSZE;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Indexed Table Pre-Processing
//!
//! \param
//!		[out]pdest			Pointer to Array of Destination Buffers
//!		[in]szeRec			Size of one Destination Buffer in the array
//!		[in]numRec			Number of Dest Buffers in the array
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec )
{
    UBYTE index;

    /* Get array index into table from subfield */
    index = CvtBin( *pSFData );      

    if ( (index == 0) || (index > numRec) ) 
    {
        /* No valid buffer to load */
        B60DBytesLeft = 0;            
    }
    else
    {
    	/* Destination buffer */
    	pB60DAddress  = pdest + szeRec*(index-1);        
    
    	/* How much to copy */
    	B60DBytesLeft = szeRec;                      
    }
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup Key Expiry Table destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void KeyExpiryTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *)&KeyExpiryTable, KEYEXPSZE, KEYMAX );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for Key Expiry Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void KeyExpiryTabPost ( void )
{
	KELRC = CalcLrc( (UBYTE *)&KeyExpiryTable, KEYEXPSZE*KEYMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&KeyExpiryTable );
#endif
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to a Local Buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void P60_LDIR_EMVTabs1( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for CA Public keys
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void CAPKTabPost ( void )
{
	PKLRC = CalcLrc( (UBYTE *)&PublicKeyStore, KEYSZE*KEYMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&PublicKeyStore );
#endif
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for Revoked keys
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void REVKTabPost ( void )
{
	RKLRC = CalcLrc( (UBYTE *)&RevocationTable, REVSZE );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&RevocationTable );
#endif
}

//-----------------------------------------------------------------------------
//! \brief
//!     Validates EMV tables
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Val_EMVTabs1 ( void )
{
	UBYTE status;

  	ValidateTable( &PKFMT, (UBYTE *)&PublicKeyStore, KEYSZE*KEYMAX, &PKLRC );    

  	ValidateTable( &RKFMT, (UBYTE *)&RevocationTable, REVSZE, &RKLRC );    

	status = EMVL2_ValidateKeyStore( );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Checks validity of table
//!
//! \param[in/out] pTabFMT      Pointer to the table formatter byte
//! \param[in/out] pTabData     Pointer to the table data
//! \param[in] len              Table data length
//! \param[out] pTabLRC         Pointer to the table LRC byte
//! 
//! \return
//!     True if ok, False if bad.
//!
//! \note   If bad, clear Terminal Initialization Done Flag.
//-----------------------------------------------------------------------------
static UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len, UBYTE *pTabLRC )
{
  	UBYTE oldlrc;
  	UBYTE newlrc;

  	/* Check if Table has been formatted */
  	if ( *pTabFMT == 0x55 )
  	{
       	/* Yes; get present LRC */
       	oldlrc = *pTabLRC;
       	
       	/* Calculate LRC */
       	newlrc = CalcLrc( pTabData, len );

       	/* Check if LRCs are the same */
       	if ( newlrc == oldlrc )
		{
			/* Yes; return okay */
           	return( True );
		}
  	}

  	/* Table is bad, so fix it */
  	InitTable( pTabFMT, pTabData, len, pTabLRC );

    /* Clear Terminal Initialization Done Flag */
    Clr_TermInit( );

  	/* Table has not been initialized */
  	return( False );
}


#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
//-----------------------------------------------------------------------------
//! \brief
//!     Prints a line for any corrupt keys found
//! 
//! \return
//!     None.
//!
//! \note   uses global KeyLookupTable.
//-----------------------------------------------------------------------------
void PrintCorruptKeys( void )
{
    UBYTE index;
    char KeyPrintString[41] ; // max length of normal chars on receipt
    OS_RETURN_VALS PrinterStatus;
    Bool LineWasPrinted = False;

    PrinterStatus = SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) ;
    if ( PrinterStatus == orvOK )
    {
        // Check each key for corruption
        index = 0;
        while ((index < KEYMAX) && (KeyLookupTable[index].RID[0] != 0))
        {
            if (KeyLookupTable[index].GoodHash != True)
            {
                // Print a line for corrupt key
                SDK_PrintString ("KEY CORRUPT, RID/INDEX: ");
                SDK_PrintString ( "\n" );
                
                BfAscii( KeyPrintString, &KeyLookupTable[index].RID[0], 5);
                KeyPrintString [10] = '/';
                KeyPrintString [11] = '\0';
                SDK_PrintString( KeyPrintString );

                BfAscii( KeyPrintString, &KeyLookupTable[index].Index, 1);
                KeyPrintString [2] = '\0';
                KeyPrintString [3] = '\n';
                SDK_PrintString( KeyPrintString );

                LineWasPrinted = True;
            }

            index++;
        }

        // Close Printer
        /* Perform a form feed which cuts the paper */
        if (LineWasPrinted)
        {
       	    SDK_PrintString( "\n" );
       	    SDK_PrintString( "\n" );
       	    SDK_PrintString( "CALL HELP DESK" );
       	    SDK_PrintString( "\f" );
		    
            /* Wait till printing is done */
			while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )	//@ZRS++
            {
	             /* Give up the CPU */
	             SDK_RelinqCPU( );
            }
        }

        /* Close the printer */
        SDK_PrinterClose( );

    } // printer opened ok
    else
    {
        // display printer error
    }
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints a key table info
//! 
//! \return
//!     None.
//!
//! \note   uses global KeyLookupTable.
//-----------------------------------------------------------------------------
void PrintKeyTable( void )
{
	UBYTE Z8VersionID ;
    UBYTE index;
    char KeyPrintString[41] ; // max length of normal characters on receipt
//    OS_RETURN_VALS PrinterStatus;
    Bool LineWasPrinted = False;
    static const char NewLine[] = "\n";

    //SDK_PrinterClose();
    //PrinterStatus = 
    SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) ;
    //if ( PrinterStatus == orvOK ) 
    {

        // Print a line for key
        SDK_PrintString ("RID/INDEX");
        SDK_PrintString( (char *) NewLine );
        SDK_PrintString ("=========");
        SDK_PrintString( (char *) NewLine );

        // Check each key for corruption
        index = 0;
        while ((index < KEYMAX) && (KeyLookupTable[index].RID[0] != 0))
        {
            LineWasPrinted = True;
            
            BfAscii( KeyPrintString, &KeyLookupTable[index].RID[0], 5);
            KeyPrintString [10] = '/';
            KeyPrintString [11] = '\0';
            SDK_PrintString( KeyPrintString );

            BfAscii( KeyPrintString, &KeyLookupTable[index].Index, 1);
            KeyPrintString [2] = '\0';
            SDK_PrintString( KeyPrintString );

            if (KeyLookupTable[index].GoodHash == True)
                SDK_PrintString (" - hash good");
            else
                SDK_PrintString (" - hash bad");

            SDK_PrintString( (char *) NewLine );

            index++;
        }

        if (!LineWasPrinted)
        {
           	SDK_PrintString( "No Keys Present" );
            SDK_PrintString( (char *) NewLine );
        }

        // Report on Atmel
        SDK_PrintString( (char *) NewLine );
        SDK_PrintString( "HCP :  " );
        if (CryptSlot != 0)
        {
            BfAscii( KeyPrintString, (UBYTE *) &CryptSlot, 1);
            SDK_PrintString( KeyPrintString );
			// print out the Atmel Version
            SDK_PrintString( ". ver: " );
            SDK_PrintString( (char *) Atmel_ATR );

        }
        else
        {
            SDK_PrintString( "absent" );
        }
		// print out the scard card and Z8 version
		SDK_PrintString( (char *) NewLine );
		ScrdVersion ( (char *) KeyPrintString, &Z8VersionID ) ;
        SDK_PrintString( KeyPrintString );
        SDK_PrintString( ". Z8: " );
        KeyPrintString [0] = Z8VersionID ;
        KeyPrintString [1] = '\0' ;
        SDK_PrintString( KeyPrintString );
		SDK_PrintString( (char *) NewLine );

        // Close Printer
        /* Perform a form feed which cuts the paper */
       	SDK_PrintString( "\f" );
		
        /* Wait till printing is done */
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )	//@ZRS++
        {
	         /* Give up the CPU */
	         SDK_RelinqCPU( );
        }

        /* Close the printer */
        SDK_PrinterClose( );

    } 
}
#endif // !(MAKE_P2100) && !(MAKE_H2200)
#endif

