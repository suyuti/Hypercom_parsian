/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  EMV Level 2 Library in C                        *        */
/*  *                                                               *        */
/*  *  COMPONENT :  Generic                                         *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv_l2.h                                        *        */
/*  *                                                               *        */
/*  *  HISTORY   :  This library has been designed to comply with;  *        */
/*  *               EMV311 May 31 1998                              *        */
/*  *               EMV4   Dec 31 2000 revised by bulletins 1 - 11  *        */
/*  *                                                               *        */                                                                     
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  28 OCT 01     Ian Smith     1st Draft                        *        */
/*  *  11 JAN 02  A.Shuttleworth  Moved CandTextType from emvdata.h *        */
/*  *  14 JAN 02  A.Shuttleworth  Added Candidate Text List pointer *        */
/*  *                             param to EMVL2_FindSupportedApps  *        */
/*  *                                                               *        */
/*  *****************************************************************        */

#ifndef _EMV_L2_H_
#define _EMV_L2_H_

#include "basictyp.h"
#include "scard.h"


// Tag Definitions
// Version     Document            Chapter     Page
// EMV 3.1.1 - Card Specification, Annex B,  B13 - B 15
// EMV4      - Book 3,             Annex A,  81 - 83

#define TAG_42    0x0042  // IIN (Issuer Identification Number)
#define TAG_4F    0x004F  // AID (Application Identifier) - ICC
#define TAG_50    0x0050  // Application Label
#define TAG_57    0x0057  // Track 2 Equivalent data
#define TAG_5A    0x005A  // Application PAN
#define TAG_61    0x0061  // Application Template                            (Not included in store)
#define TAG_6F    0x006F  // FCI (File Control Information) Template         (Not included in store)
#define TAG_70    0x0070  // AEF (Application Elementary File) Data Template (Not included in store)
#define TAG_71    0x0071  // Issuer Script Template 1                        (Not included in store)
#define TAG_72    0x0072  // Issuer Script Template 2                        (Not included in store)
#define TAG_73    0x0073  // Directory Discretionary Template                (Not included in store)
#define TAG_77    0x0077  // Response Message Template Format 2              (Not included in store)
#define TAG_80    0x0080  // Response Message Template Format 1              (Not included in store)
#define TAG_81    0x0081  // Amount Authorised (Binary)
#define TAG_82    0x0082  // AIP (Application Interchange Profile)
#define TAG_83    0x0083  // Command Template                                (Not included in store)
#define TAG_84    0x0084  // DF (Dedicated File) Name
#define TAG_86    0x0086  // Issuer Script Command
#define TAG_87    0x0087  // API (Application Priority Indicator)
#define TAG_88    0x0088  // SFI (Short File Identifier)
#define TAG_89    0x0089  // Authorisation Code
#define TAG_8A    0x008A  // Authorisation Response Code
#define TAG_8C    0x008C  // CDOL1 (Card Risk Management Data Object List 1)
#define TAG_8D    0x008D  // CDOL2 (Card Risk Management Data Object List 2)
#define TAG_8E    0x008E  // CVM (Cardholder Verification Method) List
#define TAG_8F    0x008F  // Certification Authority Public Key Index - ICC
#define TAG_90    0x0090  // Issuer Public Key Certificate
#define TAG_91    0x0091  // Issuer Authentication Data
#define TAG_92    0x0092  // Issuer Public Key Remainder
#define TAG_93    0x0093  // Signed Static Application Data
#define TAG_94    0x0094  // AFL (Application File Locator)
#define TAG_95    0x0095  // TVR (Terminal Verification Results)
#define TAG_97    0x0097  // TDOL
#define TAG_98    0x0098  // TC (Transaction Certificate) Hash Value
#define TAG_99    0x0099  // Transaction PIN (Personal Identification Number)
#define TAG_9A    0x009A  // Transaction Date
#define TAG_9B    0x009B  // TSI (Transaction Status Information)
#define TAG_9C    0x009C  // Transaction Type
#define TAG_9D    0x009D  // DDF (Directory Definition File) Name
#define TAG_A5    0x00A5  // FCI (File Control Information) Proprietary Template (Not included in store)
#define TAG_5F20  0x5F20  // Cardholder Name
#define TAG_5F24  0x5F24  // Application Expiration Data
#define TAG_5F25  0x5F25  // Application Effective Data
#define TAG_5F28  0x5F28  // Issuer Country Code (numeric format)
#define TAG_5F2A  0x5F2A  // Transaction currency code
#define TAG_5F2D  0x5F2D  // Language prefereence
#define TAG_5F30  0x5F30  // Service Code
#define TAG_5F34  0x5F34  // Application PAN Sequence Number
#define TAG_5F36  0x5F36  // Transaction Currency Cxponent
#define TAG_5F50  0x5F50  // Issuer URL (EMV4 onwards)
#define TAG_5F53  0x5F53  // IBAN (International Bank Account Number - ISO 13616)
#define TAG_5F54  0x5F54  // BIC (Bank Identifier Code - ISO 9362)
#define TAG_5F55  0x5F55  // Issuer Country Code (alpha2 format)
#define TAG_5F56  0x5F56  // Issuer Country Code (alpha3 format)
#define TAG_5F57  0x5F56  // Account Type (SU Bulletin 39)
#define TAG_9F01  0x9F01  // Acquirer Identifier
#define TAG_9F02  0x9F02  // Amount Authorised (Numeric)
#define TAG_9F03  0x9F03  // Amount Other (Numeric)
#define TAG_9F04  0x9F04  // Amount Other (Binary)
#define TAG_9F05  0x9F05  // Application Discretionary Data
#define TAG_9F06  0x9F06  // AID (Application Identifier) - Terminal
#define TAG_9F07  0x9F07  // AUC (Application Usage Control)
#define TAG_9F08  0x9F08  // Application Version Number - ICC
#define TAG_9F09  0x9F09  // Application Version Number - Terminal
#define TAG_9F0B  0x9F0B  // Cardholder name, extended
#define TAG_9F0D  0x9F0D  // Issuer Action Code - Default
#define TAG_9F0E  0x9F0E  // Issuer Action Code - Denial
#define TAG_9F0F  0x9F0F  // Issuer Action Code - OnLine
#define TAG_9F10  0x9F10  // Issuer Application Data (Previously BF10)
#define TAG_9F11  0x9F11  // Issuer Code Table Index
#define TAG_9F12  0x9F12  // Application Preferred Name
#define TAG_9F13  0x9F13  // LOATC (Last online application transaction count)
#define TAG_9F14  0x9F14  // LCOL (Lower consecutive offline limit)
#define TAG_9F15  0x9F15  // Merchant Category code
#define TAG_9F16  0x9F16  // Merchant Identifier
#define TAG_9F17  0x9F17  // PIN Try Count
#define TAG_9F18  0x9F18  // Issuer Script Identifier (Not included in store)
#define TAG_9F1A  0x9F1A  // Terminal Country Code
#define TAG_9F1B  0x9F1B  // Terminal Floor Limit
#define TAG_9F1C  0x9F1C  // Terminal Identification
#define TAG_9F1D  0x9F1D  // Terminal Risk Management Data
#define TAG_9F1E  0x9F1E  // IFD (Interface Device) Serial Number
#define TAG_9F1F  0x9F1F  // Track 1 Discretionary Data
#define TAG_9F20  0x9F20  // Track 2 Discretionary Data
#define TAG_9F21  0x9F21  // Transaction Time
#define TAG_9F22  0x9F22  // Certification Authority Public Key Index - Terminal
#define TAG_9F23  0x9F23  // UCOL (Upper Consecutive Offline Limit)
#define TAG_9F26  0x9F26  // Application Cryptogram
#define TAG_9F27  0x9F27  // CID (Cryptogram Information Data)
#define TAG_9F2D  0x9F2D  // ICC PIN Encypherment Public Key Certificate
#define TAG_9F2E  0x9F2E  // ICC PIN Encypherment Public Key Exponent
#define TAG_9F2F  0x9F2F  // ICC PIN Encypherment Public Key Remainder
#define TAG_9F32  0x9F32  // Issuer Public Key Exponent
#define TAG_9F33  0x9F33  // Terminal Capabilities
#define TAG_9F34  0x9F34  // CVM (Cardholder Verification Method) results
#define TAG_9F35  0x9F35  // Terminal Type
#define TAG_9F36  0x9F36  // ATC (Application Transaction Counter)
#define TAG_9F37  0x9F37  // Unpredictable Number
#define TAG_9F38  0x9F38  // PDOL (Processing Options Data Object List)
#define TAG_9F39  0x9F39  // POS (Point of Service) Data entry Mode 
#define TAG_9F3A  0x9F3A  // Amount Reference Currency
#define TAG_9F3B  0x9F3B  // Application Reference Currency
#define TAG_9F3C  0x9F3C  // Transaction Reference Currency
#define TAG_9F3D  0x9F3D  // Transaction Reference Currency Exponent
#define TAG_9F40  0x9F40  // Additional Terminal Capabilities
#define TAG_9F41  0x9F41  // Transaction Sequence Counter
#define TAG_9F42  0x9F42  // Application Currency Code
#define TAG_9F43  0x9F43  // Application Reference Currency Exponent
#define TAG_9F44  0x9F44  // Application Currency Exponent
#define TAG_9F45  0x9F45  // Data Authentication Code
#define TAG_9F46  0x9F46  // ICC Public Key Certificate
#define TAG_9F47  0x9F47  // ICC Public Key Exponent
#define TAG_9F48  0x9F48  // ICC Public Key Remainder
#define TAG_9F49  0x9F49  // DDOL (Dynamic Data Object List)
#define TAG_9F4A  0x9F4A  // Static Data Authentication Tag List
#define TAG_9F4B  0x9F4B  // Signed Dynamic Application Data
#define TAG_9F4C  0x9F4C  // ICC Dynamic Number
#define TAG_9F4D  0x9F4D  // Transaction log SFI/Length
#define TAG_9F4E  0x9F4E  // Merchant name and location
#define TAG_9F4F  0x9F4F  // Transaction log data format
#define TAG_BF0C  0xBF0C  // FCI Issuer discretionary data template
// -- Hypercom defined tags --------------------------------------------------------------------
#define TAG_DF50  0xDF50  // Default DDOL
#define TAG_DF51  0xDF51  // Default TDOL
#define TAG_DF52  0xDF52  // Issuer script processing results
#define TAG_DF53  0xDF53  // ICC "GetChallenge" response
#define TAG_DF54  0xDF54  // Decrypted Issuer or PIN Public Key
#define TAG_DF55  0xDF55  // Decrypted ICC Public Key
#define TAG_DF56  0xDF56  // Application capabilities bitmap


