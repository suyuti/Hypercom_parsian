// force Settlement 2007 @ydeniz 

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "struct.h"
#include "string.h"
#include "appdata.h"
#include "comdata.h"
#include "fixdata.h"
#include "prtdef.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "util2.h"
#include "utllib.h"
#include "journal.h"

#include "forceSettlement.h"


// G�nsonuna zorlanmal� m� ? 
extern Bool ForceSettlement( void ) 
{       
    if ( !(TCAP.TCPOOP1 & TCPO_FORCESETTLEMENT) ) 
        return False;
    
    if ( NeedSettlement() ) 
    {
        ShowInfoMsg(SettlementRequired, BlankMsg);
        SDK_Wait(ONESECOND * 4);
        return True;
    }
    
    return False;
    
}

// Batchte kay�t var m� ? G�nsonuna gerek var m� ? 
extern Bool NeedSettlement ( void ) 
{ 
    UBYTE RtcBuffer[RTCLEN];	
    UBYTE RtcBcd[5];			// Current YYMMDDHHMM in BCD. 
    static UBYTE tmpbuf[3];
    struct trandata_rec B_TRINP ;
    static UWORD JrnIdentifier;
    
    TRINP.TRAQID = AQTAB[0].AQID;
    TRINP.TRAQPTR = (struct acquirer_rec *)&AQTAB;

    JournalLock();

    // Set up selection data in common record JSREC
    SetDefaultSelection();

    // Don't retreive any reversals or voids or settled
    JSREC.JSSTATOFF = TS1_ADV + TS1_VOID + TS1_REV + TS1_SETL;

    do {
      // JSREC will be used by JournalSelFirst()
      // JrnIdentifier will be set with first ident, if one exists.
      JrnIdentifier = JournalSelFirst();
      if (0x0000 == JrnIdentifier) 
      {
        // batch temiz isleme devam edebilir 
         JournalRelease();
         return False;
      }					
    } while (0x0000 == JrnIdentifier);
  
    //pJrnTransBuf = &B_TRINP;
    //JrnEntireFlag = True;		
    JournalRead ( JrnIdentifier, &B_TRINP );
    // ilk kayit okundu 
    //if (LOCTAB.LANGID == LNG_FA) {
    //    tmpbuf[0] = B_TRINP.TRJALALIYEAR[0];
    //    tmpbuf[1] = B_TRINP.TRJALALIDATE[0];
    //    tmpbuf[2] = B_TRINP.TRJALALIDATE[1];
   // }
    //else {
        tmpbuf[0] = B_TRINP.TRYEAR[0];
        tmpbuf[1] = B_TRINP.TRDATE[0];
        tmpbuf[2] = B_TRINP.TRDATE[1];
    //}

    SDK_RtcRead(RtcBuffer); 
    if (LOCTAB.LANGID == LNG_FA) {
        AscHex(RtcBcd, (char *)&RtcBuffer[0], 5);
    }
    else {
        AscHex(RtcBcd, (char *)&RtcBuffer[0], 5);
    }
    
    // gun gectiyse 
    if ( memcmp(&RtcBcd[2], &tmpbuf[2], 1) == 1 ) 
    {
      JournalRelease();
      return True;
    }
    // ay gectiyse 
    if ( memcmp(&RtcBcd[1], &tmpbuf[1], 1) == 1 )
    {
      JournalRelease();    
      return True;
    }
    // yil gectiyse 
    if ( memcmp(&RtcBcd[0], &tmpbuf[0], 1) == 1 )
    {
      JournalRelease(); 
      return True;
    }
   
    // Release Journal
    JournalRelease(); 
    return False;
}

// Batchte kay�t var m� ? 
// Batch i�erisindeki void, reversal ve advice olmayan kay�tlara bakar 
// e�er i�lem varsa True yoksa False d�ner . 
extern Bool HasBatchRecord( void ) 
{         
    static UWORD JrnIdentifier;
    
    TRINP.TRAQID = AQTAB[0].AQID;
    TRINP.TRAQPTR = (struct acquirer_rec *)&AQTAB;

    JournalLock();

    // Set up selection data in common record JSREC
    SetDefaultSelection();

    // Don't retreive any reversals or voids or settled
    JSREC.JSSTATOFF = TS1_ADV + TS1_VOID + TS1_REV + TS1_SETL;

    do {      
      JrnIdentifier = JournalSelFirst();
      if (0x0000 == JrnIdentifier) 
      {
          // batch temiz batchte kay�t yok 
         JournalRelease();
         return False;
      }					
    } while (0x0000 == JrnIdentifier);
    
    // Batchte kay�t var 
    JournalRelease();
    return True;
}




