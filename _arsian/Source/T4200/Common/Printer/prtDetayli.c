//=============================================================================
//                       Servus Bilgisayar
//                      (c) Copyright 2007
//                       Erdinc Taskin 05.03.2007
//                       ptrDetayli.c
//=============================================================================
//
// Veritabaný dosyalarýný ayrýntýlý biçimde yazdýrmak için

#include <string.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdktime.h"
#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "fixdata.h"
#include "msg.h"
#include "prtDetayli.h"
#include "util.h"


//@erd 28.02.2007
extern const int lenAcqTableVarName;	// Acquirer tablosunun etiketlerinin uzunluðu
extern const int lenTermTableVarName;	// Terminal tablosunun etiketlerinin uzunluðu
extern const int lenIssTableVarName;	// Issuer tablosunun etiketlerinin uzunluðu
extern const int lenCardTableVarName;	// Card Range tablosunun etiketlerinin uzunluðu

extern char LineBuffer[S_LineBuffer];

static ENTRY_STR YNEntry;		/* structure for YesNoEntry() */

//-----------------------------------------------------------------------------
// @erd 22.02.2007 
// PrintDataBaseFiled  Database alanýný istenilen formatta yazdýrýr
// Parameters:
// enum printType pPrintType Verinin hangi biçimde yazdýralacaðýný belirler (ASCII , NUMERIC , RAW )
// UBYTE
//-----------------------------------------------------------------------------
extern void PrintDataBaseField( char type , UBYTE *buff , int size)
{
    //char mesaj[255];
	int i;
	ClrIt((UBYTE *)LineBuffer, S_LineBuffer);
    switch(type)
    {
		case PRINTTYPE_NUMBER:
			
			BfAscii( LineBuffer , buff , size ); 
			//Sondaki 'F' leri temizle
			for ( i = 0 ; i < size *2 ; ++i)
				if ( LineBuffer[i] == 'F')
					LineBuffer[i] = ' ';

			SDK_PrintString(LineBuffer);	
			break;

		case PRINTTYPE_ASCII:

			memcpy( LineBuffer , buff , size );
			SDK_PrintString(LineBuffer);
			break;
			/*
		case PRINTTYPE_RAW:
			PrintData(N_NullStr, (UBYTE *)buff, size, False);
			break;
			*/
		case PRINTTYPE_BYTE:
			for ( i = 0 ; i < 8 ; ++i)
			{
				if ( *buff & (1<<( 8-i-1)) )
					LineBuffer[i] = '1';
				else
					LineBuffer[i] = '0';
			}
			SDK_PrintString(LineBuffer);	
			break;
		case PRINTTYPE_BOOL:
			i = (*buff & size);
			if ( i != 0 )
				SDK_PrintString("True");
			else
				SDK_PrintString("False");
			break;
    }
	SDK_PrintString("\n");	
}


