/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdkio.h
//!     \brief SDK I/O library defintions and function prototypes.<br>
//!     This file should be included in all 'C' source code files.
//!     \since 1996
//!

#ifndef SDKIO
#define SDKIO

//=============================================================================
// Module overview:
//	SDK I/O library defintions and function prototypes.
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

#include <osopts.inc>
#include <stdio.h>
#include <ddtm.h>
#if INCLPAD
#include <paddev.inc>
#endif
#if INCLTGI
#include <tgidev.inc>
#endif
#if INCLPIN
#include <pindev.inc>
#endif
#include <sdkdsp.h>

/**
	@addtogroup GroupSDK
	@{
*/

/**
	@brief Input/output status.
*/
typedef struct ioStatus
{
	OS_RETURN_VALS Error;	///< Error from the I/O function
	UBYTE CompleteFlag;	///< Completion indicator
} IOSTATUS;

#if INCLKBD

/// Length of keyboard buffer.
#define KBDLEN  1

/**
	@brief Enumeration for identifying different keyboard types.
*/
typedef enum
{
	kt_NORMAL,		///< Normal keyboard
#ifdef MAKE_T7PLUS
	kt_QWERTY,		///< QWERTY keyboard
	kt_PIN_STYLE		///< PIN Style keypad
#else
	kt_QWERTY		///< QWERTY keyboard
#endif
} KBDTYPE;

/**
	@brief Key mask structure.
*/
typedef struct KeyMask
{
	UBYTE  key1_7;		///< Row 1, Column 7
	UBYTE  key1_6;		///< Row 1, Column 6
	UBYTE  key1_5;		///< Row 1, Column 5
	UBYTE  key1_4;		///< Row 1, Column 4
	UBYTE  key1_3;		///< Row 1, Column 3
	UBYTE  key1_2;		///< Row 1, Column 2
	UBYTE  key1_1;		///< Row 1, Column 1
	UBYTE  key2_7;		///< Row 2, Column 7
	UBYTE  key2_6;		///< Row 2, Column 6
	UBYTE  key2_5;		///< Row 2, Column 5
	UBYTE  key2_4;		///< Row 2, Column 4
	UBYTE  key2_3;		///< Row 2, Column 3
	UBYTE  key2_2;		///< Row 2, Column 2
	UBYTE  key2_1;		///< Row 2, Column 1
	UBYTE  key3_7;		///< Row 3, Column 7
	UBYTE  key3_6;		///< Row 3, Column 6
	UBYTE  key3_5;		///< Row 3, Column 5
	UBYTE  key3_4;		///< Row 3, Column 4
	UBYTE  key3_3;		///< Row 3, Column 3
	UBYTE  key3_2;		///< Row 3, Column 2
	UBYTE  key3_1;		///< Row 3, Column 1
	UBYTE  key4_7;		///< Row 4, Column 7
	UBYTE  key4_6;		///< Row 4, Column 6
	UBYTE  key4_5;		///< Row 4, Column 5
	UBYTE  key4_4;		///< Row 4, Column 4
	UBYTE  key4_3;		///< Row 4, Column 3
	UBYTE  key4_2;		///< Row 4, Column 2
	UBYTE  key4_1;		///< Row 4, Column 1
	UBYTE  key5_7;		///< Row 5, Column 7
	UBYTE  key5_6;		///< Row 5, Column 6
	UBYTE  key5_5;		///< Row 5, Column 5
	UBYTE  key5_4;		///< Row 5, Column 4
	UBYTE  key5_3;		///< Row 5, Column 3
	UBYTE  key5_2;		///< Row 5, Column 2
	UBYTE  key5_1;		///< Row 5, Column 1
	UBYTE  key6_7;		///< Row 6, Column 7
	UBYTE  key6_6;		///< Row 6, Column 6
	UBYTE  key6_5;		///< Row 6, Column 5
	UBYTE  key6_4;		///< Row 6, Column 4
	UBYTE  key6_3;		///< Row 6, Column 3
	UBYTE  key6_2;		///< Row 6, Column 2
	UBYTE skey1_7;		///< Row 1, Column 7 Shifted
	UBYTE skey1_6;		///< Row 1, Column 6 Shifted
	UBYTE skey1_5;		///< Row 1, Column 5 Shifted
	UBYTE skey1_4;		///< Row 1, Column 4 Shifted
	UBYTE skey1_3;		///< Row 1, Column 3 Shifted
	UBYTE skey1_2;		///< Row 1, Column 2 Shifted
	UBYTE skey1_1;		///< Row 1, Column 1 Shifted
	UBYTE skey2_7;		///< Row 2, Column 7 Shifted
	UBYTE skey2_6;		///< Row 2, Column 6 Shifted
	UBYTE skey2_5;		///< Row 2, Column 5 Shifted
	UBYTE skey2_4;		///< Row 2, Column 4 Shifted
	UBYTE skey2_3;		///< Row 2, Column 3 Shifted
	UBYTE skey2_2;		///< Row 2, Column 2 Shifted
	UBYTE skey2_1;		///< Row 2, Column 1 Shifted
	UBYTE skey3_7;		///< Row 3, Column 7 Shifted
	UBYTE skey3_6;		///< Row 3, Column 6 Shifted
	UBYTE skey3_5;		///< Row 3, Column 5 Shifted
	UBYTE skey3_4;		///< Row 3, Column 4 Shifted
	UBYTE skey3_3;		///< Row 3, Column 3 Shifted
	UBYTE skey3_2;		///< Row 3, Column 2 Shifted
	UBYTE skey3_1;		///< Row 3, Column 1 Shifted
	UBYTE skey4_7;		///< Row 4, Column 7 Shifted
	UBYTE skey4_6;		///< Row 4, Column 6 Shifted
	UBYTE skey4_5;		///< Row 4, Column 5 Shifted
	UBYTE skey4_4;		///< Row 4, Column 4 Shifted
	UBYTE skey4_3;		///< Row 4, Column 3 Shifted
	UBYTE skey4_2;		///< Row 4, Column 2 Shifted
	UBYTE skey4_1;		///< Row 4, Column 1 Shifted
	UBYTE skey5_7;		///< Row 5, Column 7 Shifted
	UBYTE skey5_6;		///< Row 5, Column 6 Shifted
	UBYTE skey5_5;		///< Row 5, Column 5 Shifted
	UBYTE skey5_4;		///< Row 5, Column 4 Shifted
	UBYTE skey5_3;		///< Row 5, Column 3 Shifted
	UBYTE skey5_2;		///< Row 5, Column 2 Shifted
	UBYTE skey5_1;		///< Row 5, Column 1 Shifted
	UBYTE skey6_7;		///< Row 5, Column 7 Shifted
	UBYTE skey6_6;		///< Row 6, Column 6 Shifted
	UBYTE skey6_5;		///< Row 6, Column 5 Shifted
	UBYTE skey6_4;		///< Row 6, Column 4 Shifted
	UBYTE skey6_3;		///< Row 6, Column 3 Shifted
	UBYTE skey6_2;		///< Row 6, Column 2 Shifted
} KEYMASK;

#endif // INCLKBD

#if INCLLPT | INCLPRT

/**
	@brief Enumeration for identifying different printers.

	@note Values should match the defines in system software STRUCT.INC.
*/
typedef enum
{
	prt_P7SPROCKET212,	///< P7 Epson 212 (not supported)
	prt_P7SPROCKET215,	///< P7 Epson 215 (not supported)
	prt_P7ROLL,		///< P7 Epson 211 (not supported)
	prt_VERIFONE250,	///< Verifone 250 compatible (not supported)
	prt_T7P,		///< T7P Epson 192 (normal 40 character wide) (not supported)
	prt_T7P23,		///< T7P Epson 190 (special 23 character wide) (not supported)
	prt_P8ROLL,		///< P8 Star 212 (not supported)
	prt_P8SPROCKET,		///< P8 Star 221 (not supported)
	prt_P8ROLLGRAPHICS,	///< P8 Star 212 Graphics (not supported)
	prt_P8SPROCKETGRAPHICS,	///< P8 Star 221 Graphics (not supported)
	prt_T7PTHERMAL,		///< T7P MT101 Thermal (not supported)
	prt_T77THERMAL,		///< HYPERCOM MT301 T77 (not supported)
	prt_EPL1920ICE,		///< HYPERCOM EPL1920 ICE Thermal (not supported)
	prt_EPL205,		///< HYPERCOM EPL205 SmartICE Thermal (not supported)
	prt_FTP628,		///< FTP628 for ICE5500 plus, T7Plus and Optimum 2100
	prt_ADP500,					///< ADP500 for T77Plus
	prt_FTP638,					///< FTP638 3" ICE5x00Plus / T7Plus
	prt_U110,					///< T77ET/T6600 printer
	prt_LTPJ245,				// LPT_LTPJ245
	prt_NUMPRINTERS		///< Number of available printer types
} PRINTER;

/**
	@brief Printer status.
*/
typedef enum
{
	prt_PAPER_OK,		///< Paper is loaded, printer is not hot
	prt_PAPER_LOW,		///< Paper is low (not supported)
	prt_PAPER_OUT,		///< Paper is out
	prt_OVERHEAT		///< Overheat
} PRTSTATUS;

/**
	@brief Return values for image printing.
*/
typedef enum
{
	prti_OK,		///< No errors
	prti_PAPEROUT,		///< Paper out detected
	prti_OVERHEAT,		///< Overheat detected
	prti_PRINTER_ERROR,	///< Printing device error
	prti_UNSUPPORTED,	///< Function does no support image printing for selected printer
	prti_NOT_OPEN,		///< Printer is not open
	prti_INVALID_IMAGE,	///< Inavlid image data or file error
	prti_INTERNAL_ERROR_A	///< Internal error
} PRT_IMAGE_RETVAL;

/// Maximum size allowed for print data sent to the SDK
#define S_PRINTBUF 375

/**
	@brief Printer width font selection.

	@note Should be kept in sync with the same definitions in
	      MT101_printer.h in HW library.
*/
typedef enum {
#ifdef MAKE_VIKING
	prt_FONT_NORMAL = 0x16,	///< Normal font.
	prt_FONT_DOUBLE = 0x17	///< Double width font.
#else
	prt_FONT_NORMAL = 0x14,	///< Normal font.
	prt_FONT_DOUBLE = 0xe	///< Double width font.
#endif
} PRTFONT;

#endif // INCLLPT

#if INCLRTC

/// Length of RTC buffer
#define RTCLEN  14

/**
	@brief Enumeration for identifying different RTC configurations.
*/
typedef enum
{
	rt_24HRCLOCK,		///< Set RTC to 24 hour mode
	rt_12HRCLOCK		///< Set RTC to 12 hour mode
} RTCTYPE;

#endif // INCLRTC

#if INCLCOM

