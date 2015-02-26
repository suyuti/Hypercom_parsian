
#ifndef _JOURNAL_H_
#define _JOURNAL_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             journal.h
//      Journal defintions, data, and functions
//
//=============================================================================

#include "basictyp.h"
#include "jrndata.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================
extern struct trandata_rec JRREC;	//journal record
extern struct journal_rec JRBUF;	// For Header Record Building
extern struct jrnfind_rec JSREC;	// Journal select record



// Combination of linked list, fixed header and variable journal data
//********************************************************************** 
//* THE DEFINITION OF LL_DEF MUST NOT BE CHANGED.  THE ASSEMBLY
//* PROGRAMS DEPEND ON LL-DEF NOT CHANGING.  JrnHeadRec AND var_entries
//* CAN BE CHANGED BUT NOT THEIR ORDER IN STRUCTURE journal_rec.  
//***********************************************************************

struct journal_rec
{
	struct ll_def LLBUF;
	struct trandata_rec TRDATA;
} __attribute__((packed));


// Size used to allocate buffer THIS, work area for the journal code.
// THIS must be enough to hold the link list information (struct ll_def), 
// plus the uncompressed area of the journal. 
#define S_THIS	offsetof( struct journal_rec, TRDATA.TRFIXEDEND )


//=============================================================================
// Public function declarations
//=============================================================================
// Global management
extern void JournalPrepare( void );
extern Bool JournalValidate( void );
extern void JournalFormat( void );

// Semaphore processing
extern void JournalRelease( void );
extern void JournalLock( void );

// Add, read, update, delete journal records
extern UWORD JournalNew( struct trandata_rec * pTranBuf );
extern UBYTE JournalRead( UWORD Ident, struct trandata_rec * pTranBuf );
extern UWORD JournalUpdate( struct trandata_rec * pTranBuf, UWORD JrnIdent );
extern UBYTE JournalFree( UWORD JrnIdent );

// Selections
extern void SetDefaultSelection( void );
extern UWORD JournalSelFirst( void );
extern UWORD JournalSelNext( UWORD PrevIdent );
extern UWORD JournalSelPrevious( UWORD CurrIdent );

// Group operations
extern void JournalSetSettle( void );
extern void TrainJournalClear( void );
extern void JournalClearBatch( void );
extern int JournalRoom( void );

#endif // _JOURNAL_H_
