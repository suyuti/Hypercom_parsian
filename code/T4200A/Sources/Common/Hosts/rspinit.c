//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspinit.c
//      Initialize Response Parsing Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "osclib.h"
#include "utllib.h"
#include "fixdata.h"
#include "emv_l2.h"
#include "emv2_pk.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"
#include "msg.h"
#include "icepak.h"
#include "delreprt.h"
#include "comm.h"
#include "icepak.h"
#include "emvtabs2.h"
#include "perfutil.h"
#include "hstmsg.h"
#include "tables.h"
#include "tabutil.h"
#include "wrrtc.h"
#include "hstresp.h"
#include "rspunp.h"
#include "bcdbinw.h"

#include "rspinit.h"

//=============================================================================
// External variables / declarations
//=============================================================================
#ifdef MAKE_SMARTINIT
extern UBYTE *p63Data;
//! Pointer into transmit buffer 
extern UBYTE *pSendBuf;
#endif // MAKE_SMARTINIT


#ifdef	MAKE_EMV
//! init EM4 page tables
extern void InitEMVTabs1( void );
//! init Additional tags table
extern void InitADTTab( void );
//! function for EM4 page downloads
extern void P60_LDIR_EMVTabs1( void );
//! Additional Tags Table move
extern void P60_LDIR_ADTTab( void );
//! public keys table pre
extern void CAPKTabPre( void );
//! public keys table post
extern void CAPKTabPost( void );
//! Key Revocation Table pre
extern void REVKTabPre( void );
//! Key Revocation Table post
extern void REVKTabPost( void );
//! Additional Tags Table pre
extern void ADTTabPre( void );
//! Additional Tags Table post
extern void ADTTabPost( void );
extern void KeyExpiryTabPre ( void );
extern void KeyExpiryTabPost ( void );
#endif	//MAKE_EMV

extern UBYTE ExTabID;

//=============================================================================
// Public data defines
//=============================================================================
#ifdef MAKE_SMARTINIT
//! CRTAB index to support 254 card ranges
UBYTE CRIndex;
#endif // MAKE_SMARTINIT

//=============================================================================
// Private defines and typedefs
//=============================================================================

// Initialization record subfield header structure -- see initialization doc

struct subfield_hdr
{
    //! Subfield table id
	UBYTE sfTableId;
    //! Subfield length
	UWORD sfLength;
} __attribute__((packed));

//! Initialize a Table record structure
struct table_rec
{
    //! Table Type Code
	UBYTE p60F_Type;
    //! Prepare a Buffer to load
	void ( *p60F_Pre ) ( void );
    //! Move a portion of the buffer
	void ( *p60F_Mov ) ( void );
    //! Set Checksum & other post_processing
	void ( *p60F_Post ) ( void );
#ifdef MAKE_SMARTINIT
    //! Smart Init MD5 hash pointer
    UBYTE *pHASH;
#endif // MAKE_SMARTINIT
};

struct image_hdr
{
	UWORD totlen;
	UWORD id;
	UWORD width;
	UWORD height;
	UBYTE comptype;
	UWORD datalen;
} __attribute__((packed));

struct MovFldRec
{
    //! Destination 
	UBYTE *pDest;
    //! Source      
	UBYTE *pSrc;
    //! Data length 
	UBYTE dlen;
};

#define MAXTABDEF ( sizeof( TableDef ) / sizeof( struct table_rec ) )

#define N_MOVTAB ( sizeof( MOVTAB ) / sizeof( struct MovFldRec ) )


//=============================================================================
// Private function declarations
//=============================================================================

static void RespInit( void );
static void PrivateFields( void );
static void ProcB60( void );

#ifdef MAKE_SMARTINIT
static void ProcB63( void );
#endif // MAKE_SMARTINIT

static void All_Clear( void );
static void Tconf_Pre( void );
static void P60_0810_1Post( void );
static void P60_0810_IdxPreL( UBYTE * pdest, UWORD szeRec, UBYTE numRec );
static void DsTab_Pre( void );
static void V2Tab_Pre( void );
static void New_SubField( void );
static void Move_Some( void );
static void P60_LDIR( void );
static void MoveFields( void );
static void ApTab_Pre( void );
static Bool ValRsp( void );

static void IbTab_Pre( void );
static void IcePak_Pre( void );
static void GphAdd_Pre( void );
static void GphAdd_LDIR( void );
static void GphAdd_Post( void );

static void FntAdd_Pre( void );
static void FontAdd_LDIR( void );
static void FontAdd_Post( void );

static void RptAdd_Pre( void );
static void RptAdd_LDIR( void );
static void RptAdd_Post( void );
static void FtTab_Pre( void );
static void BPre_LDIR( void );
static void BPre_LRC( void );

#ifdef MAKE_EXTRA_TABLES
static void PDNTab_Pre( void );
static void PDNTab_LDIR( void );
#endif // MAKE_EXTRA_TABLES

static void AqTab_Pre( void );
static void AqTab_LDIR( void );
static void CRTab_Pre( void );
static void CRTab_LDIR( void );
static void IsTab_Pre( void );
static void IsTab_LDIR( void );

#ifdef	MAKE_EMV
static void IccTab_Pre( void );
static void P60_0810_CPost( void );
static void TaidTab_Pre( void );
static void P60_0810_DPost( void );
static void CapkTab_Pre( void );
static void P60_0810_EPost( void );
static void RevTab_Pre( void );
static void P60_0810_FPost( void );
static void AtagTab_Pre( void );
static void P60_0810_10Post( void );
static void KyexTab_Pre( void );
static void P60_0810_13Post( void );
static void P60_LDIR_1( void );
static void P60_LDIR_2( void );
static void P60_LDIR_3( void );
#endif	//MAKE_EMV

static void IpcTab_Pre( void );

//=============================================================================
// Public data definitions
//=============================================================================

extern UBYTE *pRecvData;

//=============================================================================
// Private data definitions
//=============================================================================
//! Subfield pointer                   
static UBYTE *pSFRec;
//! Subfield header pointer            
static struct subfield_hdr *pSFHdr;
//! Temp save sub field pointer for rpt ld. 
static UBYTE *pSFDataSave;
//! Bytes left in field                
static UWORD B60FBytesLeft;
//! Bytes left in subfield             
static UWORD B60SBytesLeft;
//! Index in processing function table 
static UBYTE B60FIndex;
//! Adjusted size for first report block 
static UWORD MoveReportSize;
//! Index to icepac file               
static int IpIndex;

