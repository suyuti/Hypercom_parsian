
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             text.c
//      Multilingual Module
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkgui.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "utllib.h"

#include "struct.h"
#include "sdl.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"
#include "util.h"

// Application's custom font definition (for display)
#include "fnt_custom_de.h" 

#include "text.h"

//=============================================================================
// External variables / declarations
//=============================================================================

extern const UBYTE THIS_PAGE;


//=============================================================================
// Private defines and typedefs
//=============================================================================
/*!
	To enable ENGLISH support, define macro MAKE_ENGLISH.
	To enable GERMAN support, define macro MAKE_GERMAN.
	To enable SWEDISH support, define macro MAKE_SWEDISH.
	To enable SPANISH support, define macro MAKE_SPANISH.
	To enable PORTUGUESE support, define macro MAKE_PORTUGUESE.
	Check if there are not languages definined by compiler condition
*/
#if    !defined(MAKE_ENGLISH)	\
    && !defined(MAKE_GERMAN)	\
    && !defined(MAKE_SWEDISH)	\
    && !defined(MAKE_SPANISH)	\
    && !defined(MAKE_PORTUGUESE)
      #error "One MAKE_xxxxx Language must be defined!"
#endif



/*! The default language must be defined using the macro DEFAULT_LANGUAGE
	value 0 sets the default language to ENGLISH,
	value 1 sets the default language to GERMAN,
	value 2 sets the default language to SWEDISH,
	value 3 sets the default language to SPANISH,
	value 4 sets the default language to PORTUGUESE.
*/
// Define default language according to compiler condition
#if   ( DEFAULT_LANGUAGE == 0 ) && defined( MAKE_ENGLISH )
    #define LNG_DEFAULT LNG_EN
#elif ( DEFAULT_LANGUAGE == 1 ) && defined( MAKE_GERMAN )
    #define LNG_DEFAULT LNG_DE
#elif ( DEFAULT_LANGUAGE == 2 ) && defined( MAKE_SWEDISH )
    #define LNG_DEFAULT LNG_SV
#elif ( DEFAULT_LANGUAGE == 3 ) && defined( MAKE_SPANISH )
    #define LNG_DEFAULT LNG_ES
#elif ( DEFAULT_LANGUAGE == 4 ) && defined( MAKE_PORTUGUESE )
    #define LNG_DEFAULT LNG_PT
#else
      #error "DEFAULT_LANGUAGE must be defined!"
#endif


//! Structure to describe single Language
typedef struct
{
	//! Language ID (see text.h)
    enum Languages LngID;    
	//! Language Name (multilang. text)
    enum msg_id enName;		 
	//! Standard font ID (ID definded in HSDK)
    UBYTE Std_Font_ID;           
	//! Extended font ID (ID definded in HSDK)
    UBYTE Ext_Font_ID;           
	//! If not NULL -> load this ext font into HSDK
    const FONT_HEADER *Ext_Font_ptr;  
	//! If not NULL -> printer ext remap table pointer
    const UBYTE *Prt_Remap_Tbl;       
}
Language_rec;


//=============================================================================
// Private function declarations
//=============================================================================
static Bool TextSetCurrentLanguage( UBYTE lang );


//=============================================================================
// Public data definitions
//=============================================================================

#ifdef MAKE_SWEDISH
//-----------------------------------------------------------------------------
//! Swedish printer remap table
//-----------------------------------------------------------------------------
const UBYTE Swedish_Remap_Tbl[] = 
{
//!  Char code from              Char will be printed instead.
//!  text "pcl" file             See real printer character set
//!                              using Fun_82/Character Map
	0xC4,                         0x8E,  
	0xC5,                         0x8F,  
	0xD6,                         0x99,  
	0xE4,                         0x84,  
	0xE5,                         0x86,  
	0xF6,                         0x94,  
    0x00,                         0x00, // table terminator (mandatory)
};
#endif // MAKE_SWEDISH

#ifdef MAKE_GERMAN
//-----------------------------------------------------------------------------
//! German printer remap table
//-----------------------------------------------------------------------------
const UBYTE German_Remap_Tbl[] = 
{
//!  Char code from               Char will be printed instead.
//!  text "pcl" file              See real printer character set
//!                               using Fun_82/Character Map
	0xC4,                         0x8E,  
	0xC5,                         0x8F,  
	0xD6,                         0x99,  
	0xE4,                         0x84,  
	0xE5,                         0x86,  
	0xF6,                         0x94,  
    0x00,                         0x00, // table terminator (mandatory)
};
#endif // MAKE_GERMAN