/**
	@brief Enumeration for identifying different COM line configurations.

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	cl_LEASEDLINE,		///< Leased Line
	cl_DIALUP,		///< Dial Up
	cl_NORMALLAN,		///< Low Speed Lan
	cl_HIGHSPEEDLAN,	///< High Speed Lan
	cl_ETHERNET,		///< Ethernet
	cl_DIRECT,		///< Direct access to the Com line device
	cl_NUMCOMLINES		///< Number of available Com Line types
} COMLINE;

/**
	@brief Enumeration for identifying different COM status requests.

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	ci_COMSTATUS,		///< Comms Module Status
	ci_MODEMTYPE,		///< C0 if 2400, 00 if 1200
	ci_HANDSETUP,		///< True if Up, False if Down
	ci_DIALNUMBER,		///< True if Primary, False if Secondary
	ci_EOTRECEIVED,		///< True if EOT received, else False
	ci_LINEBUSY,		///< True if Line Available, else False
	ci_LINESTATUS,		///< Line Status
	ci_SIGNALLEVEL,		///< Comms Signal Level
	ci_NUMCOMSTATUS		///< Number of available Com status requests
} COMINFO;

#ifdef MAKE_VIKING
/**
	@brief Enumeration for identifying different Com config requests

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	cc_ATCMD,		///< Set additional AT command
	cc_PADLOGON,		///< Set PAD logon script
	cc_SCCCONF,		///< Set SCC mode and timeout
	cc_PPPCONF,		///< PPP configuration
	cc_TCPIPDEST,		///< Set IP destination
	cc_LOCALIPCONF,		///< Set local IP configuration
	cc_POSLANDEST,		///< Set POS LAN destination
	cc_LOCALPOSLANCONF,	///< Set local POS LAN configuration
	cc_BTCONF,		///< Set BlueTooth config
	cc_SSLCONF,		///< Set SSL config
	cc_HEXLENGTH,		///< Set hex length
	cc_COUNTRYCODE		///< Set country code
} COMCONFIG;

/**
	@brief Configuration passing data.

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef union
{
	const void *Ptr;	///< Pointer type data.
	UWORD Val;		///< Value type data.
} COMCONFIGDATA;

/**
	@brief Modes available for packetizing by serial driver.

	@note Should be kept in sync with HypCSerialPort.hpp.
	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	COM_SCC_NORMAL=0,	///< Normal mode without framing.
	COM_SCC_APACS40,	///< APACS40 mode.
	COM_SCC_POSMINI,	///< POS MINI NAK mode.
	COM_SCC_POSTRANSPAD,	///< POS TransPAD mode.
	COM_SCC_V80,		///< V80 mode.
	COM_SCC_PPP,		///< PPP mode.
	COM_SCC_HYPVISA,	///< Hypercom Visa mode.
	COM_SCC_HEMS,		///< HEMS mode.
	COM_SCC_GS1,		///< GS1 mode.
	COM_SCC_HFT,		///< HFT mode.
	COM_SCC_SIBS,		///< SIBS mode.
	COM_SCC_V80_HALFDUPLEX	///< V80 mode with enforced packet dropping during transmission.
} COMSCCMODE;

/**
	@brief SCC mode setup structure

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	COMSCCMODE Mode;	///< Packeting mode.
	OS_TIMES Timeout;	///< Interpacket timeout.
} COMSCCCONF;

/**
	@brief PPP mode

	@note Should be kept in sync with hw.h.
	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum 
{
	PPP_DISABLE,		///< Disabled.
	PPP_CHAP_PAP,		///< CHAP or PAP, authentication optional.
	PPP_PAP,		///< PAP, authentication optional.
	PPP_CHAP,		///< CHAP, authentication optional.
	PPP_NONE,		///< Authentication disabled.
	PPP_AUTH_CHAP_PAP,	///< CHAP or PAP, authentication mandatory.
	PPP_AUTH_PAP,		///< PAP, authentication mandatory.
	PPP_AUTH_CHAP		///< CHAP, authentication mandatory.
} PPPMODE;

/**
	@brief PPP configuration data.

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	PPPMODE Mode;		///< PPP mode.
	UBYTE Login[16];	///< Login name.
	UBYTE Password[16];	///< Password.
	OS_TIMES ConnectionTimeout; ///< Connection timeout.
	OS_TIMES LCPTimeout;	///< LCP timeout.
} PPPCONF;

/**
	@brief TCP/IP destination data

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	UDWORD Address;		///< IP address.
	UWORD Port;		///< Destination port.
	OS_TIMES Timeout;	///< Connection timeout.
} TCPIPDEST;

/**
	@brief Local IP configuration table

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	UDWORD Address;		///< Address.
	UDWORD Mask;		///< Mask.
	UDWORD Gateway;		///< Gateway.
	UDWORD Parameters;	///< Additional IP parameters.
} LOCALIPCONF;

/**
	@brief Modes available for transmitting via POS LAN

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	COM_POS_PERMANENT=0,	///< Link permanent.
	COM_POS_ONDEMAND	///< On-demand connection.
} COMPOSMODE;

/**
	@brief  POS LAN destination data

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	UBYTE TPDU[5];		///< POS TPDU
	COMPOSMODE Mode;	///< POS LAN mode
	OS_TIMES Timeout;	///< Connection timeout
} POSLANDEST;

/**
	@brief BlueTooth configuration data

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	const char *bt_addr;	///< BlueTooth address.
	const char *bt_uuid;	///< BlueTooth UUID.
	UBYTE bt_encr;		///< BlueTooth encryption required flag.
	UBYTE bt_auth;		///< BlueTooth authentication required flag.
} BTCONF;

/**
	@brief SSL configuration data

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	UBYTE bEnable;		///< SSL enable flag.
	int (*pCheckCertFunc)(void*);	///< SSL check certificate callback.
} SSLCONF;

/**
	@brief Baudrates available for POS LAN.

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef enum
{
	COM_POS_4800=0,		///< Low speed POS LAN.
	COM_POS_19200		///< High speed POS LAN.
} COMPOSBAUDRATE;

/**
	@brief Local POS LAN configuration table

	@note Not applicable when communication is done via HPOSFC directly.
*/
typedef struct
{
	UBYTE Address;		///< CU Address.
	COMPOSBAUDRATE BaudRate; ///< Baudrate.
	OS_TIMES Timeout;	///< SNRM timeout.
} LOCALPOSLANCONF;
#endif // MAKE_VIKING

#endif // INCLCOM

#if INCLPIN

/**
	@brief Enumeration used to specify PINPAD mode.

	@note Currently applied to CAT33 only.
*/
typedef enum
{
	pm_Terminal,		///< Terminal mode.
	pm_PinPad		///< PINPAD mode.
} PINMODE;

/**
	@brief Protocol options.
*/
typedef enum
{
	PIN_NO_ACK_NAK = (1 << PINO_NOACKNAK),	///< No ACK / NAK option flag, pinopt parameter of SDK_PinOpen.
	PIN_NO_EOT = (1 << PINO_NOEOT)	///< No EOT option flag, pinopt parameter of SDK_PinOpen.
} PINOPT;

/**
	@brief Enumeration used to select protocol.

	@note Some of the protocols below may not be available - dependent
	upon O/S configuration of terminal.
*/
typedef enum
{
	pp_PROTOFF,		///< All PRT protocols are InActive (SIO is Off).
	pp_CAT33,		///< Omron CAT 33 pinpad protocol.
	pp_DUKPT,		///< Dukpt pinpad protocol.
	pp_UOBCIS,		///< UOB CIS pinpad protocol.
	pp_POSMINI,		///< POS / MiniNak protocol.
	pp_NCR5991,		///< NCR 5991 signature capture device protocol.
	pp_CHKRD,		///< Check reader protocol.
	pp_TRANS,		/**< Transparent data protocol (no message
				     framing, no control characters). */
	pp_KEYB,		///< External keyboard protocol.
	pp_GS1,			///< GS1 protocol.
	pp_HFT,			///< HFT protocol.
	pp_IECI,		///< IECI protocol.
	pp_SIBS			///< SIBS key GUN protocol.
} PINPROTOCOL;

/**
	@brief Enumeration used to specify serial data formats.
*/
typedef enum
{
	pc_7N1,			///< 7 data bits, no parity, 1 stop bit.
	pc_7O1,			///< 7 data bits, odd parity, 1 stop bit.
	pc_7E1,			///< 7 data bits, even parity, 1 stop bit.
	pc_7N2,			///< 7 data bits, no parity, 2 stop bits.
	pc_7O2,			///< 7 data bits, odd parity, 2 stop bits.
	pc_7E2,			///< 7 data bits, even parity, 2 stop bits.
	pc_8N1,			///< 8 data bits, no parity, 1 stop bit.
	pc_8O1,			///< 8 data bits, odd parity, 1 stop bit.
	pc_8E1,			///< 8 data bits, even parity, 1 stop bit.
	pc_8N2,			///< 8 data bits, no parity, 2 stop bits.
	pc_8O2,			///< 8 data bits, odd parity, 2 stop bits.
	pc_8E2			///< 8 data bits, even parity, 2 stop bits.
} PINCONFIG;

/**
	@brief Enumeration used to specify baud rates.
*/
typedef enum
{
	pb_115200,		///< 115200 BPS.
	pb_57600,		///< 57600 BPS.
	pb_38400,		///< 38400 BPS.
	pb_19200,		///< 19200 BPS.
	pb_9600,		///< 9600 BPS.
	pb_4800,		///< 4800 BPS.
	pb_2400,		///< 2400 BPS.
	pb_1200,		///< 1200 BPS.
	pb_300			///< 300 BPS.
} PINBAUDRATE;

/**
	@brief Data used to configure the pinpad port.
*/
typedef struct pinData
{
	PINPROTOCOL Protocol;	///< Protocol.
	PINMODE Mode;		///< Mode.
	PINCONFIG Config;	///< Serial data format.
	PINBAUDRATE Baud;	///< Baud rate.
} PINDATA;

#if defined(PINC_WIRELS) && PINC_WIRELS

/// Packet retry counter
#define PKT_RETRY 10
/// Link acquisition timeout
#define LINK_ACQ_TO 3
/// Packet length
#define PKT_LEN 50

/**
	@brief Wireless transmitter state.
*/
typedef enum
{
	PIN_WRLSTX_OFF,		///< Turn off (disable) the wireless transmitter
	PIN_WRLSTX_ON		///< Turn on (enable) the wireless transmitter
} PIN_WRLS_STATE;

/**
	@brief Wireless protocol setup record structure.
*/
typedef struct
{
	UWORD	WID;		///< Wireless ID (MSbyte, LSbyte)
	UBYTE	RETRY;		///< Packet retry count
	UBYTE	ACQ;		///< Link acquisition timeout (in seconds)
	UBYTE	IMAX;		///< Information field's maximum size
} PIN_WRLS_SETUP;

#endif // PINC_WIRELS

#endif // INCLPIN

#if INCLPRT

/**
	@brief Enumeration used to specify RS232 mode.

	@note Some of the modes below may not be available - dependent
	      upon O/S configuration of terminal.
*/
typedef enum
{
	rm_HYPP_4800E72,	///< Hypercom Serial Printer Protocol
	rm_RESERVED2,		///< Reserved
	rm_CHKRD_9600E71,	///< Check Reader Protocol
	rm_IRT_4800E71,		///< IRT - Post Office Protocol
	rm_TM290_9600N81,	///< Epson TM-290 Slip Printer Protocol
	rm_POSMINI_9600N81,	///< POS / MiniNAC Protocol
	rm_VF250_9600N81,	///< Verifone 250 Roll Printer Protocol
	rm_IX50_9600N82,	///< IX50 Smart Card Protocol
	rm_PST150_9600N81,	///< PST150 ECR Protocol
	rm_OKI182_1200E71,	///< Okidata 182 Printer Protocol
	rm_ECR1200_1200E71,	///< ECR Device Protocol @ 1200 Baud
	rm_Configurable = 0x0f	///< user configurable protocol, baud rate, and format
} RS232MODE;

/**
	@brief Enumeration for identifying different RS232 status requests.
*/
typedef enum
{
	ri_DEVICESTATUS,		///< DTR status, orv_OK or orv_ERRDNA.
#if defined(MAKE_OPTIMUM)
    ri_CTS= ri_DEVICESTATUS,	///< CTS status, orv_OK or orv_ERRDNA.
    ri_DSR                  ///< DSR status, orv_OK or orv_ERRDNA.
#endif // MAKE_OPTIMUM
} RS232INFO;

#if defined(MAKE_OPTIMUM)
/**
*   @brief Enumeration to handle DSR signal in RS232 device.
*/
typedef enum
{
    RS232_SET_DSR_LOW,		// Set DSR Signal Low
    RS232_SET_DSR_HIGH,		// Set DSR Signal High
    RS232_SET_RTS_LOW,		// Set RTS Signal Low
    RS232_SET_RTS_HIGH,		// Set RTS Signal High
    RS232_SET_DTR_LOW,		// Set DTR Signal Low
    RS232_SET_DTR_HIGH,		// Set DTR Signal High
} RS232_SET_DSR;
#endif // MAKE_OPTIMUM

/**
	@brief Enumeration used to select protocol.

	@note Some of the protocols below may not be available - dependent
	      upon O/S configuration of terminal.
*/
typedef enum
{
	rp_PROTOFF,		///< All PRT Protocols are InActive (SIO is Off).
	rp_HYPPRT,		///< Hypercom serial printer protocol.
	rp_CHKRD,		///< Check reader protocol.
	rp_TRANS,		///< Transparent data protocol (no message framing, no control characters).
	rp_POSMINI,		///< POS / MiniNak protocol.
	rp_VF250,		///< Verifone 250 roll printer protocol.
	rp_ECR,			///< ECR device protocol.
	rp_T102A,		///< Epson T102A printer protocol.
	rp_SPS,			///< Speedpass.
	rp_CAT33 = 0x0a,	///< Omron CAT 33 pinpad protocol.
	rp_IECI = 0x0b,		///< IECI protocol.
	rp_SIBS = 0x0c		///< SIBS key GUN protocol.
} RS232PROTOCOL;

/**
	@brief Enumeration used to specify serial data formats.
*/
typedef enum
{
	rc_7N1,			///< 7 data bits, no parity, 1 stop bit.
	rc_7O1,			///< 7 data bits, odd parity, 1 stop bit.
	rc_7E1,			///< 7 data bits, even parity, 1 stop bit.
	rc_7N2,			///< 7 data bits, no parity, 2 stop bits.
	rc_7O2,			///< 7 data bits, odd parity, 2 stop bits.
	rc_7E2,			///< 7 data bits, even parity, 2 stop bits.
	rc_8N1,			///< 8 data bits, no parity, 1 stop bit.
	rc_8O1,			///< 8 data bits, odd parity, 1 stop bit.
	rc_8E1,			///< 8 data bits, even parity, 1 stop bit.
	rc_8N2,			///< 8 data bits, no parity, 2 stop bits.
	rc_8O2,			///< 8 data bits, odd parity, 2 stop bits.
	rc_8E2			///< 8 data bits, even parity, 2 stop bits.
} RS232CONFIG;

/**
	@brief Enumeration used to specify baud rates.
*/
typedef enum
{
	rb_115200,		///< 115200 BPS.
	rb_57600,		///< 57600 BPS.
	rb_38400,		///< 38400 BPS.
	rb_19200,		///< 19200 BPS.
	rb_9600,		///< 9600 BPS.
	rb_4800,		///< 4800 BPS.
	rb_2400,		///< 2400 BPS.
	rb_1200,		///< 1200 BPS.
	rb_300			///< 300 BPS.
} RS232BAUDRATE;

