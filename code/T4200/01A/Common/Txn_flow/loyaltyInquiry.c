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


#include "loyaltyInquiry.h"

#ifdef MAKE_KIB

extern void LoyaltyInquiry( void )
{
  // parametre yüklü mü
   if (0x00 == TERM.TERMInit)
      return;
  
   if ( ForceSettlement () ) 
      return;

   if ( JournalFullTest(  ) )
      return;	
  
   if ( !CheckCurrency() ) 
      return;

   TRINP.TRKEY = LOYALTY_INQ;
   CurrentEvent.NEXTEVENT = 0;

   ShowIssuer_Tran(  );
   if ( !ReadData(  ) )
      return;	

   if ( !AcctProc () )
      return;

   if ( INJECTKEY && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN) )
   {
      ShowErrMsg( Msg_NoManEntry );
      CurrentEvent.NEXTEVENT = 0;	// Back to idle 
      return;	
   }

   if ( !CheckCardExp(  ) )
      return;

   if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

   ForcedOnline = True;
  // Perform a Pre-Dial if allowed 
  PreDial ();   

  FlagHostCntl = FLAG_ALLOW_CANCEL;
  FlagHost(  );
  SaveResponseText(  );
  RespDisplay ();
  
  // tek Nüsha slip basýlmasý için 
  TRINP.TRREPRINT = 0x01;

  Receipt ();
}

#endif
