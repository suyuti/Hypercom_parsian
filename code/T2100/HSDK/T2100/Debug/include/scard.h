#ifndef SCARD_H
#define SCARD_H
/*                                                                           */
/*    *************************************************************          */
/*    *                    Hypercom Inc.                          *          */
/*    *                 (c) copyright 2001                        *          */
/*    *                                                           *          */
/*    *  product   :  Smart Card Library                          *          */
/*    *                                                           *          */
/*    *  component :  Smart card function prototypes              *          */  
/*    *                                                           *          */
/*    *  filename  :  scard.h                                     *          */
/*    *                                                           *          */
/*    *                                                           *          */
/*    *                                                           *          */
/*    *************************************************************          */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ARMCC_VERSION
#define EMV_FN(FN) FN
#else
#define EMV_FN(FN) EMV_##FN
#endif

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(A)
#endif

#ifndef BASICTYP  // make sure basictyp.h is not included
/// Define a basic Boolean type as used by smart card functions
typedef enum { False = 0, True } __attribute__((packed)) Bool;
#endif

//-------------------------------------------------------------------
// Smart Card definitions

/**
	@defgroup GroupSCR SmartCard Library Public Interface

	The purpose of this section is to describe the functions provided by
	this Smart Card API library.

	The general idea behind having @ref weak "weak aliases" in the library
	is to provide possibility for the application developer to reload
	functions called from EMV L2 Library (for example, for supporting
	external PIN PAD) and still be able to call the original functions.

	@{
*/

/// Smart card devices.
typedef enum
{
	scrd_MCD,	///< Memory card interface (not supported)
	scrd_SC1,	///< Merchant SAM \#1
	scrd_SC2,	///< Merchant SAM \#2
	scrd_SC3,	///< Merchant SAM \#3
	scrd_SC4,	///< Merchant SAM \#4
	scrd_SCR	///< Smart card interface
} SCRDDEVICE;

#ifndef EMVL1_OK
/// EMV level 1 exit status.
enum scard_EMVL1_Return {
	EMVL1_OK,		///< Function completed successfully
	EMVL1_FAIL		///< Failure detected
};
#endif

/// Smart card insertion status.
enum scard_Insertion_Status {
	SCRD_INS_NO,		///< No Card Inserted
	SCRD_INS_YES		///< Card Inserted
};

/// Smart card Answer-To-Reset (ATR) status.
enum scard_ATR_Status {
	ATR_BAD,		///< Invalid Answer to Reset
	ATR_NONE,		///< No Answer to Reset
	ATR_ERR,		///< Error processing Answer to Reset
	ATR_GOOD		///< Valid Answer to Reset
};

/// Smart card command status.
enum scard_Command_Status {
	ICCCMD_ERR,		///< Command Error
	ICCCMD_BAD,		///< Command okay with bad response code (non-0x90xx)
	ICCCMD_OK		///< Command okay with good response code (0x90xx)
};

/// Smart card protocol types.
enum scard_Protocol {
	PROT_T0,		///< T=0 Protocol
	PROT_T1			///< T=1 Protocol
};

/**
	@internal

	Define command codes (CMD) for SCR
*/
enum scard_CMD_SCR {
	SCR_CMD_DATA_TRANSFER = 0x39,
	SCR_CMD_RESET = 0x3C,
	SCR_CMD_RELOAD = 0x3F,
	SCR_CMD_LOAD_BK = 0x42,
	SCR_CMD_RESTORE = 0x45,
	SCR_CMD_POWER_ON = 0x60,
	SCR_CMD_POWER_OFF = 0x63,
	SCR_CMD_STATUS_REQUEST = 0x90,
	SCR_CMD_STATUS_SSM = 0x93,
	SCR_CMD_GET_SN = 0x96,
	SCR_CMD_STORE_KEY = 0x9F
};

/**
	@internal

	Define command codes (CMD) for SAM
*/
enum scard_CMD_SAM {
	SAM_CMD_POWER_ON = 0x60,
	SAM_CMD_DATA_TRANSFER = 0x39,
	SAM_CMD_POWER_OFF = 0x63
};

/**
	@internal

	Define card selection command
*/
enum scard_CMD_Select {
	SCR_CARDSELECT_CMD = 0x36
};

