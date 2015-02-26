//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2006
//=============================================================================
//
// persistence.c
//
// This file contains functions for storing and restoring application data
// that requires persistence 
//
//=============================================================================
// Make sure file system calls ARE NOT replaced for HFS file calls.
// We need to access the initialization files received along with the app.
//#define _DO_NOT_USE_HFS_

#ifdef _DO_NOT_USE_HFS_
	#define USE_FLASH_QUEUE 1
#endif

//=============================================================================
//=============================================================================
#include "basictyp.h"
#include "struct.h"
#include "rs232tab.h"
#include "comdata.h"
#include "sdkos.h"
#include <string.h>
#include "appdata.h"
#include "utllib.h"
#include "assert.h"
#include "FlashQueueIO.h"
#include "hfs.h"
#include "fixdata.h"
#ifdef MAKE_EMV
#include "emv_l2.h"
#include "emv2_pk.h"
#include "emvtabs2.h"
#endif // MAKE_EMV

#include "DbgTrace.h"

extern void RestoreJournalData( void );


//=============================================================================
// Private defines and typedefs
//=============================================================================
typedef struct
{
	char *filename;   // filename to store table
	UBYTE * pFrmFlag; // format byte  
	UBYTE * pTab;     // pointer to table or first table
	UWORD len;        // length of table
	UWORD count;      // number of tables	
	UBYTE * pLRC;     // LRC byte of table
} TABLE_DEF;


//=============================================================================
// Private function declarations
//=============================================================================
static int RestoreTable( char *filename, UBYTE * pFrmFlag, UBYTE * pTab,
                         UWORD count, UWORD len, UBYTE * pLRC );
static FILE * OpenFileForRead( const char *filename );
static void CloseFile( FILE *hfile );
static int FindTableIndex( UBYTE * pTab );

#if !defined( USE_FLASH_QUEUE )
static FILE * OpenFileForWrite( const char *filename );
#endif

//=============================================================================
// Public data definitions
//=============================================================================