#ifdef _MAKE_AM186
#define APP_CAP   0xDF60   // AIM186 (HFT117) tag DF56 already used for other purposes.
#else
#define APP_CAP   TAG_DF56   
#endif


// Default tag values for data with no defined parent
// values allocated to allow unique mapping under hash function
// 'tag mod 12'
#define TAG_IC    0x00F2  // Identifies data originating from the ICC
#define TAG_TE    0x00F5  // Identifies data originating from the terminal



// Custom defined tags
#define TAG_9F4F  0x9F4F    // Lower limit
#define TAG_9F50  0x9F50 
#define TAG_9F51  0x9F51 
#define TAG_9F52  0x9F52 
#define TAG_9F53  0x9F53  
#define TAG_9F54  0x9F54   
#define TAG_9F55  0x9F55   
#define TAG_9F56  0x9F56  
#define TAG_9F57  0x9F57   
#define TAG_9F58  0x9F58   
#define TAG_9F59  0x9F59   
#define TAG_9F5A  0x9F5A   
#define TAG_9F5B  0x9F5B   
#define TAG_9F5C  0x9F5C   
#define TAG_9F5D  0x9F5D   
#define TAG_9F5E  0x9F5E   
#define TAG_9F5F  0x9F5F  
#define TAG_9F60  0x9F60  
#define TAG_9F61  0x9F61   
#define TAG_9F62  0x9F62  
#define TAG_9F63  0x9F63   
#define TAG_9F64  0x9F64
#define TAG_9F65  0x9F65   
#define TAG_9F66  0x9F66 
#define TAG_9F67  0x9F67 
#define TAG_9F68  0x9F68 
#define TAG_9F69  0x9F69 
#define TAG_9F6A  0x9F6A 
#define TAG_9F6B  0x9F6B
#define TAG_9F6C  0x9F6C 
#define TAG_9F6D  0x9F6D 
#define TAG_9F6E  0x9F6E 
#define TAG_9F6F  0x9F6F 
#define TAG_9F70  0x9F70
#define TAG_9F71  0x9F71 
#define TAG_9F72  0x9F72 
#define TAG_9F73  0x9F73 
#define TAG_9F74  0x9F74 
#define TAG_9F75  0x9F75 
#define TAG_9F76  0x9F76 
#define TAG_9F77  0x9F77 
#define TAG_9F78  0x9F78 
#define TAG_9F79  0x9F79 
#define TAG_9F7A  0x9F7A 
#define TAG_9F7B  0x9F7B 
#define TAG_9F7C  0x9F7C 
#define TAG_9F7D  0x9F7D 
#define TAG_9F7E  0x9F7E 
#define TAG_9F7F  0x9F7F
#define TAG_9F80  0x9F80  // Upper limit
 


