/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	__siodev_h
#define	__siodev_h

#include <sccdef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief SIO data format parameter definitions (passed to SIO?_CONFIG).
*/
enum SIO_DATAFORMAT {
	SIO_PNONE = 0x00,		/**< No parity. */
	SIO_PEVEN = (PARITY+EVEN),	/**< Even parity. */
	SIO_PODD = PARITY,		/**< Odd parity. */
	SIO_PMASK = (PARITY+EVEN),	/**< Parity mode mask. */

	SIO_1STOP = STOP1,		/**< 1 stop bit. */
	SIO_15STOP = STOP15,		/**< 1-1/2 stop bits (not supported). */
	SIO_2STOP = STOP2,		/**< 2 stop bits. */
	SIO_STOP_MASK = STOP2,		/**< # of stop bits mask. */

	SIO_5BIT = RX5,			/**< 5 data bits (not supported). */
	SIO_6BIT = RX6,			/**< 6 data bits (not supported). */
	SIO_7BIT = RX7,			/**< 7 data bits. */
	SIO_8BIT = RX8,			/**< 8 data bits. */
	SIO_BIT_MASK = RX8,		/**< # of data bits mask. */

	SIO_7NONE = (SIO_7BIT+SIO_PNONE),	/**< 7 data bits, no parity. */
	SIO_7EVEN = (SIO_7BIT+SIO_PEVEN),	/**< 7 data bits, even parity. */
	SIO_7ODD = (SIO_7BIT+SIO_PODD),		/**< 7 data bits, odd parity. */

	SIO_8NONE = (SIO_8BIT+SIO_PNONE),	/**< 8 data bits, no parity. */
	SIO_8EVEN = (SIO_8BIT+SIO_PEVEN),	/**< 8 data bits, even parity. */
	SIO_8ODD = (SIO_8BIT+SIO_PODD),		/**< 8 data bits, odd parity. */

	SIO_7NONE1 = (SIO_7NONE+STOP1),	/**< 7 data bits, no parity, 1 stop bit. */
	SIO_7EVEN1 = (SIO_7EVEN+STOP1),	/**< 7 data bits, even parity, 1 stop bit. */
	SIO_7ODD1 = (SIO_7ODD+STOP1),	/**< 7 data bits, odd parity, 1 stop bit. */

	SIO_7NONE2 = (SIO_7NONE+STOP2),	/**< 7 data bits, no parity, 2 stop bits. */
	SIO_7EVEN2 = (SIO_7EVEN+STOP2),	/**< 7 data bits, even parity, 2 stop bits. */
	SIO_7ODD2 = (SIO_7ODD+STOP2),	/**< 7 data bits, odd parity, 2 stop bits. */

	SIO_8NONE1 = (SIO_8NONE+STOP1),	/**< 8 data bits, no parity, 1 stop bit. */
	SIO_8EVEN1 = (SIO_8EVEN+STOP1),	/**< 8 data bits, even parity, 1 stop bit. */
	SIO_8ODD1 = (SIO_8ODD+STOP1),	/**< 8 data bits, odd parity, 1 stop bit. */

	SIO_8NONE2 = (SIO_8NONE+STOP2),	/**< 8 data bits, no parity, 2 stop bits. */
	SIO_8EVEN2 = (SIO_8EVEN+STOP2),	/**< 8 data bits, even parity, 2 stop bits. */
	SIO_8ODD2 = (SIO_8ODD+STOP2),	/**< 8 data bits, odd parity, 2 stop bits. */
};

/**
	@brief SIO baud rate parameter definitions (passed to SIO?_BAUDRATE).
*/
enum SIO_BAUDRATE {
	SIO_BEXT = 0,		/**< Use external clock (not supported). */
	SIO_B300 = 3,		/**< 300 BPS. */
	SIO_B1200 = 12,		/**< 1200 BPS. */
	SIO_B2400 = 24,		/**< 2400 BPS. */
	SIO_B4800 = 48,		/**< 4800 BPS. */
	SIO_B9600 = 96,		/**< 9600 BPS. */
	SIO_B19200 = 192,	/**< 19200 BPS. */
	SIO_B38400 = 384,	/**< 38400 BPS. */
	SIO_B57600 = 576,	/**< 57600 BPS. */
	SIO_B115200 = 1152,	/**< 115200 BPS. */
};

/**
	@brief Buffer size for IOCS emulated serial devices.
*/
#if defined(MAKE_VIKING)
    #define SIO_BUFSIZE	4096
#elif defined(MAKE_OPTIMUM)
    #define SIO_BUFSIZE	1024
#endif

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __siodev_h
