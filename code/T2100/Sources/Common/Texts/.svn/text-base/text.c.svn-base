
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

// To enable ENGLISH support, define macro MAKE_ENGLISH.
// To enable GERMAN support, define macro MAKE_GERMAN.
// To enable SWEDISH support, define macro MAKE_SWEDISH.
// To enable SPANISH support, define macro MAKE_SPANISH.
// To enable PORTUGUESE support, define macro MAKE_PORTUGUESE.
//
// Check if there are not languages definined by compiler condition
#if    !defined(MAKE_ENGLISH)
    && !defined(MAKE_TURKISH)
      #error "One MAKE_xxxxx Language must be defined!"
#endif



// The default language must be defined using the macro DEFAULT_LANGUAGE
//		value 0 sets the default language to ENGLISH,
//		value 1 sets the default language to GERMAN,
//		value 2 sets the default language to SWEDISH,
//		value 3 sets the default language to SPANISH,
//		value 4 sets the default language to PORTUGUESE.

// Define default language according to compiler condition
#if   ( DEFAULT_LANGUAGE == 0 ) && defined( MAKE_ENGLISH )
    #define LNG_DEFAULT LNG_EN
#elif ( DEFAULT_LANGUAGE == 1 ) && defined( MAKE_TURKISH )
    #define LNG_DEFAULT LNG_TR
#else
      #error "DEFAULT_LANGUAGE must be defined!"
#endif