//-----------------------------------------------------------------------------
//! Main table of language description.
//! Note, the order of language description must be the same as Languages 
//! enumeration in text.h file.
//-----------------------------------------------------------------------------
const Language_rec lngTab[] = 
{
//! Empty language - do not delete  !!!
	{
		//! Language ID
        LNG_NULL,      
		//! Language text name for display
        Lng_English,       
		//! Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_SFONT,            
		//! Extended font ID (chars 0x80...0xFF) for display
        DEFAULT_EFONT,             
		//! Extended custom font pointer for display
        NULL,              
		//! Printer remap character table pointer
        NULL,                       
    },

#ifdef MAKE_ENGLISH
//! English
	{
		//! Language ID
        LNG_EN,                
		//! Language text name for display
        Lng_English,         
		//! Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_SFONT,  
		//! Extended font ID (chars 0x80...0xFF) for display
        DEFAULT_EFONT,          
		//! Extended custom font pointer for display
        NULL,                  
		//! Printer remap character table pointer
        NULL,                      
    },
#endif // MAKE_ENGLISH


#ifdef MAKE_GERMAN
//! German
	{
		//! Language ID
        LNG_DE,             
		//! Language text name for display
        Lng_German,        
		//! Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_SFONT,    
		//! Extended font ID (chars 0x80...0xFF) for display
        FONT_EX05NN07,        
		//! Extended custom font pointer for display
        &FONT_EX_DE_05NN07_DEF,   
		//! Printer remap character table pointer
        German_Remap_Tbl,           
    },
#endif // MAKE_GERMAN


#ifdef MAKE_SWEDISH
//! Swedish
	{
		//! Language ID
        LNG_SV,          
		//! Language text name for display
        Lng_Swedish,   
		//! Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_SFONT,       
		//! Extended font ID (chars 0x80...0xFF) for display
        FONT_EX05NN07,     
		//! Extended custom font pointer for display
        &FONT_EX_DE_05NN07_DEF,  
		//! Printer remap character table pointer
        Swedish_Remap_Tbl,          
    },
#endif // MAKE_SWEDISH


#ifdef MAKE_SPANISH
//! Spanish
	{
		//! Language ID
        LNG_ES,       
		//! Language text name for display
        Lng_Spanish,   
		//! Standard font ID (chars 0x00...0x7F) for display
        DEFAULT_SFONT,     
		//! Extended font ID (chars 0x80...0xFF) for display
        DEFAULT_EFONT,     
		//! Extended custom font pointer for display
        NULL,     
		//! Printer remap character table pointer
        NULL,                       
    },
#endif // MAKE_SPANISH


#ifdef MAKE_PORTUGUESE
//! Portuguese
	{
		//! Language ID
        LNG_PT,      
		//! Language text name for display
        Lng_Portugues,       
		//! Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_SFONT,     
		//! Extended font ID (chars 0x80...0xFF) for display
        DEFAULT_EFONT,     
		//! Extended custom font pointer for display
        NULL,    
		//! Printer remap character table pointer
        NULL,                       
    },
#endif // MAKE_PORTUGUESE

};




//=============================================================================
// Private data definitions
//=============================================================================
//! This index corresponds to language positions in database ptrs,
//! and contains the language ID
short language_index[MAXLANG];

static OS_RETURN_VALS isitok;

static short ret_tblnum;

static struct dbtable_ptr DbTable[MAXTABLE * MAXLANG];


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Set Current Language Font
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
extern void SetCurrentLanguageFont( void )
{
	// Check if need to load custom extended font
	if ( NULL != lngTab[language_index[LOCTAB.LANGID-1]].Ext_Font_ptr )
	{
       SDK_GuiLoadExtCustomFont( lngTab[language_index[LOCTAB.LANGID-1]].Ext_Font_ID, THIS_PAGE,
                                 lngTab[language_index[LOCTAB.LANGID-1]].Ext_Font_ptr, 0);
	}

	// Set current font (standard and extended)
	SDK_GuiSetFont( lngTab[language_index[LOCTAB.LANGID-1]].Std_Font_ID, 
                    lngTab[language_index[LOCTAB.LANGID-1]].Ext_Font_ID );
}

//-----------------------------------------------------------------------------
//!
//!      Select language menu processing
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
extern void TextLanguageSelectMenu( void )
{
	UBYTE Status;

    // Draw Language selection menu
    DrawMenu( LANGUAGE_MENU );

    // Wait entry or timeout
    Status = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

    // Route
    switch ( Status )
    {
		case B_PREVIOUS_NEW_MENU:	// Go back one menu level 
            return;

		case B_PREVIOUS:	// decrement issuer index 
			DrawMenu( MENU_PREVIOUS );
			break;

		case B_NEXT:
			DrawMenu( MENU_NEXT );
			break;

		case 0:		// Cancel or time-out 
		case CANCEL_KY:
		case CLEAR_KY:
			return;

        default:
            // New languange!
			if ( !TextSetCurrentLanguage( Status ) )	
            {
                // Unable to set selected language
                SDK_Beeper( TENMS * 40 );
    		}
            break;
    }
}

//-----------------------------------------------------------------------------
//!
//!      Get the pointer of printer remap table according current language
//!
//! \param
//!		
//! \return
//!     UBYTE		Selected language ID
//!
//! \note
//!
const UBYTE* GetRemapTblPtr ( void )
{
    return lngTab[language_index[LOCTAB.LANGID-1]].Prt_Remap_Tbl;
}

