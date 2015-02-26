//=============================================================================
//                           Altius-It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             Farsi_print.c
//      Support print for Farsi
//=============================================================================

#ifdef MAKE_FARSI

#include <string.h>
#include <stdlib.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"

#include "Farsi_print.h"
#include "Farsi_prnCodePage.h"

//=============================================================================
// External variables / declarations
//=============================================================================



//=============================================================================
// Private defines and typedefs
//=============================================================================

// Structure to define line buffer
typedef struct
{
    const CharDefinition* pSymbol;
    UBYTE font;
    UBYTE style;
}
__attribute__((packed)) LineRecord;


#define EscCmdSize 5
#define ThreadSize maxDotPerLine/8 //48 bytes

// Structure to print single thread using "Esc-g" printer command
typedef struct
{
    UBYTE EscG[EscCmdSize];
    UBYTE Thread[ThreadSize];
}
__attribute__((packed)) ThreadType;

// GrfBuf: buffer to print single character line
//
//               ESC   g  |   384 bits of data   |
// thread 01: " 0x1B 0x67 0xA0 0xBB 0xF6 .... 0x00"
// thread 02: " 0x1B 0x67 0xA0 0xBB 0xF6 .... 0x00"
//
// thread nn: " 0x1B 0x67 0xA0 0xBB 0xF6 .... 0x00"


//=============================================================================
// Private function declarations
//=============================================================================
static void           GetNextLine( void );
static void           MoveChar( const CharDefinition* pSymbol,
                                UBYTE font,
                                UBYTE style);