static FILE *iphandle;
static FILE *ReportFileHandle;
static FILE *FontFileHandle;

//! Entry # exceeds maximum entry #
static Bool ExceedMaxEntryNum;

// *INDENT-OFF*

//! Initialize a Table structure
static const struct table_rec TableDef[] = {
//! Clear All Tables              
{
  0, 
  All_Clear,	  
  NULL,       
  NULL,			
#ifdef MAKE_SMARTINIT
  NULL,			
#endif // MAKE_SMARTINIT
},	

//! Terminal Config               
{ 
  1, 
  Tconf_Pre,   
  P60_LDIR,   
  P60_0810_1Post,	
#ifdef MAKE_SMARTINIT
  TCONF_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Card Range Table
{ 
  2, 
  CRTab_Pre,   
  CRTab_LDIR, 
  SetCRTabLRC,
#ifdef MAKE_SMARTINIT
  CARD_HASH,			
#endif // MAKE_SMARTINIT
},  

//! Issuer Table
{ 
  3, 
  IsTab_Pre,   
  IsTab_LDIR, 
  SetISTabLRC,		
#ifdef MAKE_SMARTINIT
  ISS_HASH,			
#endif // MAKE_SMARTINIT
},  

//! Acquirer Table
{ 
  4, 
  AqTab_Pre,   
  AqTab_LDIR, 
  SetAQTabLRC,		
#ifdef MAKE_SMARTINIT
  ACQ_HASH,			
#endif // MAKE_SMARTINIT
},  

//! Descriptor Table              
{ 
  5, 
  DsTab_Pre,   
  P60_LDIR,   
  SetDSTabLRC,		
#ifdef MAKE_SMARTINIT
  DESC_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Visa 2 Table
{ 
  6, 
  V2Tab_Pre,   
  P60_LDIR,   
  SetV2TabLRC,      
#ifdef MAKE_SMARTINIT
  V2_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Downloaded PCL report script (file)  
{ 
  8, 
  RptAdd_Pre,  
  RptAdd_LDIR,
  RptAdd_Post,		
#ifdef MAKE_SMARTINIT
  RPT_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Add. Prompt Table              
{ 
  9, 
  ApTab_Pre,   
  P60_LDIR,   
  SetAPTabLRC,		
#ifdef MAKE_SMARTINIT
  ADPR_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Idle Screen Button Table  	
{
  11, 
  IbTab_Pre,   
  BPre_LDIR,  
  BPre_LRC,   		
#ifdef MAKE_SMARTINIT
  IDLB_HASH,			
#endif // MAKE_SMARTINIT
},

//! ICEPAK configuration table     
{
  10, 
  IcePak_Pre,  
  P60_LDIR,   
  SetIPTabLRC,		
#ifdef MAKE_SMARTINIT
  IPAK_HASH,			
#endif // MAKE_SMARTINIT
},	

#ifdef	MAKE_EMV

//! ICC Table
{
  12, 
  IccTab_Pre,  
  P60_LDIR_2, 
  P60_0810_CPost,	
#ifdef MAKE_SMARTINIT
  ICC_HASH,			
#endif // MAKE_SMARTINIT
},	

//! TAID Table
{
  13, 
  TaidTab_Pre, 
  P60_LDIR_2, 
  P60_0810_DPost,	
#ifdef MAKE_SMARTINIT
  TAID_HASH,			
#endif // MAKE_SMARTINIT
},	

//! CA Public Keydata Table
{
  14, 
  CapkTab_Pre, 
  P60_LDIR_1, 
  P60_0810_EPost,	
#ifdef MAKE_SMARTINIT
  CAPK_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Revocation Table
{
  15, 
  RevTab_Pre,  
  P60_LDIR_1, 
  P60_0810_FPost,	
#ifdef MAKE_SMARTINIT
  REVC_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Additional Tags Table
{
  16, 
  AtagTab_Pre, 
  P60_LDIR_3, 
  P60_0810_10Post,  
#ifdef MAKE_SMARTINIT
  ADTG_HASH,			
#endif // MAKE_SMARTINIT
},  

//! Key Expiry Dates Table
{
  19, 
  KyexTab_Pre, 
  P60_LDIR_1, 
  P60_0810_13Post,	
#ifdef MAKE_SMARTINIT
  KEXP_HASH,			
#endif // MAKE_SMARTINIT
}, 	

#endif	//MAKE_EMV

#ifdef MAKE_EXTRA_TABLES
//! PDN table
{
  18, 
  PDNTab_Pre,  
  PDNTab_LDIR,
  SetPDNTabLRC,    
#ifdef MAKE_SMARTINIT
  PDN_HASH,			
#endif // MAKE_SMARTINIT
},  
#endif // MAKE_EXTRA_TABLES

//! IP Connect Table              
{
  30, 
  IpcTab_Pre,  
  P60_LDIR,   
  SetIPCTabLRC,	
#ifdef MAKE_SMARTINIT
  IPC_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Download ICEPAK graphics files 
{
  56, 
  GphAdd_Pre,  
  GphAdd_LDIR, 
  GphAdd_Post,		
#ifdef MAKE_SMARTINIT
  GRPH_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Download printer font files 
{
  57, 
  FntAdd_Pre,  
  FontAdd_LDIR,
  FontAdd_Post,	
#ifdef MAKE_SMARTINIT
  PFNT_HASH,			
#endif // MAKE_SMARTINIT
},	

//! Footer configuration table     
{
  73, 
  FtTab_Pre,   
  P60_LDIR,   
  SetFTTabLRC,		
#ifdef MAKE_SMARTINIT
  FOOT_HASH,			
#endif // MAKE_SMARTINIT
},	

};

//! Table of Fields to Move 
static const struct MovFldRec MOVTAB[] = {
    //! Transaction Time  
	{ TRREQ.TRTIME,	TRRSP.TRTIME,	S_TRTIME},
    //! Transaction Date  
    { TRREQ.TRDATE,	TRRSP.TRDATE,	S_TRDATE},
    //{ TRREQ.TRJALALIDATE,	TRRSP.TRDATE,	S_TRDATE},
    //! Retrieval Ref Num 
	{ TRREQ.TRRRN,	TRRSP.TRRRN,	S_TRRRN},
    //! Auth ID Resp Code 
	{ TRREQ.TRAUTH,	TRRSP.TRAUTH,	S_TRAUTH},
    //! Response Code     
	{ TRREQ.TRRSPC,	TRRSP.TRRSPC,	S_TRRSPC},
    //! Additnl Resp Data 
	{ TRREQ.TRRSPA,	TRRSP.TRRSPA,	S_TRRSPA},
    //! Surcharge         
	{ TRREQ.TRSURC,	TRRSP.TRSURC,	S_TRBASE},
    //! Total with Fee    
	{ TRREQ.TRTOTF,	TRRSP.TRTOTF,	S_TRBASE}
};

// *INDENT-ON*


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Handles Response to Initialization
//!
//! \param
//!		[in]ReqCommStatus		status Req Communication
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspInit( OS_RETURN_VALS ReqCommStatus )
{
	OS_RETURN_VALS CommStatus;
	
	// Loop till Response Done 
	while ( True )
	{
		// Set Initial Responses 
		RespInit(  );

		// Check if Request Packet was sent okay 
		if ( ReqCommStatus == orvOK )
		{
			// Yes; get Response Message from COM 
			RecvMessage ();
			CommStatus = HostOSRetval;
		}
		else
		{
			// No; set Comm status to reflect the send status 
			CommStatus = ReqCommStatus;
		}

		// Check if Normal Data Was Received 
		if ( HostResp( CommStatus ) )
		{
			// Yes, normal data; set Receive Buffer Pointer to TPDU 
			pRecvData = RECVBUF;

			// Store the TPDU 
			memcpy( TRRSP.TRTPDU, pRecvData, S_TRTPDU );

			// Check if an NMS Message 
			if ( TRRSP.TRTPDU[0] == 0x68 )
			{
				// Yes; return Host Not Available Response 
				memcpy( TRRSP.TRRSPC, RSP_NA, S_TRRSPC );

				// Response is Done 
				break;
			}

			// Check if ISO Format Message 
			if ( TRRSP.TRTPDU[0] != 0x60 )
			{
				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

                // Not ISO format; get the next Message 
				continue;
			}

			// Point to the Response Message Type 
			pRecvData += S_TRTPDU;

			// Save Response Message Type 
			memcpy( TRRSP.TRMTYPE, pRecvData, S_TRMTYPE );

			// Check if Message Type is 0810 
			if ( ( TRRSP.TRMTYPE[0] != 0x08 ) ||
				 ( TRRSP.TRMTYPE[1] != 0x10 ) )
			{
				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

                // Not 0810 msg type; get the next message 
				continue;
			}

			// Point to the Bitmap 
			pRecvData += S_TRMTYPE;

			// Unpack the Message and check if completed okay 
			if ( RspUnPackMsg(  ) && ValRsp(  ) )
			{
				// Yes; Handle data of Fields 60-64 then get out of the loop 
				PrivateFields(  );
				break;
			}
			else
			{
				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

                // Unpack error. Get the next Message 
				continue;
			}
		}
		else
		{
			// Not normal data  
			// If download of icepac images fails then be sure  
			// to close the open file.              
			if ( iphandle )
			{
				// First close the open file.                   
				fclose( iphandle );

				// Then remove the file because it may be corrupted.    
				remove( IcePak[IpIndex].pFileName );
			}

			// Get out of the loop 
			break;
		}
	}

	// The Message is unbuilt 

	// Move fields from TRRSP to TRREQ 
	MoveFields(  );

	// Look up Response in TRREQ 
	RspLookUp( &TRREQ );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Stores data from Response to a table
//!
//! \param
//!		[in]pTabData			Pointer to table data start
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void StoreTabData( UBYTE *pTabData )
{
    // Get Length of the field to transfer 
	B60FBytesLeft = BcdBinW( pTabData );

	// Skip over field length (2 bytes) 
	pTabData += 2;

	// Set address of first subfield pointer 
	pSFRec = pTabData;
	pSFHdr = ( struct subfield_hdr * ) pTabData;
	pSFData = pSFRec + sizeof( struct subfield_hdr );

	do
	{
		// Check if continuation of same subfield 
		if ( B60SBytesLeft == 0 )
		{
			// No; extract data from subfield header 
			// and set up to process subfield 
			ExceedMaxEntryNum = False;

			New_SubField();

			if( ExceedMaxEntryNum )
			{
				if( 0xFF == ExTabID )
					ExTabID = pSFHdr->sfTableId;
			}
		}

		// Move some or all of subfield 
		Move_Some();

		// Check if finished with subfield yet 
		if ( B60SBytesLeft == 0 )
		{
			// Yes; verify that field is valid 
			if ( B60FIndex != MAXTABDEF )
			{
				// Yes; perform Post-Move Processing 
				if ( TableDef[B60FIndex].p60F_Post != NULL )
					TableDef[B60FIndex].p60F_Post();
			}
		}
	}
	while ( B60FBytesLeft );
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Sets Initial Values for Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RespInit( void )
{
	/* Approved Response */
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );

    /* Terminal ID */
    memcpy( TRRSP.TRTID, TRREQ.TRTID, S_TRTID );

    /* Trace Number */
    memcpy( TRRSP.TRSTAN, TRREQ.TRSTAN, S_TRSTAN );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Data for 60-63
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrivateFields( void )
{
	// Process Bit 60 Data in Response 
	ProcB60(  );

#ifdef MAKE_SMARTINIT
        if ( SmartInitEnabled )          
	// Process Bit 63 Data in Response 
	ProcB63(  );
#endif // MAKE_SMARTINIT
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Bit 60 Data
//!
//! \return
//!     None
//!
//! \note
//!     Process Bit 60 in the Response Message. For 0810 Responses, this is
//!     Initialization Data:<br>
//!     Type    0   Clear All Tables,<br>
//!             1   Terminal Configuration,<br>
//!             2 - Card Range Table Entry,<br>
//!             3 - Issuer Table Entry,<br>
//!             4 - Acquirer Table Entry,<br>
//!             5 - Descriptor Table Entry<br>
//!     <br>
//!     The message will look like:<br>
//!         0LLL    Total Field Length<br>
//!     <br>
//!     Then one or more of...<br>
//!     <br>
//!     N2      Type of Table (1 byte),<br>
//!     0LLL    Length of this Table (2 bytes),<br>
//!     ....    Data<br>
//!     <br>
//!     If the "length of this table" is greater than the "length of this
//!     field" then the field data will be continued in the next message
//!     immediately following the "total field length" portion of the message.
//!     These continuation messages are repeated until the table is filled,
//!     at which time any additional bytes in the message start a new field.
//-----------------------------------------------------------------------------
static void ProcB60( void )
{
	// Return if no data to process 
	if ( p60Data == NULL )
		return;

	// Make sure it is an initialization or icepak download response 
	if ( TRREQ.TRKEY != INITIALIZE && TRREQ.TRKEY != ICEPAK_DOWNLOAD )
	{
		// Not one; return 
		return;
	}

	// Check if first 0800/0810 message 
	if ( TRREQ.TRAGAIN == 0x00 )
	{
		// Yes; there is no subfield continuation 
		B60SBytesLeft = 0;

		// Check if Initialization is being done 
		if ( INITIALIZE == TRREQ.TRKEY )
		{
			// Yes; force an INIT if it fails from here 
			Clr_TermInit(  );
		}
	}
    StoreTabData( p60Data );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Clears all Initialization Tables
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void All_Clear( void )
{
#ifdef MAKE_SMARTINIT
    if ( SmartInitEnabled )
        // Clear main HASH
        memset ( MAIN_HASH, 0x00, SM_HASH_LEN );
    else
#endif // MAKE_SMARTINIT
    {
	// Terminal Configuration 
	InitTable( &TCFMT, ( UBYTE * ) & TCONF, TCSZE, &TCLRC );

	// Card Range Table
	InitTable( &CRFMT, (UBYTE *)CRTAB,  CRMAX*CRSZE, &CRLRC );  

	// Card Issuer Table
	InitTable( &ISFMT, (UBYTE *)ISTAB,  ISMAX*ISSZE, &ISLRC );  

	// Card Acquirer Table
	InitTable( &AQFMT, ( UBYTE * ) &AQTAB, AQMAX * AQSZE, &AQLRC );

	// Visa 2 Table
	InitTable( &V2FMT, ( UBYTE * ) &V2TAB, V2MAX * V2SZE, &V2LRC );

	// Descriptor Tables 
	InitTable( &DSFMT, ( UBYTE * ) DSTAB, DSMAX * DSSZE, &DSLRC );

	// ICE Idle Screen Button Table 
	if ( True == LOCTAB.IBTAB_INIT )
	{
		// ICE Idle Screen Button Table
		InitTable( &IBFMT, ( UBYTE * ) & IBTAB, IBSZE, &IBLRC );
	}

	// ICEPAC Configuration Table 
	InitTable( &IPFMT, ( UBYTE * ) & IPTAB, IPSZE, &IPLRC );

    // Clear IcePak images 
	IcePak_RemoveAll ();	

    // Delete downloaded reports 
	DeleteReports ();	

	// Additional Prompts Tables 
	InitTable( &APFMT, ( UBYTE * ) APTAB, APMAXENT * APSZE, &APLRC );

#ifdef MAKE_EXTRA_TABLES
	// PDN Table
	InitTable( &PDNFMT, (UBYTE *)PDNTAB,  PDNMAX*PDNSZE, &PDNLRC );  
#endif // MAKE_EXTRA_TABLES

	// Receipt Footer Table   
	InitTable( &FTFMT, ( UBYTE * ) & FTTAB, FTSZE, &FTLRC );

	// Performance Report Database
    InitRptTable ();

	// IP Connect Table   
	InitTable( &IPCFMT, ( UBYTE * ) & IPCTAB, IPCMAX * IPCSZE, &IPCLRC );

#ifdef	MAKE_EMV
    // Init CAPK, REVK and KeyExp tables
    InitEMVTabs1 ( );			

    // Init ICC table
    InitICCTab(  );			    

    // Init TAID table
    InitTAIDTab(  );			    

    // Init Additional tags table
    InitADTTab(  );			    
#endif	// MAKE_EMV
    }
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Terminal Configuration Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void Tconf_Pre( void )
{
	// Destination buffer 
	pB60DAddress = ( UBYTE * ) & TCONF;

	// How much to copy 
	B60DBytesLeft = TCSZE;

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &TCONF )
    {
        // Table received, clear old table
        InitTable( &TCFMT, ( UBYTE * ) & TCONF, TCSZE, &TCLRC );

        // Clear HASH
        memset ( TCONF_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT

}


//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Terminal Configuration Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_1Post( void )
{
	// Set Terminal Date/Time 
    if( bFileInit == False )
        Update_Sys_Time(  );

	// Initialize Terminal Invoice No. 
	InitTermInv(  );

	// Set TCONF's LRC 
	SetTCTabLRC(  );

	// Check for empty string 
	if ( ( 0xFF == TCONF.TCPWD[0] ) && ( 0xFF == TCONF.TCPWD[1] ) )
	{
		TCONF.TCPWD[0] = 0;
		TCONF.TCPWD[1] = 0;
	}

	// Save the new password 
	BfAscii( ( char * ) TERM.TERMPwd, TCONF.TCPWD,
			 sizeof( TERM.TERMPwd ) / 2 );

	// Recalcuate Terminal Table LRC 
	SetTERMTabLRC(  );

#ifdef MAKE_EMV
    // move termconf value into changeable global
    memcpy ( (void *) LocalAppCap, (void *) TCONF.TCAPPCAP, 3);
#endif
}


//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Indexed Table
//!
//! \param
//!		[out]pdest			Pointer to Array of Destination Buffers
//!		[in]szeRec			Size of one Dest Buffer in the array
//!		[in]numRec			Number of Dest Buffers in the array
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_IdxPreL( UBYTE * pdest, UWORD szeRec, UBYTE numRec )
{
	UBYTE index;

	// Get array index into table from subfield 
	index = CvtBin( *pSFData );

	if ( ( index == 0 ) )
	{
		// No valid buffer to load 
		B60DBytesLeft = 0;
	}
	else if( index > numRec )
	{
		// Array index is greater than the maximum table entry number
		ExceedMaxEntryNum = True;

		// No valid buffer to load 
		B60DBytesLeft = 0;
	}
	else
	{
		// Destination buffer 
		pB60DAddress = pdest + szeRec * ( index - 1 );

		// How much to copy 
		B60DBytesLeft = szeRec;
	}
}



//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Descriptor Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void DsTab_Pre( void )
{
	P60_0810_IdxPreL( ( UBYTE * ) DSTAB, DSSZE, DSMAX );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &DSTAB )
    {
        // Table received, clear old table
        InitTable( &DSFMT, ( UBYTE * ) DSTAB, DSMAX * DSSZE, &DSLRC );

        // Clear HASH
        memset ( DESC_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT

}


//-----------------------------------------------------------------------------
//! \brief
//!     Extracts data from subfield header and set up to process new subfield
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void New_SubField( void )
{
	UBYTE SFType;				// Subfield type 
	UBYTE i;					// Index 

	// Length of the subfield data 
	B60SBytesLeft = BcdBinW( ( UBYTE * ) & pSFHdr->sfLength );

	// Point to start of subfield data 
	pSFData = ( UBYTE * ) pSFHdr + sizeof( struct subfield_hdr );

	// Sub-field type 
	SFType = *( UBYTE * ) & pSFHdr->sfTableId;

	// Reset index in processing function table 
	B60FIndex = MAXTABDEF;

	// Look up the TableDef[] ... 
	// to find the entry for this subfield 
	for ( i = 0; i < MAXTABDEF; i++ )
	{
		if ( SFType == TableDef[i].p60F_Type )
		{
			// Set index in processing function table 
			B60FIndex = i;

			break;
		}
	}

	// Check if subfield type found in processing function table 
	if ( B60FIndex != MAXTABDEF )
	{
		// Yes; call Pre-Processing Routine 
		if ( TableDef[B60FIndex].p60F_Pre != NULL )
			TableDef[B60FIndex].p60F_Pre(  );
	}
	else
	{
		// No; so move nothing 
		B60DBytesLeft = 0;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves some data from the subfield in the message to the destination table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void Move_Some( void )
{
	UWORD MinField;				// Minimum bytes left between field and subfield 

	// Remove overhead field bytes from count (table number and size) 
	B60FBytesLeft -= 3;

	// Calculate minimum bytes left between sub-field and field 
	if ( B60SBytesLeft < B60FBytesLeft )
		MoveSize = B60SBytesLeft;
	else
		MoveSize = B60FBytesLeft;

	// Save the minimum 
	MinField = MoveSize;

	// Calculate number of bytes to move (minimum of sub-field, field,
	// and destination bytes left) 
	if ( B60DBytesLeft < MoveSize )
		MoveSize = B60DBytesLeft;

	// Check if something to move 
	if ( MoveSize != 0 )
	{
		// Yes; check if subfield type found in processing function 
		// table 
		if ( B60FIndex != MAXTABDEF )
		{
			// Yes; call Move Data Routine 
			if ( TableDef[B60FIndex].p60F_Mov != NULL )
				TableDef[B60FIndex].p60F_Mov(  );
		}
	}

	// Destination for next part 
	pB60DAddress += MinField;

	// Field bytes left to process 
	B60FBytesLeft -= MinField;

	// Sub-field bytes left to process 
	B60SBytesLeft -= MinField;

	// Update pointer to next sub-field 
	pSFRec += sizeof( struct subfield_hdr ) + MinField;
	pSFHdr = ( struct subfield_hdr * ) pSFRec;

	// Destination bytes left to process 
	B60DBytesLeft -= MoveSize;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to a Local Buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_LDIR( void )
{
	// Check if any data to move 
	if ( MoveSize != 0 )
	{
		// Yes; move the data 
		memcpy( pB60DAddress, pSFData, MoveSize );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves fields from TRRSP to TRREQ
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void MoveFields( void )
{
    static const struct MovFldRec * ptr;

    /* Move fields from TRRSP to TRREQ */
    for ( ptr = MOVTAB; ptr < &MOVTAB[N_MOVTAB]; ptr++ )
    {
        memcpy( ptr->pDest, ptr->pSrc, ptr->dlen );
    }
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Additional prompt Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ApTab_Pre( void )
{
	P60_0810_IdxPreL( ( UBYTE * ) APTAB, APSZE, APMAXENT );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &APTAB )
    {
        // Table received, clear old table
        InitTable( &APFMT, ( UBYTE * ) APTAB, APMAXENT * APSZE, &APLRC );

        // Clear HASH
        memset ( ADPR_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Visa2 Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void V2Tab_Pre( void )
{
	P60_0810_IdxPre( ( UBYTE * ) V2TAB, V2SZE, V2MAX );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &V2TAB )
    {
        // Table received, clear old table
        InitTable( &V2FMT, ( UBYTE * ) &V2TAB, V2MAX * V2SZE, &V2LRC );

        // Clear HASH
        memset ( V2_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Idle Screen Button Table Description line 2
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IbTab_Pre( void )
{
	if ( True == LOCTAB.IBTAB_INIT )
	{
		// Destination buffer 
		pB60DAddress = ( UBYTE * ) & IBTAB;

		// How much to copy 
		B60DBytesLeft = IBSZE;

#ifdef MAKE_SMARTINIT
        // Table received, clear old table
		InitTable( &IBFMT, ( UBYTE * ) & IBTAB, IBSZE, &IBLRC );

        // Clear HASH
        memset ( IDLB_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Processing for ICE Idle Screen Button Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void BPre_LDIR( void )
{
	if ( True == LOCTAB.IBTAB_INIT )
	{
		// Check if any data to move 
		if ( MoveSize != 0 )
		{
			// Yes; move the data 
			memcpy( pB60DAddress, pSFData, MoveSize );
		}

	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for ICE Idle Screen Button Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void BPre_LRC( void )
{
	if ( True == LOCTAB.IBTAB_INIT )
	{
		SetIBTabLRC(  );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for ICEPAK Configuration Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IcePak_Pre( void )
{
	// Destination buffer 
	pB60DAddress = ( UBYTE * ) & IPTAB;

	// How much to copy 
	B60DBytesLeft = IPSZE;

#ifdef MAKE_SMARTINIT
	// ICEPAC Configuration Table 
	InitTable( &IPFMT, ( UBYTE * ) & IPTAB, IPSZE, &IPLRC );

    // Clear IcePak images 
	IcePak_RemoveAll ();	

    // Clear HASH
    memset ( IPAK_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for ICEPAK Graphics Downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void GphAdd_Pre( void )
{

	struct image_hdr *pIh;

	pIh = ( struct image_hdr * ) pSFData;

	// search table for matching id 
	for ( IpIndex = 0; IpIndex < NUM_OF_IP_IMAGES; IpIndex++ )
	{
		if ( pIh->id == IcePak[IpIndex].id )
		{
			break;
		}
	}

	if ( IpIndex == NUM_OF_IP_IMAGES )	// if no match 
	{
		iphandle = 0;			// just ignore 
	}
	else
	{
		iphandle = fopen( IcePak[IpIndex].pFileName, "w" );	// create a new file 
	}

	// copy all the bytes 
	B60DBytesLeft = BcdBinW( ( UBYTE * ) & pSFHdr->sfLength );

#ifdef MAKE_SMARTINIT
    // Clear HASH
    memset ( GRPH_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Processing for ICEPAK Graphics Downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void GphAdd_LDIR( void )
{
	if ( iphandle )				// if file is open 
		fwrite( pSFData, 1, MoveSize, iphandle );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for ICEPAK Graphics Downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void GphAdd_Post( void )
{
	if ( iphandle )				// if file is open 
		fclose( iphandle );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Printer font downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void FntAdd_Pre( void )
{
	iphandle = fopen( "optimum.fnt", "wb" );	// create a new file 

	// copy all the bytes 
	B60DBytesLeft = (255 - 32) * 8;	// Maximum file size

#ifdef MAKE_SMARTINIT
    // Clear HASH
    memset ( PFNT_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Processing for Printer font downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void FontAdd_LDIR( void )
{
	if ( iphandle )	fwrite( pSFData, 1, MoveSize, iphandle );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Printer font downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void FontAdd_Post( void )
{
	if ( iphandle )	fclose( iphandle );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for report downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RptAdd_Pre( void )
{
	char filename[12];

#ifdef MAKE_SMARTINIT
	// Delete downloaded reports
    DeleteReports ();	

    // Clear HASH
    memset ( RPT_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT

	// First check the file system for a downloaded report with this number 
	memcpy( filename, "Report  rpt", sizeof( filename ) );

	// Save data address for second and suceeding calls.    
	pSFDataSave = pSFData;

	// Adjust number of bytes to be moved for first block, because  
	// first block contains a report id byte as first byte.         
	MoveReportSize = B60FBytesLeft;
	MoveReportSize -= 4;

	BufAsc( &filename[6], pSFData++ );
	ReportFileHandle = fopen( filename, "w" );

	// copy all the bytes except report number 
	B60DBytesLeft = BcdBinW( ( UBYTE * ) & pSFHdr->sfLength );

}


//-----------------------------------------------------------------------------
//! \brief
//!     Report downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RptAdd_LDIR( void )
{
	// On first block MoveReportSize will be set with adjusted size  
	// of data to account for the report number in the first byte.   
	if ( 0 == MoveReportSize )
		MoveReportSize = MoveSize;

	if ( ReportFileHandle )		// if file is open 
		fwrite( pSFData, 1, MoveReportSize, ReportFileHandle );

	// First time through account for report id byte, then      
	// on following blocks don't.  Report id is only on first.  
	pSFData = pSFDataSave;
	MoveReportSize = 0;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Report downloading
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RptAdd_Post( void )
{
	if ( ReportFileHandle )		// if file is open 
		fclose( ReportFileHandle );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Validates Response is for this Terminal
//!
//! \return
//!     True if response for this terminal otherwise False
//-----------------------------------------------------------------------------
static Bool ValRsp( void )
{
	// Make sure the following fields are the same in the Response as
	// in the Request 

	// Check Terminal ID 
	if ( memcmp( (const void*)TERM.TERMSer, TRRSP.TRTID, S_TRTID ) == 0 )
	{
		// The same; check if Trace Number exists 
		if ( !NullComp( ( char * ) TRRSP.TRSTAN, S_TRSTAN ) )
		{
			// Yes; check Trace Number 
			if ( memcmp( TRREQ.TRSTAN, TRRSP.TRSTAN, S_TRSTAN ) == 0 )
			{
				// The same; response for this Terminal 
				return ( True );
			}
		}
		else
		{
			// No; response for this Terminal 
			return ( True );
		}
	}

	// Response not for this Terminal 
	return ( False );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Footer Table Configuration Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void FtTab_Pre( void )
{
	// Destination buffer 
	pB60DAddress = ( UBYTE * ) & FTTAB;

	// How much to copy 
	B60DBytesLeft = FTSZE;

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &FTTAB )
    {
        // ICEPAC Configuration Table 
        InitTable( &FTFMT, ( UBYTE * ) & FTTAB, FTSZE, &FTLRC );

        // Clear HASH
        memset ( FOOT_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

#ifdef MAKE_EXTRA_TABLES
//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for PDN Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PDNTab_Pre( void )
{
	PDNTabPre ();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &PDNTAB )
    {
        // Table received, clear old table
        InitTable( &PDNFMT, ( UBYTE * ) PDNTAB, PDNSZE * PDNMAX, &PDNLRC );

        // Clear HASH
        memset ( PDN_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves PDN Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PDNTab_LDIR( void )
{
    /* Check if any data to move */
    if ( MoveSize != 0 )
	{
		P60_LDIR_PDNTab ();
	}
}
#endif // MAKE_EXTRA_TABLES

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Acquirer Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void AqTab_Pre( void )
{
	AqTabPre ();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &AQTAB )
    {
        // Yes, clear old table
        InitTable( &AQFMT, ( UBYTE * ) &AQTAB, AQMAX * AQSZE, &AQLRC );

        // Clear HASH
        memset ( ACQ_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT

}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves Acquirer Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void AqTab_LDIR( void )
{
    /* Check if any data to move */
    if ( MoveSize != 0 )
	{
		P60_LDIR_AqTab ();
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Card Range Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void CRTab_Pre( void )
{
#ifndef MAKE_SMARTINIT

    CRTabPre ();

#else // MAKE_SMARTINIT

    // Check for first part of table received
    if ( CRIndex == 1 )
    {
        // Table received, clear old table
        InitTable( &CRFMT, (UBYTE *)CRTAB,  CRMAX*CRSZE, &CRLRC );  

        // Clear HASH
        memset ( CARD_HASH, 0x00, SM_HASH_LEN );
    }

    // Find destination address and size to copy
    if ( CRIndex <= CRMAX )
    {
        pB60DAddress = ( UBYTE* ) &CRTAB[ CRIndex - 1 ];
    	B60DBytesLeft = CRSZE;                      
    }
    else
    {
    	B60DBytesLeft = 0;                      
    }

#endif // MAKE_SMARTINIT

}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves Card Range Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void CRTab_LDIR( void )
{
    /* Check if any data to move */
    if ( MoveSize != 0 )
	{
        // Copy single CRTAB entry
		P60_LDIR_CRTab ();

#ifdef MAKE_SMARTINIT
        // Ignore BCD enumeration from TM Request Manager,
        // apply internal BIN enumeration
        CRTAB[CRIndex-1].CRNUM = CRIndex;

        // Prepare index for next entry
        CRIndex++;
#endif // MAKE_SMARTINIT
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Issuer Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IsTab_Pre( void )
{
	IsTabPre ();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &ISTAB )
    {
        // Table received, clear old table
        InitTable( &ISFMT, (UBYTE *)ISTAB,  ISMAX*ISSZE, &ISLRC );  

        // Clear HASH
        memset ( ISS_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT

}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves Issuer Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IsTab_LDIR( void )
{
    /* Check if any data to move */
    if ( MoveSize != 0 )
	{
		P60_LDIR_IsTab ();
	}
}

#ifdef MAKE_EMV
//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to a Buffer on EMV page
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_LDIR_1( void )
{
	/* Check if any data to move */
	if ( MoveSize != 0 )
	{
		/* Yes; move the data */
		P60_LDIR_EMVTabs1(  );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to a Buffer on EMV page
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_LDIR_2( void )
{
	/* Check if any data to move */
	if ( MoveSize != 0 )
	{
		/* Yes; move the data */
		P60_LDIR_EMVTabs2 ();
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves ADT Table Data to a Buffer on EMV page
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_LDIR_3( void )
{
	/* Check if any data to move */
	if ( MoveSize != 0 )
	{
		/* Yes; move the data */
		P60_LDIR_ADTTab(  );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for ICC Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IccTab_Pre( void )
{
	ICCTabPre ();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &ICCTAB )
    {
        // Table received, clear old table
        InitTable( &ICCFMT, (UBYTE *)ICCTAB, ICCSZE*ICCMAX, &ICCLRC );  

        // Clear HASH
        memset ( ICC_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT

}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for ICC Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_CPost( void )
{
	ICCTabPost ();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for TAID Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void TaidTab_Pre( void )
{
	TAIDTabPre ();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &TAIDTAB )
    {
        // Table received, clear old table
        InitTable( &TAIDFMT, (UBYTE *)TAIDTAB, TAIDSZE*TAIDMAX, &TAIDLRC );  

        // Clear HASH
        memset ( TAID_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for TAID Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_DPost( void )
{
	TAIDTabPost ();
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for CAPK Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void CapkTab_Pre( void )
{
	CAPKTabPre(  );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &PublicKeyStore )
    {
        // Table received, clear old table
        InitTable( &PKFMT, (UBYTE *)&PublicKeyStore, KEYSZE*KEYMAX, &PKLRC );  

        // Clear HASH
        memset ( CAPK_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for CAPK Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_EPost( void )
{
	CAPKTabPost(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Revocation Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RevTab_Pre( void )
{
	REVKTabPre(  );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &RevocationTable )
    {
        // Table received, clear old table
        InitTable( &RKFMT, (UBYTE *)&RevocationTable, REVSZE, &RKLRC );  

        // Clear HASH
        memset ( REVC_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Revocation Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_FPost( void )
{
	REVKTabPost(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Additional Tags Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void AtagTab_Pre( void )
{
	ADTTabPre(  );

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &ADTData )
    {
        // Table received, clear old table
        InitTable( &ADTFMT, (UBYTE * ) &ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC );  

        // Clear HASH
        memset ( ADTG_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Additional Tags Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_10Post( void )
{
	ADTTabPost(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for Key Expiry Dates Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void KyexTab_Pre( void )
{
	KeyExpiryTabPre();

#ifdef MAKE_SMARTINIT
    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &KeyExpiryTable )
    {
        // Table received, clear old table
        InitTable( &KEFMT, (UBYTE *)&KeyExpiryTable,  KEYMAX*KEYEXPSZE, &KELRC );  

        // Clear HASH
        memset ( KEXP_HASH, 0x00, SM_HASH_LEN );
    }
#endif // MAKE_SMARTINIT
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-Processing for Key Expiry Dates Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void P60_0810_13Post( void )
{
	KeyExpiryTabPost();
}
#endif	//MAKE_EMV

//-----------------------------------------------------------------------------
//! \brief
//!     Updates the printer font
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void FontInit( void )
{
	UBYTE charbuf[9];

	if ( ShowDialStatus ) strcpy( CSTMSG, "FONT PROCESSING" );

	/* Try to open font file */
	if ( (FontFileHandle = fopen( "optimum.fnt", "rb" )) != NULL )
	{
		if ( (TCONF.TCOPT2 & TC2_PRINT) && (PRTSTAT & 0x01) )
		{
			if ( orvOK == SDK_PrinterOpen(TCONF.TCPRINTER) )
			{
				while ( sizeof(charbuf) == fread( charbuf, 1, sizeof(charbuf), FontFileHandle ) )
				{
					if ( ShowDialStatus )
					{
						strcpy( CSTMSG2, "CHAR: __" );
						BfAscii( (char*)&CSTMSG2[6], charbuf, 1 );
						ShowInfoMsg( CustomMsg2, N_Printer );
					}
					
					/* Update character */
					if ( orvOK != SDK_PrinterDefChar( charbuf[0], &charbuf[1] ) ) 
					{
						if ( ShowDialStatus )
						{
							strcpy( &CSTMSG2[8], " ERROR" );
							ShowInfoMsg( CustomMsg2, N_Printer );
							SDK_BeepIt( 3 );
							OS_Wait( ONESECOND*2 );
						}
						break;
					}
				}

				// Close printer on same page where it was opened.
				SDK_PrinterClose(  );
			}
		}
		
		/* Close file */
		fclose(FontFileHandle);

		/* Remove file */
		remove( "optimum.fnt" );
	}

	// Clear font update flag
	TCONF.TCOPT2 &= ~TC2_FONTUPD;
	// Set the Table's LRC
	SetTCTabLRC(  );

	if ( ShowDialStatus )
	{
		strcpy( CSTMSG2, "UPDATE ACCOMPLISHED" );
		ShowInfoMsg( CustomMsg2, N_Printer );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Pre-Processing for IP Connect Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IpcTab_Pre( void )
{
	P60_0810_IdxPreL( ( UBYTE * ) &IPCTAB, IPCSZE, IPCMAX );

    // Check for first part of table received
    if ( pB60DAddress == ( UBYTE * ) &IPCTAB )
    {
        // Table received, clear old table
        InitTable( &IPCFMT, ( UBYTE *) & IPCTAB, IPCMAX * IPCSZE, &IPCLRC );  

#ifdef MAKE_SMARTINIT
        // Clear HASH
        memset ( IPC_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }
}


#ifdef MAKE_SMARTINIT
//-----------------------------------------------------------------------------
//! \brief
//!     Builds field 63, table "I1"
//!
//! \return
//!     The number of hashes added
//!
//! \note
//!     pSendBuf as the global input: pointer to the buffer where build it.
//!     pSendBuf will be updated.
//-----------------------------------------------------------------------------
UBYTE Build_I1_hash ( void )
{
    UBYTE count = 0;
    int i;

    for (i = 0; i < MAXTABDEF; i++)
    {
        if ( NULL != TableDef[i].pHASH )
        {
            // Move table ID
            *pSendBuf = TableDef[i].p60F_Type;
            pSendBuf++;

            // Move its hash
            memcpy ( pSendBuf, TableDef[i].pHASH, SM_HASH_LEN );
            pSendBuf += SM_HASH_LEN;

            // Update length
            count++; 
        }
    }

    return count;
}
#endif // MAKE_SMARTINIT

#ifdef MAKE_SMARTINIT
//-----------------------------------------------------------------------------
//! \brief
//!     Processes Bit 63 Data
//!
//! \return
//!     None
//!
//! \note
//!     Process Bit 63 in the Response Message for 0810 Responses, this is
//!     Initialization Data.
//-----------------------------------------------------------------------------
static void ProcB63( void )
{
    UWORD B63FBytesLeft;
    UWORD IxBytesLeft;
    UBYTE Count, i, TabID;
	UBYTE *pResp;				// Pointer to Bit 60 data 
    int tblentry;

	// Return if no data to process 
	if ( p63Data == NULL )
		return;

	// Get the Bit 63 data pointer 
	pResp = p63Data;

	// Make sure it is an initialization or icepak download response 
	if ( TRREQ.TRKEY != INITIALIZE && TRREQ.TRKEY != ICEPAK_DOWNLOAD )
	{
		// Not one; return 
		return;
	}

	// Get Length of the field to transfer 
	B63FBytesLeft = BcdBinW( pResp );

    // Check the length
    if ( 0 == B63FBytesLeft )
    {
        // Nothing to parse; return
        return;
    }

	// Skip over field length (2 bytes) 
	pResp += 2;

    while ( True )
    {
        // Get the length of "Ix" tag
        IxBytesLeft = BcdBinW( pResp );
        pResp += 2;
        B63FBytesLeft -= 2;

        // Check if not zero
        if ( 0 == IxBytesLeft)
        {
            // Nothing to parse; return
            return;
        }

        // Get the name of tag. 
        // It must starts by "I" ( I1 or I2 tag expected)
        if ( *pResp != 'I' )
        {
            // Invalid tag name; return
            return;
        }
        pResp += 1;
        B63FBytesLeft -= 1;

        // Parse I1 or I2 tag
        switch ( *pResp )
        {
            // "I1" tag received
            case '1':  
                // Mark one byte parsed
                pResp += 1;
                B63FBytesLeft -= 1;

                // Get the number of hashes received
                Count = *pResp;

                // Validate Count
                if ( 0 == Count )
                {
                    return;
                }

                // Mark one byte parsed
                pResp += 1;
                B63FBytesLeft -= 1;

                // Check if count mismatch the data left
                if ( Count * (SM_HASH_LEN + 1) != B63FBytesLeft )
                {
                    return;
                }

  TabID = *pResp; // DWG

                // Loop for count
                for ( i = 0; i < Count; i++ )
                {
                    // Find Table ID
                    for ( tblentry = 0; tblentry < MAXTABDEF; tblentry++ )
                    {
                        // Found
                        if ( *pResp == TableDef[tblentry].p60F_Type )
                        {
                            // Is hash storage available
                            if ( NULL != TableDef[tblentry].pHASH )
                            {
                                // Yes, store it
                                memcpy ( TableDef[tblentry].pHASH, &pResp[1], SM_HASH_LEN );
                            }

                            // To next hash received
                            break;
                        }
                    }

                    // Mark bytes parsed
                    pResp += ( SM_HASH_LEN + 1 );
                    B63FBytesLeft -= ( SM_HASH_LEN + 1 );

                    // Check if done
                    if ( B63FBytesLeft < SM_HASH_LEN )
                    {
                        return;
                    }

                }


                break;

            // "I2" tag received
            case '2':
                // Mark one byte parsed
                pResp += 1;
                B63FBytesLeft -= 1;

                // The length of "I2" tag must be 18 bytes
                if ( SM_HASH_LEN + 2 != IxBytesLeft ) 
                {
                    return;
                }

                // Store All Table hash
//DWG                memcpy ( MAIN_HASH, pResp, SM_HASH_LEN );

                // Mark bytes parsed
                pResp += SM_HASH_LEN;
                B63FBytesLeft -= SM_HASH_LEN;

                break;

            // Wrong tag received
            default:  
                return;
        }

        // Check if there is any data to parse
        if ( B63FBytesLeft < SM_HASH_LEN )
        {
            // Nothing to parse; return
            return;
        }

    }
}
#endif // MAKE_SMARTINIT

