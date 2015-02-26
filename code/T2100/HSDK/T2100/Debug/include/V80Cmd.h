//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================

#ifndef _V80CMD_HPP_
#define _V80CMD_HPP_

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief v80 commands definition, see ITU-T V.80 standard.
*/
enum V80_CMD
{
	EM = 0x19,              ///< In-band escape character.
	EMp = 0x99,             ///< In-band escape character with parity.
	DC1 = 0x11,             ///< Disconnect circuit 1.
	DC3 = 0x13,             ///< Disconnect circuit 3.
			
	rtsOFF = 0x42,          ///< Circuit 105 is OFF.
	rtsON = 0x43,           ///< Circuit 105 is ON.
	singleEMp = 0x58,       ///< Single EMp character.
	doubleEMp = 0x59,       ///< Double EMp characters.
	singleEM = 0x5c,        ///< Single EM character.
	doubleEM = 0x5d,        ///< Double EM characters.

	ctsOFF = 0x62,          ///< Circuit 106 is OFF.
	ctsON = 0x63,           ///< Circuit 106 is ON.
	dcdOFF= 0x66,           ///< Circuit 109 is OFF.
	dcdON = 0x67,           ///< Circuit 109 is ON.

	singleEMpDCE = 0x76,    ///< The DCE shall encode this as one 99h in user data.
	doubleEMpDCE = 0x77,    ///< The DCE shall encoded this as 99h, 99h in user data.
	singleEMDCE = 0x7C,     ///< The DCE shall encode this as one 19h in user data.
	doubleEMDCE = 0x7D,     ///< The DCE shall encoded this as 19h, 19h in user data.
	extend0 = 0x60,         /**< The DCE shall encode this as a sequence of
	                             3 + (@<length@> - 1Fh) characters. */
	extend1 = 0x61,         /**< The DCE shall encode this as a sequence of
	                             3 + (@<length@> - 1Fh) characters. */
	extendmfgx = 0x30,      /**< The DCE shall encode this as a sequence of
	                             3 + (@<length@> - 1Fh) characters. The
	                             meaning of @<rest of cmd@> is manufacturer
	                             specific. */

	t1 = 0x5c,              ///< One 19h pattern
	t2 = 0x76,              ///< one 99h pattern
	t3 = 0xA0,              ///< DC1
	t4 = 0xA1,              ///< DC3
	t5 = 0x5d,              ///< two 19h patterns
	t6 = 0x77,              ///< two 99h patterns
	t7 = 0xA2,              ///< two DC1 patterns
	t8 = 0xA3,              ///< two DC3 patterns
	t9 = 0xA4,              ///< 19h, 99h
	t10 = 0xA5,             ///< 19h, DC1
	t11 = 0xA6,             ///< 19h, DC3
	t12 = 0xA7,             ///< 99h, 19h
	t13 = 0xA8,             ///< 99h, DC1
	t14 = 0xA9,             ///< 99h, DC3
	t15 = 0xAA,             ///< DC1, 19h
	t16 = 0xAB,             ///< DC1, 99h
	t17 = 0xAC,             ///< DC1, DC3
	t18 = 0xAD,             ///< DC3, 19h
	t19 = 0xAE,             ///< DC3, 99h
	t20 = 0xAF,             ///< DC3, DC1

	mark = 0xB0,            ///< HDLC abort detected in framed sub-mode.
	flag = 0xB1,            /**< Non-flag to flag transistion detected.
	                             Preceeding data was valid frame; FCS valid
	                             if CRC checking was enabled. */
	err = 0xB2,             /**< Non-flag to flag transistion detected.
	                             Preceeding data was not a valid frame. */
	under = 0xB4,           ///< Transmit data underrun.
	tover = 0xB5,           ///< Transmit data overrun.
	rover = 0xB6,           ///< Receive data overrun.
	bnum = 0xB8,            /**< The following octets @<octnum0@>@<octnum1@>
	                             specifies the number of octets in the
	                             transmit data buffer. */
	unum = 0xB9,            /**< The following octets @<octnum0@>@<octnum1@>
	                             specifies the number of discarded octets. */
	eot = 0xBA,             /**< Terminate carrier, return to command
	                             state. Loss of carrier detected, return to
	                             command state. */
	rate = 0xBE,            /**< Retrain/renegatiation completed; following
	                             octets @<tx@>@<rx@> indicate tx and rx
	                             rates. */
	ecs = 0xBB              ///< Go to on-line command state.
};

/**
	@brief Synchronous access mode command/indication bit rate values.
*/
enum V80_BitRate
{
	p12 = 0x20,             ///< 1200 bit/s
	p24 = 0x21,             ///< 2400 bit/s
	p48 = 0x22,             ///< 4800 bit/s
	p72 = 0x23,             ///< 7200 bit/s
	p96 = 0x24,             ///< 9600 bit/s
	p120 = 0x25,            ///< 12000 bit/s
	p144 = 0x26,            ///< 14400 bit/s
	p168 = 0x27,            ///< 16800 bit/s
	p192 = 0x28,            ///< 19200 bit/s
	p216 = 0x29,            ///< 21600 bit/s
	p240 = 0x2A,            ///< 24000 bit/s
	p264 = 0x2B,            ///< 26400 bit/s
	p288 = 0x2C,            ///< 28800 bit/s
	p312 = 0x2D,            ///< 31200 bit/s
	p336 = 0x2E             ///< 33600 bit/s
};

/**
	@}
*/

#endif //_V80CMD_HPP_