// Library operating modes
#define EMVL2_v311     1  // EMV 3.1.1 (EMV 96 + errata)
#define EMVL2_v4       2  // EMV 4     (EMV 2000)

// Kernel configuration modes
#define EMVL2_DOPSE  0x01 // Perform PSE processing
#define EMVL2_RESEL  0x02 // Re-select parent when performing PSE processing
//                   0x04 // RFU
//                   0x08 // RFU
//                   0x10 // RFU
//                   0x20 // RFU
//                   0x40 // RFU
//                   0x80 // RFU

// Bitmap values for TermMaster control byte
#define EMVL2_EXACT  	0x01  // EMV311 Exact match required, EMV4 single match required
#define EMVL2_ISSFLOOR  0x02  // Use issuer floor limit
#define EMVL2_NO_FALLB  0x04  // No fallback during LOA if candidate list empty because this AID was blocked
#define EMVL2_AAR_ONL  	0x08  // Spare
#define EMVL2_E_ENTRY  	0x10  // Application supports easy entry
#define EMVL2_ADDTAGS  	0x20  // Application has proprietary tags
#define EMVL2_SKIPEMV  	0x40  // Non-EMV card, skip EMV processing
#define EMVL2_AUTOSEL   0x80  // Application supports auto selection



/* -- Transaction types -- */
#define EMVL2_GOODS_SERVICES  0x00 /* goods or services  */
#define EMVL2_CASH     		     0x01 /* cash     */
#define EMVL2_CASHBACK 		     0x09 /* cashback */
#define EMVL2_REFUND          0x20 // refund   */ txn type not defined by EMV but required to support chip refunds