/**
	@internal

	Define some parameter values used in commands for SCR
	POWER ON with EMV mode and Vcc=5V
*/
enum scard_SCR_PowerOn_Parameter {
	SCR_CMD_POWER_ON_ISO = 0x00,
	SCR_CMD_POWER_ON_EMV = 0x01,
	SCR_CMD_RESET_PAR = 0xC3,
	SCR_CMD_RELOAD_PAR = 0xF3
};

/**
	@internal

	Define some parameter values used in commands for SAM
	POWER ON with ISO mode and Vcc=5V
*/
enum scard_SAM_PowerOn_Parameter {
	SAM_CMD_POWER_ON_ISO = 0x00,
	SAM_CMD_POWER_ON_EMV = 0x01
};

/**
	@internal

	Define result code included in response from SCR/SAM
*/
enum scard_Result {
	SCR_SAM_RESULT_BUSY = 0x00,
	SCR_SAM_RESULT_OK = 0x01,
	SCR_SAM_RESULT_LRC_ERROR = 0x04,
	SCR_SAM_RESULT_CMD_ILLEGAL = 0x06,
	SCR_SAM_RESULT_CMD_FORMAT = 0x07,
	SCR_SAM_RESULT_CMD_ERROR = 0x09,
	SCR_SAM_RESULT_SN_INVALID = 0x0B,
	SCR_SAM_RESULT_BK_ERROR = 0x0C,
	SCR_SAM_RESULT_MK_ERROR = 0x0D,
	SCR_SAM_RESULT_OVERFLOW = 0x0E,
	SCR_SAM_RESULT_ERROR = 0x0F,
	SCR_SAM_RESULT_KEY_ERROR = 0x14,
	SCR_SAM_RESULT_UNK = 0x90
};

/**
	@internal

	Define max length (in bytes) of a SCR/SAM command/response 
	(including CMD field or RES field) transferred by use of the
	SPB/MPB bus
*/
#define MAX_LEN_SPB_BUFFER 1024

// Define buffers temporarly used during send and receive
// When defining size of send buffer add 3 bytes for overhead bytes used
// in some versions
extern UBYTE scrd_buf_send[MAX_LEN_SPB_BUFFER+3];	//< @internal
extern UBYTE scrd_buf_recv[MAX_LEN_SPB_BUFFER];		//< @internal


/**
	@brief Initializes smart card SDK internal IO data relating
	       to EMV hardware and firmware.

	@return OSRETURNCODES

	@note This function must be called once during application startup on
	      the pages that use Smartcard IO functions. Be sure this routine
	      is called prior to any Smartcard IO functions.

	@see @ref ScrdInitIO
*/
extern OSRETURNCODES EMV_ScrdInitIO( void );

/**
	@brief Weak alias for @ref EMV_ScrdInitIO().
*/
extern OSRETURNCODES ScrdInitIO( void );

/**
	@example ScrdInitIO.c
	This is an example of how to use the ScrdInitIO() function.
*/

/**
	@brief Determines if a card is inserted in the given smart card device.

	This function will determine if there is a smart card inserted. If the
	smart card has been re-inserted since the original ScrdGetATR function
	it will return an error ERRDSC.  If this is called on a SAM port it
	will always return YES (regardless of whether there is a device in the
	socket or not).

	@param ScrdDevice Smart card device.
	@param pCardStatus Pointer to Card Insertion status return data
	                   (see @ref scard_Insertion_Status).
	@return OSRETURNCODES

	@note This function checks if another smart card device is already
	      open, and if so, closes it.

	@note This function will always return SCRD_INS_YES as the insertion
	      status when addressing a Merchant SAM (SC1, SC2, SC3, and SC4).

	@see @ref ScrdInserted
*/
extern OSRETURNCODES EMV_ScrdInserted( SCRDDEVICE ScrdDevice,
                                       UBYTE *pCardStatus );

/**
	@brief Weak alias for @ref EMV_ScrdInserted().
*/
extern OSRETURNCODES ScrdInserted( SCRDDEVICE ScrdDevice,
                                   UBYTE *pCardStatus );

/**
	@example ScrdInserted.c
	This is an example of how to use the ScrdInserted() function.
*/

/**
	@brief Eject the card in the given smart card device.

	This function deactivates the card and ends the session with the given
	smart card device.  If the hardware supports it, the card is also
	physically ejected from the card reader.

	@param ScrdDevice Smart card device.
	@return OSRETURNCODES

	@note The physical ejection of the card will be performed only on
	      hardware that supports it.  Also, the card is not physically
	      ejected from the Merchant SAMs (SC1, SC2, SC3, and SC4).

	@see @ref ScrdEject
*/
extern OSRETURNCODES EMV_ScrdEject( SCRDDEVICE ScrdDevice );