/**
	@brief Data used to configure the RS232 port.
*/
typedef struct Rs232Data
{
	RS232PROTOCOL Protocol;	///< Protocol.
	RS232CONFIG Config;	///< Serial data format.
	RS232BAUDRATE Baud;	///< Baud rate.
} RS232DATA;

#endif // INCLPRT

#if INCLTGI

/**
	@brief Tailgate interface modes.
*/
typedef enum
{
	tgi_MODE_CAT33,		///< CAT 33 Protocol
	tgi_MODE_CONFIG,	///< Configurable Protocol & Control Characters
	tgi_MODE_POSMINI,	///< POS / MiniNAC Protocol
	tgi_MODE_NCR5991,	///< NCR 5991 Signature Capture Device Protocol
	tgi_MODE_CHKRD,		///< Check Reader Protocol
	tgi_MODE_TRANS		///< Transparent Data Protocol (Terminal Mode)
} TGIMODE;

#endif // INCLTGI

#if INCLLAN

/**
	@brief LAN interface modes.
*/
typedef enum
{
	ln_CAT033_2400,		///< CAT 033 STX/ETX 2400 baud
	ln_CONCORD_9600		///< CONCORD Lan, 9600 baud
} LANMODE;

#endif // INCLLAN

#ifdef INCLZ8I

/// Length of a single key
#define Z8SINGLELEN 8
/// Length of a double key
#define Z8DOUBLELEN 16
/// Length of a triple key
#define Z8TRIPLELEN 24

/**
	@brief Key type for Z8 processor.
*/
typedef enum
{
	z8ktSINGLE,		///< Single key
	z8ktDOUBLE,		///< Double key
	z8ktTRIPLE		///< Triple key
} Z8KEYTYPE;

/**
	@brief DES encryption options.
*/
typedef enum
{
	z8deXORKEY = 0x01,	///< Special DUKPT (XOR Data into Data)
	z8deKEYVARIANT = 0x02,	///< DUKPT Key Variant (XOR Key with ffh)
	z8deENCRYPTLRL = 0x04	///< ENCRYPT R-L-R with Data
} Z8DESENCOPT;

/**
	@brief Z8 key load options.
*/
typedef enum
{
	z8loKEYENCYPT = 0x01,	///< Key is encrypted
	z8loTOINTERNAL = 0x02	///< Load to internal
} Z8LOADOPT;

/**
	@brief Z8 derive options.
*/
typedef enum
{
	z8doDERIVE_DEFAULT = 0x00, ///< Default operation, no special processing.
	z8doXORKEY = 0x01,	///< Special DUKPT (XOR Data into Data)
	z8doXORDATA = 0x04,	///< XOR Data with result
	z8doRLRDATA3 = 0x10,	///< Encrypt R-L-R with Data 3
	z8doRLRDATA2 = 0x20,	///< Encrypt R-L-R with Data 2
	z8doRLRDATA1 = 0x40	///< Encrypt R-L-R with Data 1
} Z8DERIVEOPT;

/**
	@brief Z8 operation modes.
*/
typedef enum
{
	z8doENCRYPT,		///< Use des encryption.
	z8doDECRYPT		///< Use des decryption.
} Z8OPERATION;

#endif // INCLZ8I

#ifdef INCLPAD

/**
	@brief Structure for Touch PAD Position Record.
*/
typedef struct
{
	UWORD X_Pos;		///< X-Axis Position
	UWORD Y_Pos;		///< Y-Axis Position
	UBYTE PenState;		///< Pen State (PEN_UP, PEN_DOWN)
} PADDATA;

/// Size of Touch PAD Position Record
#define PADDATASZE sizeof( PADDATA )

/**
	@brief Pen State (PADDATA.TPPos_Pen field of Response Record).
*/
typedef enum
{
	PAD_PEN_UP = 0x00,	// Pen is Up (Not Touching the PAD)
	PAD_PEN_DOWN = 0xFF,	// Pen is Down (Touching the PAD)
	PAD_PEN_SIGCAP = 0xFE	/**< Pen is in the Signature Capture Area
				     (Pen is out of the Touch PAD Area). */
} PADPENSTATE;

/**
	@brief Signature Capture Parameters.
*/
typedef struct
{
	UBYTE *pSigBuf;		/**< Pointer to the buffer where signature data
	                             is to be stored. Usually, at least 800
	                             bytes long. */
	UWORD BufLen;		///< Length of the buffer pointed to by pSigBuf.
	UWORD PenUpTime;	/**< Time in 2 ms units that a pen can remain
	                             off the pad after a signature capture
	                             starts. If the pen is off the pad for more
	                             than this time, the signature capture
	                             session completes with the error code set
	                             to orvOK. This time does not apply until
	                             after the pen touches the pad for the
	                             first time after the call to
	                             SDK_StartSigCapture(). If this parameter
	                             is set to zero, the timeout is unlimited,
	                             and the signature capture session does not
	                             end until SDK_StopSigCapture() is called,
	                             an overall timeout occurs, or the end of
	                             the data buffer is reached. */
	UWORD LeftX;		///< Left x of signature capture area.
	UWORD TopY;		///< Top y of signature capture area.
	UWORD Width;		///< Width of signature capture area.
	UWORD Height;		///< Height of signature capture area.
} SIGCAPPARMS;

#endif

#if INCLCRB|INCLCRD|INCLCR3

/**
	@brief Card reader modes.
*/
typedef enum
{
        cr_NORMAL,		/**< Data is returned in normal format per ISO
				     standards. This is the default mode. */
        cr_RAW,			/**< Raw data directly from the magnetic stripe
				     is returned. No decoding is performed. */
        cr_PARAMS		/**< User sets the parameters used to decode
				     the magnetic stripe data. */
} CARDREAD_MODE;

#endif // INCLCRB|INCLCRD|INCLCR3

#ifdef MAKE_H2200

/**
	@brief Motorized card reader commands.
*/
typedef enum {
	mci_FEED_ENABLE,
	mci_SENSOR_FRONT,
	mci_SENSOR_MID,
	mci_SENSOR_REAR,
	mci_CARD_INSERTED,
	mci_CARD_REMOVED,
	mci_CARD_EJECTED,
	mci_MAXITEM
} MOTORCARDITEM;

#endif  // MAKE_H2200

#if INCLATM

/**
	@brief Configuration commands for ATM driver.
*/
typedef enum
{
	ATM_RS232_CTRL,		/**< This command is used to route the ASYNC
	                             serial connection of the modem device to
	                             the RS232 port on the back of the printer
	                             modem module. */
	ATM_BOOT_CTRL,		/**< This command is used for loading new
	                             software to the modem through the external
	                             RS232 port. */
	ATM_DETECT_MODEM,	/**< This command is used to detect the
	                             attached wireless modem type. */
	ATM_WAKEUP_CTRL,	/**< This command is used to enable and disable
	                             the WAKEUP power mode in the ATM driver. */
	ATM_DTR_CTRL,		/**< When the ATM driver receives the enable
	                             DTR config command, the ATM driver turns
	                             on the DTR signal. */
	ATM_RTS_CTRL,		/**< When the ATM driver receives the enable
	                             RTS config command, the ATM driver turns
	                             on the RTS signal. */
	ATM_HWSTATUS		/**< This command is used to get the current
	                             hardware line status. */
} ATM_CONFIG_CMD;

/**
	@brief Modem to RS232 Loopback Type definitions for ATM_RS232_CTRL.
*/
typedef enum
{
	ATM_RS232_OFF = 0x00,	///< Turn off (disable) RS232 loopback.
	ATM_RS232_ON = 0x01	///< Turn on (enable) RS232 loopback.
} ATMLOOPBACK;

/**
	@brief Special boot loading method definitions for ATM_BOOT_CTRL.
*/
typedef enum
{
	ATM_BOOT_OFF = 0x00,	///< Turn off (disable) BOOT method.
	ATM_BOOT_ON = 0x01	///< Turn on (enable) BOOT method.
} ATMBOOTMETHOD;

/**
	@brief Defintions for ATM_DETECT_MODEM config command.
*/
typedef enum
{
	ATM_RADIOGSM = 0x01,	///< GSM Radio installed (Falcom)
	ATM_RADIOCDP = 0x02	///< CDPD Radio installed (Novatel)
} ATMMODEM;

/**
	@brief Types for ATM_WAKEUP_CTRL config command.

	When WAKEUP mode is disabled, the ATM driver will turn off the
	ATM WAKEUP control under the following conditions.<ol>
	<li>ATM Driver receives a close command.</li>
	<li>ATM Driver receives a rewind command.</li></ol>

	When the ATM Driver receives the ATM_WAKEUP_ON command, the ATM driver
	turns on the ATM WAKEUP power. As long as the ATM driver is OPEN,
	WAKEUP will be active.
*/
typedef enum
{
	ATM_WAKEUP_OFF = 0x00,	///< Turn off (disable) WAKEUP.
	ATM_WAKEUP_ON = 0x01	///< Turn on (enable) WAKEUP.
} ATMWAKEUP;

/**
	@brief Types for ATM_DTR_CTRL config command.
*/
typedef enum
{
	ATM_DTR_OFF = 0x00,	///< Turn off (disable) DTR.
	ATM_DTR_ON = 0x01	///< Turn on (enable) DTR.
} ATMDTRCTRL;

/**
	@breif Types for ATM_RTS_CTRL config command.
*/
typedef enum
{
	ATM_RTS_OFF = 0x00,	///< Turn off (disable) RTS.
	ATM_RTS_ON = 0x01	///< Turn on (enable) RTS.
} ATMRTSCTRL;

/**
	@brief Types for ATM_HWSTATUS config command (bitmask).
*/
typedef enum
{
	ATM_ONI = 0x01,		///< Modem on indicator (input)
	ATM_CTS = 0x02,		///< CTS Status (input)
	ATM_DSR = 0x04,		///< DSR Status (input)
	ATM_ISPARE = 0x08,	///< Spare input status bit (input)
	ATM_WKUP = 0x10,	///< Wakeup status (output)
	ATM_DTR = 0x20,		///< DTR status (output)
	ATM_RTS = 0x40,		///< RTS status (output)
	ATM_OSPARE = 0x80	///< Spare output status bit (output)
} ATMHWSTATUS;

#endif // INCLATM

#if INCLRAP

/**
	@brief Radio protocol status.
*/
typedef struct
{
	UBYTE TXMSTA;		///< Transmit MPAK status
	UBYTE RADSTA;		///< Radio Status
	UBYTE BATLVL;		///< Battery level
	UBYTE SINGDATA;		///< Radio signal strength
	UWORD INTFAL;		///< Internal fault bits
	UBYTE LOCMAN[4];	///< Local radio address
	UBYTE RAPVER;		///< RAP version
	UBYTE SPARE;		///< Spare byte
} RADIOSTAT;

#endif // INCLRAP

/**
 	@brief Initializes internal IO data and data blocks required by the
	       device drivers.

	This function initializes the internal Input and Output data and data
	blocks used by device drivers.
*/
extern void SDK_InitIO( void );

#if defined(MAKE_VIKING) && !defined(MAKE_P2100) && !defined(MAKE_H2200)

/**
 	@brief Return RS0 associated device

	@return PORT
*/
extern PORT SDK_RS0GetDevice( void );

/**
	@brief Associates RS0 with PORT

	@param port Can be PIN or PRT.

	@return orvOK on success or orvDEVICEBUSY on error.

	RS2 will be associated with the opposite device. Both PIN and PRT
	devices should be closed.
*/
extern OS_RETURN_VALS SDK_RS0SetDevice( PORT port );

#endif

#if INCLCOM

/**
	@brief Resets and configures the communication port.

 	@param LineType Comm line type
 	@param pDialInfo Dialing information
 	@param Timeout Time to wait for the connection to be completed, in
	               units of OS_TIMES as defined in sdk.h.
 	@param pStatus An IOSTATUS pointer structure that contains the flag and
	               error code set by this function.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.
*/
extern OS_RETURN_VALS SDK_ComConnect( COMLINE  LineType,
				      DIALDATA *pDialInfo,
				      OS_TIMES Timeout,
				      IOSTATUS *pStatus );

/**
	@example SDK_ComConnect.c
	This is an example of how to use the SDK_ComConnect() function.
*/

/**
	@brief Resets the communication port.

	@return See @ref OS_RETURN_VALS.

	This function resets the communication port and should be called after
	communication ends.

	@note Not applicable when communication is done via HPOSFC directly.
*/
extern OS_RETURN_VALS SDK_ComReset( void );

/**
 	@brief Gets information or status regarding various aspects of the
	       communication port.

 	@param Type Type of information requesting

	@return	Information requested.

	@note Not applicable when communication is done via HPOSFC directly.

 	@See @ref COMINFO
*/
extern UBYTE SDK_ComInfo( COMINFO Type );

/**
	@example SDK_ComInfo.c
	This is an example of how to use the SDK_ComInfo() function.
*/

