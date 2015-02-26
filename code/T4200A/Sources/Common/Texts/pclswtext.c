
#ifdef MAKE_SWEDISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             pclswtext.c
//      PCL Swedish Text Table.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "pcltextinit.h"
#include "pcltext.h"
#include "syserr.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//! Because the text field processing is limited to up to 255 records 
//! an extended table capability has been included. Now PCL is supporting 
//! 2 tables - "PclEstext1" referred as "TX_Default" and "PclEstext2" 
//! referred as "TX_Extended". 


//! FIRST Text table - TX_DEFAULT
static const char *const PclSwText1[] = {
    "sv",                                       // Swedish
    "HANDLARE #: ",                             // MERCHANT-  5
    "SRV : ",                                   // SERVER -   7
    "EXP : ",                                   // CASHIER -  7
    " TILL : ",                                 // TILL -     7
    "OGILTIG ",                                 // VOID_S -   9
    "MAT",                                      // FOOD -     10
    "KONTANT",                                  // CASH -     10
    " FÖRMÅN",                                  // BENEFIT -  10
    "OGILTIG",                                  // VOID   -   10
    "UTG. DATUM: ",                             // EXPIRE -   10 
    "OMGÅNG: ",                                 // BATCH -    11
    "FAKTURA: ",                                // INVOICE -  11
    "DATUM: ",                                  // DATE -     12
    "TID: ",                                    // TIME -     12
    "RRN: ",                                    // RRN: -     13 
    "IDENT NR: ",                               // AUTH NO -  13 
    "FCS ID: ",                                 // FCS -      13 
    "AVS: ",                                    // AVS -      13 
    "TILLG SALDO",                              // AVAIL_BAL -15,18,19
    "IDAG",                                     // TODAY -    15
    "BAS",                                      // BASE -     15
    "SKATT",                                    // TAX -      15
    "BAR",                                      // BAR -      15 
    "TILLÄGGSAVG",                              // SURCHARGE - 15
    "DRICKS",                                   // TIP -      15
    "TOTAL",                                    // TOTAL -    15
    "DRICKS GUIDE",                             // TIP GUIDE - 15
    "FÖRS BEL.",	                             // SALE_AMT - 15
    "PENGAR TILLB",                             // CASH_BACK -15
    "AVGIFT",                                   // FEE
    "SUMMA FÖRF:",                              // AMOUNT DUE
    "*** INGA ÅTERBETALNINGAR MÖJLIGA",         // NoRefundsAllow
    "* STATUS EJ ANSLUTEN *",                   // OfflineEntered
    "MAT SALDO =",                              // FoodBal
    "KONTO SALDO =",                            // Cash Bal
    "*** DUBBLERAD ***",                        // DUPLICATE
    "SIGNATUR BEHÖVS EJ",                       // SignatureNotReq
    " - AVS AVBRUTEN",                          // AvsCanceled
    "**** TRÄNINGS MODE ****",                  // TrainingMode
    "JAG GODKÄNNER ATT HANDLAREN KONVERTERAR",  // IAuthorizeElect
    "MIN CHECK TILL EN ELEKTRONISK TRANSAKTION",// FundFromMyAccount
    "ELLER PAPPERS KOPIA, OCH ATT DEBITERING",  // OrPaperDraft 
    "AV MITT KONTO SKER PÅ SUMMAN.",            // ForTheAmount
    "IFALL CHECKEN BLIR RETURNERAD",            // InTheEvent
    "OBETALD, FÖRSTÅR JAG ATT ",                // Unpaid
    ".",                                        // Point
    "RETURNERA",                                // Return
    "AVGIFT KAN EV DRAS FRÅN MITT KONTO",       // IEM_FEE
    "JAG GODKÄNNER TRANSAKTIONEN AV BELOPPET",  // Transfer_funds
    "FÖR SUMMAN AV DENNA TRANSAKTION",          // ForTAmount
    "JAG GODKÄNNER ATT BETALA SUMMAN",          // IAGREE
    "ENLIGT KORTUTFÄRDARENS AVTAL",             // ACCORDING 
    "HANDLARENS AVTAL VID RABATTKUPONG",        // MerchantAgreement
    "TRANSAKTIONEN MISSLYCKADES",               // TransactFailed
    "NORMAL FONT ",                             // NormalFont
    "ABCDEFGHIJ",                               // ABCDEFGHI
    "KLMNOPQRST",                               // KLMNOPQRST
    "UVWXYZÅÄÖ",                                // UVWXYZ
    "abcdefghi",                                // abcdefghi
    "klmnopqrst",                               // klmnopqrst
    "uvwxyzåäö0123456789",                      // vwxyz0123
    "!#$%$()*+-./://<=>?@",                     // SetSimbols1
    "[\\]^_'{|}",                               // SetSimbols2
    "NORMAL",                                   // Normal
    "DUBBEL",                                   // Double
    "FONT BYTE",                                // FontChange
    "SLUT",                                     // THE END
    "GRANSKA RAPPORT",                          // AuditReport
    "ALLA INDR",		                        // AllAcquirers  
    "TRANS FAKT#",                              // InvTrans      
    "SERVER ",                                  // Server        
    "EXPEDIT",                                  // Cashr         
    "IDEN",                                     // AUTH          
    "KORT NUMMER",                              // CardNumber
    "KORT TYP",                                 // CardType
    "VÄX TB",		                            // Cashbk        
    "DRICKS TOTAL",                             // TipTotal
    "KONTANT TOTAL",                            // CashTotal
    "TRANS TOTAL",                              // TransTotal
    "VÄXLA ",                                   // Shift
    "*",                                        // Star
    "ÖPPNA TABELL",                             // OpenTabs      
    "TRANS FAKT#",                              // InvTrans      
    "TRANSAKTION TOTAL",                        // TransTotals
    "AV UTFÄRDAREN",                            // By Issuer
    "EXPEDIT : ",                               // Cashier:
    " SERVER :  ",                              // PCLServer:
    "   TILL : ",                               // Till:
    "STOR TOTAL",	                            // GranTotals    
    "INGET ATT SKRIVA UT",                      // NothingToPrint
    "OJUSTERAD",                                // Unadj      
    "GRANSKA",                                  // Audit
    "TERMINAL",                                 // Terminal
    "DATABAS RAPPORT ",                         // DatabaseReport
    "TERMINAL EPROM KONFIG",                    // TermEpromConfig
    "TERMINAL KONFIGURATION",                   // termConfig
    "INLÖSARE ID :",                            // AcqId      
    "UTFÖRDAR ID : ",                           // IssuerID
    "ID NUMMERSERIE: ",                         // CardRangeId
    "BESKRIVNINGS ID",                          // Id_Descriptor
    "VISA 2 ID : ",                             // Visa2Id
    "TILLÄGGS PROMPT ID",                       // AddPromptId
    "RÅ DATA (HEX BYTES) :",                    // RawData
    "MOTTAGET DATA :",                          // ReceivedData
    "SEND DATA :",                              // SendData
    "VÄRD TOTAL : ",                            // HostTotals
    "DEBIT",                                    // Debit
    "INLÖSARE TABELL",                          // AcqTable   
    "HANDLARE #:",                              // Merchant#
    "TERMINAL ID. :",                           // TerminalID
    "FÖRSÄLJNING  ",                            // Sales
    "ÅTERBETALNINGAR",                          // Refunds
    "KONTANT BK",                               // CashBk
    "SUMMA  ",                                  // Amount
    "LOGG",                                     // LOG
    "PÅ",                                       // ON
    "RAPPORT",                                  // Report
    "BETALNINGS STATUS",                        // Settlement Status
    "NOTIFIKATION OM UPPLADDAT KVITTO",         // NotOfrRec
    "ANTAL UPLADDADE KVITTON: ",                // NumOfRec
    "MISSLYCKADES ATT LADDA KVITTO",            // RecUpload
    "1) BETALA MANUELLT",                       // PleasSettl
    "2) OM LADDNINGEN MISSLYCKAS IGEN, KONTAKTA",// UploadFaiL
    "HELP DESK.",                               // HelpDesk
    "PRESTANDA RAPPORT",                        // lblPerfRpt
    "Godkänna:  " ,                             // lblTotAprorv  
    "Avslag  :  " ,                             // lblTotDenials 
    "Manuella  :  " ,                           // lblTotManual  
    "Drag kort    :  " ,                        // lblTotSwipe    
    "Transaktioner  " ,                         // lblTransCount 
    "Konto        :  ",                         // TransCount    
    "Återuppringningar      :  " ,              // lblTotRedials 
    "Timeout      :  " ,                        // lblTottimeout 
    "Omsändningar  :  " ,                       // lblTotRetran  
    "Period: ",                                 // Period        
    "  Till  ",                                 // To            
    "KOMMUNIKATIONS STATUS",                    // Comm          
    "KONFIGURATIONS RAPPORT",                   // lblConfig_Rpt 
    "LJUSTERA",                                 // Adjust         
    "AV",                                       // OFF               
    "CARD HOLDER NAME",
    "STAN:",
    "SIGN:",
    "", //Kart i'tibari
    "", // Parsian
    " ", // Space
};

