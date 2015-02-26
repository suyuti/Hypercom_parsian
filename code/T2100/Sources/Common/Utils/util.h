
#ifndef _UTIL_H_
#define _UTIL_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             util.h
//      Utility functions
//
//=============================================================================


//=============================================================================
// Public defines and typedefs
//=============================================================================
//  screen data structure 
//
//  If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//  pointed to by pTitle of ENTRY_STR is used.  
//
//  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then the text 
//  identified by the TitleMid identifier is used.  
//                  
//  The above applies also to Line1Mid and pLine1 and Line2Mid 
//  and pLine2
//

typedef struct
{
	enum msg_id TitleMid;
	enum msg_id Line1Mid;
	enum msg_id Line2Mid;
}
ENTRY_STR;



//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
extern const ENTRY_STR YNEntryTrk2;
extern UBYTE GetOneKey( UWORD map, OS_TIMES tmout );

extern UBYTE NumEntry( char DspMode, char Maxlen, char Minlen,
					   const ENTRY_STR * pEntry );
extern UBYTE AlphaNumEntry( char DspMode, char Maxlen, char Minlen,
							const ENTRY_STR * pEntry );
extern UBYTE YesNoEntry( const ENTRY_STR * pEntry );
extern UBYTE YesNextEntry( const ENTRY_STR * pEntry );

extern void ShowIssuer_Tran( void );
extern UBYTE GetPAN4( void );
extern Bool JournalFullTest( void );
extern UBYTE ContinueEntry( const ENTRY_STR * pEntry );
extern UBYTE ContinueEntryTimeout(const ENTRY_STR *pEntry);
extern void ClrCstMsgs( void );
extern Bool InputCashier( enum msg_id Mid, Bool AllowNull );
extern UBYTE read_io( UWORD map, OS_TIMES tmout, Bool *pbPinStatus, UWORD *pDevFlag );
extern UBYTE GetLastInputType( void );

#endif // _UTIL_H_
