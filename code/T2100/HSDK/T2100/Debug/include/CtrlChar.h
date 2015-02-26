//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPECTRLCHAR_HPP_
#define _HYPECTRLCHAR_HPP_

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief VISA control characters
*/
enum HypECtrlChar
{
		/// Start of header
		VISASOH = 0x01,
		/// Start of text
		VISASTX = 0x02,
		/// End of text
		VISAETX = 0x03,
		/// End of transmission
		VISAEOT = 0x04,
		/// Data link escape
		VISADLE = 0x10,
		/// Line feed
		VISALF = 0x0a,
		/// Carriage return
		VISACR = 0x0d,
		/// Enquiry
		VISAENQ = 0x05,
		/// Affirmative acknowledgment
		VISAACK = 0x06,
		/// Affirmative acknowledgment
		VISABEL = 0x07,
		/// Device control 2 (initiating 3s delay)
		VISADC2 = 0x12,
		/// Negative acknowledgment
		VISANAK = 0x15,
		/// End of transmission block
		VISAETB = 0x17,
		/// Not a regular symbol start
		NOSYMB = 0x100,
		/// Parity error character
		VISA_PE_CHAR = 0x101,
		/// Data link escape + end of transmission
		VISA_DLEEOT = 0x102
};

/**
	@brief Mask for definition set of control characters.
*/
enum HypECtrlCharMask
{
		/// Enquiry
		MASKENQ = 0x01,
		/// Affirmative acknowledgment
		MASKACK = 0x02,
		/// Negative acknowledgment
		MASKNAK = 0x04,
		/// Start of text
		MASKSTX = 0x08,
		/// End of text
		MASKETX = 0x10,
		/// End of transmission
		MASKEOT = 0x20,
		/// Affirmative acknowledgment
		MASKBEL = 0x40,
		/// Device control 2 (initiating 3s delay)
		MASKDC2 = 0x80,
		/// Data link escape
		MASKDLE = 0x100,
		/// End of transmission block
		MASKETB = 0x200,
		/// Line feed
		MASKLF = 0x400,
		/// Parity error
		MASK_PE_CHAR = 0x800
};

/**
	@}
*/

#endif //_HYPECTRLCHAR_HPP_
