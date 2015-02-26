#ifdef MAKE_KIB

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
#include "checkCurrency.h"
#include "receipt.h"
#include "respdsp.h"
#include "predial.h"
#include "amtproc.h"
#include "flaghost.h"
#include "acctproc.h"
#include "amtproc.h"
#include "app1.h"
#include "prtdrv.h"
#include "hstprint.h"
#include "app2.h"

#include "loyalty.h"

static void MaximumRatio( UBYTE *maxratio ) ;
static void BcdToByte( UBYTE *dest, const UBYTE *source, UBYTE len );
static void ByteToBcd( UBYTE *dest, const int *source, UBYTE len );

extern void LoyaltyInquiry( void )
{
   UBYTE status;
   UBYTE ZeroAmount[S_TRTOTAM]= { 0x00,0x00,0x00,0x00,0x00,0x00 };

  // parametre y�kl� m�
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
   
   TRINP.TRLOYALTYSTATUS |= TS_POINT;

   ShowIssuer_Tran(  );

   if ( !Reading(  ) )
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
  
  // tek N�sha slip bas�lmas� i�in 
  TRINP.TRREPRINT = True;

  Receipt ();  

  // Hi� Puan Yoksa Men� Se�imi Yapt�rma
  if ( !memcmp( TRINP.AVAIL_LOYALTY_AMT, ZeroAmount, S_TRTOTAM ) ) 
      return;
  
  // iki n�sha slip bas�lmas�n� aktive et 
  TRINP.TRREPRINT = False;

  // puan sorgulama i�lemi ba�ar� ile tamamland�ysa 
  if ( TRINP.TRRSPOPT & R_APPROVED )
  {               
      DrawMenu( MENU_LOYALTY_LIST );
      
      do
      {   
          status = GetOneKey( DEV_KBD | STAY_OPEN0, UserTimeout*4 );
          switch( status )
          {
              case '1' :         // puanl� sat��             
                   LoyaltySale();
                   return;     
                                      
              case '2' :        // puanl� taksitli sat��
                   LoyaltyInstallment();
                   return;

              case '3' :        // indirimli puanl� sat��
                    TRINP.TRLOYALTYSTATUS |= TS_DISCOUNT;
                    LoyaltySale();
                    return;

              case '4' :       // indirimli taksitli sat��
                    TRINP.TRLOYALTYSTATUS |= TS_DISCOUNT;
                    LoyaltyInstallment();
                  return;

              default :
                  return;
      }  
    }while((status != CANCEL_KY) && (status != CLEAR_KY));
  }
  
}

extern void LoyaltySale( void )
{  

  // e�er �ipli i�lem yap�ld�ysa EMVPROC'a y�nlendirilir  
  if ( IsSaleAnInstalment() ) 
      return;  

  TRINP.TRKEY = SALE;
  CurrentEvent.NEXTEVENT = 0;
  
  SetKIBTranName(CurrentEvent.TRNTEXT, SALE);
  SetDefaultTitle();
  ShowIssuer_Tran();

  ForcedOnline = True;
  if ( !INJECTKEY )
    PreDial ();

  
  if ( !LoyaltyProcessing() )
      return;
  
  if ( InInvoiceNum(  ) )
      return;
  
  if ( ((TRINP.TRISPTR->ISOPT1) & IS1_PIN) && !((TRINP.TRISPTR->ISPINTRAN) & ISPIN_SALE) )
      TRINP.TRKEY = DEBIT;

  do
  {                
    // Get PIN from PIN PAD, if required 
    if ( !GetPIN () )
    {
      // Finish receipt printing, if preprint is enabled 
      if ( (AQ4_PREPRINT & (TRINP.TRAQPTR->AQOPT4)) && TRINP.TRPINRETRY )
	Receipt ();
	return;
    }
                
    

    // Set flags to print the receipt header and allow user cancel. 
    FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

    // Notify host packet is ready and wait for response.                  
    FlagHost(  );

    // Save host response text in terminal flag table.  
    SaveResponseText(  );

    // Display Response 
    RespDisplay ();
  }while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );
  
  Receipt ();

}


extern void LoyaltyInstallment( void )
{  
  // terminalin taksitli i�lem izni yok   
  if (!(TCONF.TCOPT4 & TC4_INSPERMIT)) 
  {  
    ShowErrMsg( TerminalNotAllowed );
    return;
  }
  
  // kart�n taksitli i�lem yapma izni yok 
  if (TRINP.TRISPTR->ISOPT4 & IS4_INSBLOK) 
  {
    ShowErrMsg( Msg_InstallmentNotAllowed );
    return;
  }

  TRINP.TRKEY = INSTALLMENT;
  CurrentEvent.NEXTEVENT = 0;

  SetKIBTranName(CurrentEvent.TRNTEXT, INSTALLMENT);
  SetDefaultTitle();
  ShowIssuer_Tran();

  ForcedOnline = True;
  if ( !INJECTKEY )
    PreDial ();
  
  if ( !LoyaltyProcessing() )
      return;
  
  if( !InstallmentProcessing() )
    return;
  
  if ( InInvoiceNum(  ) ) return;  

  FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

  FlagHost(  );
	
  SaveResponseText(  );

  RespDisplay ();

  Receipt (); 
}


