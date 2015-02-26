/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __pindev_h
#define __pindev_h

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
enum PIN_IOCSMODE {
	PIN_CFG_PROT = 0x00,	/**< @brief Select PIN Driver Protocol, PIN Driver Mode
				            (Terminal / PinPad), SIO Data Format, &
				            SIO Baud Rate.

	This config command is used to select the active PIN driver protocol,
	the active PIN driver mode (Terminal / PinPad), the SIO data format,
	and the SIO baud rate. If the selected PIN driver protocol or PIN
	driver mode is not valid, the PIN driver returns the configuration not
	valid (ERRCNV) error status.

	When the PIN driver receives this config command, the PIN driver will
	start using the message framing and control characters for the passed
	PIN driver protocol, will start performing the unique actions for the
	passed PIN driver mode, and the PIN driver will change the SIO to use
	the passed SIO data format and the passed SIO baud rate.

	The new PIN driver protocol, PIN driver mode, and SIO configuration
	will be used by the PIN driver until the PIN driver receives one of
	the following commands.

	@arg PIN driver receives a new select protocol (PIN_CFG_PROT)
	     config command.
	@arg PIN driver receives a close command

	Format of PIN driver protocol config record is specified by PINPRC
	structure.

	When the PIN driver is in terminal mode, the PIN driver performs the
	following unique actions.

	@arg For the CAT 33 protocol, expects to receive an EOT in response to
	     a transmitted ACK.
	@arg If the response timeout timer expires while the PIN driver is
	     waiting to receive an ACK, a NAK, or a valid message, the PIN
	     driver will cycle power on the PIN port before re-transmitting
	     the message (not supported).

	When the PIN driver is in pinpad mode, the PIN driver performs the
	following unique actions.

	@arg For the CAT 33 protocol, transmits an EOT in response to a
	     received ACK.
	@arg If the response timeout timer expires while the PIN driver is
	     waiting to receive an ACK, a NAK, or a valid message, the PIN
	     driver will just re-transmit the message.
				*/
	PIN_PROTOPT = 0x01,	    /**< Config command to enable some special
				                 protocol options in the PIN driver. */
	PIN_SETTIMEOUT = 0x02,	/**< Set response timeout timer value (not supported). */
	PIN_SETICT = 0x03,	    /**< Set inter-character timer value (not supported). */
	PIN_CONST_PWR = 0x04,	/**< Enable / disable constant power mode (not supported). */
	PIN_KEYB_CMD = 0x05	    /**< External keyboard command (not supported). */
};

/**
	Enumerated type for PIN driver protocol equates.
*/
enum PIN_PROTOCOL {
	PIN_PROTOFF = 0x00,	/**< All PIN protocols are inactive (SIO is Off)  (not supported). */
	PIN_CAT33 = 0x01,	/**< Omrom CAT 33 pinpad protocol. */
	PIN_DUKPT = 0x02,	/**< Dukpt pinpad protocol. */
	PIN_UOBCIS = 0x03,	/**< UOB CIS pinpad protocol (not supported). */
	PIN_POSMINI = 0x04,	/**< POS / MiniNAC protocol. */
	PIN_NCR5991 = 0x05,	/**< NCR 5991 signature capture device protocol (not supported). */
	PIN_CHKRD = 0x06,	/**< Check reader protocol (not supported). */
	PIN_TRANS = 0x07,	/**< Transparent data protocol
				             (no message framing, no control characters). */
	PIN_KEYB = 0x08,	/**< External keyboard protocol (not supported). */
	PIN_GS1 = 0x09,		/**< GS1 protocol. */
	PIN_HFT = 0x0a,		/**< HFT protocol. */
	PIN_IECI = 0x0b,	/**< IECI protocol (CAT33 clone with STX included in LRC). */
	PIN_SIBS = 0x0c,	/**< SIBS key GUN protocol. */
#if defined(MAKE_OPTIMUM)
	PIN_PROTO_MAX = PIN_KEYB
#else
	PIN_PROTO_MAX
#endif
};

/**
	Enumerated type for PIN driver mode equates.
*/
enum PIN_MODE {
	PIN_TERMINAL = 0x00,	/**< Terminal mode. */
	PIN_PINPAD = 0x01	    /**< Pinpad peripheral mode (not supported). */
};

/**
	@brief The PIN driver's protocol config record structure.
*/
typedef struct
{
    unsigned char protocol;	    /**< Set to one of PIN_PROTOCOL. */
    unsigned char pinMode;	    /**< Set to one of PIN_MODE. */
    unsigned char formatSIO;	/**< Set to one of SIO_DATAFORMAT. */
    unsigned short baud;	    /**< Set to one of SIO_BAUDRATE. */
} __attribute__((packed)) PINPRC;

/**
	Enumerated type for PIN driver constant power mode state equates.
*/
typedef enum {
	PIN_CPWR_OFF = 0x00,	/**< Turn off (disable) constant power mode. */
	PIN_CPWR_ON = 0x01	    /**< Turn on (enable) constant power mode. */
} PIN_CPWR;

/**
	@brief Enumerated type for PRT_PROTOPT config command (for byte 0).

	Byte 1 is not used in this implementation.

	@note All spare option flags (byte 0 & byte 1) must be set to 0.
*/
enum PIN_PROTOCOL_OPTION {
	PINO_RXICT = 0,		/**< Enable receive inter-character timer (not supported). */
	PINO_NOLRC = 1,		/**< Enable skipping the LRC check on received
				     messages (not supported). */
	PINO_BINLEN = 2,	/**< Use 16-bit binary length instead of
				     BCD length (not supported). */
	PINO_LRC_FF = 3,	/**< Start LRC calculation with 0xFF flag
				     instead of 0x00 (not supported). */
	PINO_NOEOT = 4,		/**< Disables sending / receiving the EOT
				     control character (not supported). */
	PINO_NOACKNAK = 5,	/**< Disables sending / receiving the ACK & NAK
				     control characters). */
	PINO_BIT6 = 6,		/**< Spare bit */
	PINO_BIT7 = 7		/**< Spare bit */
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __pindev_h