static void           InsertBitStream( UBYTE* pDest, UWORD Strem, UBYTE Width );
static OS_RETURN_VALS PrintGrfBuf( void );
static void           BuildFixedBuffer( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static UBYTE * pCurrentLine;
static UBYTE * pNextLine;
static UWORD LineLength;
static UWORD NumOfInputChars;
static UWORD NumOfCharProcessed;
static LineRecord LineBuf[FixedBufLength]; // Not mirrored buffer of fixed current Line
static ThreadType GrfBuf[ CharHeight ]; // Graphic image of the line
static UWORD Position;
static Bool Double;
static Bool DoubleHigh;



//=============================================================================
// Public function definitions
//=============================================================================

OS_RETURN_VALS Farsi_Print( UBYTE *pInpBuff, UWORD InpLen )
{
    OS_RETURN_VALS Result = orvOK;
    UWORD i;

    // Check if anything to print
    if( InpLen == 0 )
    {
        // Nothing to print, exit
        return 0;
    }

    // Set initial values
    pCurrentLine = pInpBuff;
    pNextLine = pInpBuff;
    LineLength = 0;
    NumOfInputChars = InpLen;
    NumOfCharProcessed = 0;

    // Loop to parse all the lines in input buffer
    do
    {
        // Get next line from input buffer
        GetNextLine( );
        Double = 0;
        DoubleHigh = 0;

        // Check if there is any data to print
        if( LineLength == 0 )
        {
            // Whole input buffer processed, exit
            return 0;
        }

        // Check if Line Feed only in the current line
        // This processing prevents the build of empty line in graphic
        // to speed up overall printing
        if( ( LineLength == 1 ) &&
            ( Farsi_prnCodePage_table[ *pCurrentLine ].fpi_options & o_LF ) )
        {
            // Yes, print empty line
//            Result = SDK_PrintString( "\x0a" );
            Result = SDK_PrintString( "\n" );
            if( Result != orvOK )
                return Result;
        }

        // Check if Form Feed only in the current line
        // This processing prevents the build of empty line in graphic
        // to speed up overall printing
        if( ( LineLength == 1 ) &&
            ( Farsi_prnCodePage_table[ *pCurrentLine ].fpi_options & o_FF ) )
        {
            // Yes, print form feed
            Result = SDK_PrintString( "\x0c" );
            if( Result != orvOK )
                return Result;
        }

        // Check if print Barcode command
        else if( ( LineLength > 2 ) && 
                 ( pCurrentLine[0] == 0x0B ) && 
                 ( pCurrentLine[1] == '*' ) )
        {
            // Yes, print Barcode image
            Result = SDK_PrintBuffer( pCurrentLine, LineLength );
            if( Result != orvOK )
                return Result;
        }

        // Check if print Firmware revision command
        else if( ( LineLength > 2 ) && 
                 ( pCurrentLine[0] == 0x1B ) && 
                 ( pCurrentLine[1] == 0x49 ) )
        {
            // Yes, print Barcode image
            Result = SDK_PrintBuffer( pCurrentLine, LineLength );
            if( Result != orvOK )
                return Result;
        }

        // Legacy character line processing
        else
        {
            // Build fixed buffer
            BuildFixedBuffer( );
    
            // Format graphic buffer
            for( i = 0; i < CharHeight; i++ )
            {
                memcpy( GrfBuf[i].EscG, ( UBYTE* )"\x1B\x47\x30\x00\x01", EscCmdSize );
                memset( &GrfBuf[i].Thread, 0x00, sizeof( ThreadType ) );
            }

            // Clear graphic buffer position
            Position = maxDotPerLine;
            //Position = 0;
            
            // Move characters from fixed buffer into graphic buffer
            for( i = LineLength; i > 0; i-- )
            {
                MoveChar( LineBuf[i-1].pSymbol,
                          LineBuf[i-1].font,
                          LineBuf[i-1].style);
            }

            // Print it
            Result = PrintGrfBuf( );
            if( Result != orvOK )
                return Result;

        }

    } while( 1 ); // end of "do"
    

    if( Result == orvOK ) 
        return 0;
    else
        return Result;
}

//-----------------------------------------------------------------------------
//
// Show printer character set
//
//-----------------------------------------------------------------------------
void Farsi_PrintCharacterSet( void )
{
    int i;
    int linecount = 0;
    UBYTE Buffer_To_Print[2*8*28+3];
    UBYTE *p;

    p = &Buffer_To_Print[0];

    for( i=0x20; i<0xFF; i++ )
    {
        *p++ = i;
        linecount++;
        if( linecount == 8 )
        {
            linecount = 0;
            *p++ = 0x0a;
        }
        else
        {
            *p++ = ' ';
        }
    }

    *p++ = 0xFF;
    *p++ = 0x0a;
    *p++ = 0x0a;
    *p++ = 0x0a;
    *p++ = 0x0a;

    // Print it
    Farsi_Print( Buffer_To_Print, sizeof(Buffer_To_Print) );
}



//=============================================================================
// Private function definitions
//=============================================================================


//------------------------------------------------------------------------
// Get the pointer to the next line in the input buffer
// Get also length of next line
//
// The result is placed on globals: pCurrentLine, LineLength
//
// Globals used: NumOfCharProcessed, NumOfInputChars, pNextLine
//
//------------------------------------------------------------------------
void GetNextLine( void )
{
    int i;
    UBYTE chr;

    // Assign current line pointer and length
    pCurrentLine = pNextLine;
    LineLength = 0;
    Double = 0;
    DoubleHigh = 0;

    // Check if whole inpun buffer processed
    if( NumOfCharProcessed >= NumOfInputChars )
        return;

    // Loop to find <LF>
    for( i = 0; i < Farsi_max_char_per_line + 1; i++ )
    {
        // Get current character
        chr = *pNextLine;

        // Mark this character as processed
        NumOfCharProcessed++;
        pNextLine++;
        LineLength++;

        // Check if current character requires Line Feed 
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_LF)
            break;

        // Check if whole input buffer processed
        if( NumOfCharProcessed >= NumOfInputChars )
            break;
    }
}


