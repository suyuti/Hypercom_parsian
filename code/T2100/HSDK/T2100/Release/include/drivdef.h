/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	__drivdef_h
#define	__drivdef_h

#include <hyctypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

#ifdef ACK
#undef ACK
#endif

/**
	@brief Control character equates.
*/
enum CONTROL_CHARS_T {
	SOH = 0x01, 		/**< Start of header. */
	STX = 0x02, 		/**< Start of text. */
	ETX = 0x03, 		/**< End of text. */
	EOT = 0x04, 		/**< End of transmission. */
	ENQ = 0x05, 		/**< Enquiry. */
	ACK = 0x06, 		/**< Acknowledge. */
	ACK1 = 0x07,  		/**< Acknowledge 1. */
	DLE = 0x10, 		/**< Data link escape. */
	NAK = 0x15, 		/**< Negative acknowledge. */
	SYN = 0x16, 		/**< Synchronous idle. */
	ETB = 0x17, 		/**< End of transmission block. */
	IFS = 0x1C, 		/**< Inter field separator. */
	US = 0x1F,		/**< Unit separator. */
	STXp = 0x82,		/**< STX with even parity. */
	NAKp = 0x95,		/**< NAK with even parity. */
	EOTp = 0x84,		/**< EOT with even parity. */
};

/**
	@brief Dial mode definitions.
*/
enum DIALMODE_T {
	DIALMODE_DTMF = 0xAF,		/**< DTMF dialing. */
	DIALMODE_PULSE = 0xCF,		/**< Pulse dialing. */
	DIALMODE_CELL = 0xE0,		/**< AMPS CELL dialing (not supported). */
	DIALMODE_CDPD_TCP = 0xE1,	/**< CDPD TCP/IP dialing (not supported). */
	DIALMODE_CDPD_UDP = 0xE2,	/**< CDPD UDP dialing (not supported). */
	DIALMODE_RAD_MPAK = 0xE1,	/**< MPAK format radio modem dialing (not supported). */
	DIALMODE_RAD_DT4000 = 0xE1,	/**< DataTAC 4000 host format radio modem dialing (not supported). */
	DIALMODE_RAD_DT5000 = 0xE2,	/**< DataTAC 5000 host format radio modem dialing (not supported). */
	DIALMODE_RAD_DT6000 = 0xE3,	/**< DataTAC 6000 host format radio modem dialing (not supported). */
	DIALMODE_RAD_SDUP = 0xE4	/**< SDU peer->peer format radio modem dialing (not supported). */
};

/**
	@brief PABX pause definition.
*/
enum PABXPAUSE_T {
	PABX_NOPAUSE = 0xFF,     	/**< No PABX pause. */
	PABX_PAUSE = 0xBF		/**< PABX pause. */
};

/**
	@brief Telephone number structure.

	Data used to identify the telephone number.
*/
typedef struct
{
     UBYTE   DialMode;		/**< Dial mode (pulse or DTMF). */
     UBYTE   Pabx[4];		/**< PABX number (BCD format). */
     UBYTE   PabxPause;		/**< Pause after PABX (not used). */
     UBYTE   TeleNo[12];	/**< Telephone number (BCD format). */
     UBYTE   ConnectWait;	/**< Connection wait time (in seconds). */
     UBYTE   DialAttempts;	/**< Number of dial attempts. */
} TELEDATA;