// Level 1 exit status
#define EMVL1_OK             0 // (0 = normal exit)


// Exit Codes

#define EMVL2_OK                     0 // (0 = normal exit)
#define EMVL2_FAIL                   1 // Generic fail, reason unspecified
#define EMVL2_BAD_LEN                2 // Data length exceeds tag limits 
#define EMVL2_TOO_DEEP               3 // TLV nesting exceeds 4 levels
#define EMVL2_OUT_OF_DATA            4 // TLV parser, unexpected end of data
#define EMVL2_BIG_TAG                5 // TLV Tag coding exceeds 4 bytes
#define EMVL2_BIG_LEN                6 // TLV length coding exceeds 4 bytes
#define EMVL2_TOO_LONG               7 // TLV data length exceeds 255 bytes
#define EMVL2_NOT_FOUND              8 // Specified tag does not exist
#define EMVL2_DO_LOA                 9 // Perform list of AIDs processing
#define EMVL2_ICC_BLOCKED           10 // Card is blocked
#define EMVL2_NON_EMV               11 // Card does not support EMV "Select" command (mandatory)
#define EMVL2_NO_9F37               12 // DOL does not contain tag 9F37
#define EMVL2_DOUBLE_DATA           13 // Attempted overwrite of ICC data detected
#define EMVL2_MNDTY_DATA            14 // Mandatory data missing 
#define EMVL2_HIGH_CHAR             15 // ASCII character > 0x7F present
#define EMVL2_ADF_FOUND             16 // Buffer contains ADF
#define EMVL2_BAD_FORMAT            17 // Invalid data detected
#define EMVL2_FOUND                 18 // Data detected
#define EMVL2_BAD_RANGE             19 // Out of range data encountered
#define EMVL2_AID_INVALID           20 // Identifies AID on candidate list which is to be deleted.
#define EMVL2_NOT_SELECTED          21 // No application was selected.
#define EMVL2_EASY_ENTRY            22 // Application is Visa "Easy Entry" type
#define EMVL2_NO_MORE_RECS          23 // ICC has returned 6A83 (no more records)
#define EMVL2_NO_MORE_APPS          24 // Candidate list has been exhausted
#define EMVL2_MAG_STRIPE            25 // No supported apps, mag stripe to be performed
#define EMVL2_SELECTED              26 // Application selection successful
#define EMVL2_PSE                   27 // Payment services environment processing is being performed
#define EMVL2_LOA                   28 // List of AIDs processing is being performed
#define EMVL2_NO_KEYBD              29 // Keyboard not present but App needs manual confirmation
#define EMVL2_DDF_FOUND             30 // Buffer contains DDF
#define EMVL2_DF_BLOCKED            31 // Select DF response = 6283
#define EMVL2_NO_9F33               32 // DOL does not contain tag 9F33
#define EMVL2_NO_A5                 33 // Tag A5 missing (FCI template)
#define EMVL2_REVOKED               34 // Certificate revoked
#define EMVL2_BAD_SW12              35 // Bad status returned by card
#define EMVL2_PK_ABSENT             37 // Public key missing (or expired)
#define EMVL2_ATMEL_ERROR           38 // Atmel cryptoprocessor has returned SW1 SW2 != 90 00
#define EMVL2_ATMEL_FAIL            39 // Atmel cryptoprocesor comms failure
#define EMVL2_STORE_FULL            40 // Data store full
#define EMVL2_NOT_STORED            41 // Data not stored
#define EMVL2_AUTO_SELECT           42 // Library wishes to auto select an application.
#define EMVL2_ENCRYPT_FAIL          43 // PIN encryption has failed (key error ATMEL failure etc)
#define EMVL2_SCRIPT_FAIL           44 // Script command has returned SW1 != 90 || 62 || 63
#define EMVL2_CONSTRUCTED_TAG       45 // Constructed tag found in DOL list 
#define EMVL2_UNKNOWN_TAG           46 // Unknown tag found in DOL list 
#define EMVL2_TAG_SOURCE            47 // Non terminal source tag found in PDOL
#define EMVL2_CVM_OFE               49 // CVM process requires off-line encrypted PIN
#define EMVL2_CVM_DONE              50 // CVM process complete
#define EMVL2_CVM_SIG	            51 // CVM process requires signature 
#define EMVL2_CVM_OFP               52 // CVM process requires off-line plain text PIN
#define EMVL2_CVM_ONE	            53 // CVM process requires on-line encrypted PIN
#define EMVL2_CVM_PIN_INCORRECT     54 // CVM process found incorrect PIN
#define EMVL2_CVM_PIN_FAIL          55 // CVM process found failed PIN
#define EMVL2_CVM_PIN_FAIL_SIG      56 // CVM process found failed PIN, SIG required
#define EMVL2_CVM_PIN_BLOCKED       57 // CVM process found blocked PIN
#define EMVL2_CVM_PIN_BLOCKED_SIG   58 // CVM process found blocked PIN, SIG required
#define EMVL2_LOGIC_ERR             59 // ICC has returned cryptogram exceeding that requested
#define EMVL2_ONLINE_CANCELLED      60 // Host connection cancelled
#define EMVL2_CVM_CUST_COND         61 // CVM process needs app to validate custom condition
#define EMVL2_CVM_CUST_METHOD       62 // CVM process needs app to perform custom CVM method
#define EMVL2_NOT_SUPPORTED         63 // Command has returned 6A81 (function not supported) 