//-----------------------------------------------------------------------------
//!
//!      Creates a new text index for the database and returns its 
//!      identifier. This index corresponds to language positions
//!      in database ptrs, and contains the language code.
//!
//! \param
//!		
//! \return
//!     short		0 - Invalid, other value, index assigned.
//!
//! \note
//!		Value returned in TBLDID
//!
extern short TextIndexNew( void )
{
	if ( ret_tblnum >= MAXTABLE )
	{
		return 0;
	}

	ret_tblnum += 1;

	return ret_tblnum;
}

//-----------------------------------------------------------------------------
//!
//!      Clear database pointers and pages
//!
//! \param
//!		
//! \return
//!
//! \note
//!		This database will be used to store the pointer
//!		and pages of the messages tables used for Dynamic
//!		multilingual support.  
//!
extern void TextDatabaseInit( void )
{
	int i;

	for ( i = 0; ( MAXTABLE - 1 ) * MAXLANG >= i; i++ )
	{
		DbTable[i].page = 0;
		DbTable[i].pTable = 0;
	}

	for ( i = 0; MAXLANG > i; i++ )
	{
		language_index[i] = 0;
	}

	ret_tblnum = 0;
}

//-----------------------------------------------------------------------------
//!
//!      Add new message table
//!
//! \param
//!		dbtbl			DB Table to add the new message table
//!	\param
//!		dblang			Message table language ID
//!	\param
//!		texttab			Message table to be added
//!	\param
//!		pgnum			Page where resides texttab
//!		
//! \return
//!		Bool		True - Succesful, False -  Error.
//!
//! \note
//!
extern Bool TextIndexAddTable( short dbtbl, UBYTE dblang,
							   const char *const *texttab, short pgnum )
{
    int i;

	// Language ID over maximum number
	if ( LNG_LAST <= LOCTAB.LANGID )
	{
		return False;
	}

	// Get index to language if already used or get index to first empty array element
	for ( i = 0; 0 != language_index[i] && language_index[i] != dblang; i++ )
	{
		// array has no space for another language, size set by MAXLANG
		if ( MAXLANG - 1 == i )
		{
			return False;
		}
	}

	// Put language id into empty array element 
	if ( 0 == language_index[i] )
		language_index[i] = dblang;

	// Put the pointer into the table. 
	DbTable[( ( dbtbl - 1 ) * MAXLANG ) + i].pTable = texttab;

	// Put page number into table
	DbTable[( ( dbtbl - 1 ) * MAXLANG ) + i].page = pgnum;

	return True;
}

//-----------------------------------------------------------------------------
//!
//!      Get pointer and page of requested table
//!
//! \param
//!		MLGetMessage			Message from Multilingual Database
//!		
//! \return
//!		Bool		True - Succesful, False -  Error.
//!
//! \note
//!		Returns message from Multilingual Database. String returned in MLBuffer.
//!
extern Bool TEXTGETSTRING( 	struct MLGetMsg_str *MLGetMessage )
{
	const char *const *pTable;
	PAGE_NUMBERS pgnum;
	UBYTE dblang_index;

	// Table id is not in database
	if ( MLGetMessage->tblid > ret_tblnum )
	{
		return False;
	}

	// set language index into array
	dblang_index = LOCTAB.LANGID - 1;

	// Get table pointer

	pTable =
		DbTable[( ( MLGetMessage->tblid - 1 ) * MAXLANG ) + LOCTAB.LANGID -
				1].pTable;

	// If table pointer is not in database
	if ( !pTable )
	{
		return False;
	}

	// point to requested message in table
	pTable += MLGetMessage->msgid;

	// Get the page number
	pgnum =
		DbTable[( ( MLGetMessage->tblid - 1 ) * MAXLANG ) + LOCTAB.LANGID -
				1].page;

	// Get the table pointer in another page
	isitok =
		SDK_ReadPage( ( UBYTE * ) & pTable, sizeof( char * ), pgnum,
					  ( UBYTE * ) pTable );

	// Get the message from the table and put it in Dspbuf
	isitok =
		SDK_ReadPage( ( UBYTE * ) MLBuffer, sizeof( MLBuffer ), pgnum,
					  ( UBYTE * ) pTable );

	return True;
}

//-----------------------------------------------------------------------------
//!
//!      Set default language ID in LOCTAB
//!
//! \param
//!
//!	\Global Inputs
//!		MLGetMessage
//!
//! \return
//!
//! \note
//!		String returned in MLBuffer.
//!
void SetLanguageDefault ( void )
{
    TextSetCurrentLanguage( LNG_DEFAULT );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Set Current Language
//!
//! \param
//!		lang		  Language ID that current language is to be set to  
//!
//!	\Global Inputs
//!
//! \return
//!		Bool		True - Succesful, False -  Error.
//!
//! \note
//!
Bool TextSetCurrentLanguage( UBYTE lang )
{
    if ( (lang > LNG_NULL) && ( lang < LNG_LAST) )
    {
        LOCTAB.LANGID = lang;
        SetLOCTabLRC(  );

        FontSize = DEFAULT_FONT_SIZE;
        SetCurrentLanguageFont();

        return True;
    }
    else
    {
        return False;
    }
}