/**
	@brief Weak alias for @ref EMV_ScrdEject().
*/
extern OSRETURNCODES ScrdEject( SCRDDEVICE ScrdDevice );

/**
	@example ScrdEject.c
	This is an example of how to use the ScrdEject() function.
*/

/**
	@brief Close the session with the given smart card device.

	@param ScrdDevice Smart card device.
	@return OSRETURNCODES

	@note This function may be called prior to establishing a session with
	      a different smart card device. It may be necessary to call this
	      function prior to using the pinpad or communication devices.

	@see @ref ScrdEndSession
*/
extern OSRETURNCODES EMV_ScrdEndSession( SCRDDEVICE ScrdDevice );

/**
	@brief Weak alias for @ref EMV_ScrdEndSession().
*/
extern OSRETURNCODES ScrdEndSession( SCRDDEVICE ScrdDevice );

/**
	@example ScrdEndSession.c
	This is an example of how to use the ScrdEndSession() function.
*/

/**
	@brief Gets the Answer-To-Reset (ATR) bytes from the card
               in the given smart card device.

	This function will power on the card and return the Answer-To-Reset
	(ATR) bytes for the given smart card device. If the card is already
	powered on it will remove the power before applying power again to
	start a new session.

	The driver needs to recognize if the smart card has been removed
	during a transaction.  Therefore if the card is removed while the
	power is on, and re-applied the driver will use the OS flag SCRFLG to
	determine if this event occurs and return an OS error ERRDSC.

	@param ScrdDevice Smart card device.
	@param EMVFlag EMV compliance wanted.
	@param pATRBytes Pointer to ATR Bytes return value.
	@param pATRLen Pointer to length of ATR Bytes return value.
	@param pHistBytes Pointer to pointer to Historical Bytes within the
	                  ATR bytes.
	@param pHistLen	Pointer to length of Historical Bytes return value.
	@param pCardStatus Pointer to card ATR status return value (see @ref 
	                   scard_ATR_Status).
	@return OSRETURNCODES

	@note The buffer for the ATR Bytes must be large enough to handle the
	      maximum allowed, 33 bytes.

	@see @ref ScrdGetATR
*/
extern OSRETURNCODES EMV_ScrdGetATR( SCRDDEVICE ScrdDevice,
                                     Bool EMVFlag,
                                     UBYTE *pATRBytes, 
                                     UWORD *pATRLen, 
                                     UBYTE *pHistBytes, 
                                     UBYTE *pHistLen, 
                                     UBYTE *pCardStatus );

/**
	@brief Weak alias for @ref EMV_ScrdGetATR().
*/
extern OSRETURNCODES ScrdGetATR( SCRDDEVICE ScrdDevice,
                                 Bool EMVFlag,
                                 UBYTE *pATRBytes, 
                                 UWORD *pATRLen, 
                                 UBYTE *pHistBytes, 
                                 UBYTE *pHistLen, 
                                 UBYTE *pCardStatus );

/**
	@example ScrdGetATR.c
	This is an example of how to use the ScrdGetATR() function.
*/

/**
	@brief Gets the Protocol type of the card.

        Gets the Protocol type of the card in the given smart card device
	(SCR, SC1, SC2, SC3 and SC4). This function requires that an ATR must
	have been performed on the card previously.

	@param ScrdDevice Smart card device.
	@param pProtocol Pointer to protocol type (see @ref scard_Protocol).
	@return OSRETURNCODES

	@note A session must be established with the given smart card device
	      prior to the usage of this function.

	@note This function is not for the smart card device MCD or for
	      Synchronous cards.

	@see @ref ScrdGetProtocol
*/
extern OSRETURNCODES EMV_ScrdGetProtocol( SCRDDEVICE ScrdDevice, 
                                          UBYTE *pProtocol );

/**
	@brief Weak alias for @ref EMV_ScrdGetProtocol().
*/
extern OSRETURNCODES ScrdGetProtocol( SCRDDEVICE ScrdDevice, 
                                      UBYTE *pProtocol );

/**
	@example ScrdGetProtocol.c
	This is an example of how to use the ScrdGetProtocol() function.
*/