//----------------------------------------------------------------------------------------
// @erd 01.03.2007
// printDBTable			DB Tablosunu formatlý bir biçimde kaðýda basar
//
// Return:
//  UBYTE Yazdýrmaya devam edilip edilmeyeceðini belirler
// ( Birden fazla olan tablolar için kullanýlýr ( Issuer ve Card Range )
//  Deðer 1 ise yazdýrmaya devam et
//  Deðer 0 ise yazdýrmayý iptal et
//  
// Parameters:
// tableVarName : Yazdýrýlacak DB tablosu verilerin etiketlerinin dizisinin adý ( xTableVarName.h dosyasýnda)
// tablVarTypeAndLen : Yazdýrýlacak DB tablosundaki verilerin tipleri ve uzunluklarýnýn dizisinin adý ( xTableVarName.h dosyasýnda)
// pData : DBden çekilmiþ verilerin saklandýðý struct ý gösterir
// lenTableVarName : Yazdýrýlacak DB tablosundaki etiketlerin dizisinin uzunluðu
//----------------------------------------------------------------------------------------
extern UBYTE printDBTable( const IMAGE_ID tableVarName ,const IMAGE_ID tableVarTypeAndLen ,const UBYTE * pData ,const int lenTableVarName  ) 
{
    return 1;
    /*
	UBYTE *ptrBaslik;			// Yazdýrýlacak alanýn Etiketi	
	UBYTE *ptrBaslikBegining;	// Yazdýrýlacak alanýn Etiketi ( herzaman  en baþýný gösterir)
	UBYTE *ptrVarType;			// Yazdýrýlacak alanýn tipi
	UBYTE *ptrData;				// Yazdýrýlacak Veri
        UBYTE ICCEntryStatus;                   // Yazdýrmaya devam-tamam sorusunun cevabý
	char tip;
	int len ;
	int boolValueIndeks ;		// Boolean deðiþkenin hangi bitinde olduðunu saklar
        int satirSayisi;
        
	ptrBaslik = GetImage( tableVarName );
	ptrBaslikBegining = ptrBaslik;
	ptrVarType = GetImage( tableVarTypeAndLen );

	boolValueIndeks = 0;
        satirSayisi = 0;

	// Verilerin saklandýðý struct yapýsý (struct.h)
	ptrData = pData;
	YNEntry.TitleMid = N_DBFormattedReport ;
	YNEntry.Line1Mid = N_Print;
	//YNEntry.Line2Mid = CONTINUE_MSG;
	//YNEntry.Line3Mid = N_NullStr;
	//YNEntry.Line4Mid = N_NullStr;

	//ICCEntryStatus = YesNoEntry( &YNEntry );

	while ( (ptrBaslik - ptrBaslikBegining) < lenTableVarName )
	{
                // 20 satýrda bir devam edip etmeyeceðini sor
                ++satirSayisi;
                if ( satirSayisi % 20 == 0)
                {
                    
                      while ( 1 )
                      {
                          ICCEntryStatus = YesNoEntry( &YNEntry );

                          if (  ICCEntryStatus == CLEAR_KY  ||
                                ICCEntryStatus == CANCEL_KY ||
								ICCEntryStatus == UP_KY)
										return 0;	// Gerikalanýný yazdýrma
                          else if ( ICCEntryStatus == ENTER_KY ||
                                    ICCEntryStatus == DOWN_KY)
                                        break;		// Yazdýrmaya devam et
                      }

                }
                  

		//Verinin tipini ve uzunluðunu al
		tip = ptrVarType[0];
		len = ptrVarType[1];
		ptrVarType +=2;
		
		// Alan Pointer yada ... ise yazdýrma
		if ( tip == PRINTTYPE_NOTPRINT)
			continue;

		// Etiketin baþýna git
		while ( *ptrBaslik != 0)
			++ptrBaslik;
		++ptrBaslik;

		//Baþlýðý yazdýr
		SDK_PrintString((char *) ptrBaslik);
		SDK_PrintString("\n");	
		
		// Deðeri Yazdýr
		PrintDataBaseField( tip , ptrData , len );

		// Eðer deðiþken Boolean ise 8 kez ptrData deðiþkenini arttýrmaz
		if ( tip == PRINTTYPE_BOOL ) 
		{
			// Ayný Boolean deðiþkeninin içinde
			++boolValueIndeks;

			//Yeni Bir Boolean baþladý
                        if ( boolValueIndeks >= 8 )
                        {
                              boolValueIndeks = 0;	
                              ++ptrData;				// Veri pointerýný bir bayt ötele
                        }
                 }
                 else
                 {
                        // Boolean harici bir deðiþken veya boolena deðiþkeninin sonu
                        boolValueIndeks = 0;
                        ptrData += len;
                 }
                  
	}
    
        return 1;
   */
}


//------------------------------------------------------------------------
// @erd 01.03.2007
// printTerminalTable :		Terminal tablosunu formatlý bir biçimde kaðýda basar
// 
//------------------------------------------------------------------------
extern void printTerminalTable()
{
	UBYTE *pData ;
	pData =(UBYTE*) &TCONF;
	SDK_PrintString("TERMINAL TABLOSU\n");
	SDK_PrintString("----------------\n");
	/*printDBTable( TERMINAL_TABLE_VAR_NAMES , 
					TERMINAL_TABLE_VAR_TYPE_AND_LENGTH,
					pData,
					lenTermTableVarName);
    */

}