// Values starting from 128 (0x80) form a bitmap, do not reassign them !
// Bitmap used to communicate outcome of 1st and 2nd Generate AC
// bitmap = w000xynn

// nn = card decision using usual code
// 00 = AAC
// 01 = TC
// 10 = ARQC
// 11 = AAR

// y = ICC requests information advice message to be transmitted
// z = Informational advice required because script processing was performed.

#define	EMVL2_DECLINE	             128 // Transaction declined                      AAC                    
#define EMVL2_ACCEPT	              129 // Transaction accepted                      TC                    
#define	EMVL2_ONLINE	              130 // Transaction requests online authorisation ARQC  
#define EMVL2_REFERRAL	            131 // Transaction requests voice authorisation  AAR  
//   Remaining constants           132 // as above + advice required
//   not defined, assume           133               "
//   bitmap decoding will          134               "
//   be used                       135               "
//                                 136 // as above + script required              
//                                 137
//                                 138
//                                 139
//                                 140 // as above + advice + script required              
//                                 141
//                                 142
//                                 143
// more to come !!

#define BITMAP_RETVAL               0x80
#define ADVICE_REQUESTED            0x04
#define SCRIPT_PERFORMED            0x08

enum online_desicion 
{
   AUTHORISED_ONLINE, 
   DECLINED_ONLINE, 
   COMMS_FAIL, 
   COMMS_FAIL_FORCEDECLINE,
   AAR_ACCEPT,
   AAR_DECLINE
};