//=============================================================================
// Private data definitions
//=============================================================================
static TABLE_DEF tables[] = 
{
	{
		"d:\\termtab.cfg",
		( UBYTE * )&TERMFMT,
		( UBYTE * )&TERM,
		sizeof(TERM),
		1,
		( UBYTE * )&TERMLRC
	},

	{
		"d:\\tconf.cfg",
		&TCFMT,
		( UBYTE * )&TCONF,
		sizeof(struct termconf_rec),
		1,
		&TCLRC
	},

	{
		"d:\\aqtab.cfg",
		&AQFMT,
		( UBYTE * )&AQTAB,
		sizeof(struct acquirer_rec),
		AQMAX,
		&AQLRC
	},

    {
        "d:\\istab.cfg",
        &ISFMT,
        ( UBYTE * )&ISTAB,
        sizeof(struct issuer_rec),
        ISMAX,
        &ISLRC
    },

	{
		"d:\\crtab.cfg",
		&CRFMT,
		( UBYTE * )&CRTAB,
		sizeof(struct card_rec),
		CRMAX,
		&CRLRC
	},

	{
		"d:\\dstab.cfg",
		&DSFMT,
		( UBYTE * )&DSTAB,  sizeof(struct desc_rec),
		DSMAX,
		&DSLRC
	},

	{
		"d:\\ibtab.cfg",
		&IBFMT,
		( UBYTE * )&IBTAB,
		sizeof(struct idlescr_rec),
		1,
		&IBLRC
	},

	{
		"d:\\cashtab.cfg",
		&CASHFMT,
		( UBYTE * )&CASHTAB,
		sizeof(struct cashier_rec),
		CASHIER_MAX,
		&CASHLRC
	},

	{
		"d:\\aptab.cfg",
		&APFMT,
		( UBYTE * )&APTAB,
		sizeof(struct addprompt_rec),
		APMAXENT,
		&APLRC
	},

	{
		"d:\\battab.cfg",
		&BATFMT,
		( UBYTE * )&BATTAB,
		sizeof(struct batch_rec),
		AQMAX,
		&BATLRC
	},

	{
		"d:\\tftab.cfg",
		&TFFMT,
		( UBYTE * )&TFTAB,
		sizeof(struct termflg_rec),
		1,
		&TFLRC
	},

	{
		"d:\\usrdtab.cfg",
		&USRDFMT, 
		( UBYTE * )&USRDTAB,
		sizeof(USRDTAB),
		1,
		&USRDLRC
	},

	{
		"d:\\v2tab.cfg",
		&V2FMT,
		( UBYTE * )&V2TAB,
		sizeof(struct v2_rec),
		V2MAX,
		&V2LRC
	},

	{
		"d:\\iptab.cfg",
		&IPFMT,
		( UBYTE * )&IPTAB,
		sizeof(struct stipac),
		1,
		&IPLRC
	},

	{
		"d:\\fttab.cfg",
		&FTFMT,
		( UBYTE * )&FTTAB,
		sizeof(struct ft_table),
		1,
		&FTLRC
	},

    {
        "d:\\schtab.cfg",
        &SCHFMT,
        ( UBYTE * )&SCHTAB,
        sizeof(struct schedule_rec),
        SCHMAX,
        &SCHLRC
    },

	{
		"d:\\loctab.cfg",
		&LOCFMT,
		( UBYTE * )&LOCTAB,
		sizeof(struct localvalues_rec),
		1,
		&LOCLRC
	},

	{
		"d:\\pftab.cfg",
		&PFFMT,
		( UBYTE * )&PFTAB,
		sizeof(struct pf_table),
		1,
		&PFLRC
	},

    {
        "d:\\ipctab.cfg",
        &IPCFMT,
        ( UBYTE * )&IPCTAB,
        sizeof(struct ipcontab),
        IPCMAX,
        &IPCLRC
    },

    {
        "d:\\rstab.cfg",
        &RSFMT,
        ( UBYTE * )&RSTAB,
        sizeof(struct rs232_rec),
        1,
        &RSLRC
    },

#ifdef MAKE_EXTRA_TABLES
    {
        "d:\\pdntab.cfg",
        &PDNFMT,
        ( UBYTE * )&PDNTAB,
        sizeof(PDNSTRUCT),
        PDNMAX,
        &PDNLRC
    },
#endif // MAKE_EXTRA_TABLES

#ifdef MAKE_EMV					
    {
        "d:\\icctab.cfg",
        &ICCFMT,
        ( UBYTE * )&ICCTAB,
        ICCSZE,
        ICCMAX,
        &ICCLRC
    },
    {
        "d:\\taidtab.cfg",
        &TAIDFMT,
        ( UBYTE * )&TAIDTAB,
        TAIDSZE,
        TAIDMAX,
        &TAIDLRC
    },
    {
        "d:\\adttab.cfg",
        &ADTFMT,
        ( UBYTE * )&ADTData,
        sizeof(ADTData[0]),
        EMVL2_ADT_MAX,
        &ADTLRC
    },

    {
        "d:\\pkstab.cfg",
        &PKFMT,
        ( UBYTE * )&PublicKeyStore,
        sizeof(PublicKeyStore[0]),
        KEYMAX,
        &PKLRC
    },
    {
        "d:\\ketab.cfg",
        &KEFMT,
        ( UBYTE * )&KeyExpiryTable,
        sizeof(KeyExpiryTable[0]),
        KEYMAX,
        &KELRC
    },
    {
        "d:\\rktab.cfg",
        &RKFMT,
        ( UBYTE * )&RevocationTable,
        sizeof(RevocationTable),
        1,
        &RKLRC
    },
#endif // MAKE_EMV
};



//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//! \brief
//!     Restores persistent data areas for application.
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
void InitPersistentData( void )
{
	int i;
	TABLE_DEF *pTable;

	// restore the tables
	for( i=0;i<(int)(sizeof( tables )/sizeof( TABLE_DEF )); i++ )
	{
		pTable = &tables[i];
		
		if( !RestoreTable( pTable->filename,
		                   pTable->pFrmFlag, pTable->pTab, pTable->len,
		                   pTable->count, pTable->pLRC ) )
		{
			// restore has failed , clear format byte
			*pTable->pFrmFlag = 0;
			DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "Persistence: Could not restore %s.\n", pTable->filename );
		}
		else
		{
			DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "Persistence: Restored %s, size %d, count %d.\n", pTable->filename, pTable->len, pTable->count );
		}
	}
	// restore journal configuration data
	RestoreJournalData();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Attempts to store table to file system. Failure is ignored.
//!
//!	\param
//!		pTab            pointer to table
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
void StoreTable( UBYTE * pTab )
{
#if !defined( USE_FLASH_QUEUE )
	FILE *hfile;
	size_t numbytes;
	UBYTE temp[2];
#endif
	TABLE_DEF *pTableDef;
	int i;

	i = FindTableIndex( pTab );
	// table not found
	if( i < 0 )
	{
        DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "Persistence: Unknown table! Break table storing\n" );
        assert( 0 );
	}
	
	pTableDef = &tables[i];

