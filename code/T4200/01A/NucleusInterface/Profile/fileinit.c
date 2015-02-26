//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      fileinit.c
//
// File Description:
//      Initialize terminal tables from a file.
//
//=============================================================================


// Make sure file system calls ARE NOT replaced for HFS file calls.
// We need to access the initialization files received along with the app.
#define _DO_NOT_USE_HFS_

#include <stdlib.h>
#include <string.h>
#include "basictyp.h"
#include "struct.h"
#include "comdata.h"
#include "fixdata.h"
#include "appdata.h"
#include "usevhdtfilesys.h"
#include "persistence.h"
#include "bit8583.h"

#include "DbgTrace.h"

//=============================================================================
// External variables / declarations
//=============================================================================

// Public from other modules

//! Initializes the Table indicated
extern void InitTable( UBYTE *, UBYTE *, UWORD, UBYTE * );


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define S_PARSE_BUFF 10000
#define POLYNOMIAL (unsigned long)0xedb88320


//=============================================================================
// Private function declarations
//=============================================================================
//! Initilization tables from a file
static Bool FileInit( void );
//! Reads TermMaster initilization tables from a file
static Bool ReadInitFile( char *InitFileName );
//! Clears all Initialization Tables
static void Tables_Clear( void );
//! writes each crc_table entry exactly once, with the ccorrect final value
static void make_crc_table( void );
//! Converts binary word into 4 digits BCD value
static void BinToBcd( UWORD,UBYTE * );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
//! CRC table
static unsigned long crc_table[256];
//! message string
static const char szInitializing[]= "Initializing...";
//! Termmaster database file
static const char TM_FILENAME[]= "c:\\Application.cfg";
// CRC database
static const char TM_FILE_CRC[]= "c:\\Application.crc";


//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//! \brief
//!     Checks CRC of TM init file to see if we need to update tables with this file
//!
//! \return
//!     True if update was successful
//-----------------------------------------------------------------------------
Bool UpdateTables( void )
{
    unsigned char   nData[64];
	unsigned long   nLoadCrc;
	unsigned long   nFileCrc;
    FILE            *pFile;
    size_t          i, nReadIn;
    Bool            bRes= False;

    DBGTRACE_ENTER();
    pFile = fopen( TM_FILENAME, "r" );
    if ( pFile != NULL )
    {
        bRes= True;
        // CRC lookup table generating
        make_crc_table();
        // Calculates CRC value of file content
        nFileCrc= 0xffffffff;
        while ( (nReadIn= fread( nData, 1, sizeof( nData ), pFile )) != 0 )
        {
            for( i= 0; i < nReadIn; ++i )
                nFileCrc= (nFileCrc >> 8) ^ crc_table[(nFileCrc ^ nData[i]) & 0xff];
        }
        nFileCrc^= 0xffffffff;
        fclose( pFile );

        // Read CRC value from a file
        nLoadCrc= 0;
        if ( NULL != (pFile = fopen( TM_FILE_CRC, "rb" )) )
        {
            fread( &nLoadCrc, sizeof( nFileCrc ), 1, pFile );
            fclose( pFile );
        }

        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "nLoadCrc != nFileCrc...\n" );
        if ( nLoadCrc != nFileCrc )
        {
            DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "STEP 2 AYDENÝZ :)...\n" );

            SDK_DisplayGrphText( (char*)szInitializing, 0, 0, strlen( szInitializing ) );
            Tables_Clear();

            if ( FileInit() )
            {
                // udpate the CRC file
                if ( NULL != (pFile = fopen( TM_FILE_CRC, "wb" )) )
                {
                    fwrite( &nFileCrc, sizeof( nFileCrc ), 1, pFile );
                    fclose( pFile );
                }
                Set_TermInit();
            }
            else
            {
                bRes= False;
            }
        }
		else if( 0 == TERM.TERMInit )
		{
			Set_TermInit();
		}
    }
    DBGTRACE_LEAVE();
    return bRes;
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Initilization tables from a file
//!
//! \return
//!     True on success otherwise False
//-----------------------------------------------------------------------------
static Bool FileInit( void )
{
	if ( !ReadInitFile( (char*)TM_FILENAME ) )
	{
		// Not normal data  
		// If download of icepac images fails then be sure  
		// to close the open file.              
#if 0 //ndef MAKE_T7PLUS
		if ( iphandle )
		{
			// First close the open file.                   
			fclose( iphandle );

			// Then remove the file because it may be corrupted.    
			remove( IcePak[IpIndex].pFileName );
		}
#endif
		return False;
	}
	return True;
}
		
