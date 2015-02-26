/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __comdev_h
#define __comdev_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Config mode equates (IOCSMODE) for COM device.
*/
enum COM_IOCSMODE {
	COM_LEASE_LINE = 0x00,      /**< Perform leased line connection (not supported). */
	COM_LOW_LAN = 0x01,	    /**< Perform low-speed LAN connection (not supported). */
	COM_DIAL = 0x02,	    /**< Perform dial-up connection. */
	COM_HANGUP = 0x03,	    /**< Perform hang-up of connection. */
	COM_ANSWER 	= 0x04,	    /**< Perform auto-answer (not supported). */
	COM_MODEM_HW = 0x05,        /**< Return modem type (not supported). */
	COM_HANDSET_UP 	= 0x06,     /**< Return handset up status (not supported). */
	COM_VOICE_DIAL = 0x07,      /**< Perform voice dialing. */
	COM_ACTION = 0x08,	    /**< Return communication status. */
	COM_HI_LAN = 0x09,	    /**< Perform high-speed LAN connection (not supported). */
	COM_PRI_NUM = 0x0A,	    /**< Return primary number flag (not supported). */
	COM_RX_EOT = 0x0B,	    /**< Return EOT received flag (not supported). */
	COM_LINE_AVAIL = 0x0C,      /**< Return line available status (not supported). */
	COM_XMOD_TX = 0x0D,	    /**< Start X-Modem mode transmit file (not supported). */
	COM_XMOD_RX = 0x0E,	    /**< Start X-Modem mode receive file (not supported). */
	COM_XMOD_TXLRC = 0x0F,      /**< Start X-Modem mode transmit continue LRC (not supported). */
	COM_XMOD_TXCRC = 0x10,      /**< Start X-Modem mode transmit continue CRC (not supported). */
	COM_SNDMAT = 0x11,	    /**< Table of Send & Match Strings for use by
				                 the next dial-up connection (partially supported
				                 for extracting NUA only, different scripting
				                 language is available to use in different way). */
	COM_REQMS = 0x12,	    /**< Request last match string status (not supported). */
	COM_SCC_MODEM = 0x13,       /**< Override SCC & modem configurations when
				                 communicating with the Host System (# data
				                 baud rate, modem protocol, modem standard)
				                 (not supported directly, but supported in
				                 different way). */
	COM_XMOD_FILL = 0x14,       /**< Set X-Modem fill character (not supported). */
	COM_GPS = 0x15,		    /**< Start Global Payment Systems protocol (not supported). */
	COM_XOFF_THRS = 0x16,       /**< Set XOFF threshold for XON/XOFF protocol
	                                 and Enable XON/XOFF processing (not supported). */
	COM_SPC_DATA = 0x17,        /**< Special data needed by the current Host
	                                 protocol (not supported). */
	COM_PROTOPT = 0x18,	    /**< Config command to enable some special
	                                 protocol options in the COM driver (not supported). */
	COM_TEST = 0x19,            /**< Test modes for COM (modem/LAN) testing (not supported). */
	COM_CARRIER = 0x1A,         /**< Return debounced carrier detect (not supported). */
	COM_SPEEDSIZE = 0x1B,       /**< Return connection speed and max message size
	                                 (not supported). */
	COM_BBTEST = 0x1C,	    /**< Back-to-back test setup data (not supported). */
	COM_CU_STATIC = 0x1D,       /**< Return pointer to NMS statistics (not supported). */
	COM_GETDIAG = 0x1E,	    /**< Get diagnostic data (not supported). */
	COM_SIGNAL_LEVEL = 0x1F,    /**< Return line signal level. */
	COM_LINE_STATUS = 0x20,     /**< Return line status. */
	COM_DIRECT = 0x21,	    /**< Select direct access to AT command
	                                 set modem. */
	COM_PREINIT,		    /**< Preinit modem. */
	COM_CONF_MAX                /**< Highest mode supported. */
};

/**
	@brief Enumerated type for COM_SNDMAT config mode command for More flag.
*/
enum COM_SNDMAT_MORE_FLAG {
	COM_NOMORE = 0x00,	/**< No More Send & Match String tables will be sent. */
	COM_MORE = 0x01		/**< Another Send & Match String table will be sent. */
};

/**
	@brief Enumerated type for COM_SNDMAT config mode command for Match
	       String command equates.
*/
enum COM_SNDMAT_COMMANDS {
	MS_END = 0xFF,		/**< Marks the end of the Match String. */
	MS_ANY = 0xFE,		/**< Used to match any single character. */
	MS_WILD = 0xFD,		/**< Used to Match any string of characters. */
	MS_OR = 0xFC,		/**< Used to match 1 out of a group of strings
				             (Match string 1 OR string 2 OR string N). */
	MS_OREND = 0xFB,	/**< Marks the end of the group of OR strings. */
	MS_RETRY = 0xFA,	/**< Marks this as a retry Match String. */
	MS_RETRYLAST = 0xF9	/**< Marks this as the last retry Match String in
				             a group of retry Match Strings. */
};

/**
	@brief Enumerated type for COM_SNDMAT config mode response.
*/
enum COM_SNDMAT_RESPONSE {
	COM_BADFMT = 1,		/**< Send & Match String Table format is bad. */
	COM_TBLACT = 2		/**< Another Send & Match String table is already
				             active. */
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __comdev_h