//------------------------------------------------------------------------
// Build the fixed, not mirrored line from input buffer
// The length will be fixed
//
// The result is placed on globals:
//
// Globals used: LineLength, pCurrentLine
//
//------------------------------------------------------------------------
void BuildFixedBuffer( void )
{
    UBYTE chr;
    int OutCharNun = 0;
    int i;

    // Prepare FARSI processing Flags
    UBYTE Font = 2;
    UBYTE Initial = 1;
    UBYTE Style = 0;  // This is a bitmap ( see Legend of fpi_command ):
                      // 0x01 - reverse style
                      // 0x02 - underline
                      // 0x04 - bold
                      // 0x08 - italic
                      // 0x10 - barcode
                      // 0x20 - number (must not be mirrored)

    // Clear destination buffer
    memset( LineBuf, 0x00, sizeof(LineBuf) );

    // Check if there is too long line
    if( LineLength > FixedBufLength )
    {
        // Cut over the sourse line and print notification warning
        LineLength = FixedBufLength;
        SDK_PrintString( "\x02" ); // set default font
        SDK_PrintString( "WARNING1: FixedBufLength is too short" );
        SDK_PrintString( "to parse the following FARSI string:" );
    }

    // Loop to see all the characters in the current line
    for( i = 0; i < LineLength; i++ )
    {
        // Get the character
        chr = pCurrentLine[i];

//----------------------------------------------
// Special processing for FONT switch commands
//----------------------------------------------
        // Check if this is FONT (NORMAL/DOUBLE etc.) selector
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_FN )
        {
            // Just apply this font
            Font = chr;
        }

//----------------------------------------------
// Special processing for STYLE switch commands
//----------------------------------------------
        // Check if this is STYLE selector
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_ST )
        {
            // Check what we need to do: set or reset style flag
            if( Farsi_prnCodePage_table[ chr ].fpi_command & 0x8000 )
            {
                // To set
                Style |= ( 0x00FF & Farsi_prnCodePage_table[ chr ].fpi_command );
            }
            else
            {
                // To reset
                Style &= ~( 0x00FF & Farsi_prnCodePage_table[ chr ].fpi_command );

            }
        }

        // Check if this may be part of whole number
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_NB )
        {
            // Yes, add number tracing
            Style |= 0x20;
        }
        else
        {
            // No, clear number tracing
            Style &= ~0x20;
        }

//----------------------------------------------
// Special processing for BRACKET
//----------------------------------------------
        // Check if this is STYLE selector
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_BK )
        {
            switch( chr )
            {
                case '(': 
                    chr = ')'; 
                    break;
                case ')': 
                    chr = '('; 
                    break;
                case '[': 
                    chr = ']'; 
                    break;
                case ']': 
                    chr = '['; 
                    break;
                case '{': 
                    chr = '}'; 
                    break;
                case '}': 
                    chr = '{'; 
                    break;
            }
        }