// cryptogram information data equates
// note: these values represent the 2 most significant bits (7 & 6)

#define	EMVCID_AAC 	0x00	// AAC  - DECLINE
#define	EMVCID_TC  	0x40	// TC   - APPROVE
#define	EMVCID_ARQC	0x80	// ARQC - ONLINE REQUEST
#define	EMVCID_AAR 	0xC0	// AAR  - REFERRAL


// *************************************
//        Values for PIN Status       
// *************************************
// formerly enum, now UBYTE
#define  EMVL2_PIN_OK           0  // PIN entry process completed successfully on external PED
#define  EMVL2_PIN_ENTERED      1  // Plain PIN block present in input buffer (see also EMVL2_EPIN_ENTERED)
#define  EMVL2_PINPAD_FAILURE   2  // Comms error to external PED              (TVR bit will be set)
#define  EMVL2_PIN_BLOCKED      3  // PIN blocked on external PED              (TVR bit will be set)
#define  EMVL2_PIN_BYPASSED     4  // PIN entry bypassed interal or local PED  (TVR bit will be set)
#define  EMVL2_PINICC_FAIL      5  // External PED reports fatal customer ICC error
#define  EMVL2_PINSIM_FAIL      6  // External PED reports SIM error
#define  EMVL2_EPIN_ENTERED     7  // Externally encrypted PIN block present in input buffer (see also EMVL2_PIN_ENTERED)
#define  EMVL2_PINICC_STOP      8  // External PED reports non fatal customer ICC error
#define  EMVL2_PIN_CANCELLED	9  // User cancel (EMVL2_FAIL and status bitmap bit set)

// Values 8-9 have been skipped to provide for any future need

// Values passed back into CVM after processing custom method
#define EMVL2_CUST_METHOD_FAIL        10
#define EMVL2_CUST_METHOD_SUCCESS     11
#define EMVL2_CUST_METHOD_UNKNOWN     12

// Values for ConditionMet
#define EMVL2_CONDITION_MET_TRUE      13
#define EMVL2_CONDITION_MET_FALSE     14
#define EMVL2_CONDITION_MET_CUSTOM    15

#define EMVL2_UNUSED_CVM_STATUS       255
    
// *************************************


typedef struct 
{
   UBYTE a_length;   // length of the aid 
   UBYTE aid[16];    // aid
   UBYTE control;    // bitmap of supported features
   UBYTE adt_id;     // ID of corresponding Additional Data Tag record; 
} SupportedAIDType;  // see EMV Terminal Spec, Annex A, page A-9 or ISO 8859


// Candidate list text structure
typedef struct 
{
   UBYTE orig_index; // Original table index of this AID 
   UBYTE text[17];   // null terminated text supplied by app, could be 9F12 or 50 or 84(converted to ascii) 
   UBYTE code_table; // character code table index (Value '1' - 'A') or 0 = not specified (ISO 8859)
   UBYTE iccode2[3]; // Issuer country code (alpha2 format)(first byte is length indicator, 0 = not present)
   UBYTE iccode3[4]; // Issuer country code (alpha3 format)(first byte is length indicator, 0 = not present)
   UBYTE iin[4];     // Issuer identification number       (first byte is length indicator, 0 = not present)
   UBYTE bic[12];    // Bank identifier code               (first byte is length indicator, 0 = not present)
   UBYTE iban[35];   // Internation bank account number    (first byte is length indicator, 0 = not present)
} CandTextType;      



// TermMaster additional tag record structure
#define EMVL2_ADT_MAXLEN 128   // Maximum length of ADT record
#define EMVL2_ADT_MAX     10   // Maximum number of TermMaster additional data tag records

