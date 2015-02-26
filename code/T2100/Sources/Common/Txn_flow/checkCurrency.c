// check currency

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

#include "checkCurrency.h"

// currency kontrolü sonucunda günsonu yapýlmasý 
// gerekli ise "False" ile geri döner. 
extern Bool CheckCurrency( void )				
{
	static UBYTE RtcBuff[RTCLEN];			// RTC Read Buffer
	static UBYTE TempDate[6];
	static UBYTE BatchRecordExists;
	static UBYTE CurrentBlockChanged;		//@brk1810
	UBYTE JrnIdentifier;
        UBYTE TempBuf[8];
        UBYTE Temp[2];
	UBYTE retval = True;


	CurrentBlockChanged = 0x00;
	BatchRecordExists = 0x00;
//@brk2608{		Batchde (reversal hariç) iþlem olup olmadýðý kontrol ediliyor.
	JournalLock();
	SetDefaultSelection();

	JSREC.JSSTATOFF = TS1_REV + TS1_SETL + TS1_VOID + TS1_ADV;
	JrnIdentifier = JournalSelFirst();
	JournalRelease();
	if (!(0x0000 == JrnIdentifier))
		BatchRecordExists = 1;
//@brk2608}
	
	SDK_RtcRead(RtcBuff);								// Tarih/Saat okunuyor.
	
	if ((!((TCAP.TCCURCD2[0] == 0x30) && (TCAP.TCCURCD2[1] == 0x30) && (TCAP.TCCURCD2[2] == 0x30))) && 
		(!((TCAP.TCCURCD2[0] == 0x00) && (TCAP.TCCURCD2[1] == 0x00) && (TCAP.TCCURCD2[2] == 0x00)))) {		// Parametre ile 1. kayýt var
		// Tarihinin geçerliliðini kontrol et.
		BfAscii((char *)TempDate, &TCAP.TCACTDT2[3], 1);
		BfAscii((char *)&TempDate[2], &TCAP.TCACTDT2[1], 1);
		BfAscii((char *)&TempDate[4], &TCAP.TCACTDT2[0], 1);
		if (memcmp(RtcBuff, TempDate, 6) >= 0) {		// Tarih ayný veya geçti.
                        TempBuf[0] = 0x30;
                        memcpy( &TempBuf[1], TCAP.TCCURCD2, 3);
			AscHex(Temp, TempBuf, 2);
			if ((memcmp(TCONF.TCCURRCODE, Temp, 2)) || 
				(memcmp(TCONF.TCLEADC, TCAP.TCCURSIGN2, 3))){	// Current ile aynýysa bypass
				CurrentBlockChanged = 1;
                                TempBuf[0] = 0x30;
                                memcpy( &TempBuf[1], TCAP.TCCURCD2, 3);
				AscHex(TCONF.TCCURRCODE, TempBuf, 2);
				memcpy(TCONF.TCLEADC, TCAP.TCCURSIGN2, 3);

                                TCLRC = CalcLrc((UBYTE *)&TCONF, TCSZE);
			}
			
                        if (BatchRecordExists && CurrentBlockChanged) 
                        {	
                            ShowInfoMsg(SettlementRequired, BlankMsg);
                            SDK_Wait(ONESECOND * 4);   
                            retval = False;
                            return retval;
			}	
                                
			memset(TCAP.TCCURCD2, 0x00, 3);	// Bu kayýt kullanýldý, temizle.
			memset(TCAP.TCCURSIGN2, 0x00,3);
			memset(TCAP.TCACTDT2, 0x00,4);
			memset(TCAP.TCCURCD1, 0x00,3);	// Ýkinci tarih geçtiðine göre ilki zaten geçmiþtir.
			memset(TCAP.TCCURSIGN1, 0x00,3);
			memset(TCAP.TCACTDT1, 0x00,4);			
			TCAPLRC = CalcLrc( (UBYTE *)&TCAP, TCAPSZE );
			
					
			return retval;
		}
	}
	if ((!((TCAP.TCCURCD1[0] == 0x30) && (TCAP.TCCURCD1[1] == 0x30) && (TCAP.TCCURCD1[2] == 0x30))) && 
		(!((TCAP.TCCURCD1[0] == 0x00) && (TCAP.TCCURCD1[1] == 0x00) && (TCAP.TCCURCD1[2] == 0x00)))) {		// Parametre ile 1. kayýt var
		// Tarihinin geçerliliðini kontrol et.
		BfAscii((char *)TempDate, &TCAP.TCACTDT1[3], 1);
		BfAscii((char *)&TempDate[2], &TCAP.TCACTDT1[1], 1);
		BfAscii((char *)&TempDate[4], &TCAP.TCACTDT1[0], 1);
		if (memcmp(RtcBuff, TempDate, 6) >= 0) {		// Tarih ayný veya geçti.
                        TempBuf[0] = 0x30;
                        memcpy( &TempBuf[1], TCAP.TCCURCD1, 3);
                        AscHex(Temp, TempBuf, 2);
			if ((memcmp(TCONF.TCCURRCODE, Temp, 2)) || 
				(memcmp(TCONF.TCLEADC, TCAP.TCCURSIGN1, 3))){	// Current ile aynýysa bypass
				CurrentBlockChanged = 1;
                                TempBuf[0] = 0x30;
                                memcpy( &TempBuf[1], TCAP.TCCURCD1, 3);
				AscHex(TCONF.TCCURRCODE, TempBuf, 2);
				memcpy(TCONF.TCLEADC, TCAP.TCCURSIGN1, 3);

                                TCLRC = CalcLrc((UBYTE *)&TCONF, TCSZE);
			}
			
                        if (BatchRecordExists && CurrentBlockChanged)
                         {	// Batchde iþlem varsa günsonuna zorla.
                            ShowInfoMsg(SettlementRequired, BlankMsg);
                            SDK_Wait(ONESECOND * 4);  
                            retval = False;
                            return retval;
			}	

			memset(TCAP.TCCURCD1, 0x00,3);	// Bu kayýt kullanýldý, temizle.
			memset(TCAP.TCCURSIGN1, 0x00,3);
			memset(TCAP.TCACTDT1, 0x00,4);
			
			TCAPLRC = CalcLrc( (UBYTE *)&TCAP, TCAPSZE );
			
						
			return retval;
		}
	}
      return retval;
}

