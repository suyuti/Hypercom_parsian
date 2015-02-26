
//=============================================================================
//                       Servus Bilgisayar
//                      (c) Copyright 2006
//=============================================================================
//
// Module overview:     applicationexpire.c
//
//=============================================================================

#include "string.h"

#include "eftos.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "struct.h"
#include "utllib.h"
#include "comdata.h"
#include "appdata.h"
#include "fixdata.h"
#include "taginfo.h"

#include "tlib.h"

extern void SetTranTime( UBYTE * pDateTime );
static UBYTE CompareDates(char *date1, char *date2);
extern void PrintInit( void );
extern Bool PrintIt( char *SrcPtr, UWORD DataLen, Bool ForceIt );
extern void TerminalQuickReset( void );

extern void CheckApplicationExpire ( void )
{
   UBYTE now[6];  
//   UBYTE dummy[4];
   UBYTE result;
   char *str;
   char filever[] = {VREV, VMOD};
   UBYTE index;
//   UBYTE status;
   FILE *f;

   static const unsigned char SA_Tag_Info[] =
   {
      // Name       Type        Length
      'S','A',    'T','T',    0x00,0x01,
   };

#ifndef NEWAPPLICATION
   static const unsigned char DA_Tag_Info[] =
   {
      // Name       Type        Length
      'D','A',    'T','T',    0x00,0x00,
   };
#endif

	// If the terminal requires initialization, no need to check application expiry
	if ( 0x00 == TERM.TERMInit )
		return;

#ifdef NEWAPPLICATION
   result = 1;
#else
   result = 2;
#endif

   ParseCommandFile ( cfp_cmd_ApplicationList );

   // Check if in development mode
   if ( ! Number_Of_Applications )
   {
      return;
   }

   // Get System Time
   SetTranTime (&now[0]);

   // Check Application Expiry
   if (CompareDates(now, TCAP.TCAPPEXPDT) != result)
   {
      // Check if there is no other application
      if ( Number_Of_Applications < 2 )
      {
         // Beep 5 times and restart the terminal
         SDK_BeepIt(5);
      }
      else
      {
         // Create command file to notify the Application Manager
         f = fopen(COMMAND_FILE, "wb");

#ifndef NEWAPPLICATION
         // Check Application Deletion
         if (CompareDates(now, TCAP.TCAPPDELDT) != result)
         {
            // Delete Application
            fwrite( DA_Tag_Info, 1, sizeof(DA_Tag_Info), f );
         }
//         else
#endif
         {
            for (index = 0; index < 4; index++)
            {
               switch (index)
               {
                  case 0 :  
                     str = CSTMSG2;
                     break;
                  case 1 :
                     str = CSTMSG3;
                     break;
                  case 2 :
                     str = CSTMSG4;
                     break;
                  case 3 :
                     str = CSTMSG5;
                     break;
                  
                  default :
                     str = "";
                     break;
               }

               str += strlen(str) - 3;
               if ( strcmp(filever, str) )
                  break;
            }
            
            if (index < 4)
            {
               // Change Application
               fwrite( SA_Tag_Info, 1, sizeof(SA_Tag_Info), f );

               // Restore and append slot number selected
               fwrite( &index, 1, 1, f );
            }
            else
            {
               PrintInit();
               PrintIt( "Unable to load application.", 27, 0);
               PrintIt( "\n", 1, 0 ); 
               PrintIt( "Terminal has been locked.", 25, 0);
               PrintIt( "\n", 1, 1 ); 

               // Lock terminal
               for(;;);
            }
         }

         // Close file created
         fclose (f);
      }

      // Invoke Application Manager to perform fast start
      TerminalQuickReset ( );
   }
}


//-----------------------------------------------------------------------------
//  CompareDates
//-----------------------------------------------------------------------------
//  Parameters:     
//  Global Inputs   None
//  Returns:        0 Equal
//                  1 First One is Bigger
//                  2 Second One is Bigger
//  Notes:          sets length to length of read data
//-----------------------------------------------------------------------------
static UBYTE CompareDates(char *date1, char *date2)
{
   int idate1;
   int idate2;

   idate1 = (((int) date1[0]) << 16) + (((int) date1[1]) << 8) + date1[2];
   idate2 = (((int) date2[0]) << 16) + (((int) date2[1]) << 8) + date2[2];

   if (idate1 == idate2)
      return 0;
   
   if (idate1 > idate2)
      return 1;

   return 2;
}