/**
	@brief Dial command structure.

	Data used to configure the comms port.
*/
typedef struct dialData
{
    UBYTE    CuAddress;		/**< CU address for SDLC modem modes. */
    UBYTE    ModemMode;		/**< Modem mode (see Optimum 2100 SDK Programming Starters Guide.doc). */
    UBYTE    PulseRatio;	/**< Dial pulse ratio (0-60/40, 1-67/33,
				     other-left as default). */
    UBYTE    PulseMode;		/**< Pulse mode (not supported). */
    UBYTE    DTMFSpeed;		/**< DTMF speed (0-slow, 1-normal, 2-fast). */
    UBYTE    HookWaitTime;	/**< Hook wait time (0 - blind dialing,
				     non-zero activates line busy wait feature). */
    UBYTE    ToneWaitTime;	/**< Dial tone wait time (100 ms, rounds to the
				     nearest second). */
    UBYTE    PauseTime;		/**< Pause wait time (100 ms) (rounds to the
				     nearest second). */
    UBYTE    MinDiscTime;	/**< Minimum disconnect time (100 ms) (wait no
				     less than the time set in this parameter
				     from the line idle detected till start
				     dialing). */
    TELEDATA PriTeleNo;		/**< Primary telephone number. */
    TELEDATA SecTeleNo;		/**< Secondary telephone number. */
    UBYTE    NumofRings;	/**< Number of rings to answer after (not supported). */
    UWORD    MaxOffTime;	/**< Max time off during ring (10 milli secs) (not supported). */
    UWORD    MinRingDuration;	/**< Minimum ring duration (10 milli secs) (not supported). */
    UWORD    MaxRingDuration;	/**< Maximum ring duration (10 milli secs) (not supported). */
    UWORD    MinIdle;		/**< Minimum idle between rings (10 milli secs) (not supported). */
    UWORD    MaxIdle;		/**< Maximum idle between rings (10 milli secs) (not supported). */
    UBYTE    HoldTimeIndex;	/**< Hold time index (NAC-1 dialing only) (not supported). */
    UBYTE    LoginMode;		/**< Login mode 0-normal, 1-Tymnet (not supported). */
    UBYTE    TransmitLevel;	/**< Transmit level (in dB). */
    UBYTE    VDialRelayWait;	/**< Wait after finishing voice dialing before
				     changing the relay back (10 ms units) (not supported). */
    UBYTE    DtmfIDWTime;	/**< Interdigit pause (10 ms units) (not supported). */
    UBYTE    DtmfLevel;		/**< DTMF level (in dB) (not supported). */
    UBYTE    DtmfToneDuration;	/**< Dial tone duration time (100ms units) (not supported). */
    UBYTE    DtmfToneInt;	/**< Dial tone interruption time (1.66ms units) (not supported). */
    UBYTE    DialToneStartWait;	/**< Dial tone start wait time (not supported). */
} DIALDATA;

/**
	@brief Size of DIALDATA structure.
*/
#define DBSZE sizeof(DIALDATA)

/**
	@brief Comms module status.
*/
typedef enum
{
    cs_IDLE,		/**< Idle state. */
    cs_CHECKLINE,	/**< Checking the line: check if parallel phone handset
			     is down and line available during MinDiscTime
			     before start dialing. */
    cs_WAITLINE,	/**< Waiting for the line: handset is up or no line
			     attached. */
    cs_DIALTONE,	/**< Waiting for a dial tone: check if there's dial
			     tone in line and till starting number dialing (not
			     supported). */
#ifdef MAKE_OPTIMUM
    cs_CONNECTING,      /**< Sent ATDT to modem to connect. */
#endif
    cs_DIALING,		/**< Dialing: from ATD command issue till connect or
			     error status. */
    cs_ANSWER,		/**< Waiting for answer: from ATA command issue till
			     connect or error status (not supported). */
    cs_SYNCHING,	/**< Synchronizing with other modem: from finishing
			     dialing till connect or error status (not
			     supported). */
#ifdef MAKE_OPTIMUM
    cs_CONNECTED,       /**< Modems connected. */
#endif
    cs_RECEIVING,	/**< Communication in progress. */
    cs_SENDING,		/**< Logon to network: from connect till the end of
			     chat script. */
#ifdef MAKE_OPTIMUM
    cs_DISCONNECTING,   /** Disconnecting modem. */
#endif
    cs_REDIAL,		/**< Redialing: same as cs_DIALING but after the first
			     try. */
    cs_HANDSETUP,	/**< User is asked to raised handset for start of voice
			     dialing. */
    cs_INITIALIZING,	/**< Initializing modem. */
    cs_PPPCONNECTING,	/**< Connecting to peer via PPP. */
    cs_TCPCONNECTING,	/**< Connecting to peer via TCP/IP. */
    cs_SSLCONNECTING,	/**< Connecting to peer via SSL (not supported). */
    cs_PAGING,		/**< Radio paging. */
    cs_GATECONNECTING	/**< D2100 gate establishing connection (available only
			     via direct interface to HPOSFC library). */
} COMSTATUS;

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __drivdef_h