#ifdef MAKE_VIKING
/**
 	@brief Gets the status of the requested info.

 	@param Type Requested config
 	@param Data Assotiated configuration data

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@see @ref COMCONFIG.
*/
extern OS_RETURN_VALS SDK_ComConfig( COMCONFIG Type,
                                     COMCONFIGDATA Data );
#endif

/**
 	@brief Opens and reads data from the communication port.

 	@param pBuffer Buffer where the data is to be placed.
 	@param pBufLen Contains the maximum length of the buffer. On return, it contains the number of bytes read.
 	@param Timeout Time to wait for a connection to be completed in units of OS_TIMES as defined in sdk.h.
 	@param pStatus Pointer to the IOSTATUS structure that contains the flag and error code set by this function.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.
*/
extern OS_RETURN_VALS SDK_ComRead( UBYTE *pBuffer,
				   UWORD *pBufLen,
				   OS_TIMES Timeout,
				   IOSTATUS *pStatus );

/**
	@example SDK_ComRead.c
	This is an example of how to use the SDK_ComRead() function.
*/

/** 	@brief Sets a write on the communication port.
 	@param pBuffer Buffer.
 	@param BufLen Length of data to write.
 	@param Timeout Time to wait for write completion in units of OS_Times as defined in sdk.h.
 	@param pStatus Pointer to the IOSTATUS structure that contains the flag and error code set by this function.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.
*/
extern OS_RETURN_VALS SDK_ComWrite( UBYTE    *pBuffer,
				    UWORD    BufLen,
				    OS_TIMES Timeout,
				    IOSTATUS *pStatus );

/**
	@example SDK_ComWrite.c
	This is an example of how to use the SDK_ComRead() function.
*/

/**
 	@brief Set XMODEM fill character

 	@param FillChar Fill character

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComXModemFill( UBYTE FillChar );

/**
	@brief Start XMODEM Mode Receive File.
 	@param BlockSize Block size to use

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComXModemRx( UWORD BlockSize );

/**
	@brief Start XMODEM Mode Transmit File

 	@param BlockSize Block size to use.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComXModemTx( UWORD BlockSize );

/**
	@brief 	Start XMODEM Mode Transmit Continue CRC

 	@param BlockSize Block size to use.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComXModemTxCRC( UWORD BlockSize );

/**
	@brief Start XMODEM Mode Transmit Continue LRC

 	@param BlockSize Block size to use.

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComXModemTxLRC( UWORD BlockSize );

/**
	@brief Performs a voice dial

 	@param pDialInfo Pointer to dial information
	@param Timeout Timeout
 	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.
*/
extern OS_RETURN_VALS SDK_ComVoiceDial(DIALDATA *pDialInfo, OS_TIMES Timeout, IOSTATUS *pStatus);

/**
	@brief Configures the communication port to perform an auto answer.

	This function resets and configures the communication port to perform an auto answer.

	@param pDialInfo Dialing information
	@param Timeout Time to wait for the auto answer be completed, in units of OS_TIMES as defined in sdk.h
	@param pStatus An IOSTATUS pointer structure that contains the flag and error code set by this function

	@return See @ref OS_RETURN_VALS.

	@note Not applicable when communication is done via HPOSFC directly.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_ComAutoAnswer(DIALDATA *pDialInfo, OS_TIMES Timeout, IOSTATUS *pStatus);

/**
	@example SDK_ComAutoAnswer.c
	This is an example of how to use the SDK_ComAutoAnswer() function.
*/

#endif // INCLCOM

#if INCLPIN

/**
	@brief Opens the PIN pad for reading.

	This function opens the PIN pad and prepares it for reading.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param Timeout Time to complete reading
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PinRead( UBYTE    *pBuffer,
				   UWORD    *pBufLen,
				   OS_TIMES Timeout,
				   IOSTATUS *pStatus );

/**
	@example SDK_PinRead.c
	This is an example of how to use the SDK_PinRead() function.
*/

/**
	@brief Sets the PIN pad for writing.

	This function enables the write capability for the PIN pad.

	@param pBuffer Pointer to data to output
	@param BufLen Length of data
	@param Timeout Time to complete writing
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PinWrite( UBYTE    *pBuffer,
				    UWORD    BufLen,
				    OS_TIMES Timeout,
				    IOSTATUS *pStatus );

/**
	@example SDK_PinWrite.c
	This is an example of how to use the SDK_PinRead() function.
*/

/**
	@brief Opens and configure the pin port

	This function opens the PIN pad port with the desired parameters as indicated by pPinInfo.

	@param pPinInfo PIN data (see @ref PINDATA).
	@param pinopts Special options (see @ref PINOPT).

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PinOpen( PINDATA *pPinInfo, UWORD pinopts );

/**
	@example SDK_PinOpenClose.c
	This is an example of how to use the SDK_PinOpen() and SDK_PinClose()
	functions.
*/

/**
	@brief Closes the pinpad port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PinClose( void );

/**
	@brief Resets the PIN device.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PinReset( void );

/**
	@brief Enables or disables the PIN port constant power mode.

	This function is used to Enable/Disable the Constant Power Mode for the
	PIN Driver. If the selected Constant Power Mode State is not valid,
	orvINVCONFIGURATION is returned.

	Constant Power only works for terminals with external modules (like the
	Printer/Modem Module). If the PIN Driver is used in a terminal without
	an external module attached, this function will then return
	orvDEVICEUNAVAILABLE.

	When Constant Power Mode is disabled (PIN_CPWR_OFF), the PIN Driver
	will turn off the PIN Port Power under the following conditions.<ol>
	<li> When PIN Driver is closed using SDK_PinClose();</li>
	<li> When PIN Driver is reset using SDK_PinReset();</li>
	<li> PIN Driver's response timeout timer expires and the PIN Driver
	     needs to retransmit a Message</li></ol>

	When the Constant Power Mode is enabled (PIN_CPWR_ON), the PIN Driver
	turns on the PIN Port Power. As long as Constant Power Mode is enabled,
	the PIN Driver will leave the PIN Port Power On.

	The new Constant Power Mode State will be used by the PIN Driver until
	the PIN Driver receives a new change Constant Power Mode
	(PIN_CONST_PWR).

	@param setval PIN_CPWR_OFF or PIN_CPWR_ON.

	@return See @ref OS_RETURN_VALS.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_PinConstantPower(PIN_CPWR setval);

/**
	@brief Sends a command to the external keyboard.

	@param Command Command to send to external keyboard.

	@return See @ref OS_RETURN_VALS.

	@note Not supported on Optimum 2100 family terminals.
*/
extern OS_RETURN_VALS SDK_PinExtKeybCommand( UBYTE Command );

#if defined(PINC_WIRELS) && PINC_WIRELS

/**
	@brief Open the wireless device.

        This function opens and configures the wireless device. The wireless
	device is used to communicate with other terminals equipped with a
	wireless device.

        The protocol, serial port baud rate and format for the wireless device
	is indicated by pPinInfo which points to a structure of type PINDATA.
	The PINDATA structure is used because the wireless device shares the
	same serial port as the PIN device. Since the wireless device and the
	PIN device share the same port, the two devices cannot be used at the
	same time.

        For the wireless device, Protocol in structure PINDATA must be set to
	pp_WIRLS indicating the wireless protocol. If a different protocol is
	used, this function fails and returns orvINVDATA.

        WID in the PIN_WRL_SETUP structure is the 2 byte wireless ID, which
	is used in packet headers for communications with other wireless
	terminals.

        For optimal performance, the corresponding three parameters of the
	PIN_WRL_SETUP structure should be set to pre-defined values: PKT_RETRY,
	LINK_ACQ_TO, PKT_LEN.

        @param  pPinInfo Pointer to PINDATA structure containing protocol info,
	                 baud rate, and serial data format
        @param  pWrlsSetup Pointer to PIN_WRLS_SETUP structure containing
	                   wireless ID (or address) and other wireless protocol
	                   specific options.

	@return See @ref OS_RETURN_VALS.
*/ 
extern OS_RETURN_VALS SDK_WrlsOpen( PINDATA *pPinInfo, 
                                    PIN_WRLS_SETUP *pWrlsSetup );

/**
	@example SDK_WrlsOpen.c
	This is an example of how to use the SDK_WrlsOpen() function.
*/

/**
	@brief Close the Wireless device.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_WrlsClose( void );

/**
	@brief Read data from the wireless device.

	This function issues a read request to the wireless device. 

        Upon receiving a data packet, this function places data into buffer
	pointed to by pBuffer. When the function is called, *pBufLen is the
	maximum bytes of data to read. This function returns immediately to the
	caller and will set pStatus->CompleteFlag to non-zero when the function
	is completed. Upon completion, the function sets *pBufLen to the amount
	of data actually read.

        The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when the maximum amount of data is written, a timeout
	occurs, an error occurs, or when a received packet is complete. After
	pStatus->CompleteFlag is set, pStatus->Error can be used to check for
	an error or timeout. The Error parameter of IOSTATUS is not valid
	unless the CompleteFlag is set. This function automatically clears the
	CompleteFlag when first called.

        The first five bytes of data placed in the buffer after a successful
	read contain a message ID, a 2 byte receiver ID, and 2 byte sender ID.
	A received data buffer appears as follows:

		buffer[0] = message ID ( same as sender's buffer)<br>
		buffer[1] = low byte of receiver ID (wireless ID of this
		            terminal)<br>
		buffer[2] = high byte of receiver ID (wireless ID of this
		            terminal)<br>
		buffer[3] = low byte of sender ID (wireless ID of terminal
		            sending the data)<br>
		buffer[4] = high byte of sender ID (wireless ID of terminal
		            sending the data)<br>
		buffer[n] = data

	The sender ID can be used to return a response to the terminal that set
	the data.

	@param pBuffer Buffer
        @param pBufLen Pointer to maximum length of data to read. Contains
	               actual length read on completion of function
        @param pStatus Pointer to the IOSTATUS structure that contains the
	               flag and error code set by this function

	@return See @ref OS_RETURN_VALS.
*/ 
extern OS_RETURN_VALS SDK_WrlsRead( UBYTE      *pBuffer,
                                    UWORD      *pBufLen,
                                    IOSTATUS   *pStatus );

/**
	@example SDK_WrlsRead.c
	This is an example of how to use the SDK_WrlsRead() function.
*/
                                 
/**
	@brief	Writes to the wireless device.

        The function writes the data pointed to by pBuffer. This function
	returns immediately to the caller and will set pStatus->CompleteFlag to
	non-zero when the function is completed.

        The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when the data is written, a timeout occurs, or other
	error occurs. After CompleteFlag is set, the Error parameter of
	IOSTATUS can be used to check for an error or timeout. The Error
        parameter is not valid unless the CompleteFlag was set. This function
	automatically clears the CompleteFlag when first called.

        In order to write properly to another wireless terminal, the first five
	bytes of the data buffer pointed to by pBuffer must contain a message
	ID, a 2 byte  destination ID, and 2 byte source ID. The data buffer
	should be set up as follows:

		buffer[0] = message ID (any number dependent upon
		            implementation)<br>
		buffer[1] = low byte of destination ID<br>
		buffer[2] = high byte of destination ID<br>
		buffer[3] = low byte of source ID <br>
		buffer[4] = high byte of source ID<br>
		buffer[n] = data

	Destination ID is the wireless ID as setup by SDK_WrlOpen() in the
	terminal which receives the data.

	Source ID is the wireless ID as setup by SDK_WrlOpen() in this terminal.

	ID is specified in the WID parameter of the PIN_WRLS_SETUP structure.
	Refer to SDK_WrlsOpen().

	@param pBuffer Buffer
	@param BufLen Length of data to write
        @param pStatus Pointer to the IOSTATUS structure that contains the
	               flag and error code set by this function

	@return See @ref OS_RETURN_VALS.
*/ 
extern OS_RETURN_VALS SDK_WrlsWrite( UBYTE       *pBuffer,
                                     UWORD       BufLen,
                                     IOSTATUS    *pStatus );

/**
	@example SDK_WrlsWrite.c
	This is an example of how to use the SDK_WrlsWrite() function.
*/

/**
	@brief Set the ON/OFF state of the wireless transmitter.

        This function sets the ON/OFF state of the wireless transmitter. The
	wireless device must be open for this function to have an affect. The
	following values below could be returned by this function.

	The setval parameter is of type PIN_WRLS_STATE which is an enumeration defined in SDKIO.H.

        Note: When the wireless device is first opened using SDK_WrlsOpen(), the device defaults to
        the ON state and there is no need to call this function.

	@param setval Either PIN_WRLSTX_OFF or PIN_WRLSTX_ON

	@return orvOK: Wireless transmitter state was successfully changed.<br>
	        orvDEVICENOTOPEN: Device is not open.<br>
	        orvINVCONFIGURATION: If setval is something other than
	                             PIN_WRLSTX_OFF or PIN_WRLSTX_ON<br>
	        orvDEVICEUNAVAILABLE: PIN Port does not support Wireless.
*/ 
OS_RETURN_VALS SDK_WrlsSetState(PIN_WRLS_STATE setval);

#endif // PINC_WIRELS

#endif // INCLPIN

#if INCLCRB|INCLCRD|INCLCR3