// TermMaster additional tag record structure
typedef struct
{
	  UBYTE	ADTNUM;	                   // POSITION IN TABLE 
	  UBYTE	ADTID;   	                 // ID 
	  UBYTE	ADTTAGS[EMVL2_ADT_MAXLEN];	 // ADDITIONAL TAGS 
} ADTRecType;



#define EMVL2_DF_SIZE       10 


#define EMVL2_MAX_CBUF 800     // Max length of common_buffer
#define EMVL2_CBUF_REF 271     // Reference point for writing into common_buffer

// Global smart card buffers
#define MaxSC   260
#define SC_EMV  True



// Debug information begins ------
#define EMVL2_DEBUG_SIZE    14  // Size of debug buffer (in bytes)

// Identifiers for each debug byte
#define EMVL2_DEBUG_ICCSW1   0  // ICC SW1
#define EMVL2_DEBUG_ICCSW2   1  // ICC SW2
#define EMVL2_DEBUG_ATMELSW1 2  // Atmel SW1
#define EMVL2_DEBUG_ATMELSW2 3  // Atmel SW2
#define EMVL2_DEBUG_PROG     4  // Bitmap indicating transaction progress
#define EMVL2_DEBUG_CRYPT    5  // Status of cryptography process (DDA and Encrypted PIN)
#define EMVL2_DEBUG_CVM      6  // Status of CVM process
#define EMVL2_DEBUG_SCRIPT   7  // Status of script processing
#define EMVL2_DEBUG_GAC1REQ  8  // 1st GenerateAC cryptogram type requested.
#define EMVL2_DEBUG_GAC1RSP  9  // 1st GenerateAC cryptogram type received.
#define EMVL2_DEBUG_GAC2REQ 10  // 2nd GenerateAC cryptogram type requested.
#define EMVL2_DEBUG_GAC2RSP 11  // 2nd GenerateAC cryptogram type received.
#define EMVL2_DEBUG_AUX1    12  // Auxiliary debug data


// Bitmap values for transaction progress (PROG) indicator byte 
#define EMVL2_PROG_OT    0x01  // Complete online transaction has been attempted
#define EMVL2_PROG_AA    0x02  // Action analysis has been attempted
#define EMVL2_PROG_VT    0x04  // Validate transaction has been attempted
#define EMVL2_PROG_CV    0x08  // Cardholder verification has been attempted
#define EMVL2_PROG_DA    0x10  // Data authentication has been attempted
#define EMVL2_PROG_SA    0x20  // Final application selection has been attempted
#define EMVL2_PROG_LOA   0x40  // List of AIDs processing has been attempted
#define EMVL2_PROG_PSE   0x80  // PSE processing has been attempted 

// Values for cryptography (CRYPT) byte
// Bits 0 and 1 used to store authentication method as follows
#define EMVL2_CRYPT_NONE         0  // (00)  No method performed
#define EMVL2_CRYPT_SDA          1  // (01)  SDA attempted
#define EMVL2_CRYPT_DDA          2  // (10)  DDA attempted
#define EMVL2_CRYPT_GENAC_DDA    3  // (11)  Combined GenAC/DDA attempted

// Bits 2 - 7 are a bitmap defined as follows
#define EMVL2_CRYPT_DD2_REC   0x04  // (Cert 05) Signed dynamic data (DDA/GenAC2)
#define EMVL2_CRYPT_DD1_REC  	0x08  // (Cert 05) Signed dynamic data (DDA - DDA/GenAC1)
#define EMVL2_CRYPT_PIN_REC   0x10  // (Cert 04) PIN encryption public key recovered
#define EMVL2_CRYPT_ICC_REC   0x20  // (Cert 04) ICC public key recovered
#define EMVL2_CRYPT_SSD_REC   0x40  // (Cert 03) Signed static data recovered
#define EMVL2_CRYPT_ISS_REC   0x80  // (Cert 02) Issuer public key recovered

