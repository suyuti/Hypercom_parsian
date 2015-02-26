
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

#include "autoinit.h"

// 
extern void AutoInitialise( void )
{
   UBYTE RtcBuffer[RTCLEN];	// RTC Read Buffer 
   static UBYTE prev_min[2];
   struct schedule_rec *pSchedulePtr;  
   UBYTE i, retval;

   SDK_RtcRead( RtcBuffer );

   if ( memcmp( prev_min, &RtcBuffer[8], 2 ) )
   {
      prev_min[0] = RtcBuffer[8];
      prev_min[1] = RtcBuffer[9];
      
      // parametre yükleme komutu içeren schedule var mý 
      for (i= 1, pSchedulePtr = SCHTAB; pSchedulePtr < &SCHTAB[SCHMAX]; pSchedulePtr++, i++)
      {
          if ( (pSchedulePtr->SCHFLAG & SCH_USED) && (pSchedulePtr->SMTYPE == SCH_REQ) )
              break;
          
      }
      
      // kayýt yok 
      if ( i >= SCHMAX ) 
        return;
      
      
      // kayýt var tarih kontrolü yap
      retval = IsTimeReach(pSchedulePtr->SMWHEN);
      if( retval )
      {   
          // parametre yüklenmeli flagini set ediyoruz ve iþlem için hazýrlanýyoruz 
          flagTag42 |= FLG_NEEDINIT;

          // Batchte Kayýt Varsa Günsonu Yapmaya Hazýrlanmalý yoksa Parametre Yüklenmeye hazýrlanmalý
          if ( HasBatchRecord() )
             ReadySettlement();    
          else 
             ReadyInitialize();
                                   
          // Schedule ile ilgili tüm bilgileri aldým 
          // schedule'i sonlandýrýyorum           
          pSchedulePtr->SCHFLAG &= ~SCH_USED;
          SetSCHTabLRC();
      }
      else // tarih geçmedi 
        return;         
  }

}


// ilgili tarih geçtimi kontrolü yapar , tarih geçtiyse veya eþitse true geçmediyse False 
// olarak döner 
// pTime = YYMMDDHHMM
extern Bool IsTimeReach( UBYTE *pTime ) 
{
   UBYTE RtcBuffer[RTCLEN];
   UBYTE RtcBcd[5];		// Current YYMMDDHHMM in BCD.  

   SDK_RtcRead( RtcBuffer );
   AscHex( RtcBcd, ( char * ) &RtcBuffer[0], 5 );

   if( memcmp( RtcBcd, pTime, 5 ) >= 0 )
      return True;
   
   return False;
}

// günsonuna gidebilmek için gerekli hazýrlýklarý yapar 
extern void ReadySettlement( void )
{
  struct batch_rec *pBatchRec;

  pBatchRec = FindBatTab( AQTAB[0].AQID);
  
  BfAscii((char *)TRINP.TRBATNO, pBatchRec->BATCUR, S_BATCUR);
  TRINP.TRAQID  = AQTAB[0].AQID;
  TRINP.TRAQPTR = (struct acquirer_rec *)&AQTAB;
  TRINP.TRKEY   = CLBATCHA;  
  flagTag42 |= FLG_NEEDSETTLE;

}

extern void ReadyInitialize( void )
{
  // baþarýlý settlement yapýldý ise settlementa gerek yok 
  if ( flagTag42 & FLG_NEEDSETTLE )
      flagTag42 &= ~FLG_NEEDSETTLE;
  
  // parametre yüklenmesi gerekli ise 
  if ( flagTag42 & FLG_NEEDINIT ) 
  {
      flagTag42 &= ~FLG_NEEDINIT;
      flagTag42 = FLG_INIT; // hemen parametre yükle 
  }
}