// Structure to describe single Language
typedef struct
{
    enum Languages LngID;             // Language ID (see text.h)
    enum msg_id enName;		          // Language Name (multilang. text)
    UBYTE Std_Font_ID;                // Standard font ID (ID definded in HSDK)
    UBYTE Ext_Font_ID;                // Extended font ID (ID definded in HSDK)
    const FONT_HEADER *Ext_Font_ptr;  // If not NULL -> load this ext font into HSDK
    const UBYTE *Prt_Remap_Tbl;       // If not NULL -> printer ext remap table pointer
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
// Swedish printer remap table
//-----------------------------------------------------------------------------
const UBYTE Swedish_Remap_Tbl[] = 
{
//  Char code from                Char will be printed instead.
//  text "pcl" file               See real printer character set
//                                using Fun_82/Character Map
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
// German printer remap table
//-----------------------------------------------------------------------------
const UBYTE German_Remap_Tbl[] = 
{
//  Char code from                Char will be printed instead.
//  text "pcl" file               See real printer character set
//                                using Fun_82/Character Map
	0xC4,                         0x8E,  
	0xC5,                         0x8F,  
	0xD6,                         0x99,  
	0xE4,                         0x84,  
	0xE5,                         0x86,  
	0xF6,                         0x94,  
    0x00,                         0x00, // table terminator (mandatory)
};
#endif // MAKE_GERMAN

#ifdef MAKE_TURKISH
//-----------------------------------------------------------------------------
// Turkish printer remap table
//-----------------------------------------------------------------------------
const UBYTE Turkish_Remap_Tbl[] = 
{
//  Char code from                Char will be printed instead.
//  text "pcl" file               See real printer character set
//                                using Fun_82/Character Map
	0xC7,                         0x80, // Ç 
	0xD0,                         0xC9, // Ð
	0xD6,                         0x99, // Ö 
	0xDC,                         0x9A, // Ü 
	0xDD,                         0xC2, // Ý
	0xDE,                         0xCC, // Þ
	0xE7,                         0x87, // ç   
	0xF0,                         0xCA, // ð
	0xF6,                         0x94, // ö       
	0xFC,                         0x81, // ü         
	0xFD,                         0xC1, // ý
	0xFE,                         0xCD, // þ
	0x00,                         0x00, // table terminator (mandatory)
};
#endif



//-----------------------------------------------------------------------------
// Main table of language description.
// Note, the order of language description must be the same as Languages 
// enumeration in text.h file.
//-----------------------------------------------------------------------------
const Language_rec lngTab[] = 
{
// Empty language - do not delete  !!!
	{
        LNG_NULL,                   // Language ID
        Lng_English,                // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_EFONT,              // Extended font ID (chars 0x80...0xFF) for display
        NULL,                       // Extended custom font pointer for display
        NULL,                       // Printer remap character table pointer
    },

#ifdef MAKE_ENGLISH
// English
	{
        LNG_EN,                     // Language ID
        Lng_English,                // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_EFONT,              // Extended font ID (chars 0x80...0xFF) for display
        NULL,                       // Extended custom font pointer for display
        NULL,                       // Printer remap character table pointer
    },
#endif // MAKE_ENGLISH

#ifdef MAKE_TURKISH
	// Türkçe
	{
        LNG_TR,                     // Language ID
        Lng_Turkish,                // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        FONT_EX05NN07,              // Extended font ID (chars 0x80...0xFF) for display
        &FONT_ET05NN07_DEF,         // Extended custom font pointer for display
        Turkish_Remap_Tbl,          // Printer remap character table pointer		
    },
#endif


#ifdef MAKE_GERMAN
// German
	{
        LNG_DE,                     // Language ID
        Lng_German,                 // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        FONT_EX05NN07,              // Extended font ID (chars 0x80...0xFF) for display
        &FONT_EX_DE_05NN07_DEF,     // Extended custom font pointer for display
        German_Remap_Tbl,           // Printer remap character table pointer
    },
#endif // MAKE_GERMAN


#ifdef MAKE_SWEDISH
// Swedish
	{
        LNG_SV,                     // Language ID
        Lng_Swedish,                // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        FONT_EX05NN07,              // Extended font ID (chars 0x80...0xFF) for display
        &FONT_EX_DE_05NN07_DEF,     // Extended custom font pointer for display
        Swedish_Remap_Tbl,          // Printer remap character table pointer
    },
#endif // MAKE_SWEDISH


#ifdef MAKE_SPANISH
// Spanish
	{
        LNG_ES,                     // Language ID
        Lng_Spanish,                // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_EFONT,              // Extended font ID (chars 0x80...0xFF) for display
        NULL,                       // Extended custom font pointer for display
        NULL,                       // Printer remap character table pointer
    },
#endif // MAKE_SPANISH


#ifdef MAKE_PORTUGUESE
// Portuguese
	{
        LNG_PT,                     // Language ID
        Lng_Portugues,              // Language text name for display
        DEFAULT_SFONT,              // Standard font ID (chars 0x00...0x7F) for display 
        DEFAULT_EFONT,              // Extended font ID (chars 0x80...0xFF) for display
        NULL,                       // Extended custom font pointer for display
        NULL,                       // Printer remap character table pointer
    },
#endif // MAKE_PORTUGUESE

};




//=============================================================================
// Private data definitions
//=============================================================================
// This index corresponds to language positions in database ptrs,
// and contains the language ID
short language_index[MAXLANG];

static OS_RETURN_VALS isitok;

static short ret_tblnum;

static struct dbtable_ptr DbTable[MAXTABLE * MAXLANG];


//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//  SetCurrentLanguageFont  Set Current Language Font
//
//  Parameters:    None
//
//  Global Inputs:
//
//  Returns:       None
//
//  Notes:
//
//-----------------------------------------------------------------------------

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
//  TextLanguageSelectMenu   Select language menu processing
//                  
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            
//
//  Notes:
//
//-----------------------------------------------------------------------------

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
//  GetRemapTblPtr      Get the pointer of printer remap table
//                      according current language
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            UBYTE  Selected language ID
//
//  Notes:
//
//-----------------------------------------------------------------------------

const UBYTE* GetRemapTblPtr ( void )
{
    return lngTab[language_index[LOCTAB.LANGID-1]].Prt_Remap_Tbl;
}

//-----------------------------------------------------------------------------
//  TEXTINDEXNEW    Creates a new text index for the database and returns its 
//                  identifier. This index corresponds to language positions
//                  in database ptrs, and contains the language code.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        short   0 - Invalid,
//                          other value, index assigned.
//
//  Notes:          Value returned in TBLDID.
//
//-----------------------------------------------------------------------------

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
//  TextDatabaseInit    Clear database pointers and pages.
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:              This database will be used to store the pointer
//                      and pages of the messages tables used for Dynamic
//                      multilingual support.  
//
//-----------------------------------------------------------------------------

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
//  TextIndexAddTableFuncName
//  
//
//  Parameters:     short dbtbl                     DB Table to add the new 
//                                                  message table.
//
//                  UBYTE  dblang          Message table language ID.
//
//                  const char *const *texttab      Message table to be added.
//
//                  short pgnum                     Page where resides texttab.
//
//  Global Inputs:
//
//  Returns:        Bool        True    Succesful
//                              False   Error.
//  Notes:
//
//-----------------------------------------------------------------------------

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
//  TEXTGETSTRING   Get pointer and page of requested table.
//
//  Parameters:     None.
//
//  Global Inputs:  none.
//
//  Returns:        Message from Multilingual Database.
//
//  Notes:          String returned in MLBuffer.
//
//-----------------------------------------------------------------------------

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
//  SetLanguageDefault   Set default language ID in LOCTAB
//
//  Parameters:     None.
//
//  Global Inputs:  MLGetMessage.
//
//  Returns:        Message from Multilingual Database.
//
//  Notes:          String returned in MLBuffer.
//
//-----------------------------------------------------------------------------
void SetLanguageDefault ( void )
{
    TextSetCurrentLanguage( LNG_DEFAULT );
}


//=============================================================================
// Private function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//  TextSetCurrentLanguage  Set Current Language
//
//  Parameters:             UBYTE lang Language ID that current
//                                              language is to be set to
//
//  Global Inputs:
//
//  Returns:                None
//
//  Notes:
//
//-----------------------------------------------------------------------------

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