/**
	@brief Send a command to the card and get the response.

	This function sends a command to the card in the given smart card
	device and gets its response.

	@param ScrdDevice Smart card device.
	@param EMVFlag EMV compliance wanted 
	@param pCmdBuf Pointer to smart card command.
	@param CmdLen Length of smart card command.
	@param pRspBuf Pointer to smart card response.
	@param pRspLen Pointer to length of smart card response (including
	               status bytes).
	@param pSWBytes Pointer to status bytes (SW1/SW2).
	@param pCmdStatus Pointer to card command status return value
	                  (see @ref scard_Command_Status).
	@param pNAD Pointer to NAD, used for non-EMV resultant NAD returned in
	            this byte.
	@return OSRETURNCODES

	@note This function will establish a session with the given smart card
	      device if one is not already underway with it

	@see @ref ScrdSendCmd
*/
extern OSRETURNCODES EMV_ScrdSendCmd( SCRDDEVICE ScrdDevice,
                                      Bool EMVFlag,
                                      UBYTE *pCmdBuf, 
                                      UWORD CmdLen,
                                      UBYTE *pRspBuf,
                                      UWORD *pRspLen,
                                      UBYTE *pSWBytes, 
                                      UBYTE *pCmdStatus,
                                      UBYTE *pNAD );

/**
	@brief Weak alias for @ref EMV_ScrdSendCmd().
*/
extern OSRETURNCODES ScrdSendCmd( SCRDDEVICE ScrdDevice,
                                  Bool EMVFlag,
                                  UBYTE *pCmdBuf, 
                                  UWORD CmdLen,
                                  UBYTE *pRspBuf,
                                  UWORD *pRspLen,
                                  UBYTE *pSWBytes, 
                                  UBYTE *pCmdStatus,
                                  UBYTE *pNAD );

/**
	@example ScrdSendCmd.c
	This is an example of how to use the ScrdSendCmd() function.
*/

/**
	@brief Starts a session with the given smart card device.

	This function may be used to start a session with the given smart card
	device.

	@param ScrdDevice Smart card device.
	@return OSRETURNCODES

	@note This function checks if another smart card device is already
	      open, and if so, closes it.

	@see @ref ScrdStartSession
*/
extern OSRETURNCODES EMV_ScrdStartSession( SCRDDEVICE ScrdDevice );

/**
	@brief Weak alias for @ref EMV_ScrdStartSession().
*/
extern OSRETURNCODES ScrdStartSession( SCRDDEVICE ScrdDevice );

/**
	@example ScrdStartSession.c
	This is an example of how to use the ScrdStartSession() function.
*/

/**
	@brief Return the Smart Card API version string.

	This function returns the current version of the Smart Card API Library
	and the version number of the firmware.

	@param returnBuffer Pointer to the buffer to copy Smart Card Library
	                    version.
	@param pZ8VersionID Pointer to the byte to put Z8 version (always
	                    filled with 'x' in this implementation).

	@see @ref ScrdVersion
*/
extern void EMV_ScrdVersion( char *returnBuffer, UBYTE *pZ8VersionID ) ;

/**
	@brief Weak alias for @ref EMV_ScrdVersion().
*/
extern void ScrdVersion( char *returnBuffer, UBYTE *pZ8VersionID ) ;

/**
	@example ScrdVersion.c
	This is an example of how to use the ScrdVersion() function.
*/

/**
	@internal

	@brief Send a command to the card and get the response.

	@param ScrdDevice Smart card device.
	@param pCmdBuf Pointer to smart card command
	@param CmdLen Length of smart card command.
	@param pRspBuf Pointer to smart card response.
	@param pRspLen On input contains pointer to the size of smart card
	               response buffer; on output filled with the length of
	               smart card response.
	@return OSRETURNCODES

	@see @ref ScrdCommand, @ref EMV_ScrdSendCmd
*/
extern OSRETURNCODES EMV_ScrdCommand( SCRDDEVICE ScrdDevice,
                                      UBYTE* pCmdBuf,
                                      UWORD CmdLen,
                                      UBYTE* pRspBuf,
                                      UWORD* pRspLen );

/**
	@internal

	@brief Weak alias for @ref EMV_ScrdCommand().
*/
extern OSRETURNCODES ScrdCommand( SCRDDEVICE ScrdDevice,
                                  UBYTE* pCmdBuf,
                                  UWORD CmdLen,
                                  UBYTE* pRspBuf,
                                  UWORD* pRspLen );
                                                  
