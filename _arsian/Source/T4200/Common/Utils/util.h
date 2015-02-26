
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
//! Get One Key from NUMERIC Layout or AlphaNumeric layout
extern UBYTE GetOneKey( UWORD map, OS_TIMES tmout );
//! Accept numeric input from user & save in Dspbuf.
extern UBYTE NumEntry( char DspMode, char Maxlen, char Minlen,
					   const ENTRY_STR * pEntry );
//! Accept numeric input from user to virtual Pin Pad 
extern void VPP_PinEntry( char DspMode, char Maxlen, char Minlen, const ENTRY_STR * pEntry );
//! Accept numeric input from user to virtual Pin Pad 
extern UBYTE VPP_PinEntry_( char DspMode, char Maxlen, char Minlen,
							const ENTRY_STR * pEntry, UBYTE* dest );
//! Accept alpha and numeric input from user & save in Dspbuf.
extern UBYTE AlphaNumEntry( char DspMode, char Maxlen, char Minlen,
							const ENTRY_STR * pEntry );
//! Accept YES or NO from user.
extern UBYTE YesNoEntry( const ENTRY_STR * pEntry );
//! Accept YES or Next from user.
extern UBYTE YesNextEntry( const ENTRY_STR * pEntry );
//! Display Issuer name and transaction type on top line
extern void ShowIssuer_Tran( void );
//! Get last 4 digits of PAN and memcmpe with that from card swipe.
extern UBYTE GetPAN4( void );
//! Test for journal full or nearly full.  Also, if Signature
//! Capture test for file system full or nearly full.
extern Bool JournalFullTest( void );
//! ContinueEntry   Accept Continue or Cancel from user.
extern UBYTE ContinueEntry( const ENTRY_STR * pEntry );
//! Accept Continue or Cancel from user
extern UBYTE ContinueEntryTimeout(const ENTRY_STR *pEntry);
//! Clears the Custom messages, CSTMSG and CSTMSG2
extern void ClrCstMsgs( void );
extern Bool InputCashier( enum msg_id Mid, Bool AllowNull );
//! Performs input from various devices
extern UBYTE read_io( UWORD map, OS_TIMES tmout, Bool *pbPinStatus, UWORD *pDevFlag );
//! Returns last read device.
extern UBYTE GetLastInputType( void );

#endif // _UTIL_H_