#if 0
//-----------------------------------------------------------------------------
//!	\brief
//!	    This functions lists all BCD fields that are to be corrected in sense
//!     of replacing 0xF's with zeros, that is, virtually all BCD fields in
//!     the database
//-----------------------------------------------------------------------------
void CorrectBCDfields()
{
	DBGTRACE_ENTER();

	int iCnt;

	CorrectBCDfield( &TCONF.TCMERDBS[0], sizeof( TCONF.TCMERDBS[0] ) );
	CorrectBCDfield( &TCONF.TCMERDBS[1], sizeof( TCONF.TCMERDBS[1] ) );
	CorrectBCDfield( &TCONF.TCTIPPCT_1, sizeof ( TCONF.TCTIPPCT_1 ) );
	CorrectBCDfield( &TCONF.TCTIPPCT_2, sizeof ( TCONF.TCTIPPCT_2 ) );
	CorrectBCDfield( &TCONF.TCTIPPCT_3, sizeof ( TCONF.TCTIPPCT_3 ) );
    
	SetTCTabLRC();

    for ( iCnt = 0; iCnt < AqidCount(); iCnt++ )
	{
		CorrectBCDfield( AQTAB[iCnt].AQNII, sizeof ( AQTAB[iCnt].AQNII ) );
        CorrectBCDfield( &AQTAB[iCnt].AQSMAXTRIES, sizeof ( &AQTAB[iCnt].AQSMAXTRIES ) );
		CorrectBCDfield( &AQTAB[iCnt].AQTOUT, sizeof ( AQTAB[iCnt].AQTOUT ) );
		CorrectBCDfield( AQTAB[iCnt].AQBATCUR, sizeof ( AQTAB[iCnt].AQBATCUR ) );
		CorrectBCDfield( AQTAB[iCnt].AQBATNXT, sizeof ( AQTAB[iCnt].AQBATNXT ) );
	}
	SetAQTabLRC();

    for ( iCnt = 0; ( ISTAB[iCnt].ISID ) && ( iCnt < ISMAX ); iCnt++ )
	{
        CorrectBCDfield( &ISTAB[iCnt].ISDEFAC, sizeof ( ISTAB[iCnt].ISDEFAC ) );
		CorrectBCDfield( ISTAB[iCnt].ISFLOOR, sizeof ( ISTAB[iCnt].ISFLOOR ) );
		CorrectBCDfield( &ISTAB[iCnt].ISREAUTH, sizeof ( ISTAB[iCnt].ISREAUTH ) );
	}
	SetISTabLRC();

	for ( iCnt = 0; iCnt < CRMAX; iCnt++ )
	{
		CorrectBCDfield( &CRTAB[iCnt].CRPLEN, sizeof( CRTAB[iCnt].CRPLEN ) );
	}
	SetCRTabLRC();

	SetTERMTabLRC();

	DBGTRACE_LEAVE();
	return;
}
#endif


