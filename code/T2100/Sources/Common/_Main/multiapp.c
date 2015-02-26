
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:     multiapp.c
//      Functions used to apply multiapplication enviroment
//
//=============================================================================

// System includes

// HSDK includes
#include "execdef.inc"
#include "tlib.h"
#include "clib.h"
#include "basictyp.h"

// Application includes
#include "struct.h"
#include "appdata.h"

// This module external declaration
#include "taginfo.h"


// Max tag data length
#define MaxDataLen  32


// Tag header stucture
struct cmd_file_tag_header
{
	UBYTE TAGNAME[2];			// Tag Name
	UBYTE TAGTYPE[2];			// Tag Type - not used at the moment
	UBYTE TAGLENG[2];			// Tag Data Length
} __attribute__((packed));

#define TagHdrSze sizeof(struct cmd_file_tag_header)   

//-----------------------------------------------------------------------------
// ParseCommandFile
//      Reads the command file and adjust parameters for multiapplication
//      environment.
//
// Parameters:
//          cfp_cmd_JournalName       - set journal file name
//          cfp_cmd_ApplicationList   - get the list of awailable applications
//      
//
// Returns:
//          cfp_retval_Completed      - OK
//          cfp_retval_NoCommandFile  - unable to open command file
//          cfp_retval_FormatError    - command file format error detected
//          cfp_retval_TooLongData    - tag data is too long, unable to parse
//
//
// Global Returns: Number_Of_Applications 
//
//-----------------------------------------------------------------------------