/**
	@brief Opens the card reader for reading Track 1.

	This function opens the card reader for Track 1 reading. The
	CompleteFlag parameter in the IOSTATUS structure pointed to by pStatus
	is set when a card swipe is detected. After CompleteFlag is set, the
	Error parameter of IOSTATUS can be used to check for a card read error
	or timeout. The Error parameter is not valid unless the CompleteFlag
	was set.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param TimeOut Time to complete reading
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_GetTrk2, @ref SDK_GetTrk3
*/
extern OS_RETURN_VALS SDK_GetTrk1( UBYTE    *pBuffer,
				   UWORD    *pBufLen,
				   OS_TIMES TimeOut,
				   IOSTATUS *pStatus );

/**
	@example SDK_GetTrk1.c
	This is an example of how to use the SDK_GetTrk1() function.
*/

/**
	@brief Opens the card reader for reading Track 2.

	This function opens or enables the card reader for reading Track 2.
	The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when a card swipe is detected. After CompleteFlag is
	set, the Error parameter of IOSTATUS can be used to check for a card
	read error or timeout. The Error parameter is not valid unless the
	CompleteFlag was set.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param TimeOut Time to complete reading
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_GetTrk1, @ref SDK_GetTrk3
*/
extern OS_RETURN_VALS SDK_GetTrk2( UBYTE    *pBuffer,
				   UWORD    *pBufLen,
				   OS_TIMES TimeOut,
				   IOSTATUS *pStatus );

/**
	@brief Opens the card reader for reading Track 3.

	This function opens or enables the card reader for reading Track 3. The
	CompleteFlag parameter in the IOSTATUS structure pointed to by pStatus
	is set when a card swipe is detected. After CompleteFlag is set, the
	Error parameter of IOSTATUS can be used to check for a card read error
	or timeout. The Error parameter is not valid unless the CompleteFlag
	was set.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param TimeOut Time to complete reading
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_GetTrk1, @ref SDK_GetTrk2
*/
extern OS_RETURN_VALS SDK_GetTrk3( UBYTE    *pBuffer,
				   UWORD    *pBufLen,
				   OS_TIMES TimeOut,
				   IOSTATUS *pStatus );

/**
	@brief Closes the Track 1 reader.

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_Trk2Close, @ref SDK_Trk3Close
*/
extern OS_RETURN_VALS SDK_Trk1Close( void );

/**
	@brief Closes the Track 2 reader.

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_Trk1Close, @ref SDK_Trk3Close
*/
extern OS_RETURN_VALS SDK_Trk2Close( void );

/**
	@brief Closes the Track 3 reader.

	@return See @ref OS_RETURN_VALS.

	@see @ref SDK_Trk1Close, @ref SDK_Trk2Close
*/
extern OS_RETURN_VALS SDK_Trk3Close( void );

/**
	@brief Sets the track data decode mode for track 1.

        This function specifies the data decode mode for Track 1 of the card
	reader. The data decode mode determines the format of data returned by
	SDK_GetTrk1().

        SDK_SetTrk1Mode() can be called at anytime and the specified mode will
	be in effect for subsequent calls to SDK_GetTrk1().

        When the card reader device is opened, it is initialized to default
	operating parameters. These parameters are defined by a seven byte
	block for track 1 as follows: @include SDK_SetTrk1Mode.txt

	The values above are overwritten by data pointed to by pData when mode
	cr_PARAMS is specified.

	@param mode Desired decode mode.
	@param pData Pointer to seven byte block of data if mode is cr_PARAMS,
	             else ignored.

	@note To have effect, this function must be called prior to calling
	      SDK_GetTrk1().

	@note Not supported on Optimum 2100 family terminals.
*/
extern void SDK_SetTrk1Mode(CARDREAD_MODE mode, UBYTE *pData);

/**
	@example SDK_SetTrk1Mode.c
	This is an example of how to use the SDK_SetTrk1Mode() function.
*/

/**
	@brief Sets the track data decode mode for track 2.

        This function specifies the data decode mode for Track 2 of the card
	reader. The data decode mode determines the format of data returned by
	SDK_GetTrk2().

        SDK_SetTrk2Mode() can be called at anytime and the specified mode will
	be in effect for subsequent calls to SDK_GetTrk2().

        When the card reader device is opened it, is initialized to default
	operating parameters. These parameters are defined by a seven byte
	block for track 2 as follows: @include SDK_SetTrk2Mode.txt

	The values above are overwritten by data pointed to by pData when mode
	cr_PARAMS is specified.

	@param mode Desired decode mode.
	@param pData Pointer to seven byte block of data if mode is cr_PARAMS,
	             else ignored.

	@note To have effect, this function must be called prior to calling
	      SDK_GetTrk2().

	@note Not supported on Optimum 2100 family terminals.
*/
extern void SDK_SetTrk2Mode(CARDREAD_MODE mode, UBYTE *pData);

/**
	@example SDK_SetTrk2Mode.c
	This is an example of how to use the SDK_SetTrk2Mode() function.
*/

/**
	@brief Sets the track data decode mode for track 3.

        This function specifies the data decode mode for Track 3 of the card
	reader. The data decode mode determines the format of data returned by
	SDK_GetTrk3().

        SDK_SetTrk3Mode() can be called at anytime and the specified mode will
	be in effect for subsequent calls to SDK_GetTrk3().

        When the card reader device is opened it, is initialized to default
	operating parameters. These parameters are defined by a seven byte
	block for track 3 as follows: @include SDK_SetTrk3Mode.txt

	The values above are overwritten by data pointed to by pData when mode
	cr_PARAMS is specified.

	@param mode Desired decode mode.
	@param pData Pointer to seven byte block of data if mode is cr_PARAMS,
	             else ignored.

	@note To have effect, this function must be called prior to calling
	      SDK_GetTrk2().

	@note Not supported on Optimum 2100 family terminals.
*/
extern void SDK_SetTrk3Mode( CARDREAD_MODE mode, UBYTE *pData);

/**
	@example SDK_SetTrk3Mode.c
	This is an example of how to use the SDK_SetTrk3Mode() function.
*/

#endif // INCLCRB|INCLCRD|INCLCR3

#ifdef MAKE_H2200
/**
	@brief This function will stop the motor.

	Motor control function will be enabled. After a short time interval
	the motor will be started in reverse direction.

	@note This function returns immediately after the card eject has been
	started, i.e. there is no wait for the motor to start in reverse
	direction.

	@return	orvOK
*/
extern OS_RETURN_VALS SDK_MrdCardEject( void );

/**
	@brief Enable/disable motor control.

	Enable/disable motor control for feeding the card into the reader.

	@param bEnable Selected mode of motor control for card feeding:<br>

	FALSE	Function will stop the motor at the first and try to eject a
		card if it is still present in the reader. Motor control
		function will be disabled, i.e. motor will not be started even
		if a card is inserted into the slot.

	TRUE	Function will start motor in forward direction when a card is
		inserted into the slot.
*/
extern OS_RETURN_VALS SDK_MrdCardFeedEnable( UBYTE bEnable );

/**
	@brief Status of card motor control module's items.

	Check and return status of different card motor control module's items.

	@param Item Requested item.

	@return	orvOK: Status of item is active.<br>
		orvUNKNOWNCMD: Requested item doesn't exist.<br>
		orvDEVICEFAULT: Other errors.
*/
extern OS_RETURN_VALS SDK_MrdCardState( MOTORCARDITEM Item );

/**
	@brief Repeats the card swiping to reread magnetic stripes.

	This function partially ejects the card from the reader until the card
	doesn't leave the magnetic head and immediately feed in it back to the
	reader.

	@return	orvOK: Reinsertion flows properly.<br>
	        orvDEVICEFAULT: Error has occurred.
*/
extern OS_RETURN_VALS SDK_MrdCardReinsert( void );

/**
	@brief Checks if an irreducible error occurs during card eject.

	This function checks the motor handler for an irreducible error. It
	can be during card ejecting, if handler could not eject card from
	reader by some technical issue.

	@return	orvOK: Success.<br>
	        orvDEVICEFAULT: Error has occurred.
*/
extern OS_RETURN_VALS SDK_MrdCardError( void );
#endif  // MAKE_H2200

#if INCLPRT | INCLLPT
#if !defined(MAKE_P2100) && !defined(H2200)

/**
	@brief Opens the printer device.

	This function opens and configures a specific printer device.

	@param Printer Printer type to open.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PrinterOpen( PRINTER Printer );

/**
	@example SDK_PrinterOpen.c
	This is an example of how to use the SDK_PrinterOpen() function.
*/

/**
	@deprecated This is an obsolete function not recommended for
	            development. All new development should use
	            SDK_PrintString() or SDK_PrintBuffer().

	@brief Prints the data on a given port.

	This function simply prints the data pointed to by pBuffer. The number
	of bytes of data to be printed is indicated by BufLen. Since the length
	is specified, zeros or NULLs can be sent to the printer. This is most
	useful for printing graphics. The printer must be opened before this
	function works properly. 

	@param pBuffer Pointer to the data to print; can contain NULLs.
	@param BufLen Length of the buffer to print.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PrintData( UBYTE *pBuffer, UWORD BufLen );

/**
	@brief Define font for specified set of printer character.

	@param nChar Character of set to be defined.
	@param pData Pointer to bytes of data defining the font.

	@return See @ref OS_RETURN_VALS.

	@note Supported on I/F-COM firmware only.
*/
extern OS_RETURN_VALS SDK_PrinterDefChar( BYTE nChar, UBYTE* pData );

/**
	@brief Sends the given data buffer to the printer.

	This function is used to print data pointed to by pBuffer. The number
	of bytes of data to be printed is indicated by BufLen. Since the length
	is specified, zeros or NULLs can be sent to the printer. This function
	is useful for printing graphics and for changing special printer
	settings such as fonts. The printer must be opened before this function
	works properly.

	@param pBuffer Pointer to the data to print; can contain NULLs.
	@param BufLen Length of the buffer to print.

	@return See @ref OS_RETURN_VALS.
	@note	Use SDK_PrinterOpen() first.
*/
extern OS_RETURN_VALS SDK_PrintBuffer( UBYTE *pBuffer, UWORD BufLen );

/**
	@example SDK_PrintBuffer.c
	This is an example of how to use the SDK_PrintBuffer() function.
*/

/**
	@brief Prints the given null-terminated string to the printer.

	This function is used to print a given string, which must be null-terminated.

	@param pBuffer Pointer to a null-terminated string.

	@return See @ref OS_RETURN_VALS.
	@note	Use SDK_PrinterOpen() first.
*/
extern OS_RETURN_VALS SDK_PrintString( char *pBuffer );

/**
	@brief Close the printer device.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PrinterClose( void );

/**
	@brief Checks to see if the printer is busy.

	@return	Non zero: Printer is busy.<br>
	        Zero: Printer is not busy.
	@note	Use SDK_PrinterOpen() first.
*/
extern UBYTE SDK_PrinterBusy( void );

/**
	@brief Checks the paper status of the printer.

	This function returns the paper status of the printer. The status
	placed in *pStat, which is a PRTSTATUS type variable.

	The printer must be open using SDK_PrinterOpen() before you use this
	function. If the printer is not opened, SDK_PrinterStatus() returns the
	OS_RETURN_VALS of orvDEVICENOTOPEN. If the printer is open, this
	function always returns orvOK and accurately reports the status in
	*pStat. Therefore, if you know the printer is open, it is not necessary
	to check the return value of this function.

	@param pStat - location where the paper status is stored

	@return See @ref OS_RETURN_VALS.

	@note prt_PAPER_LOW is not supported in currently available printers.
*/
extern OS_RETURN_VALS SDK_PrinterStatus( PRTSTATUS *pStat );

/**
	@brief Prints an image from a buffer

	This function prints an image from a buffer. The function is the same
	as SDK_PrintFileImage() except that the image data is stored in a
	buffer rather than a file. You must open the printer with
	SDK_PrinterOpen() before using this function. Caller should flush the
	print buffer before calling this function.

	@param page Page number where the image data resides.
	@param address Pointer to the image data buffer.

	@return See @ref PRT_IMAGE_RETVAL.

	@see SDK_PrintFileImage
*/
PRT_IMAGE_RETVAL SDK_PrintImage( PAGE_NUMBERS page, UBYTE *address );

/**
	@example SDK_PrintImage.c
	This is an example of how to use the SDK_PrintImage() function.
*/

/**
	@brief Prints an image from a file. 

	This function prints an image from a file. You must open the printer
	with SDK_PrinterOpen() before using this function.

	Image format: @include ImageFormat.txt

	prti_INVALID_IMAGE is returned for one or more of the following
	conditions:<ul>
	<li>Invalid file handle.</li>
	<li>Missing data in the image header.</li>
	<li>Column width incompatible with the selected printer.</li></ul>
	
	@param *handle - handler of the file with the image data

	@return See @ref PRT_IMAGE_RETVAL.

	@see SDK_PrintImage
*/
PRT_IMAGE_RETVAL SDK_PrintFileImage( FILE *handle );

/**
	@example SDK_PrintFileImage.c
	This is an example of how to use the SDK_PrintFileImage() function.
*/