//-----------------------------------------------------------------------------
//! \brief
//!     Reads TermMaster initilization tables from a file
//!
//!	\param
//!		InitFileName    Path & Name of the file containing the tables.
//!
//! \return
//!     True on success otherwise False.
//!
//! \note
//!     The message will look like:<br>
//!         Table i Name Length     n2 ( 1 byte BCD )<br>
//!         Table i Name            ans var<br>
//!         Table i Data Length     n4 ( 2 bytes, 0 to 9999 BCD )<br>
//!         Table i Data            ans var<br>
//!<br>
//!         Table i Data:
//!            Table Id             n2 ( 1 byte )<br>
//!            Sub Field Length     n4 ( 2 bytes, 0 to 9999 BCD )
//!     
//-----------------------------------------------------------------------------
static Bool ReadInitFile( char *InitFileName )
{
	FILE *fpInit;

	UBYTE TabNameLen;
	UWORD TabDataLen;

	UBYTE TabID;
    UWORD FldLen;
	UWORD FldLeftLen;
    UWORD SubFldLen;
    UBYTE *pBuff;
    char  TabName[32];

	DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s: Processing Cfg file %s.\n", __FUNCTION__, InitFileName );

    // Open the file containig init data
	fpInit = fopen( InitFileName, "r" );
	if ( fpInit == NULL )
	{
		DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not open cfg file!\n", __FUNCTION__ );
		return False;
	}

    pBuff= (UBYTE*)malloc( S_PARSE_BUFF );
    if ( NULL == pBuff )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not allocate buffer!\n", __FUNCTION__ );
        return False;
    }

    // Loop for talbes, read table name length
	while ( 1 == fread( &TabNameLen, sizeof( TabNameLen ), 1, fpInit ) )
	{
		// Convert table name length from BCD to binary
		TabNameLen = CvtBin( TabNameLen );

		// Table name
        memset( TabName,0,sizeof(TabName) );
		if ( 1 != fread( TabName, TabNameLen, 1, fpInit ) )
		{
			fclose( fpInit );
            free( pBuff );
			DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not read TabNameLen!\n", __FUNCTION__ );
			return False;
		}
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "TabName: " );
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s \n",TabName );
		// Read table data length
		if ( 1 != fread( ( UBYTE * )&TabDataLen, sizeof( TabDataLen ), 1, fpInit ) )
		{
			fclose( fpInit );
            free( pBuff );
			DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not read TabDataLen!\n", __FUNCTION__ );
			return False;
		}

		// Convert table data length from BCD to binary
		TabDataLen = BcdBinW( ( UBYTE * )&TabDataLen );
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "Table --> [%s]\n", TabName );

		// Loop for sub-fields
		while ( TabDataLen > 0 )
		{
			// Read table Id
			if ( 1 != fread( ( UBYTE * )&TabID, sizeof( TabID ), 1, fpInit ) )
			{
				fclose( fpInit );
                free( pBuff );
				DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not read TabID!\n", __FUNCTION__ );
				return False;
			}	
			
			// Read sub-field length
			if ( 1 != fread( ( UBYTE * )&FldLen, sizeof( FldLen ), 1, fpInit ) )
			{
				fclose( fpInit );
                free( pBuff );
				DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not read FldLen!\n", __FUNCTION__ );
				return False;
			}

			// Convert sub-field length from BCD to binary
			FldLen = BcdBinW( ( UBYTE * ) &FldLen );

			// Loop for blocks of a sub-field
			for ( FldLeftLen = FldLen; FldLeftLen > 0; )
			{
				if ( FldLeftLen > S_PARSE_BUFF - 5 )
				{
					SubFldLen = S_PARSE_BUFF - 5;
				}
				else
				{
					SubFldLen = FldLeftLen;
				}

				// Total block length
                BinToBcd( SubFldLen + 3,&pBuff[0] );

				// Table ID
				if ( FldLeftLen == FldLen )
				{
					// The first block
					pBuff[2] = TabID;
				}
				else
				{
					// Blocks after the first one
					pBuff[2] = 0xFF;
				}

				// Block length
                BinToBcd( SubFldLen,&pBuff[3] );

				// Read block data
				if ( fread( &pBuff[5], SubFldLen, 1, fpInit ) != 1 )
				{
					fclose( fpInit );
                    free( pBuff );
					DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "%s Error: Could not read data block!\n", __FUNCTION__ );
					return False;
				}

				// Save data in tables
                bFileInit= True;
                StoreTabData( pBuff );
                bFileInit= False;

				FldLeftLen -= SubFldLen;
			}

			// Update length of data left
			TabDataLen -= ( sizeof( TabID ) + sizeof( FldLen ) + FldLen );
		}
	}

	fclose( fpInit );
    free( pBuff );

	// change 0xF in BCD fields to nulls
