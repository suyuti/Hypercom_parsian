//=============================================================================
//                       Servus Bilgisayar
//                      (c) Copyright 2007
//                       Erdinc Taskin 05.03.2007
//                       ptrDetayli.c
//=============================================================================
//
// Veritaban� dosyalar�n� ayr�nt�l� bi�imde yazd�rmak i�in

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
extern const int lenAcqTableVarName;	// Acquirer tablosunun etiketlerinin uzunlu�u
extern const int lenTermTableVarName;	// Terminal tablosunun etiketlerinin uzunlu�u
extern const int lenIssTableVarName;	// Issuer tablosunun etiketlerinin uzunlu�u
extern const int lenCardTableVarName;	// Card Range tablosunun etiketlerinin uzunlu�u

extern char LineBuffer[S_LineBuffer];

static ENTRY_STR YNEntry;		/* structure for YesNoEntry() */

//-----------------------------------------------------------------------------
// @erd 22.02.2007 
// PrintDataBaseFiled  Database alan�n� istenilen formatta yazd�r�r
// Parameters:
// enum printType pPrintType Verinin hangi bi�imde yazd�ralaca��n� belirler (ASCII , NUMERIC , RAW )
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
// printDBTable			DB Tablosunu formatl� bir bi�imde ka��da basar
//
// Return:
//  UBYTE Yazd�rmaya devam edilip edilmeyece�ini belirler
// ( Birden fazla olan tablolar i�in kullan�l�r ( Issuer ve Card Range )
//  De�er 1 ise yazd�rmaya devam et
//  De�er 0 ise yazd�rmay� iptal et
//  
// Parameters:
// tableVarName : Yazd�r�lacak DB tablosu verilerin etiketlerinin dizisinin ad� ( xTableVarName.h dosyas�nda)
// tablVarTypeAndLen : Yazd�r�lacak DB tablosundaki verilerin tipleri ve uzunluklar�n�n dizisinin ad� ( xTableVarName.h dosyas�nda)
// pData : DBden �ekilmi� verilerin sakland��� struct � g�sterir
// lenTableVarName : Yazd�r�lacak DB tablosundaki etiketlerin dizisinin uzunlu�u
//----------------------------------------------------------------------------------------
extern UBYTE printDBTable( const IMAGE_ID tableVarName ,const IMAGE_ID tableVarTypeAndLen ,const UBYTE * pData ,const int lenTableVarName  ) 
{
    return 1;
    /*
	UBYTE *ptrBaslik;			// Yazd�r�lacak alan�n Etiketi	
	UBYTE *ptrBaslikBegining;	// Yazd�r�lacak alan�n Etiketi ( herzaman  en ba��n� g�sterir)
	UBYTE *ptrVarType;			// Yazd�r�lacak alan�n tipi
	UBYTE *ptrData;				// Yazd�r�lacak Veri
        UBYTE ICCEntryStatus;                   // Yazd�rmaya devam-tamam sorusunun cevab�
	char tip;
	int len ;
	int boolValueIndeks ;		// Boolean de�i�kenin hangi bitinde oldu�unu saklar
        int satirSayisi;
        
	ptrBaslik = GetImage( tableVarName );
	ptrBaslikBegining = ptrBaslik;
	ptrVarType = GetImage( tableVarTypeAndLen );

	boolValueIndeks = 0;
        satirSayisi = 0;

	// Verilerin sakland��� struct yap�s� (struct.h)
	ptrData = pData;
	YNEntry.TitleMid = N_DBFormattedReport ;
	YNEntry.Line1Mid = N_Print;
	//YNEntry.Line2Mid = CONTINUE_MSG;
	//YNEntry.Line3Mid = N_NullStr;
	//YNEntry.Line4Mid = N_NullStr;

	//ICCEntryStatus = YesNoEntry( &YNEntry );

	while ( (ptrBaslik - ptrBaslikBegining) < lenTableVarName )
	{
                // 20 sat�rda bir devam edip etmeyece�ini sor
                ++satirSayisi;
                if ( satirSayisi % 20 == 0)
                {
                    
                      while ( 1 )
                      {
                          ICCEntryStatus = YesNoEntry( &YNEntry );

                          if (  ICCEntryStatus == CLEAR_KY  ||
                                ICCEntryStatus == CANCEL_KY ||
								ICCEntryStatus == UP_KY)
										return 0;	// Gerikalan�n� yazd�rma
                          else if ( ICCEntryStatus == ENTER_KY ||
                                    ICCEntryStatus == DOWN_KY)
                                        break;		// Yazd�rmaya devam et
                      }

                }
                  

		//Verinin tipini ve uzunlu�unu al
		tip = ptrVarType[0];
		len = ptrVarType[1];
		ptrVarType +=2;
		
		// Alan Pointer yada ... ise yazd�rma
		if ( tip == PRINTTYPE_NOTPRINT)
			continue;

		// Etiketin ba��na git
		while ( *ptrBaslik != 0)
			++ptrBaslik;
		++ptrBaslik;

		//Ba�l��� yazd�r
		SDK_PrintString((char *) ptrBaslik);
		SDK_PrintString("\n");	
		
		// De�eri Yazd�r
		PrintDataBaseField( tip , ptrData , len );

		// E�er de�i�ken Boolean ise 8 kez ptrData de�i�kenini artt�rmaz
		if ( tip == PRINTTYPE_BOOL ) 
		{
			// Ayn� Boolean de�i�keninin i�inde
			++boolValueIndeks;

			//Yeni Bir Boolean ba�lad�
                        if ( boolValueIndeks >= 8 )
                        {
                              boolValueIndeks = 0;	
                              ++ptrData;				// Veri pointer�n� bir bayt �tele
                        }
                 }
                 else
                 {
                        // Boolean harici bir de�i�ken veya boolena de�i�keninin sonu
                        boolValueIndeks = 0;
                        ptrData += len;
                 }
                  
	}
    
        return 1;
   */
}