// tutar� al�r , kullan�lacak puan� hesaplar 
extern Bool LoyaltyProcessing( void )
{  
  BYTE retval;
  UBYTE TempTotam[S_TRTOTAM];

  // Tutar ekran�
  if ( !AmtProc() ) 
      return False;
  
  // Puan ekran� 
  //if ( !InputAnAmount( LoyaltyAmountEnt, 0 ) )
  //   return False;

  if ( !Discount())
      return False;

  // indirimli i�lem ise indirimli tutar �zerinden puan kontrolleri yap�l�r 
  if ( TRINP.TRLOYALTYSTATUS & TS_DISCOUNT )
      memcpy(TempTotam, TRINP.TRTOTAMAFTDISCOUNT, S_TRTOTAM);
  else
      memcpy(TempTotam, TRINP.TRTOTAM, S_TRTOTAM);

  retval = memcmp(TRINP.AVAIL_LOYALTY_AMT, TempTotam, S_TRTOTAM);

    // puan >= tutar ise kismi puan� harca 
  if ( retval >= 0 ) 
  {
    // i�lem taksitli i�lemi ise puan�n tamam� harcanamaz
    if ( TRINP.TRKEY == INSTALLMENT) 
    {
      ShowErrMsg( Msg_NotPermitLoyIns );
      
      PrintInit(  );	
      PrintDriver( RP_POINTINSTFAIL, &TRINP );
      SDK_PrinterClose(  );	
      if ( PrinterOK != PrinterErrorFlags )
          DisplayPrintError ();

      return False;
    }
    memcpy(TRINP.TRUSEDLOYALTYAMOUNT, TempTotam, S_TRTOTAM);    
  }
  else if ( retval < 0 ) // puan < tutar ise tutar kadar puan harca (t�m puan� harca)
  {
      memcpy(TRINP.TRUSEDLOYALTYAMOUNT, TRINP.AVAIL_LOYALTY_AMT, S_TRTOTAM);
      TRINP.TRLOYALTYSTATUS |= TS_ALLPOINT;
  }
  
  // Tutar - Puan (Sat��)
  SubAmt( TRINP.TRTOTAMAFTPOINT, TempTotam, TRINP.TRUSEDLOYALTYAMOUNT );
  return True;  
}

// taksitli i�lem se�eneklerini kontrol eder
// taksit giri�i varsa True , yoksa False d�ner.
extern Bool InstallmentProcessing( void ) 
{ 
    unsigned char tempBuf[30];
static const ENTRY_STR InstNumEntry = {
	InstNum,					// Default Title
	InstNum,					// Blank Line (line 1) 
	CustomMsg,					// Card Present? (line 2)
};
    // taksitli i�lemi de�ilse ak��a devam et 
    if ( !(TRINP.TRKEY == INSTALLMENT) )
        return True;

    // varsay�lan taksit aktif ise varsay�lan taksit tutar� kullan�l�r.
    if ( TCAP.TCPOOP1 & TCPO_DEFINS )
    {
      TRINP.TRINSTNUM = TCAP.TCPDEFINS;
      return True;
    }

    do {
        memset( tempBuf, 0x20, 30);
        BfAscii( tempBuf, &TCAP.TCPMININS, 1);
        memcpy( &tempBuf[2], "-", 1);
        BfAscii( &tempBuf[3], &TCAP.TCPMAXINS, 1);   
              
        memcpy(CSTMSG, tempBuf, 30);                  
        if (NumEntry(1, 2, 1, &InstNumEntry)) 
        {
          TRINP.TRINSTNUM = Dspbuf[0] - '0';
          if (Dspbuf[1] != 0)						
          {
            TRINP.TRINSTNUM <<= 4;
            TRINP.TRINSTNUM |= (Dspbuf[1] - '0');
          }

          if ( TRINP.TRINSTNUM > TCAP.TCPMAXINS )
              ShowErrMsg( Msg_OverInsNum );
          else if (  TRINP.TRINSTNUM < TCAP.TCPMININS ) 
              ShowErrMsg( Msg_LowerInsNUm );
          else 
              break;                
    }
    else 
        return False;
  } while ( 1 ); 
    
  return True;
}