//	CorrectBCDfields();
    SetBatchNum();

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Clears all Initialization Tables.
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
static void Tables_Clear( void )
{
//	DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "Fileinit: Clearing all tables!\n" );

	// Terminal Configuration 
	InitTable( &TCFMT, ( UBYTE * ) & TCONF, TCSZE, &TCLRC );

    // Terminal Capabilities
    InitTable( &TCAPFMT, ( UBYTE * ) & TCAP, TCAPSZE, &TCAPLRC );

    // Card Range Table 
	InitTable( &CRFMT, ( UBYTE * ) CRTAB, CRMAX * CRSZE, &CRLRC );

	// Card Issuer Table 
	InitTable( &ISFMT, ( UBYTE * ) ISTAB, ISMAX * ISSZE, &ISLRC );

	// Card Acquirer Table 
	InitTable( &AQFMT, ( UBYTE * ) AQTAB, AQMAX * AQSZE, &AQLRC );

	// Descriptor Tables 
	InitTable( &DSFMT, ( UBYTE * ) DSTAB, DSMAX * DSSZE, &DSLRC );
	PurgeTable( ( UBYTE * ) DSTAB );	// Get rid of file for optional tables.

	// Idle Screen Button Table 
	if ( True == LOCTAB.IBTAB_INIT )  
	{
		// Idle Screen Button Table
		InitTable( &IBFMT, ( UBYTE * ) & IBTAB, IBSZE, &IBLRC );
		PurgeTable( ( UBYTE * ) & IBTAB );	// Get rid of file for optional tables.
	}

	// ICEPAC Configuration Table 
	InitTable( &IPFMT, ( UBYTE * ) & IPTAB, IPSZE, &IPLRC );
//###	PurgeTable( ( UBYTE * ) & IPTAB );	// Get rid of file for optional tables.

//	IcePak_RemoveAll( );	// clear IcePak images 

	// Downloaded reports 
//	DeleteReports(  );	// Delete downloaded reports 

	// Additional Prompts Tables 
	InitTable( &APFMT, ( UBYTE * ) APTAB, APMAXENT * APSZE, &APLRC );
//###	PurgeTable( ( UBYTE * ) APTAB );	// Get rid of file for optional tables.

	// Visa 2 Tables   
	InitTable( &V2FMT, ( UBYTE * ) V2TAB, V2MAX * V2SZE, &V2LRC );
	PurgeTable( ( UBYTE * ) V2TAB );	// Get rid of file for optional tables.

	// Performance Report Database
    InitRptTable( );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Converts binary word into 4 digits BCD value.
//!
//! \param
//!		nVal            value to be converted
//! \param
//!		pBcd            BCD buffer
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
static void BinToBcd( UWORD nVal,UBYTE *pBcd )
{
    register UBYTE      nBcd;

    nBcd= (nVal/1000) << 4;
    nVal%= 1000;
    *pBcd++ = nBcd | (nVal/100);
    nVal%= 100;
    nBcd= (nVal/10) << 4;
    nVal%= 10;
    *pBcd = nBcd | nVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     This routine writes each crc_table entry exactly once,
//!     with the ccorrect final value.  Thus, it is safe to call
//!     even on a table that someone else is using concurrently.
//!
//! \return
//!     
//! \note
//!		Copyright abandoned; this code is in the public domain.
//!     Provided to GNUnet by peter@horizon.com
//!
//-----------------------------------------------------------------------------
static void make_crc_table()
{
   unsigned int i, j;
   unsigned long h = 1;
   crc_table[0] = 0;
   for (i = 128; i; i >>= 1)
   {
     h = (h >> 1) ^ ((h & 1) ? POLYNOMIAL : 0);
     /* h is now crc_table[i] */
     for (j = 0; j < 256; j += 2*i)
       crc_table[i+j] = crc_table[j] ^ h;
   }
 }