#if defined(MAKE_M2)
/** 	@brief	Send a form feed to the MT printer
	@return	orvOK on success else OS error value
	@note
*/
extern OS_RETURN_VALS SDK_MTFormFeed( void );
#endif //MAKE_M2

#endif // !defined(MAKE_P2100) && !defined(H2200)
#endif // INCLLPT | INCLPRT

#if INCLKBD

/**
	@brief Gets the type of the keyboard.

	This function is used to determine the type of keyboard connected to
	the system.

	@param pType Pointer to the location where the keyboard type is to be
	             placed.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_KbdType( KBDTYPE *pType );

/**
	@brief Configures the Paper Feed key.

        This function closes the keyboard. If any keyboard error occurred, this
	function can be used to reset the keyboard.

	@param ScanCode Scan code of Paper Feed key.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_KbdPaperFeed( UBYTE ScanCode );

/** 	@brief Open, configure, and read the keypad (blocking).

	Opens keyboard device, configures the translation KeyMap, and returns
	the first key read.

	@param pBuffer Pointer to storage area.
	@param pBufLen Pointer to length area.
	@param pKeyMask	Translation keymap.
	@param Timeout Time to wait for read.
	@param pStatus pointer to status return data.

	@return See @ref OS_RETURN_VALS.

	@note If pKeyMask is NULL, raw scan codes are returned.

	@see SDK_StartReadKey
*/
extern OS_RETURN_VALS SDK_GetKey( UBYTE *pBuffer,
				  UWORD *pBufLen,
				  const KEYMASK *pKeyMask,
				  OS_TIMES Timeout,
				  IOSTATUS *pStatus );

/**
	@example SDK_GetKey.c
	This is an example of how to use the SDK_GetKey() function.
*/

/**
	@brief Resets or closes the keyboard.

        This function resets or closes the keyboard. If any keyboard error
	occurred, this function is used to reset it.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_KbdClose( void );

/**
	@brief Open, configure, and read the keypad (non-blocking).

	This function opens and configures the keypad. This function also tells
	the operating system to read the next key press.

        When SDK_StartReadKey() is called, the function immediately returns.
	The CompleteFlag parameter of the IOSTATUS structure is set to non-zero
	when a key press is available or a critical error occurs. In typical
	use, the function is called and the caller simply loops until the
	CompleteFlag is non-zero.

        After the CompleteFlag is set, the caller should check the Error
	parameter of the IOSTATUS structure. If Error is set to orvOK, the key
	press is valid.

        For this function, Error is always orvOK unless a serious operating
	system failure occurred.
        
        The SDK_StartReadKey function automatically clears the CompleteFlag
	when called. Therefore, user initialization is not required.

        The Error parameter of the IOSTATUS structure should be set to orvOK
	during the initialization process. In normal use, the SDK_StartReadKey
	function would be called once at the start of an application.
	Additional calls to SDK_ReadKey() would be used to retrieve keys other
	than the first one.

	@param pKey Location where the key is to be stored.
	@param pKeyMask Optional pointer to the key mapping data area.
	@param pStatus Pointer to the status return data.

	@return See @ref OS_RETURN_VALS.

	@note The keypad is configured based on the pKeyMask parameter. If
	      pKeyMask is NULL, the keypad is configured in the NORMAL mode and
	      scan codes are returned in the location pointed to by pKey.

	      If pKeyMask points to a key map data area, then the keys are
	      mapped as specified.

	      Example of a KEYMASK structure:

	KEYMASK const KEYMSK = 
	{
		DISABLED, DISABLED, UP_KY,    DOWN_KY, CANCEL_KY, SELECT_KY, DISABLED,
		DISABLED, DISABLED, '3',      '2',      '1',      DISABLED, DISABLED,
		DISABLED, DISABLED, '6',      '5',      '4',      DISABLED, DISABLED,
		DISABLED, DISABLED, '9',      '8',      '7',      DISABLED, DISABLED,
		DISABLED, DISABLED, ENTER_KY, '0',      CLEAR_KY, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
		DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED
	};

	@see SDK_GetKey, SDK_ReadKey
*/
extern OS_RETURN_VALS SDK_StartReadKey( UBYTE    *pKey,
                                        const KEYMASK  *pKeyMask,
                                        IOSTATUS *pStatus );

/**
	@example SDK_StartReadKey.c
	This is an example of how to use the SDK_StartReadKey() function.
*/

/**
	@brief Reads the keypad.

	This function tells the operating system to read the next key pressed.
	You must call SDK_StartReadKey() before using this function. 

	When called, this function immediately returns. The CompleteFlag
	parameter of the IOSTATUS structure is set to non-zero when a key press
	is available or a critical error occurred. 

	In typical use, this function is called, and the caller loops until the
	CompleteFlag is non-zero. If a key press is available in the operating
	system buffer, this function sets the CompleteFlag and immediately
	returns with pKey set to the key value.

	After CompleteFlag is set, the caller should check the Error parameter
	of the IOSTATUS structure. If Error is set to orvOK, the key press is
	valid. For this function, Error can always be orvOK unless a serious
	operating system failure occurs. 

	The SDK_ReadKey() function automatically clears CompleteFlag when
	called; therefore, user initialization is not required. The Error
	parameter of the IOSTATUS structure is set to orvOK. In normal use,
	this function is called once at the start of an application. Later
	calls to SDK_ReadKey() are used to retrieve keys other than the first
	one. 

        @param pKey Location where the key is to be stored.
	@param pStatus Pointer to the status return data.

	@return See @ref OS_RETURN_VALS.

        @note The operating system automatically buffers a number of key
	      presses. Therefore, key presses are not lost between calls to the
	      SDK_ReadKey() function.

	      The key buffer can be flushed by looping with SDK_ReadKey() calls
	      or by closing the keypad with SDK_KbdClose() and re-opening it
	      with SDK_StartReadKey().

	@see SDK_StartReadKey
*/
extern OS_RETURN_VALS SDK_ReadKey( UBYTE *pKey, IOSTATUS *pStatus );

/**
	@example SDK_ReadKey.c
	This is an example of how to use the SDK_ReadKey() function.
*/

#endif // INCLKBD

#if INCLRTC

/**
	@brief Set the real-time clock.

        This function sends data to the real-time clock (RTC) port. The size of
	the buffer for this port is 14 ASCII characters. The format of the
	buffer written to the RTC is YYMMDDHHMMSSAW, where: A is A for A.M. and
	P for P.M. in a 12-hour mode; W is 0..6 for day of the week.

	Format example: 991215012300A1 means 01:23:00 AM on Monday, December
	15, 1999.

	@param pBuffer Pointer to data to write.
	@param BufLen Length of data to write.

	@note If buffer is shorter than 14 characters the RTC will be read
	      back before and then first BufLen characters will be updated.

	@return See @ref OS_RETURN_VALS.

	@see SDK_RtcRead, SDK_RtcConfig
*/
extern OS_RETURN_VALS SDK_RtcWrite( UBYTE *pBuffer, UWORD BufLen );

/**
	@brief Read data from the real-time clock.

        This function is used to read data from the real-time clock port and
	store it in an ASCII buffer. The format of the 14-character ASCII
	buffer read from the RTC is YYMMDDHHMMSSAW, where: A is A for A.M. and
	P for P.M. in a 12-hour mode; W is 0..6 for day of the week.

	@param pBuffer Pointer to storage area.

	@return See @ref OS_RETURN_VALS.

	@see SDK_RtcWrite, SDK_RtcConfig
*/

extern OS_RETURN_VALS SDK_RtcRead( UBYTE *pBuffer );

/**
	@brief Sets the real-time clock mode.

	This function sets the real-time clock (RTC) mode to either a 12-hour
	clock or a 24-hour clock.

	@param Mode 12 or 24 hour clock

	@return See @ref OS_RETURN_VALS.

	@see SDK_RtcWrite, SDK_RtcRead
*/
extern OS_RETURN_VALS SDK_RtcConfig( RTCTYPE Mode );

#endif // INCLRTC

#if INCLBUZ

/**
	@brief Turn the beeper on for a given duration (non-blocking).

	Same as SDK_Beeper() except that this function returns immediately and
	does not wait for beep to complete.

  	@param Duration The time to keep beeper ON in units of OS_TIMES as
	                defined in sdk.h.
*/
extern void SDK_BeeperNoWait( UBYTE Duration );

/**
	@brief Turn the beeper on for a given duration (blocking).

	This function turns the beeper on for a given duration and returns
	after the specified time expires.

  	@param Duration The time to keep beeper ON in units of OS_TIMES as
	                defined in sdk.h.
*/
extern  void SDK_Beeper( UBYTE Duration );

/**
	@example SDK_Beeper.c
	This is an example of how to use the SDK_Beeper() function.
*/

/**
	@brief Beeps a specific number of times.

	This function causes the beeper to sound a given number of times. Each
	beep lasts 120 ms followed by 120 ms of silence. The function returns
	after all beeps are completed.

  	@param Beeps The number of beeps.
*/
extern void SDK_BeepIt( UBYTE Beeps );

/**
	@brief Resets the beeper device.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_BeeperReset( void );

#endif // INCLBUZ

#if INCLPRT

/**
	@brief Reads from the RS232 port.

	This function is used to open and read from the RS232 port.

	@param pBuffer Pointer to storage area.
	@param pBufLen Pointer to length area.
	@param Timeout Time to wait for read.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_RS232Read( UBYTE *pBuffer,
				     UWORD *pBufLen,
				     OS_TIMES Timeout,
				     IOSTATUS *pStatus );

/**
	@example SDK_RS232Read.c
	This is an example of how to use the SDK_RS232Read() function.
*/

/**
	@brief Writes to the RS232 port

	@param pBuffer Pointer to data to output.
	@param BufLen Length of data.
	@param Timeout Time to wait for write.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_RS232Write( UBYTE *pBuffer,
				      UWORD BufLen,
				      OS_TIMES Timeout,
				      IOSTATUS *pStatus );

/**
	@example SDK_RS232Write.c
	This is an example of how to use the SDK_RS232Write() function.
*/

/**
	@brief Opens and configures the RS232 port.

        This function is used to open and configure the RS232 port. The pData
	parameter is ignored for modes other than rm_Configurable.

	@param Mode Type of RS232 configuration
	@param pData If mode is rm_Configurable, specifies protocol, baud rate,
	             and data format

	@return See @ref OS_RETURN_VALS.

        @note The mode rm_Configurable is not available for the T7 version of
	      this function.
*/
extern OS_RETURN_VALS SDK_RS232Open( RS232MODE Mode, RS232DATA *pData );

/**
	@example SDK_RS232Open.c
	This is an example of how to use the SDK_RS232Open() function.
*/

/**
	@brief Closes the RS232 port

	This function is used to reset the RS232 port

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_RS232Close( void );

/**
	@brief Resets the RS232 port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_RS232Reset( void );

/**
	@brief Get RS232 port info.

	Gets information or status regarding various aspects of the
	communication port.

	This function is used to open and read the status of the RS232 port.

	@param Type Type of information to get (see @ref RS232INFO).

	@return See @ref OS_RETURN_VALS.

    @note There is a confusion related to old SDK
        compatibility. Old SDK guessed that a terminal running
        as DCE and ri_DEVICESTATUS means as DTR(DCE) although
        getting status CTS line of serial port. This function
        is able to retrieve status of real serial flow control
        signals (CTS[ri_CTS],DSR[ri_DSR]) but DSR line is
        shared by OptimumX hardware with different port and if
        given argument addresses to DSR first time than the
        hardware multiplexer automatically is configuring to use
        DSR/DTR lines and switching off these signals from
        different serial port. Also exists problem with
        Hypercom 810162-001 cable using. This cable has DSR (pin
        9) shorted with pin 5 and in that case DSR does not
        work. CTS/RTS signals with that cable work properly.
*/
extern OS_RETURN_VALS SDK_RS232Info( RS232INFO Type );
#if defined(MAKE_OPTIMUM)
/**
	@brief Sets the DSR/CTS signal state.

    Sets the DSR/CTS signal state of the communication port.

	@param nState Type of information to get (see @ref RS232INFO).

	@return See @ref OS_RETURN_VALS.

   @note There is a confusion related to old SDK compatibility.
         Old SDK guessed that a terminal running as DCE and
         RS232_SET_DSR_LOW/RS232_SET_DSR_HIGH means access to
         DSR(DCE) although affecting RTS state line of serial
         port. This function is able to modify state of real
         serial flow control signals
         (RTS[RS232_SET_RTS_LOW/RS232_SET_RTS_HIGH] and
         DTR[RS232_SET_DTR_LOW/RS232_SET_DTR_HIGH]) but DTR line
         is shared by OptimumX hardware with different port and
         if given argument addresses to DTR first time than the
         hardware multiplexer automatically is configuring to
         use DSR/DTR lines and switching off these signals from
         different serial port.
*/
extern OS_RETURN_VALS SDK_RS232SetDSR( RS232_SET_DSR setdsr );
#endif // MAKE_OPTIMUM

#endif // INCLPRT

#if INCLTGI