//Values for Cardholder Verification (CVM) byte
#define EMVL2_CVM_NONE        0x01
#define EMVL2_CVM_FAIL        0x02
#define EMVL2_CVM_SIGN        0x04
#define EMVL2_CVM_OFPP        0x08
#define EMVL2_CVM_OFEP        0x10
#define EMVL2_CVM_ONEP        0x20
#define EMVL2_CVM_SIGRQD      0x40  // Txn should request signature at end of txn                             
#define EMVL2_CVM_PINKEY      0x80  // PIN encryption key identified and successfully recovered

//Values for Script processing (SCRIPT) byte
#define EMVL2_SCRIPT_71SREC   0x01  // Type 71 script received
#define EMVL2_SCRIPT_72SREC   0x02  // Type 72 script received
#define EMVL2_SCRIPT_71PERR   0x04  // Type 71 script parsing error
#define EMVL2_SCRIPT_72PERR   0x08  // Type 72 script parsing error
#define EMVL2_SCRIPT_71SRUN   0x10  // Type 71 script run
#define EMVL2_SCRIPT_72SRUN   0x20  // Type 72 script run
#define EMVL2_SCRIPT_71SERR   0x40  // Type 71 script run error
#define EMVL2_SCRIPT_72SERR   0x80  // Type 72 script run error

// Values for Aux byte 1
#define EMVL2_CAPK_MISSING    0x01  // Cert auth public key not found
#define EMVL2_CAPK_EXPIRED    0x02  // Cert auth PK expired
#define EMVL2_CAPK_BADHASH    0x04  // Cert auth PK has incorrect hash value
#define EMVL2_IPKC_REVOKED    0x08  // Issuer public key certificate revoked
//                            0x10  // Not yet defined 
//                            0x20
//                            0x40
#define EMVL2_CERT_MISMATCH   0x80  // CDA encrypted and clear certificate types do not match 


// Values for aux byte 2
// Not yet defined

// debug definition ends ----


 
// EMV Level 2 Library - Public Interface -----------------------------------------------------

// Functions from emv_sa.c
extern UBYTE EMVL2_FindSupportedApps (SupportedAIDType *AIDTable, UBYTE *app_count, CandTextType** app_list);
extern UBYTE EMVL2_SelectApplication(UBYTE candidate, UBYTE *app_count);
extern void  EMVL2_DeleteCandidates  (UBYTE *app_count);

// Functions from emv_da.c
extern UBYTE EMVL2_AuthenticateData( void );

// Functions from emv_vt.c
extern UBYTE EMVL2_ValidateTransaction ( Bool  HotCard, Bool  MerchForced, UBYTE *PrevAmount,
								                           		   		UBYTE *VersionNumbers,
								                         		   		UBYTE TargetPrecentage,
								                         		   		UBYTE MaxTargetPrecentage,
								                         		        UBYTE *ThresholdValue );

// Functions from emv_cv.c
extern UBYTE EMVL2_CardholderVerification (UBYTE *EnteredPIN,  UBYTE* PINStatus);

// Functions from emv_aa.c
extern UBYTE EMVL2_ActionAnalysis (UWORD AppAction, UBYTE *TACDenial, UBYTE *TACOnline, UBYTE *TACDefault);
extern Bool  EMVL2_CheckTVROk (UBYTE *TACDenial);

// Functions from emv_ot.c
extern UBYTE EMVL2_CompleteOnlineTransaction (UWORD OnlineResult, UBYTE *TACDefault, UBYTE *ScriptData, UBYTE ScriptLength);

// Functions from emv_tu.c
extern Bool  EMVL2_Initialise (UBYTE emv_version, SCRDDEVICE CryptInfo);
extern void  EMVL2_GetVersionString (UBYTE* buffer);
extern Bool  EMVL2_IsTagPresent (UWORD tag);
extern Bool  EMVL2_ReadTagData  (UBYTE *data, UBYTE *length, UWORD tag);
extern UBYTE EMVL2_WriteTagData (UBYTE *data, UBYTE  length, UWORD parent, UWORD tag);
extern void  EMVL2_DeleteTagData (UWORD tag);
extern void  EMVL2_GetDebugData (UBYTE** buffer);
extern Bool  EMVL2_ReadPINKeyData(UBYTE *pBuffer, UBYTE *pLength);

// Functions from emv_lc.c
extern Bool EMVL2_GetConfig(UBYTE *Config, UBYTE Length);  


// Functions from emv_pk.c
extern UBYTE EMVL2_ValidateKeyStore( void );

#endif // _EMV_L2_H_
