//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================

#ifndef _HYPUTILS_H
#define _HYPUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

/**
	@addtogroup HPOSFC
	@{
*/

typedef unsigned char uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;

/**
	@brief Calculate MOD 10 for ASCII string.

	@param src Source ASCII string.
	@param len Length of string.

	@return MOD10 character for the string or 0 on error.
*/
extern unsigned char mod10( const unsigned char *src, int len );

/**
	@brief Calculate LRC

	@param src Pointer to the data.
	@param len Length of data.

	@return LRC of the data.
*/
extern unsigned char lrc( const unsigned char *src, unsigned int len );

/// CCITT initial mask for CRC16
#define CCITT_INITIAL_CRC16_MASK	0xFFFF

/// PPP initial mask for CRC16
#define PPP_INITIAL_CRC16_MASK		0xFFFF

/// Normal initial mask for CRC16
#define NORMAL_INITIAL_CRC16_MASK	0x0000

/**
	@brief Calculate CCITT CRC16.

	Calculate CRC16 as defined in CCITT standard. The divisor polynomial
	for the CRC16 is: x^16 + x^12 + x^5 + 1

	@param data Pointer to the data.
	@param length Length of data.
	@param mask Initial mask. Default for CCITT CRC-16 is 0xFFFF.

	@return CRC16 of the data.
*/
extern unsigned short crc16_ccitt( const void *data, unsigned int length, unsigned short mask );

/// Misspelled macro for the crc16_ccitt
#define crc16_ccit(data,length,mask) crc16_ccitt(data,length,mask)

/**
	@brief Calculate reversed CCITT CRC16.

	Calculate reversed CRC16. The divisor polynomial for the CRC16 is:
	x^16 + x^12 + x^5 + 1.

	@param data Pointer to the data.
	@param length Length of data.
	@param mask Initial mask. Default for CCITT CRC-16 is CCITT_INITIAL_CRC16_MASK.

	@return Reversed CRC16 of the data.

	@note This kind of CRC16 is used in HDLC/PPP frames.
*/
extern unsigned short crc16_ccitt_r( const void *data, unsigned int length, unsigned short mask );

/// Misspelled macro for the crc16_ccitt
#define crc16_ccit_r(data,length,mask) crc16_ccitt_r(data,length,mask)

/// Macro to calculate CRC16 
#define crc16_ppp(a, b) crc16_ccitt_r(a, b, PPP_INITIAL_CRC16_MASK)

/// Good value to verify CRC of PPP frame against of.
#define PPP_GOOD_CRC 0xF0B8

/**
	@brief Calculate Hypercom CRC16.

	Hypercom imlementation based on crc16_ccit, initial CRC16 value always
	0xFFFF.

	@param data Pointer to the data.
	@param length Length of data.

	@return Hypercom CRC16 of the data.
*/
extern unsigned short crc16_hypercom( const void *data, unsigned int length );

/**
	@brief Calculate CRC16.

	Calculate normal CRC16. The divisor polynomial for the CRC16 is:
	x^16 + x^15 + x^2 + 1. CRC16 is reversed.

	@param data Pointer to the data.
	@param length Length of data.
	@param mask Initial mask.

	@return CRC16 of the data.
*/
extern unsigned short crc16( const void *data, unsigned int length, unsigned short mask );

/**
	@brief Convert BCD encoded number into binary.

	@param bcd Source BCD encoded number.

	@return Binary value or 0 if BCD number is not valid.
*/
extern unsigned int bcd2bin( unsigned int bcd );

/**
	@brief Convert binary number into BCD encoded.

	@param bin Source binary number.

	@return BCD value.

	@note Does not check for overflow.
*/
extern unsigned int bin2bcd( unsigned int bin );

#ifdef __ARMCC_VERSION
/**
	@brief Duplicate string.

	This function allocates memory and copies into it the string, including
	the terminating NULL character. It is the user's responsibility to free
	the allocated storage by calling free().

	@param s NULL-terminated string to duplicate.

	@return Pointer to a newly allocated copy of the string or NULL on
	        error.
*/
extern char *strdup( const char *s );
#endif

/**
	@brief Find a substring.

	@param str1 String to search in (not neccesary NULL-terminated).
	@param n Length of string to search in.
	@param str2 NULL-terminated substring to search for.

	@return Pointer to the first occurence of substring in string or NULL
	        if substring is not found.
*/
extern const char *strnstr( const char *str1, int n, const char *str2 );

/**
	@brief Convert integer to string.

	@param value The integer to be converted to string representation.
	@param string Pointer to the buffer that is to hold resulting string.
	@param radix Is the base of the number, a value between 2 and 36.

	@return Pointer to string.
*/
extern char *itoa( int value, char *string, int radix );

/**
	@brief Convert string to long integer.

	This function converts the string to a long int representation. It
	recognizes (in order) an optional string of spaces, an optional sign,
	an optional base indicator (0 for octal, X or x for hexadecimal), and
	a string of digits. The first unrecognized character ends the string.
	A pointer to this unrecognized character is stored in the object
	addressed by endptr, if endptr is not NULL.

	If base is non-zero, its value determines the set of recognized digits
	and overrides the optional base indicator character. If base is zero,
	string is assumed to be base 10, unless an optional base indicator
	character is given. 

	@param str Pointer to a character string to convert.
	@param maxdigits The maximum string length to attempt to convert.
	@param eptr Result parameter that, if not NULL, returns a string
	            beginning with the first character that strtol() does not
	            attempt to convert.
	@param base The base of the string, a value between 0 and 36.

	@return The converted value, or zero if no conversion was performed.
	        If the converted value overflows, this function returns
	        LONG_MAX or LONG_MIN (according to the sign of the value) and
	        sets errno to ERANGE.
*/
extern long strntol( const char *str, int maxdigits, char **eptr, int base );

/**
	@brief Convert 16-bit values between host and network byte order.

	@param hostshort Specifies the 16-bit quantity.

	@return Network byte order of hostshort.
*/
extern uint16_t htons( uint16_t hostshort );

/**
	@brief Convert 16-bit values between network and host byte order.

	@param netshort Specifies the 16-bit quantity.

	@return Host byte order of netshort.
*/
#define ntohs(netshort) htons(netshort)

/**
	@brief Convert 32-bit values between host and network byte order.

	@param hostlong Specifies the 32-bit quantity.

	@return Network byte order of hostlong.
*/
extern uint32_t htonl( uint32_t hostlong );

/**
	@brief Convert 32-bit values between network and host byte order.

	@param netlong Specifies the 16-bit quantity.

	@return Host byte order of netlong.
*/
#define ntohl(netlong) htonl(netlong)

/**
	@brief Generate random string [a-z, A-Z, 0-9] for BlueTooth passkey.

	String length is 9 including NULL terminator.

	@return Pointer to the static array contained generated string.
*/
extern const char *randomstr( void );

/**
	@brief Generate random string [0-9] for BlueTooth passkey.

	String length is 9 including NULL terminator.

	@return Pointer to the static array contained generated string.
*/
extern const char *randomnumstr( void );

/**
	@brief Converts several bytes of HEX digits into ASCII characters.

	@param pAscii Pointer to the address the ASCII characters are to be
	              placed. The length must be at least 2 * NumBytes + 1
	              bytes. String will be NULL-terminated.
	@param pBuffer Pointer to the address where the binary or BCD bytes are
	               located.
	@param NumBytes The number of input bytes.
*/
extern void hex2ascii( char *pAscii, const unsigned char *pBuffer, unsigned char NumBytes );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // _HYPUTILS_H