/**
	@brief Open TGI port for read.

	@param pBuffer Pointer to storage area.
	@param pBufLen Pointer to length area.
	@param Timeout Time to wait for read.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIRead( UBYTE *pBuffer,
				   UWORD *pBufLen,
				   OS_TIMES Timeout,
				   IOSTATUS *pStatus );

/**
	@brief Set a write on the TGI port.

	@param pBuffer Pointer to data to output.
	@param BufLen Length of data.
	@param Timeout Time to wait for write.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIWrite( UBYTE *pBuffer,
				    UWORD BufLen,
				    OS_TIMES Timeout,
				    IOSTATUS *pStatus );

#if defined(MAKE_ICE6000) && !defined(MAKE_VHDT)

/**
	@brief Opens and confige the TGI port 

	@param pTGIdata Pointer to TGIPRC structure as defined in tgidev.inc.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIOpen( TGIPRC *pTGIdata );

#elif defined( MAKE_ICE5000 )

/**
	@brief Opens and confige the TGI port

	@param Mode Type of TGI configuration.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIOpen( TGIMODE Mode );

#endif

/**
	@brief Closes the TGI port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIClose( void );

/**
	@brief Resets the TGI port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGIReset( void );

#ifdef MAKE_ICE6000

/**
	@brief Sets polling address for TGI port

	@param UBYTE PollingAddr - destried polling address

	@note Set polling address after TGI port is open, if this function is
	      not called, the default polling address of 0x68 will be in effect
	      orvINVCONFIGURATION will be returned if polling address is not
	      0x64, 0x65, ox68, or 0x69.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_TGISetPollingAddr( UBYTE PollingAddr );

#endif

#endif // INCLTGI

#if INCLZ8I

/**
	@brief Open the Z8I port.

	@return See @ref OS_RETURN_VALS.
*/ 
extern OS_RETURN_VALS SDK_Z8IOpen( void );

/**
	@brief Close the Z8I port.

	@return See @ref OS_RETURN_VALS.
*/ 
extern OS_RETURN_VALS SDK_Z8IClose( void );

/**
	@brief Performs a Z8 Function as specified by the Z8 specifications.

	@param pSendBuffer Pointer to buffer to send.
	@param SendLen Length of the buffer to send.
	@param pRecvBuffer Pointer to buffer to save data.
	@param pRecvLen Pointer to length of receive buffer.
	@param Timeout Length of time to wait for response.
	@param pStatus Pointer to status indicator.

	@return See @ref OS_RETURN_VALS.

	@note pRecvLen must be set to the length of the receive buffer on the
	      call.  
*/ 
extern OS_RETURN_VALS SDK_Z8IFunction( UBYTE *pSendBuffer,
				       UWORD SendLen,
				       UBYTE *pRecvBuffer,
				       UWORD *pRecvLen,
				       OS_TIMES Timeout,
				       IOSTATUS *pStatus );

/**
	@brief Resets the Z8I port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_Z8IReset( void );

#endif // INCLZ8I

#if INCLLAN

/**
	@brief Open Lan port for read.

	@param pBuffer Pointer to storage area.
	@param pBufLen Pointer to length area.
	@param Timeout Time to wait for read.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_LanRead( UBYTE *pBuffer,
				   UWORD *pBufLen,
				   OS_TIMES Timeout,
				   IOSTATUS *pStatus );

/**
	@brief Set a write on the Lan port

	@param pBuffer Pointer to data to output
	@param BufLen Length of data
	@param Timeout Time to wait for write
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_LanWrite( UBYTE *pBuffer,
				    UWORD BufLen,
				    OS_TIMES Timeout,
				    IOSTATUS *pStatus );

/**
	@brief Opens and confige the Lan port

	@param Mode Type of Lan configuration.
	@param PollId Polling id.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_LanOpen( LANMODE Mode, UBYTE PollId );

/**
	@brief Closes the Lan port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_LanClose( void );

/**
	@brief Resets the Lan port.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_LanReset( void );

#endif // INCLLAN

#if INCLPAD

/**
	@brief Opens and resets the touch pad.

        This function opens and resets the screen touch pad. If the pad is already open, this
        function simply returns to the caller.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PadOpen( void );

/**
	@brief Closes the screen touch pad.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_PadClose( void );

/**
	@brief Gets the coordinates and pen state for the touch-pad. 

	This function places pad coordinates and the pen state into the @ref
	PADDATA structure pointed to by the pPadData function parameter. This
	function is only useful in conjunction with SDK_SetPad(). After a call
	to SDK_SetPad(), the user waits for a flag to be set. After the flag is
	set, valid pad data can be retrieved using SDK_GetPad().
	
        The X_Pos and Y_Pos members provide the coordinates of the location
	that was touched. The PenState member is set to PAD_PEN_UP or
	PAD_PEN_DOWN as defined in SDKIO.H.

        If PenState is PAD_PEN_UP, X_Pos and Y_Pos are always zero. X_Pos and
	Y_Pos are always relative to the full screen. The virtual window has no
	effect.

	@param pBuffer Pointer to user's @ref PADDATA structure.
*/
extern void SDK_GetPad( PADDATA *pBuffer );

/**
	@example SDK_GetPad.c
	This is an example of how to use the SDK_GetPad() function.
*/
                                  
/**
	@brief Calibrates the screen touch pad.

        This function calibrates the screen touch pad. SDK_PadCalib() should be
	called once at the start of an application before using other pad
	functions.
*/
extern UBYTE SDK_PadCalib( void );

/**
	@brief Sets the sensitivity of the screen touch pad.

        This function sets the sensitivity of the screen touch pad. The
	sensitivity values define how much the x-axis and the y-axis touch pad
	values have to change before the pad driver detects a new touch pad
	position. This applies while the pen is in continuous contact with the
	pad.

	@param Sense_X X-axis sensitivity.
	@param Sense_Y Y-axis sensitivity.

	@return Status of the configuration.
*/
extern UBYTE SDK_PadSense( UBYTE Sense_X, UBYTE Sense_Y );

/**
	@brief Configure the screen touch pad.

	Configures the screen touch pad to set a flag when the pad is touched,
	an existing touch is moved, and a touch is removed from the pad.

        This function configures the screen touch pad to set a flag when the
	pad is touched, an existing touch is moved, or a touch is removed from
	the pad. The flag is also set when the function is completed due to an
	error or timeout. The set flag is part of the IOSTATUS structure as
	passed by pointer to the function. The IOSTATUS structure also contains
	an error code.

        When the CompleteFlag is set and Error is set to orvOK, the user can
	use SDK_GetPad() to retrieve valid coordinates and pen state of a touch
	or touch removal. For a touch removal, the coordinates are always 0,0,
	and the pen state is set to PAD_PEN_UP as defined in SDKIO.H.

	If the pad is touched, the coordinates contain the exact point touched,
	and the pen state is set to PAD_PEN_DOWN.

	SDK_SetPad() operates differently depending on whether the pad is
	OPENED or CLOSED.

	When the function is first called:<ul>
        <li>If the pad is closed when this function is called, SDK_SetPad()
	automatically opens and resets the pad. Under these conditions, the
	next pad touch sets the flag defined in IOSTATUS to 1.</li>
        <li>If the pad is being touched when the initial SDK_SetPad() call is
	made, the function sets the flag immediately and returns the
	coordinates.</li></ul>

	When the function is called for the second time:<ul>
        <li>The next touch, the next touch move, or the next touch removal sets
	the flag. A call to SDK_PadClose() ensures that the next call to
	SDK_SetPad() is the initial call.</li></ul>

	If the SDK_SetPad wait time expires:<ul>
        <li>If the function wait time (set by the timeout parameter) expires,
	the flag is set, and  the error code is set to
	orvDEVICETIMEOUT.</li></ul>

        SDK_SetPad() automatically clears the flag in the IOSTATUS structure to
	zero. Therefore, the caller does not need to clear the flag before
	passing the function.

        @param Timeout Time to wait for touch pad action in units of OS_TIMES
	               as defined in sdk.h. Set to 0 for no timeout period. In
	               this case, the function waits indefinitely for pad
	               action.
        @param pStatus Pointer IOSTATUS structure that contains the error code
	               and flag to be set by this function.

	@return See @ref OS_RETURN_VALS.

	@see SDK_GetPad
*/
extern OS_RETURN_VALS SDK_SetPad( OS_TIMES TimeOut, IOSTATUS *pStatus );

/**
	@example SDK_SetPad.c
	This is an example of how to use the SDK_SetPad() function.
*/

/**
	@brief Sets the signature scale used to format captured signature data.

        Sets the signature scale used to format the signature capture data as
	collected by SDK_StartSigCapture(). Only applies to format of captured
	data. There is absolutely no affect on display resolution.

	pSigScale points to the @ref SIGSCAL structure which contains the scale
	resolution values.

        Signature capture is part the touch pad device functionality. Before
	using this function, SDK_OpenPad() should be called.

        The function also enables the higher resolution signature scaling
	format which changes the header of the signature data. The original
	format can only be recovered by closing the touch pad with
	SDK_PadClose().

	@param pSigScale Pointer to SIG_RES_DEF structure

	@return	orvOK: Scaled signature resolution was successfully set.<br>
		orvDEVICENOTOPEN: Device not open error.<br>
		orvINVDATA: Scaled signature resolution record is invalid.

	@note Used to enable the Higher Resolution Signature Capture scaling.
	Default values for resolution and extende line limits are:<ul>
	<li>Scaled X-Axis Resolution - 640</li>
	<li>Scaled Y-Axis Resolution - 128</li>
	<li>X-Axis Extended Line Limit - 1</li>
	<li>Y-Axis Extended Line Limit - 0</li></ul>
*/
OS_RETURN_VALS SDK_SetSignatureScale( SIG_RES_DEF *pSigScale );

/**
	@brief Configures the screen touch pad to read a signature

        This function configures the touch pad to capture a signature. The
	capture begins when the pen first touches the pad. The signature data
	is captured and shown on the screen as it is drawn.

        The signature is completed when the pen is not touching the pad for
	more than the pen-up timeout or when the end of the data buffer is
	reached. The pen-up timeout, data buffer location, and data buffer
	length are specified in the SIGCAPPARMS structure. See the Notes
	section after the Example for more information on the structure of
	SIGCAPPARMS.

        When the signature capture is completed, a flag is set. The flag is
	also set when the overall time specified by TimeOut expires. The set
	flag is part of the IOSTATUS structure as passed to the function. The
	IOSTATUS structure also contains an error code.

        If the signature capture is completed due to the pen-up timeout, the
	error is set to orvOK. If the end of the buffer is reached, the error
	code is set to orvBUFFEROVERFLOW. If the signature capture is completed
	due to an overall timeout, the error code is set to orvDEVICETIMEOUT.
	Error codes are defined in sdk.h.

        SDK_StartSigCapture() automatically clears the flag in the IOSTATUS structure to zero.
        Therefore, the caller does not need to clear the flag before passing to the function.

        The signature capture can be completed immediately by calling SDK_StopSigCapture(). Also if
        a signature ends due to an overall timeout as indicated by an error or orvDEVICETIMEOUT,
        SDK_StopSigCapture() must be called to properly end the signature capture session. If the
        signature capture is completed and the error flag is anything but orvDEVICETIMEOUT, it is
        not necessary to call SDK_StopSigCapture().

        IMPORTANT: If a signature capture ends due to an overall timeout as specified by the TimeOut
        parameter, SDK_StopSigCapture() must be called to properly complete the signature capture
        session. Otherwise, improper operation will occur during the next call to
        SDK_StartSigCapture() An overall timeout is denoted when the error code is set to
        orvDEVICETIMEOUT. If the error code is set to anything other than orvDEVICETIMEOUT, you do
        not need to SDK_StopSigCapture().

        Often when a signature is required, buttons on the screen are used to redo the signature or
        confirm completion. This can be done easily as long as the signature capture area set in the
        SIGCAPPARMS structure does not overlap an area where buttons are to be placed. To read the
        touch pad outside of the signature area, you can use SDK_SetPad() and SDK_GetPad() in the
        normal way.

        In typical use, a call is made to SDK_SetPad() followed by a call to SDK_StartSigCapture().
        A loop is then used to wait for completion of either of the two functions.

        @param  pSigCap - pointer to SIGCAPPARMS structure, which specifies the data buffer
		        location, buffer length, capture area, and pen up timeout
        @param  TimeOut - time to wait for the signature capture action to complete in units of
		        OS_TIMES as defined in sdk.h. If set to 0, a timeout is infinite. In this case, the function
		        waits indefinitely for a buffer overflow or a pen up timeout
        @param  pStatus - pointer to the IOSTATUS that contains the flag to be set by this function
		        and the error code
        @param  pDataSize - pointer to the user data location where the length of the captured
		        signature data is returned. The length is in units of bytes

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_StartSigCapture( const SIGCAPPARMS *pSigCap,
                                           OS_TIMES TimeOut,
					   IOSTATUS *pStatus,UWORD *pDataSize );

/**
	@example SDK_StartSigCapture.c
	This is an example of how to use the SDK_StartSigCapture() function.
*/