extern int ParseCommandFile( UBYTE ParseCommand )
{
    static unsigned char buf[MaxDataLen];
    static struct cmd_file_tag_header header;

    UBYTE n = 1;

	FILE *f;
	size_t bytesRead, length;

    Number_Of_Applications = 0;

    //======================================================
    //          CLEAR DESTINATION BUFFERS
    //======================================================
    // For application list required
    if (cfp_cmd_ApplicationList == ParseCommand)
    {
        // Clear storages for menu items
        // max 7 items awailable 
        memset (CSTMSG2, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG3, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG4, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG5, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG6, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG7, 0, MAX_MSG_LEN + 1);
        memset (CSTMSG8, 0, MAX_MSG_LEN + 1);
    }
    
    //======================================================
    //          OPEN THE COMMAND FILE
    //======================================================

    // Try to open for reading
	if ((f = fopen(COMMAND_FILE, "r")) == NULL)
	{
        // Command file not exist - return 0
		return cfp_retval_NoCommandFile;
	}

	// Main Loop for tags reading and parsing
	while ( 1 )
	{
        //======================================================
        //          READ AND PARSE TAG HEADER
        //======================================================
        
        // Read Tag header (Name, Type, Length) into header
        bytesRead = fread ( &header, 1, TagHdrSze, f );

        // Check if the lengths mismatch
		if ( bytesRead != TagHdrSze)
		{
            // Check nothing readed and end of file
            if ( ( 0 == bytesRead ) && feof ( f ) )
            {
                // Yes, parsing completed with OK
                fclose(f);
				return cfp_retval_Completed;
            }
            else
            {
                // No, breack parsing and return Format Error
                fclose(f);
                Number_Of_Applications = 0;
				return cfp_retval_FormatError;
            }
		} // bytesRead != TagHdrSze


        //======================================================
        //          GET THE LENTH OF TAG DATA
        //======================================================

        // Get the data length from the header
        bytesRead = (header.TAGLENG[0] << 8) + (header.TAGLENG[1] );

        // Check if length of data is too long
        if ( bytesRead > MaxDataLen )
        {
            // Unable to read data - return Too Long Data
            fclose(f);
            Number_Of_Applications = 0;
            return cfp_retval_TooLongData;
        }

        // Read and parse Tag Data if possible
        if ( 0 != bytesRead )
        {

            //======================================================
            //          READ TAG DATA
            //======================================================

            // Read data into buf
            length = fread(buf, 1, bytesRead, f);

            // Check if lenghts mismatch
            if ( length != bytesRead)
            {
                // Breack parsing and return Format Error
                fclose(f);
                Number_Of_Applications = 0;
				return cfp_retval_FormatError;
            }


            //======================================================
            //          PARSE "JN" TAG DATA
            //======================================================
            
            // Parse "JN" tag - Journal Name
            if ( (cfp_cmd_JournalName == ParseCommand) &&
                  ( !memcmp ( header.TAGNAME, "JN", 2 ) ) )
            {
                // Set journal file name
				buf[ length ] = 0;
				ForceJournalFileName( buf );

                // Ignore other tags and return Completed status
                fclose(f);
				return cfp_retval_Completed;
            }


            //======================================================
            //          PARSE "AP" TAG DATA
            //======================================================
            // every "AP" tag moved into CSTMSG2...CSTMSG8 buffers:
            // CSTMSGx[0]...CSTMSGx[16] - ASCII App Name (null terminated)
            // CSTMSGx[MAX_MSG_LEN+1] - App slot number + 1 
            //      +1 because Application Manager enumerated slots
            //      starting by null. Note, this value will be used
            //      in menu processing, when 0 = timeout
            //======================================================

            // Parse "AP" tag - Application list
            if ( (cfp_cmd_ApplicationList == ParseCommand) &&
                  ( !memcmp ( header.TAGNAME, "AP", 2 ) ) )
            {
                // Adjust the length if it too long; must be < 18:
                // 1 byte - slot number
                // max 16 bytes - app name (text on menu button)
                // 1 byte - null terminator
                if ( length > 16 )
                {
                    length = 16;
                }

                // Increment "AP" tags counter
                // (only APPMAX applications supported !!!)
                n++;

                // Select appropriate destination menu buffer
                switch ( n )
                {
                    case 2:
                        // set application slot number
                        CSTMSG2[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG2, &buf[1], length -1 );
                        if (CSTMSG2[0]) Number_Of_Applications++;
                        break;

                    case 3:
                        // set application slot number
                        CSTMSG3[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG3, &buf[1], length -1 ); 
                        if (CSTMSG3[0]) Number_Of_Applications++;
                        break;

                    case 4:
                        // set application slot number
                        CSTMSG4[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG4, &buf[1], length -1 ); 
                        if (CSTMSG4[0]) Number_Of_Applications++;
                        break;

                    case 5:
                        // set application slot number
                        CSTMSG5[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG5, &buf[1], length -1 ); 
                        if (CSTMSG5[0]) Number_Of_Applications++;
                        break;

                    case 6:
                        // set application slot number
                        CSTMSG6[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG6, &buf[1], length -1 ); 
                        if (CSTMSG6[0]) Number_Of_Applications++;
                        break;

                    case 7:
                        // set application slot number
                        CSTMSG7[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG7, &buf[1], length -1 ); 
                        if (CSTMSG7[0]) Number_Of_Applications++;
                        break;

                    case 8:
                        // set application slot number
                        CSTMSG8[MAX_MSG_LEN + 1] = buf[0] + 1;

                        // set application name
                        memcpy (CSTMSG8, &buf[1], length -1 ); 
                        if (CSTMSG8[0]) Number_Of_Applications++;
                        break;

                    default:
                        // no more storage for "AP" tags - ignore it
                        fclose(f);
                        return cfp_retval_Completed;
                
                } // switch

            } // if this is "AP" tag


/*
            //======================================================
            //          PARSE "??" TAG DATA - this is an example
            //======================================================
            // Parse "AP" tag - Application list
            if ( (cfp_cmd_?????????????? == ParseCommand) &&
                  ( !memcmp ( header.TAGNAME, "??", 2 ) ) )
            {
                // 
                // tag data parsing function must be located here
                //
            }
*/

        } // 0 != bytesRead

    } // while (1)

}