// kart tak�l� ise �ipten okur / kart ge�irilirse manyetikten okur 
// kart tak�l� fakat �ip okunamad� ise manyetikten okur .
extern Bool Reading( void )
{
    if ( !ReadData(  ) )
    {
      // �ipli okuyamad� manyetikten okunmal� 
      if ( TRINP.TRSTATUS[1] & TS2_LOYALTYFBCK )
      {          
         if ( !ReadData(  ) )
            return False;	
      }
      else
        return False;
    }
    
    return True;
}

// t�m sat�� i�lemlerini taksitli i�leme y�nledirme processi 
// sat�� i�lemi taksitliye y�nlendrilmi�se True, y�nlendirilmemi�se False d�ner. 
extern Bool IsSaleAnInstalment( void )
{
    
    // sat�� i�leminde miyim ?
    if ( !(TRINP.TRKEY == SALE || (TRINP.TRLOYALTYSTATUS & TS_POINT) ) )
      return False;

    // sat�� i�lemlerini taksitliye y�nlendir se�ene�i aktif ise ve y�nlendirir    
    if ( TCAP.TCPOOP1 & TCPO_TRANINS )
    {                    
          // puanl� i�lemse puanl� taksitli sat��a y�nlendirilir 
          if ( TRINP.TRLOYALTYSTATUS & TS_POINT)                        
              LoyaltyInstallment();          
          else
              CurrentEvent.NEXTEVENT = INSTALLMENT;

          return True;
    }
           
    return False;
}

// indirimli sat�� i�lemi ise yap�lacak indirimi hesaplar 
// indirimli i�lem i�in indirim oran� girilmek zorundad�r girilmezse False ,
// i�lem indirimli de�ilse veya indirim hesaplanm�� ise True d�ner.
extern Bool Discount( void )
{
static const ENTRY_STR DiscRateEntry = {
	N_Discount,					// Default Title
	N_RateEntry,					// Blank Line (line 1) 
	CustomMsg,					// Card Present? (line 2)
};
    UBYTE MAXRATIO[2] = {0x00, 0x00};
    unsigned char tempBuf[30];
    
    // indirim yap�lmadan �nceki tutar �ncelikle kopyalan�r. (sliplerde kullan�l�yor)
    memcpy( TRINP.TRTOTAMAFTDISCOUNT, TRINP.TRTOTAM, S_TRTOTAM );

    // �ndirimli i�lem de�ilse 
    if( !(TRINP.TRLOYALTYSTATUS & TS_DISCOUNT) )
        return True;

    do {
       MaximumRatio( MAXRATIO );
       memset( tempBuf, 0x20, 30);
       memcpy( tempBuf, "%01-%", 5 );
       BfAscii( &tempBuf[5], &MAXRATIO[0], 1);           
       memcpy(CSTMSG, tempBuf, 30); 
       if ( NumEntry(1, 2, 1, &DiscRateEntry) )
       {
          TRINP.TRDISCOUNTRATIO[0] = Dspbuf[0] - '0';
          if (Dspbuf[1] != 0)						
          {
            TRINP.TRDISCOUNTRATIO[0] <<= 4;
            TRINP.TRDISCOUNTRATIO[0] |= (Dspbuf[1] - '0');
          }            
          // indirim oran� database'de belirlenen maksimum indirim oranlar�ndan b�y�k olamaz
          if ( TRINP.TRDISCOUNTRATIO[0] > MAXRATIO[0] )
            ShowErrMsg( Msg_OverDiscount );
          else
            break;
      }
      else // timeout or cancel, indirim oran� girilmezse 
        return False;
    }while(1);

    // indirim yap�ld�ktan sonraki tutar hesaplan�r 
    FindAftDiscount( TRINP.TRTOTAMAFTDISCOUNT, TRINP.TRTOTAM, TRINP.TRDISCOUNTRATIO );
    
    // indirim tutar� hesaplan�r
    SubAmt( TRINP.TRDISCOUNTTOTAM, TRINP.TRTOTAM, TRINP.TRTOTAMAFTDISCOUNT );

    // hesaplamalar yap�ld� 
    return True;

}


// i�lem tipine g�re maksimum indirim oran�n� maxrate i�erisine yazar
static void MaximumRatio( UBYTE *maxrate ) 
{
  if ( TRINP.TRKEY == SALE )
      memcpy( maxrate, TCAP.TCPSALEMAXDIS, 2);
  else // ( TRINP.TRKEY == INSTALLMENT ) || (TRINP.TRKEY == EMV_INSTALLMENT)
      memcpy( maxrate, TCAP.TCPINSMAXDIS, 2);  
}

extern void FindAftDiscount(UBYTE *aftdiscount, UBYTE *total, UBYTE *ratio )
{
    UBYTE yuzde100[6] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    UBYTE tmpYuzde[6] = {0x00};

    memcpy( &tmpYuzde[4], ratio, 2 );

    SubAmt( tmpYuzde, yuzde100, tmpYuzde );

    YuzdeHesapla( aftdiscount, total, &tmpYuzde[4] );
}