/**
	@brief Stop a signature capture session.

	Stops a signature capture session started by SDK_StartSigCapture().

        A signature capture session started with SDK_StartSigCapture() is
	immediately completed by calling SDK_StopSigCapture(). The
	SDK_StopSigCapture() function not only stops a signature session, but
	also completes it. Therefore, after a call is made to
	SDK_StopSigCapture(), the Complete flag from the previous call to
	SDK_StartSigCapture() is set. From this point, the user can store any
	captured signature data.

        The length of the signature data captured is set in the pDataSize
	parameter SDK_StartSigCapture().

        @note If a signature capture ends due to an overall timeout as
	specified by the TimeOut parameter of SDK_StartSigCapture() and
	SDK_StopSigCapture() must be called to properly complete the signature
	capture session.

        Otherwise, improper operation occurs during the next call to
	SDK_StartSigCapture(). An overall timeout is denoted when the error
	code returned by SDK_StartSigCapture() is set to orvDEVICETIMEOUT.

        If the error code from SDK_StartSigCapture() is set to anything other
	than orvDEVICETIMEOUT, SDK_StopSigCapture() does not need to be called.

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_StopSigCapture( void );

/**	@example
	See the example for SDK_StartSigCapture */

#endif // INCLPAD

#if INCLRS2

/**
	@brief Reads from the RS232 port 2 on ICE6000 terminals.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param Timeout Time to wait for read
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port2Read( UBYTE *pBuffer,
                                   UWORD *pBufLen,
                                   OS_TIMES Timeout,
                                   IOSTATUS *pStatus );

/**
	@brief Writes to the RS232 port 2 on ICE 6000 terminals.

	@param pBuffer Pointer to data to output
	@param BufLen Length of data
	@param Timeout Time to wait for write
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port2Write( UBYTE *pBuffer,
                                    UWORD BufLen,
                                    OS_TIMES Timeout,
                                    IOSTATUS *pStatus );

/**
	@brief Opens and configures the RS232 port 2 on ICE 6000 terminals.

	@param Mode Type of RS232 configuration
	@param pData If mode is rm_Configurable, specifies protocol, baud rate,
	             and data format

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port2Open( RS232MODE Mode, RS232DATA *pData );

/**
	@brief Closes the RS232 port 2 on ICE6000 terminals.

	@return See @ref OS_RETURN_VALS.

	@see SDK_RS232Close()
*/
OS_RETURN_VALS SDK_RS232Port2Close( void );

/**
	@brief Resets the RS232 port 2 on ICE 6000 terminals.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port2Reset( void );

/**
	@brief Get RS232 port 2 info.

	Gets information or status regarding various aspects of the
	communication port.

	@param Type Type of information to get (see RS232INFO)

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port2Info( RS232INFO Type );

#endif // INCLRS2

#if INCLRS3

/**
	@brief Reads from the RS232 port 3 on ICE6000 terminals.

	@param pBuffer Pointer to storage area
	@param pBufLen Pointer to length area
	@param Timeout Time to wait for read
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Read( UBYTE *pBuffer,
                                   UWORD *pBufLen,
                                   OS_TIMES Timeout,
                                   IOSTATUS *pStatus );

/**
	@brief Writes to the RS232 port 3 on ICE 6000 terminals.

	@param pBuffer Pointer to data to output
	@param BufLen Length of data
	@param Timeout Time to wait for write
	@param pStatus Pointer to status return data

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Write( UBYTE *pBuffer,
                                    UWORD BufLen,
                                    OS_TIMES Timeout,
                                    IOSTATUS *pStatus );

/**
	@brief Opens and configures the RS232 port 3 on ICE 6000 terminals.

	@param Mode Type of RS232 configuration.
	@param pData If mode is rm_Configurable, specifies protocol, baud rate,
	             and data format.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Open( RS232MODE Mode, RS232DATA *pData );

/**
	@brief Closes the RS232 port 3 on ICE6000 terminals.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Close( void );

/**
	@brief Resets the RS232 port 3 on ICE 6000 terminals.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Reset( void );

/**
	@brief Get RS232 port 3 info.

	Gets information or status regarding various aspects of the
	communication port.

	@param Type Type of information to get (see RS232INFO)

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RS232Port3Info( RS232INFO Type );

#endif // INCLRS3

#if INCLATM

/**
	@brief Open the ATM device.

	ATM is the AT Modem Device used to communicate with internal modems
	using the AT command set. The device is typically used to communicate
	with wireless modems as equipped with some terminals. 

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMOpen( void );

/**
	@brief Close the ATM device

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMClose( void );

/**
	@brief Read data from the ATM device.

	The function places data in buffer pointed to by pBuffer. When the
	function is called, *BufLen is the maximum bytes of data to read. This
	function returns immediately to the caller and will set
	pStatus->CompleteFlag to non-zero when the function is completed. Upon
	completion, the function sets *Buflen to the amount of data actually
	read.

	The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when the maximum amount of data is written, a timeout
	occurs, a error occurs, or when a received packet is complete depending
	upon device. After CompleteFlag is set, the Error parameter of IOSTATUS
	can be used to check for an error or timeout. The Error parameter is
	not valid unless the CompleteFlag is set. This function automatically
	clears the CompleteFlag when first called.

	@param pBuffer Buffer.
	@param *BufLen Pointer to max length of the data to read, amount of
	               data actually read stored here on function completion.
	@param Timeout Time to wait for write completion in units of OS_Times
	               as defined in sdk.h.
	@param pStatus Pointer to the IOSTATUS structure that contains the flag
	               and error code set by this function.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMRead( UBYTE *pBuffer,
                            UWORD *pBufLen,
                            OS_TIMES Timeout,
                            IOSTATUS *pStatus );

/**
	@example SDK_ATMReadWrite.c
	This is an example of how to use SDK_ATMRead and SDK_ATMWrite functions.
*/

/**
	@brief Writes to the ATM device.

	This function writes data to ATM device.

	The function writes the data pointed to by pBuffer. This function
	returns immediately to the caller and sets pStatus->CompleteFlag to
	non-zero when the function is completed.

	The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when the data is written, a timeout occurs, or other
	error occurs. After CompleteFlag is set, the Error parameter of
	IOSTATUS can be used to check for an error or timeout. The Error
	parameter is not valid unless the CompleteFlag is set. This function
	automatically clears the CompleteFlag when first called.

	@param pBuffer Buffer
	@param BufLen Length of data to write
 	@param Timeout Time to wait for write completion in units of OS_Times
	               as defined in sdk.h.
 	@param pStatus Pointer to the IOSTATUS structure that contains the flag
	               and error code set by this function

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMWrite( UBYTE *pBuffer,
                             UWORD BufLen,
                             OS_TIMES Timeout,
                             IOSTATUS *pStatus );

/**
	@brief Performs a configuration command for the ATM device.

	This function performs configuration commands for the ATM of type
	ATM_CONFIG_CMD.

	@include SDK_ATMConfig.txt

	The function returns orvOK for successful or orvDEVICENOTOPEN for
	device not open.

	@param cmd Configuration command of type @ref ATM_CONFIG_CMD.
	@param data Byte of data as required by some commands
	@param retdata Storage location for returned data as required by some
	               configuration commands.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMConfig( ATM_CONFIG_CMD cmd, UBYTE data, UBYTE *retdata );

/**
	@brief Resets the ATM device

	This is typically used to cancel a pending read request initiated by
	SDK_ATMRead().

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_ATMReset( void );

/**
	@example SDK_ATMReset.c
	This is an example of how to use the SDK_ATMReset() function.
*/

#endif // INCLATM

#if INCLRAP

/**
	@brief Opens the RAP device.

	RAP device is used to communicate on Mobitex wireless network for ICE
	4000 terminals equipped with wireless modems.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RAPOpen( void );

/**
	@brief Close the RAP device.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RAPClose( void );

/**
	@brief Get status from RAP device.

	This function retrieves status of the RAP device.

	The function returns orvOK for successful. The function returns
	immediately. Status returned in *rstat is valid only after the
	CompleteFlag parameter of *pStatus is set.

	@note The RAP device must be opened with SDK_RAPOpen() before
	SDK_RAPGetStatus() will work correctly.

	Data is stored in *rstat which points to a structure of type @ref
	RADIOSTAT.

	The CompleteFlag parameter in the IOSTATUS structure pointed to by
	pStatus is set when the status request is complete. If an error occurs,
	the status request has failed. The Error parameter of IOSTATUS can be
	used to check for an error or timeout. The Error parameter is not valid
	unless the CompleteFlag is set. This function automatically clears the
	CompleteFlag when first called.

	For further information on data in RADIOSTAT structure, refer to
	information on RADSTAT structure in file RAPDEV.INC as supplied with
	HSDK. The following should be noted concerning the LOCMAN parameter:
	LOCMAN is four bytes which make up the decimal local radio address. The
	first byte is the most significant byte of the decimal number.

	@param rstat Pointer to structure where returned RAP status is stored.
	@param pStatus Pointer to structure that contains the flag and error
	               code set by this function.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_RAPGetStatus( RADIOSTAT *rstat, IOSTATUS *pStatus );

/**
	@example SDK_RAPGetStatus.c
	This is an example of how to use the SDK_RAPGetStatus() function.
*/

/**
	@brief Get the network name from RAP device

	Retrieves the network name from the RAP device.

	@param name Pointer to string where 8 character network name is placed.
	            The string must be 9 bytes ( 8 + 1 for NULL )

	@return See @ref OS_RETURN_VALS.

	@note The RAP device must be opened with SDK_RAPOpen() before using
	SDK_RAPGetNetworkName(). Otherwise, an orvDEVICENOTOPEN error will be
	returned by SDK_RapGetNetworkName() and name will be invalid.
*/
OS_RETURN_VALS SDK_RAPGetNetworkName( char *name );

/**
	@brief Resets the RAP device

	@return See @ref OS_RETURN_VALS.
*/
extern OS_RETURN_VALS SDK_RAPReset( void );

/**
	@brief Set RAP device to 'RAW' mode to allow direct read/write access.

	@param on Non-zero to turn on 'RAW' mode, zero to turn off 'RAW' mode.

	@return See @ref OS_RETURN_VALS.

	@note When 'RAW' mode is active, SDK_RAPWrite and SDK_RAPRead functions
	are operational. In 'RAW' mode, the device driver will not ensure data
	integrity or content checking.
*/
OS_RETURN_VALS SDK_RAPSetRawMode( UBYTE on );

/**
	@brief Read from RAP modem.

	@param pBuffer Pointer to storage area.
	@param pBufLen Pointer to length area.
	@param Timeout Time to wait for read.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.

	@note RAP device must be in 'RAW' mode for this function to work.

	@see SDK_RAPSetRawMode.
*/
extern OS_RETURN_VALS SDK_RAPRead( UBYTE    *pBuffer,
                                   UWORD    *pBufLen,
                                   OS_TIMES Timeout,
                                   IOSTATUS *pStatus );

/**	@brief Write to RAP modem.

	@param pBuffer Pointer to data to output.
	@param BufLen Length of data.
	@param Timeout Time to wait for write.
	@param pStatus Pointer to status return data.

	@return See @ref OS_RETURN_VALS.

	@note RAP device must be in 'RAW' mode for this function to work.

	@see SDK_RAPSetRawMode
*/
extern OS_RETURN_VALS SDK_RAPWrite( UBYTE    *pBuffer,
                                    UWORD    BufLen,
                                    OS_TIMES Timeout,
                                    IOSTATUS *pStatus );

#endif // INCLRAP

#ifdef MAKE_ICE4000

/**
	@brief Gets the current battery power level.

	This function sets the UBYTE pointed to by pBatLevel to the battery
	power level.

	The battery power level is a percentage between 0% and 100%. This is
	the percentage of the remaining battery power.

	@param pBatLevel Pointer to UBYTE where battery power level is to be
	                 placed.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_GetBatteryLevel(UBYTE *pBatLevel);

/**
	@brief Turns off the battery power to the ICE terminal.

        Turns off the battery power. If the terminal is running only from
	battery, then the terminal will be powered off.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_SetBatteryOff( void );

/**
	@brief Gets the current battery power charging level.

	This function sets the UBYTE pointed to by pBatChgLevel to the battery
	charging power level.

	The battery charging power level is a percentage between 0% and 100%.
	This is the percentage of the battery charging power level. Any
	percentage above zero indicates that the battery is charging.

	@param pBatChgLevel Pointer to UBYTE where battery charging power level
	                    is to be placed.

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_GetBatteryChargingLevel( UBYTE *pBatChgLevel );

/**
	@example SDK_GetBatteryChargingLevel.c
	This is an example of how to use the SDK_GetBatteryChargingLevel()
	function.
*/

/**
	@brief Sets the power switch timer value.

        Sets the amount of time the power switch has to be pressed (held down)
	before the terminal gets powered off. If time is set to zero, the
	default time of 100 milliseconds is used. The default time is also the
	time set on terminal start up.

	@param time - time that the power switch is pressed before terminal turns off in 1.66 ms units

	@return See @ref OS_RETURN_VALS.
*/
OS_RETURN_VALS SDK_SetPowerSwitchTime( UWORD time );

#endif // MAKE_ICE4000

#ifdef __cplusplus
}
#endif

/**
	@}
*/

#endif /* SDKIO */
