/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __prtdev_h
#define __prtdev_h

#include <execdef.inc>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DOXYGEN
#define __attribute__(A)
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Config mode equates (IOCSMODE) for PRT device.
*/
enum PRT_IOCSMODE {
	PRT_HYPP = 0x00,	/**< @brief Hypercom serial printer protocol.

	This config command is used to select the Hypercom serial printer
	protocol using the following communication parameters:

	@arg Baud rate: 4800
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 2
	@arg Control characters: ACK NAK EOT

	The Hypercom serial printer protocol uses the following message framing
	for transmit messages. If the last byte of the message data is a CR or
	LF, it will be overwritten by the ETX. This Protocol waits for the PRT
	Port's /DTR Signal to be Low before it will transmit a message.

	STX {data} ETX LRC

	The Hypercom serial printer protocol does not support receiving framed
	messages, it only looks for the ACK, NAK, & EOT Control Characters. The
	EOT Control Character is treated like a NAK.
				*/

	PRT_DTRSTS = 0x01,	/**< Return /DTR signal status (not supported). */
	PRT_CHKRD = 0x02,	/**< @brief Check reader protocol (not supported).

	This config command is used to select the check reader protocol using the
	following communication parameters.

	@arg Baud rate: 9600
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 1
	@arg Control characters: none

	The check reader protocol does not use any message framing for transmit.

	The check reader protocol uses the following message framing for
	receive. Messages are terminated by a LF (0x0A).

	{data} LF
				*/
	PRT_IRT = 0x03,		/**< @brief IRT - post office protocol.

	This config command is used to select the IRT post office protocol
	which is just the transparent data protocol using the following
	communication parameters.

	@arg Baud rate: 4800
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 1
	@arg Control characters: none

	The transparent data protocol does not use any message framing or
	control characters. This protocol detects the end of a message by
	having 60 ms of quiet after the last data byte.
				*/
	PRT_TM290 = 0x04,	/**< @brief Epson TM-290 slip printer protocol (not supported).

	This config command is used to select the Epson TM-290 slip printer
	protocol which is just the transparent data protocol using the
	following communication parameters.

	@arg Baud rate: 9600
	@arg Data bits: 8
	@arg Parity: none
	@arg Stop bits: 1
	@arg Control characters: none

	The transparent data protocol does not use any message framing or
	control characters. This protocol detects the end of a message by
	having 60 ms of quiet after the last data byte.
				*/
	PRT_POSMINI = 0x05,	/**< @brief POS / MiniNAC protocol.

	This config command is used to select the POS / MiniNAC protocol using
	the following communication parameters.

	@arg Baud rate: 9600
	@arg Data bits: 8
	@arg Parity: none
	@arg Stop bits: 1
	@arg Control characters: ACK NAK

	The POS / MiniNAC protocol uses the following message framing.
	LLLL is a 2 byte BCD length (length only includes the data).

	STX LLLL {data} ETX LRC
				*/
	PRT_VF250 = 0x06,	/**< @brief Verifone 250 roll printer protocol (not supported).

	This config command is used to select the Verifone 250 roll printer
	protocol using the following communication parameters.

	@arg Baud rate: 9600
	@arg Data bits: 8
	@arg Parity: none
	@arg Stop bits: 1
	@arg Control characters: none

	The Verifone 250 roll printer protocol uses the following message
	framing for transmit messages. The transmit request from the
	application can contain multiple transmit messages, with each message
	being terminated by a LF. If the transmit request contains multiple
	transmit messages, the protocol delays 40 ms between transmit messages.
	This protocol waits for the PRT Port's /DTR signal to be low before it
	will transmit each data byte of the transmit message. Messages are
	terminated by a LF (0x0A).

	{data} LF

	The Verifone 250 roll printer protocol does not support receiving any
	data.
				*/
	PRT_IX50 = 0x07,	/**< @brief IX50 smart card protocol.

	This config command is used to select the IX50 smart card protocol
	which is just the transparent data protocol using the following
	communication parameters.

	@arg Baud rate: 9600
	@arg Data bits: 8
	@arg Parity: none
	@arg Stop bits: 2
	@arg Control characters: none

	The transparent data protocol does not use any message framing or
	control characters. This protocol detects the end of a message by
	having 60 ms of quiet after the last data byte.
				*/
	PRT_PST150 = 0x08,	/**< @brief PST150 ECR protocol.

	This config command is used to select the PST150 ECR device protocol
	which is just the ECR device protocol using the following communication
	parameters.

	@arg Baud rate: 9600
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 1
	@arg Control characters: ACK NAK

	The ECR Device Protocol uses the following message framing.

	STX {data} ETX LRC
				*/
	PRT_OKI182 = 0x09,	/**< @brief Okidata 182 printer protocol.

	This config command is used to select the Okidata 182 printer protocol
	which is just the transparent data protocol using the following
	communication parameters.

	@arg Baud rate: 1200
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 1
	@arg Control characters: none

	The transparent data protocol does not use any message framing or
	control characters. This protocol detects the end of a message by
	having 60 ms of quiet after the last data byte.
				*/
	PRT_ECR1200 = 0x0A,	/**< @brief ECR device protocol @ 1200 baud.

	This config command is used to select the ECR device protocol using the
	following communication parameters.

	@arg Baud rate: 1200
	@arg Data bits: 7
	@arg Parity: even
	@arg Stop bits: 1
	@arg Control characters: ACK NAK

	The ECR device protocol uses the following message framing.

	STX {data} ETX LRC
				*/
	PRT_CFG_PROT = 0x0F,	/**< @brief Select PRT driver protocol,
				                  SIO data format, & SIO baud rate.

	This config command is used to select the active prt driver protocol,
	the SIO data format, and the SIO baud rate. If the selected PRT driver
	protocol is not valid, the PRT driver returns the configuration not
	valid (ERRCNV) error status.

	When the PRT driver receives this config command, the PRT driver will
	start using the message framing and control characters for the passed
	PRT driver protocol and the PRT driver will change the SIO to use the
	passed SIO data format and the passed SIO baud rate.

	The new PRT driver protocol and SIO configuration will be used by the
	PRT driver until the PRT driver receives one of the following commands.

	@arg PRT driver receives a new select protocol (PRT_HYPP, PRT_CHKRD,
	     PRT_IRT, PRT_TM290, PRT_POSMINI, PRT_VF250, PRT_IX50, PRT_PST150,
	     PRT_OKI182, PRT_ECR1200, PRT_CFG_PROT) config command.
	@arg PRT driver receives a close command.

	Format of PRT driver protocol config record is specified by PRTPRC
	structure.
				*/
	PRT_SETTIMEOUT = 0x10,	/**< Set response timeout timer value (not supported). */
	PRT_PROTOPT = 0x11,	    /**< Config command to enable some special
				                 protocol options in the PRT driver. */
	PRT_SETICT = 0x12,	    /**< Set inter-character timer value (not supported).

	This config command is used to change the PRT driver inter-character
	timer value. The default inter-character timer value is 3 (0.06 seconds)
	for the transparent data protocol and 50 (1.00 seconds) for the receive
	inter-character timer protocol option.  The inter-character timer gets
	started after each byte of a message is received and stops when the end
	of the message is detected. For the transparent data protocol, the PRT
	driver uses the expiration of the inter-character timer to detect the
	end of the received message. For the receive inter-character timer
	protocol option, the PRT driver throws away the message being received
	if the inter-character timer expires.

	The new inter-character timer value will be used by the PRT driver until
	the PRT driver receives a close command.
				*/
	PRT_SET_DSR = 0x13,	    /**< Set /DSR signal state (not supported). */
	PRT_KEYB_CMD = 0x14,	/**< External keyboard command (not supported). */
	PRT_SET_ECR = 0x15	    /**< Set CAT33 mode on/off in ECR driver (not supported). */
};

