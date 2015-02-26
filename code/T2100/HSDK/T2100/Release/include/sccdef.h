/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	__sccdef_h
#define	__sccdef_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Definitions for the Z84C15 SIO ports.
*/
enum SCC_RX {
	RX8 = 0xC0,	/**< Receive 8 bits. */
	RX6 = 0x80,	/**< Receive 6 bits. */
	RX7 = 0x40,	/**< Receive 7 bits. */
	RX5 = 0,	/**< Receive 5 bits. */
	STOP2 = 0x0C,	/**< 2 stop bits. */
	STOP15 = 8,	/**< 1.5 stop bits. */
	STOP1 = 4,	/**< 1 stop bit. */
	EVEN = 2,	/**< Even parity. */
	PARITY = 1	/**< Parity enable. */
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __sccdef_h