#if !defined( USE_FLASH_QUEUE )
	// Attempt to open file for write
	hfile = OpenFileForWrite( pTableDef->filename );
	// Make sure the file opened ok.
	if ( !hfile )
	{
		// need an exception here ?
		return;
	}

	// Write the data to the file
	numbytes = fwrite( pTab, pTableDef->len, pTableDef->count, hfile );
    if( numbytes != (size_t) pTableDef->count )
	{
		// need an exception here ?
		CloseFile( hfile );
		return;
	}

	temp[0] = *pTableDef->pFrmFlag;
	temp[1] = *pTableDef->pLRC;

	// put format flag and lrc at end of data
	numbytes = fwrite( temp, 2, 1, hfile );
	if( numbytes != 1 )
	{
		// need an exception here ?
		CloseFile( hfile );
		return;
	}

	CloseFile( hfile );

#else

	// Writes a complete data table to single file in flash, any 
	// existing file destroyed 
	WriteTableToFlash( pTableDef->filename, pTab,
		               pTableDef->len * pTableDef->count, 
		               *pTableDef->pFrmFlag, *pTableDef->pLRC );
#endif
    DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "Persistence: Storing table %s, size %d, count %d.\n",
              pTableDef->filename, pTableDef->len, pTableDef->count );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Attempts to delete file "backing up" table. Failure is ignored.
//!
//!	\param
//!		pTab            pointer to table
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
void PurgeTable( UBYTE * pTab )
{
	TABLE_DEF *pTableDef;
	int i;

	i = FindTableIndex( pTab );
	// table not found
	if( i < 0 )
	{
		assert( 0 );
	}
	
	pTableDef = &tables[i];
	DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "Persistence: Purging file %s.\n",
              pTableDef->filename );

	// Make sure we will not interfer with an update.
	WaitForFileNotInQueue( pTableDef->filename );

	// delete the file for the table.
    remove( pTableDef->filename );
	//*pTableDef->pFrmFlag = 0;
	//*pTableDef->pLRC = 0;
}




//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Restores table from file system.
//!
//!	\param
//!		filename        name of file
//!	\param
//!		pFrmFlag        format flag of table
//!	\param
//!		pTab            pointer to table
//!	\param
//!		len             lenght of table
//!	\param
//!		pLRC            pointer to LRC
//!
//! \return
//!     Zero on error else non-zero
//-----------------------------------------------------------------------------
static int RestoreTable( char *filename, UBYTE * pFrmFlag, UBYTE * pTab, UWORD len,
                  UWORD count, UBYTE * pLRC )
{
	FILE *hfile;
	size_t numbytes;
	UBYTE temp[2];

	// Attempt to open file for write
	hfile = OpenFileForRead( filename );
	// Make sure the file opened ok.
	if ( !hfile )
	{
		return 0;
	}

	// read the table data from the file
	numbytes = fread( pTab, len, count, hfile );
	if( numbytes != count )
	{
		CloseFile( hfile );
		return 0;
	}

	// get format flag and lrc from end of table data
	numbytes = fread( temp, 2, 1, hfile );
	if( numbytes != 1 )
	{
		CloseFile( hfile );
		return 0;
	}

	*pFrmFlag = temp[0];
	*pLRC = temp[1];

	CloseFile( hfile );
	return 1;
}

#if !defined( USE_FLASH_QUEUE )
//-----------------------------------------------------------------------------
//! \brief
//!     Open a file for writing. Creates new file if required, else
//!     destroys contents or existing file.
//!
//!	\param
//!		filename        name of file
//!
//! \return
//!     File handle
//-----------------------------------------------------------------------------
static FILE * OpenFileForWrite( const char *filename )
{
	FILE *hfile;

	// Attempt to open file	 
	hfile = fopen( filename,"w");

	return hfile;
}
#endif


//-----------------------------------------------------------------------------
//! \brief
//!     Open a file for reading. Error occurs if ifle does not exist.
//!
//!	\param
//!		filename        name of file
//!
//! \return
//!     File handle
//-----------------------------------------------------------------------------
static FILE * OpenFileForRead( const char *filename )
{
	FILE * hfile;

	// Attempt to open file	 
	hfile = fopen( filename,"r");

	return hfile;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Closes a file.
//!
//!	\param
//!		hfile           file handle ( assumed to be valid )
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
static void CloseFile( FILE *hfile )
{
	fclose( hfile );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Finds the index of the table definition structure.
//!
//!	\param
//!		pTab            pointer to table
//!
//! \return
//!     -1 on error else zero based table index
//-----------------------------------------------------------------------------
static int FindTableIndex( UBYTE * pTab )
{
	int i;

	// restore the tables
	for( i=0;i<(int)(sizeof( tables )/sizeof( TABLE_DEF )); i++ )
	{
		if( pTab == tables[i].pTab )
		{
			return i;
		}
	}
	return -1;
}