//-------------------------------------------------
// Legacy processing for printable characters
//-------------------------------------------------
        // Check if this is non-printable character
        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_NP )
        {
            // Do nothing
        }
        else
        {
            // Check for Initial
            if( Initial == 1 )
            {
                // Check next char does not have middle and end (disjoint character)
                // or this is not Farsi character
                if( 
                    ( 
                      ( Farsi_prnCodePage_table[pCurrentLine[i+1]].fpi_mdl_lft == NULL ) &&
                      ( Farsi_prnCodePage_table[pCurrentLine[i+1]].fpi_end_lft == NULL ) 
                    ) 
                    ||
                    ( ! Farsi_prnCodePage_table[ chr ].fpi_options & o_FS ) 
                  )
                {
                    // Insert Allone
                    LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_aln_lft;
                    LineBuf[OutCharNun].font = Font;
                    LineBuf[OutCharNun].style = Style;
                    OutCharNun++;

                    // check if there is right part of character
                    if( Farsi_prnCodePage_table[chr].fpi_aln_rgt != NULL )
                    {
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_aln_rgt;
                        LineBuf[OutCharNun].font = Font;
                        LineBuf[OutCharNun].style = Style;
                        OutCharNun++;
                    }
                }
                else
                {
                    // Insert Initial if found
                    if( Farsi_prnCodePage_table[chr].fpi_itl_lft != NULL )
                    {
                        // Found: insert initial
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_itl_lft;
                        
                        // Next characters is not initial or allone
                        if( Farsi_prnCodePage_table[ chr ].fpi_options & o_DJ )
                            Initial = 1;
                        else
                            Initial = 0;
                    }
                    else
                    {
                        // Not found: insert allone instead initial
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_aln_lft;
                        
                        // Next characters not initial or allone
                        Initial = 1;
                    }
                    LineBuf[OutCharNun].font = Font;
                    LineBuf[OutCharNun].style = Style;
                    OutCharNun++;

                    // check if there is right part of character
                    if( Farsi_prnCodePage_table[chr].fpi_itl_rgt != NULL )
                    {
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_itl_rgt;
                        LineBuf[OutCharNun].font = Font;
                        LineBuf[OutCharNun].style = Style;
                        OutCharNun++;
                    }

                }
            }

            // This is not initial or allone
            else
            {
                // Check next char does not have middle and end (disjoint character)
                if( ( Farsi_prnCodePage_table[pCurrentLine[i+1]].fpi_mdl_lft == NULL ) &&
                    ( Farsi_prnCodePage_table[pCurrentLine[i+1]].fpi_end_lft == NULL ) )
                {
                    // Insert end
                    LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_end_lft;
                    LineBuf[OutCharNun].font = Font;
                    LineBuf[OutCharNun].style = Style;
                    OutCharNun++;

                    // check if there is right part of character
                    if( Farsi_prnCodePage_table[chr].fpi_end_rgt != NULL )
                    {
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_end_rgt;
                        LineBuf[OutCharNun].font = Font;
                        LineBuf[OutCharNun].style = Style;
                        OutCharNun++;
                    }

                    // Next characters is initial or allone
                    Initial = 1;
                }
                else
                {
                    // Insert middle
                    LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_mdl_lft;
                    LineBuf[OutCharNun].font = Font;
                    LineBuf[OutCharNun].style = Style;
                    OutCharNun++;

                    // check if there is right part of character
                    if( Farsi_prnCodePage_table[chr].fpi_mdl_rgt != NULL )
                    {
                        LineBuf[OutCharNun].pSymbol = Farsi_prnCodePage_table[chr].fpi_mdl_rgt;
                        LineBuf[OutCharNun].font = Font;
                        LineBuf[OutCharNun].style = Style;
                        OutCharNun++;
                    }

                    // Check if this is disjoint character
                    if( Farsi_prnCodePage_table[ chr ].fpi_options & o_DJ )
                        Initial = 1; // yes - do break ligature
                    else
                        Initial = 0;

                
                }
            }
        }

        // Character inserted, check for length of buffer
        if( OutCharNun > FixedBufLength - 2 )
        {
            // Cut over the sourse line and print notification warning
            SDK_PrintString( "\x02" ); // set default font
            SDK_PrintString( "WARNING1: FixedBufLength is too short" );
            SDK_PrintString( "to parse the following FARSI string:" );

            break;
        }

    } // for( i = 0; i < LineLength; i++ )


    // Assign new length of line
    LineLength = OutCharNun;