#if defined(MAKE_OPTIMUM)
enum PRT_LEADSET {
    PRT_CTS,
    PRT_RTS_LOW,
    PRT_RTS_HIGH,
    PRT_DSR,
    PRT_DTR_LOW,
    PRT_DTR_HIGH,
};
#endif // MAKE_OPTIMUM

/**
	@brief Enumerated type for PRT driver protocol equates.
*/
enum PRT_PROTOCOL {
	PRTP_PROTOFF = 0x00,	/**< All PRT protocols are inactive (SIO is Off) (not supported). */
	PRTP_HYPPRT = 0x01,	    /**< Hypercom serial printer protocol. */
	PRTP_CHKRD = 0x02,	    /**< Check reader protocol (not supported). */
	PRTP_TRANS = 0x03,	    /**< Transparent data protocol (no message
				                 framing, no control characters). */
	PRTP_POSMINI = 0x04,	/**< POS / MiniNAC protocol. */
	PRTP_VF250 = 0x05,	    /**< Verifone 250 roll printer protocol (not supported). */
	PRTP_ECR = 0x06,	    /**< ECR device protocol. */
	PRTP_KEYB = 0x08,	    /**< External keyboard protocol (not supported). */
	PRTP_CAT33 = 0x0a,	    /**< CAT33 protocol. */
	PRTP_IECI = 0x0b,	    /**< IECI protocol (CAT33 clone with STX included in LRC). */
	PRTP_SIBS = 0x0c,	    /**< SIBS key GUN protocol. */
#if defined(MAKE_OPTIMUM)
	PRTP_PROTO_MAX = PRTP_KEYB
#else
	PRTP_PROTO_MAX
#endif
};

/**
	@brief The PRT driver's protocol config record structure.
*/
typedef struct
{
    unsigned char protocol;	/**< Set to one of PRT_PROTOCOL. */
    unsigned char formatSIO;	/**< Set to one of SIO_DATAFORMAT. */
    unsigned short baud;	/**< Set to one of SIO_BAUDRATE. */
} __attribute__((packed)) PRTPRC;

/**
	@brief ICE-friendly alias for the PRTPRC structure.
*/
typedef PRTPRC prtprc;

/**
	@brief Enumerated type for PRT_PROTOPT config command (for byte 0).

	Byte 1 is not used in this implementation.

	@note All spare option flags (byte 0 & byte 1) must be set to 0.
*/
enum PRT_PROTOCOL_OPTION {
	PRTO_RXICT = 0,		/**< Enable receive inter-character timer (not supported). */
	PRTO_NOLRC = 1,		/**< Skip LRC check (not supported). */
	PRTO_BINLEN = 2,	/**< Use 16-bit binary length instead of
				             BCD length (not supported). */
	PRTO_LRC_FF = 3,	/**< Start LRC calculation with 0xFF flag
				             instead of 0x00 (not supported). */
	PRTO_CCDLY = 4,		/**< Enable a 60 ms delay before transmitting
				             a control character  (not supported). */
	PRTO_NOACKNAK = 5,	/**< Disables sending / receiving the ACK & NAK
				             control characters). */
	PRTO_BIT6 = 6,		/**< Spare bit */
	PRTO_BIT7 = 7		/**< Spare bit */
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __prtdev_h