/**
	@internal

	@brief Power on the specified smart card device.

	@param ScrdDevice Smart card device.
	@param EMVFlag EMV compliance wanted.
	@param pRspLen On input contains pointer to the size of smart card
	               response buffer; on output filled with the length of
	               smart card response.
	@return OSRETURNCODES

	@note The POWER ON command is sent to the card reader.

	@see @ref ScrdPowerOn, @ref EMV_ScrdGetATR
*/
extern OSRETURNCODES EMV_ScrdPowerOn( SCRDDEVICE ScrdDevice,
                                      Bool EMVFlag,
                                      UWORD* pRspLen );

/**
	@internal

	@brief Weak alias for @ref EMV_ScrdPowerOn().

	@see @ref ScrdGetATR
*/
extern OSRETURNCODES ScrdPowerOn( SCRDDEVICE ScrdDevice,
                                  Bool EMVFlag,
                                  UWORD* pRspLen );

/**
	@internal

	@brief Power off the specified SCR/SAM device.

	@param ScrdDevice Smart card device.
	@return OSRETURNCODES

	@note The POWER OFF command is sent to the card reader.

	@see @ref ScrdPowerOff, @ref EMV_ScrdEject
*/
extern OSRETURNCODES EMV_ScrdPowerOff( SCRDDEVICE ScrdDevice );

/**
	@internal

	@brief Weak alias for @ref EMV_ScrdPowerOff().

	@see @ref ScrdEject
*/
extern OSRETURNCODES ScrdPowerOff( SCRDDEVICE ScrdDevice );

/**
	@internal

	@brief Restarts SC&K controller in the booter state.

	This function is called to restart SC&K controller in the booter state
	in order to reload the application.

	@return First byte of SC&K response or 0x05 on error.
*/
extern UWORD ScrdReload( void );

/**
	@internal

	@brief Retrun SC&K into application state.

	In order to get the normal functionality, return back the SC&K into
	application state.

	@return First byte of SC&K response or 0x05 on error.
*/
extern UWORD ScrdRestore( void );

/**
	@internal

	@brief Load the BK key in the SC&K.

	@param pBK Pointer to BK data.
	@return First byte of SC&K response or 0x05 on error.
*/
extern UWORD ScrdLoadBK( unsigned char* pBK );

/**
	@internal

	@brief Load the PK key in the SSM.

	@param pPK Pointer to PK data.
	@return First byte of SC&K response or 0x05 on error.
*/
extern UWORD ScrdStoreKey( unsigned char* pPK );

/**
	@internal

	@brief Reset the SC&K including the key buffer.

	@return First byte of SC&K response or 0x05 on error.
*/
extern UWORD ScrdReset( void );

/**
	@}
*/

/**
	@page weak Weak Aliases

	A weak alias is a definition that is in itself weak (i.e., it yields to
	other definitions), that holds the same value as another definition in
	the same translation unit.

	Consider you have the prebuild library which reference to function A().
	You also need to provide library with the A() function in the
	interface, but at the same time user should be able to overload this
	function A() with their own function.

	There are two visible solutions:
	@li Put the body of A() into function B() and invoke function B() from
	    function A(). Function A() should be located in the different file
	    from function B(). If the user redefined the function A() in it's
	    own application the linker will take function A() from his sources
	    and user still be able to call function B().
	@li The better solution is to put body of A() into function B() and
	    make A() a weak alias for B(). If the user not reloaded A(), the
	    labels A() and B() will point to the same code, but if user
	    reloaded A() the A() from the library will be ignored due to
	    weakness but the B() will still be accessible.

	For example, for Smart Card Library user can implement ScrdInserted()
	function as follows:

	@code
	OSRETURNCODES ScrdInserted( SCRDDEVICE ScrdDevice, UBYTE *pCardStatus )
	{
		if (useExternalPinPad)
		{
			return HFT_ScrdInserted( pCardStatus );
		}
		else
		{
			return EMV_ScrdInserted( ScrdDevice, pCardStatus );
		}
	}
	@endcode

	Where HFT_ScrdInserted() is the user level function implementing call
	to the external HFT PINPAD. The weak alias ScrdInserted() from the
	Smart Card Library will be ignored but at the same time the
	function EMV_ScrdInserted() is accessible without any problem.
*/

#ifdef __cplusplus
}
#endif

#endif