//------------------------------------------------------------------------
// @erd 01.03.2007
// printTerminalTable :		Terminal tablosunu formatl� bir bi�imde ka��da basar
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
// printAcquirerTable :		Acquirer tablosunu formatl� bir bi�imde ka��da basar
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
// printIssuerTable :		Issuer tablosunu formatl� bir bi�imde ka��da basar
// 
//------------------------------------------------------------------------
extern void printIssuerTable()
{
    /*
	UBYTE *pData;
	int i ;

	// B�t�n Issuer tablolar�n� ekrana bas
	for ( i = 0 ; i < ISMAX ; ++i)
	{
		// B�yle Bir Issuer tablosu var m�?
		if (ISTAB[i].ISID) 
		{
			SDK_PrintString("ISSUER TABLOSU\n");
			SDK_PrintString("--------------\n");

			pData = &ISTAB[i];	// S�radaki Issuer tablosunu al
			// Issuer tablosunu yazd�r
                        if ( printDBTable (  ISSUER_TABLE_VAR_NAMES,
                                  ISSUER_TABLE_VAR_TYPE_AND_LENGTH,
                                  pData,
                                  lenIssTableVarName) == 0 )
                                        return; // Yazd�rmaya Devam Etme
                        
		}else
			break;
	}
    */
}

//------------------------------------------------------------------------
// @erd 01.03.2007
// printCardRangeTable :		Card Range tablosunu formatl� bir bi�imde ka��da basar
// 
//------------------------------------------------------------------------
extern void printCardRangeTable()
{
    /*
	UBYTE *pData ;	
	int i ;
        
	// B�t�n card tablolar�n� ekrana bas
	for ( i = 0 ; i < CRMAX ; ++i)
	{
		// B�yle bir kart tablosu var m�?
          if  (CRTAB[i].CRNUM ) {
			SDK_PrintString("KART TABLOSU\n");
			SDK_PrintString("------------\n");

			pData = &CRTAB[i];	// S�radaki kart tablosunu al

			//Kart tablosunun yazd�r
			if ( printDBTable( CARD_TABLE_VAR_NAMES,
                                  CARD_TABLE_VAR_TYPE_AND_LENGTH,
                                  pData,
                                  lenCardTableVarName) == 0 )
                                      return ; //Yazd�rmaya devam etme
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
		i = 0 ; // GetOneKey fonksiyonunda do�ru tu�a bas�ld���n� kontrol etmek i�in bool olarak
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
          
		  //Herhangi bir�ey yazd�r�ld�ysa ka��d� kesmek i�in biraz bo�luk b�rak
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