//---------------------------------------------------------------------
//
//  Yuzde Hesaplar
// 
//  Parametre:
//            UBYTE * sonuc : Y�zdesi hesaplanm�� tutar , BCD tipinde  (S_TRTOTAM boyutunda )
//            UBYTE * tutar : Y�zdesi hesaplanacak tutar , BCD tipinde ( S_TRTOTAM boyutunda )
//            UBYTE * yuzde : Oran , BCD tipinde ( 2 bayt boyutunda XX,YY �eklinde)
//
//  Not:
//          �arp�m yap�ld�ktan sonra herhangi bir yuvarlama i�lemi yap�lmamaktad�r.
//          
//
//---------------------------------------------------------------------
extern void YuzdeHesapla( UBYTE *sonuc , UBYTE *tutar , UBYTE *yuzde )
{     
    int elde;
    int kalan;
    int carpim;
    int i,j;
        
    UBYTE tmpTutar[12] = {0};
    UBYTE tmpYuzde[4] = {0};
    int tmpSonuc[24] = {0};
    

    BcdToByte ( tmpTutar , tutar , 6 );
    BcdToByte ( tmpYuzde , yuzde , 2 );
    
    elde = 0;
    for( i = S_TRTOTAM*2 - 1; i >= 0; i-- )  /* 2. sayi */
    {
        for( j = 3; j >= 0; j-- )  /* 1. sayi */
        {
                carpim=(tmpTutar[i]*tmpYuzde[j])+elde;
                elde=carpim / 10;
                kalan=carpim % 10;
                elde+=( (tmpSonuc[i+j+1]+kalan) / 10);
                tmpSonuc[i+j+1]=(tmpSonuc[i+j+1]+kalan) % 10;	
        }
        tmpSonuc[i+j+1] += elde;
        elde=0;
    }

    ByteToBcd( sonuc, tmpSonuc, 6);    
    return;
}
// @erd Biti�

static void BcdToByte( UBYTE *dest, const UBYTE *source, UBYTE len ) 
{
  int i;

  for ( i = 0; i < len*2; i++ )
  {
    if ( i%2 == 1 )
        dest[i] = source[i/2] & 0x0F;
    else
        dest[i] = (source[i/2] & 0xF0) >> 4;
  }

}

// len = dest uzunlu�udur , source alan�n�n uzunlu�u destin 2 kat�d�r
static void ByteToBcd( UBYTE *dest, const int *source, UBYTE len )
{
    int i;

    for( i = len - 1; i >= 0; i-- )    
      dest[i] = source[i*2+1] + (source[i*2] << 4);
  
}

// isimlendirilme maximum olarak 20 karakter olacak �ekilde
// Puanl� ve indirimli  i�lemlerinin isimlendirilmesini yapar
// Dikkat isimlendirilmelerdeki uzunluklar �nemli !
extern void SetKIBTranName( char *trntext , const UWORD EventID )
{     
#define MAX_LEN_SPUANLI			9 // Maximum Smart Puanl� text uzunlu�u
#define MAX_LEN_INDIRIMLI		4 // Maximum Indirimli text uzunlu�u

  char tempMsg[40] = {0};
  UBYTE index = 0;
  UBYTE len;
  
  if ( (TRINP.TRLOYALTYSTATUS & TS_POINT) || (TRINP.TRLOYALTYSTATUS & TS_DISCOUNT) )
  {   
      memset( tempMsg, 0x20, S_TRNTREXT );

      // puanl� ise 
      if ( TRINP.TRLOYALTYSTATUS & TS_POINT)
      {
          GetMsg( N_Point, tempMsg );
          index += MAX_LEN_SPUANLI;
      }
      
      // indirimli ise 
      if ( TRINP.TRLOYALTYSTATUS & TS_DISCOUNT )
      {
          GetMsg( N_Disc, &tempMsg[index] );
          index += MAX_LEN_INDIRIMLI;
      }
            
      switch( EventID ) 
      {
          case SALE:
              GetMsg( N_Sale, &tempMsg[index] );
              break;

          case INSTALLMENT:          
              GetMsg( N_Installment, &tempMsg[index] );
              break;
      
          default: 
            return;
      }
    
      len = strlen(tempMsg);
      // String i�inde E�er null sonda de�ilse sona �teler
      // "ORNEK\0    "  -> "ORNEK    \0"
      if ( len < S_TRNTREXT )
      {	
	memset( &tempMsg[len] ,0x20 , S_TRNTREXT - len );
      }

      tempMsg[S_TRNTREXT-1] = 0x00;		       
      memcpy( trntext, tempMsg, S_TRNTREXT );         
  }
    
}


#endif