/*
//-------------------------------------------------
// Do mirror of numbers
//-------------------------------------------------
    int i_start = 0;
    int i_end = 0;
    int j, k;
    char NumStarted = 0;
    LineRecord NumberBuf[40];    // The temporary storage for the number

    // Loop for chars in fixed buffer
    for( i = 0; i < LineLength; i++ )
    {
        // Check if number
        if( LineBuf[i].style & 0x20 )
        {
            // Yes, store current as end of number
            if( NumStarted == 0 )
            { 
                i_start = i;
            }

            NumStarted = 1;
            i_end = i;
        }

        if( !( LineBuf[i].style & 0x20 ) || ( (i+1) == LineLength ) )
        {
            NumStarted = 0;

            // Was there any number collected?
            if( ( ( i_end - i_start ) >= 1 ) &&
                 ( sizeof( NumberBuf ) > ( i_end - i_start ) ) )
            {
                // Yes, there is some number, copy it to temporary storage
                for( j=i_start, k=0; j<i_end+1; j++, k++ )
                    NumberBuf[k] = LineBuf[j];

                // Copy it back in reversed direction
                for( j=i_start, k=i_end - i_start; j<i_end+1; j++, k-- )
                    LineBuf[j] = NumberBuf[k];
                
                // Be ready for next number
                i_end = 0;
            }
            else
            {
                i_start = 0;
                i_end = 0;
            }

        }

    }
*/
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
static void MoveChar( const CharDefinition* pSymbol,
                      UBYTE font,
                      UBYTE style )
{
    int LineNum;
    UBYTE PixToInsert;

    // Check if symbol is provided by caller
    if( pSymbol == NULL )
    {
        return;
    }

    // Get character width and validate it
    PixToInsert = (UBYTE) pSymbol->CharWdth;
    
    // Check if double or normal font
    if( ( font == 0x17 ) || ( font == 0x03 ) || ( font == 0x06 ) || ( font == 0x0E ) )
        Double = 1;
    else
        Double = 0;

    // Check if double high font
    if( ( font == 0x05) || ( font == 0x06 ) || ( font == 0x07 ) )
        DoubleHigh = 1;

    // Check if line can be overflowed
    if( Double )
    {
        if( ( PixToInsert + PixToInsert ) > Position )
        {
            PixToInsert = Position >> 1;
        }
    }
    else
    {
        if( PixToInsert > Position )
        {
            PixToInsert = Position;
        }
    }

    if( PixToInsert > 0 )
    {

        // Decrease position in graphic buffer
        Position -= PixToInsert;
        if( Double ) 
            Position -= PixToInsert;

        // Insert character into graphic buffer
        for( LineNum = 0; LineNum < CharHeight; LineNum++ )
        {
            // Insert single line of character
            InsertBitStream( &GrfBuf[ LineNum ].Thread[0], 
                             (UWORD)pSymbol->CharBitMap[ LineNum ], 
                             PixToInsert );
        }

        // Check if underline style
        if( style & UND_OFF )
        for( LineNum = CharHeight-2; LineNum < CharHeight; LineNum++ )
        {
            // Insert single line of character
            InsertBitStream( &GrfBuf[ LineNum ].Thread[0], 
                             0xFFFF, 
                             PixToInsert );
        }
    }
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
void InsertBitStream( UBYTE* pDest, UWORD Strem, UBYTE Width )
{
    int n, i;
    UBYTE OutMask;
    UWORD InpMask;

    // Find byte No in destination
    n = Position >> 3;

    // Find the mask of bit in destination byte
    for( OutMask = 0x80, i = 0; i < Position - ( n << 3 ); i++ )
    {
        OutMask = OutMask >> 1;
    }

    // Find the mask of bit in source dot line
    for( InpMask = 0x8000, i = 0; i < 16 - Width; i++ )
    {
        InpMask = InpMask >> 1;
    }

    // Insert number of bits
    for( i = 0; i < Width; i++ )
    {
        if( Strem & InpMask )
        {
            pDest[n] |= OutMask;
        }

        OutMask = OutMask >> 1;
        if( OutMask == 0 )
        {
            OutMask = 0x80;
            n++;
        }
    
        if( Double )
        {
            if( Strem & InpMask )
            {
                pDest[n] |= OutMask;
            }
            OutMask = OutMask >> 1;
            if( OutMask == 0 )
            {
                OutMask = 0x80;
                n++;
            }
        }

        InpMask = InpMask >> 1;
    
    }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
OS_RETURN_VALS PrintGrfBuf( void )
{
    OS_RETURN_VALS Result;
    int i;

    // Print graphic buffer
    for( i = 0; i < CharHeight; i++ )
    {

        Result = SDK_PrintBuffer( ( UBYTE* ) &( GrfBuf[ i ] ), sizeof( ThreadType ) );
        if( Result != orvOK ) 
            goto Done;

        // Print the same again if double hight
        if( DoubleHigh )
        {
            Result = SDK_PrintBuffer( ( UBYTE* ) &( GrfBuf[ i ] ), sizeof( ThreadType ) );
            if( Result != orvOK ) 
                goto Done;
        }

    }

    // Relink
    SDK_RelinqCPU( );

    // Print interline space (several empty threeds)
    memset(GrfBuf[0].Thread, 0, ThreadSize );
    for( i = 0; i < InterLineSpace; i++ )
    {
         Result = SDK_PrintBuffer( (UBYTE*)&(GrfBuf[0]), sizeof( ThreadType ) );
    }

Done:
    // Clear graphic buffer position
    return Result;
}




#endif // MAKE_FARSI