//! Second Text table - TX_EXTENDED
static const char *const PclSwText2[] = {
    "PERCENT #1",                               // Percent1 
    "PERCENT #2",                               // Percent2 
    "PERCENT #3",                               // Percent3 
    "ACQUIRER AVAILABLE",                       // lblAcqAvail        
    " CONFIGURATION BY ACQUIRER ",              // lblConfbyAcq
    "NII",                                      // NII
    "CARD ACCEPT TERMINAL",                     // CardAcceptTerm
    "CARD ACCEPT MERCHANT",                     // CardAcceptMerc
    "VISA I TERMINAL ID",                       // VisaITermId
    "NOT A SETTLEMENT ACQ.",                    // NotASettlement
    "IGNORE HOST TIME",                         // IgnoreHostTime
	"ISDN PROTOCOL",                            // ISDNprotocol
    "ERC COMPRESSION",                          // ErcCompression
    "CURRENT BATCH",                            // CurrentBatch
    "NEXT BATCH",                               // NextBatch
    "MAX.SETTLE. ATTEMPS",                      // MaxSettleAttemp
    "TRANSACTION HOST INFORMATION",             // lblTransHostInform
    "PRIMARY",                                  // Primary
    "PHONE NUMBER",                             // PhoneNum
    "CONNECT TIME",                             // ConnectTime
    "DIAL ATTEMPS",                             // DialAttemps
    "MODEM MODE",                               // ModemMode
    "SECONDARY",                                // Secondary
    "SETTLEMENT HOST INFORMATION",              // lblSettleHostInf
    "ISSUER TABLE",                             // lblIssuerTab - Issuer Table
    "ISSUER AVAILABLE",                         // lblIssAvail
    "CONFIGURATION BY ISSUER",                  // lblConfigByIss
    "CAPTURE TRANSACTION",                      // CaptureTrans
    "PRINT A RECEIPT",                          // PrintReceipt
    "CAPTURE RECEIPT",                          // CaptureReceipt
    "FLOOR LIMIT",                              // FloorLimit
    "REAUTH PERCENT",                           // ReauthPercent
    "ACH CHECK TRANS.",                         // AchCheckTrans
    "USE 6 DIGIT AUTH",                         // Use6Digit
    "PIN ENTRY REQUIRED",                       // PinEntryReq
    "ALLOW CASH BACK",                          // AllowCashBack
    "ALLOW CASH ONLY",                          // AllowCashOnly
    "PROCCESING OPTIONS",                       // lblProcOption
    "INVOICE NUMBER",                           // InvoiceNum
    "MANUAL PAN ENTRY",                         // ManualPan
    "MOD 10 CHECK ON PAN",                      // Mod10Check
    "IGNORE TRACK 1 ERROR",                     // IgnoreTrack
    "EXPIRY DATE REQUIRED",                     // ExpiryDate
    "CHECK EXPIRY DATE",                        // CheckExpiry
    "OFFLINE ALLOWED",                          // OfflineAllow
    "ADJUST ALLOWED",                           // AdjustAllow
    "VOICE REFERRAL",                           // VoiceReferral
    "PROMPT FOR TAX",                           // PromptForTax
    "AVS OPTIONS",                              // lblAvsOptions
    "AVS ENABLED ON SALE",                      // AvsEnab
    "PROMPT ADDRESS",                           // PromptAddress
    "PROMPT ZIP CODE",                          // PromptZipCode
    "BLOCKED TRANSACTIONS",                     // lblBlockTrans
    "CARD VERIFY",                              // CardVerif
    "BALANCE INQUIRE",                          // BalInq
    "PIN REQUIRED",                             // lblPinRequired
    "CARD RANGE TABLE",                         // lblCardRangTab
    "CARD RANGE AVAILABLE",                     // lblCardRangAvai
    "CONFIGURATION BY CARD RANGE",              // lblConfByCard
    "CARD RANGE",                               // CardRang
    "PAN RANGE LOW",                            // PanRangeLow
    "PAN RANGE HIGH",                           // PanRangeHigh
    "PAN LENGTH",                               // PanLength
    "ID",                                       // lblID
    "NAME ",                                    // lblName
    "CARD NUMBER",                              // lblCrnum
    "SUPPORTED IN",                             // SupportIn
    "I UNDERSTAND AND CONSENT THAT MY CHECK",   // IUndestand
    "MAY BE CONVERTED TO A BANK DRAFT TO",      // MayBeConverted   
    "DEBIT MY ACCOUNT IN THE EVENT THAT THE",   // DebitMy
    "ITEM IS RETURNED UNPAID FOR ANY REASON,",  // ItemIs
    "I WILL PAY A SERVICE CHARGE UP TO THE",    // IWillPay
    "MAXIMUM AMOUNT AS SET FORTH BY STATE",     // MaximumAmount
    "LAW WITH THE SERVICE CHARGE DEBITED",      // LawWith
    "FROM MY ACCOUNT EITHER BY AN ELECTRONIC",  // FromMy
    "FUNDS TRANSFER (EFT), OR IN THE",          // FundsTransfer
    "MANNER SET FORTH ABOVE",                   // MannerSer
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add English PCL messages table for DML support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
extern void PclTextSw_Init( void )
{
    short pgnum;

    // Verify the number of messages match
    TableItemCount = LAST_PCLTEXTDEF_ID;
    if( TableItemCount != ( sizeof( PclSwText1 ) / sizeof( char * ) ) )
    {
        strcpy( Dspbuf, "PclSwText1" );
        strcat( Dspbuf, MsgSizeMismatch );

        // Yes; notify the user of this error condition
        SystemErrMsg( );
    }

    // Verify the number of messages match
    TableItemCount = LAST_PCLTEXTEXT_ID;
    if( TableItemCount != ( sizeof( PclSwText2 ) / sizeof( char * ) ) )
    {
        strcpy( Dspbuf, "PclSwText2" );
        strcat( Dspbuf, MsgSizeMismatch );

        // Yes; notify the user of this error condition
        SystemErrMsg( );
    }

    // Get our page number
    pgnum = SDK_ThisPage(  );

    // Put PCL text table PclEnText into database
    TextIndexAddTable( pcltext_tableDef_id, LNG_SV, PclSwText1, pgnum );
    TextIndexAddTable( pcltext_tableExt_id, LNG_SV, PclSwText2, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_SWEDISH