//------------------------------------------------------------------------
// @erd 01.03.2007
// printAcquirerTable :		Acquirer tablosunu formatlý bir biçimde kaðýda basar
// 
//------------------------------------------------------------------------
extern void printAcquirerTable()
{
    /*
	UBYTE *pData = &AQTAB;
        SDK_PrintString("ACQUIRER TABLOSU\n");
	SDK_PrintString("----------------\n");
	printDBTable( ACQUIRER_TABLE_VAR_NAMES,
					ACQUIRER_TABLE_VAR_TYPE_AND_LENGTH,
					pData,
					lenAcqTableVarName);
    */
}

//------------------------------------------------------------------------
// @erd 01.03.2007
// printIssuerTable :		Issuer tablosunu formatlý bir biçimde kaðýda basar
// 
//------------------------------------------------------------------------
extern void printIssuerTable()
{
    /*
	UBYTE *pData;
	int i ;

	// Bütün Issuer tablolarýný ekrana bas
	for ( i = 0 ; i < ISMAX ; ++i)
	{
		// Böyle Bir Issuer tablosu var mý?
		if (ISTAB[i].ISID) 
		{
			SDK_PrintString("ISSUER TABLOSU\n");
			SDK_PrintString("--------------\n");

			pData = &ISTAB[i];	// Sýradaki Issuer tablosunu al
			// Issuer tablosunu yazdýr
                        if ( printDBTable (  ISSUER_TABLE_VAR_NAMES,
                                  ISSUER_TABLE_VAR_TYPE_AND_LENGTH,
                                  pData,
                                  lenIssTableVarName) == 0 )
                                        return; // Yazdýrmaya Devam Etme
                        
		}else
			break;
	}
    */
}

//------------------------------------------------------------------------
// @erd 01.03.2007
// printCardRangeTable :		Card Range tablosunu formatlý bir biçimde kaðýda basar
// 
//------------------------------------------------------------------------
extern void printCardRangeTable()
{
    /*
	UBYTE *pData ;	
	int i ;
        
	// Bütün card tablolarýný ekrana bas
	for ( i = 0 ; i < CRMAX ; ++i)
	{
		// Böyle bir kart tablosu var mý?
          if  (CRTAB[i].CRNUM ) {
			SDK_PrintString("KART TABLOSU\n");
			SDK_PrintString("------------\n");

			pData = &CRTAB[i];	// Sýradaki kart tablosunu al

			//Kart tablosunun yazdýr
			if ( printDBTable( CARD_TABLE_VAR_NAMES,
                                  CARD_TABLE_VAR_TYPE_AND_LENGTH,
                                  pData,
                                  lenCardTableVarName) == 0 )
                                      return ; //Yazdýrmaya devam etme
		}else
			break;
	}

    */
}


extern void PrtFormattedDB( void )
{
	UBYTE i;
	UBYTE Status;

	// Notify user -- "PRINTING, WAIT..."
	ShowInfoMsg( PrintingWait, PleaseWait );

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		return;
	}

	DrawMenu(MENU_DBFORMATTED);
	// See if printer may be opened
	if ( SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) == orvOK )
	{

		//DrawMenu(MENU_DBFORMATTED);
		i = 0 ; // GetOneKey fonksiyonunda doðru tuþa basýldýðýný kontrol etmek için bool olarak
		  do{
			  Status = GetOneKey(DEV_KBD + DEV_PAD + STAY_OPEN0, UserTimeout);
			  switch( Status) 
			  {
				case 'A':
				  printAcquirerTable();
				  break;
				case 'I':
				  printIssuerTable();
				  break;
				case 'C':
				  printCardRangeTable();
				  break;
				case 'T':
				  printTerminalTable();
				  break;
				default :
				  i = 1;
		          
			  }
		  } while (i == 1 && (Status != CANCEL_KY) && (Status != CLEAR_KY) );
          
		  //Herhangi birþey yazdýrýldýysa kaðýdý kesmek için biraz boþluk býrak
		  if ( i == 0 )
			SDK_PrintString( "\f" );		
		  
		  

		// Wait till printing is done
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
		{
			// Give up the CPU
			SDK_RelinqCPU(  );
		}

		// Close the printer
		SDK_PrinterClose(  );
	}

   
  
  
}
