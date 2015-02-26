
#ifndef _TEXT_H_
#define _TEXT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002, 2005
//=============================================================================
//
// Module overview:             text.h
//      Header file for text file database
//
//=============================================================================


//=============================================================================
// Public defines and typedefs
//=============================================================================
#define MAXLANG 10
#define MAXTABLE 10


//! Do use ISO639 for language naming
enum Languages
{
	//! Empty language
    LNG_NULL = 0,

#ifdef MAKE_ENGLISH
	//! English
	LNG_EN,
#endif
    
#ifdef MAKE_GERMAN
	//! German
    LNG_DE,						
#endif
    
#ifdef MAKE_SWEDISH
	//! Swedish
    LNG_SV,						
#endif
    
#ifdef MAKE_SPANISH    
	//! Spanish
    LNG_ES,						
#endif

#ifdef MAKE_PORTUGUESE   
	//! Portuguese
    LNG_PT,						
#endif

#ifdef MAKE_FARSI   
	//! Portuguese
    LNG_FA,						
#endif
	//! Last language (empty)
    LNG_LAST                    
};

//! Number of languages available
#define NumberOfLanguages ( LNG_LAST - LNG_NULL - 1 )


//! pointer to database
struct dbtable_ptr
{
	short page;
	const char *const *pTable;
};


//=============================================================================
// Public data declarations
//=============================================================================
//! Structure to get a message across page
struct MLGetMsg_str				
{
	//! Table ID of the message
	short tblid;   
	//! Message ID in the table
	enum msg_id msgid;			
};

//=============================================================================
// Public function declarations
//=============================================================================
//! Select language menu processing
extern void TextLanguageSelectMenu( void );
//! Set Current Language Font
extern void SetCurrentLanguageFont( void );
//! Set default language ID in LOCTAB
extern void SetLanguageDefault ( void );
//! Get the pointer of printer remap table according current language
extern const UBYTE* GetRemapTblPtr ( void );
//! Clear database pointers and pages
extern void TextDatabaseInit( void );
//! Get pointer and page of requested table
extern Bool TEXTGETSTRING( struct MLGetMsg_str * );


#endif // _TEXT_H_
