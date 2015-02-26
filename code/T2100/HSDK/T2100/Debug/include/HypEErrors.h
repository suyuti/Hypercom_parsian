//=============================================================================
//                       Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================

#if !defined(_HYPEERRORS_H_)
#define _HYPEERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Error codes definition in HPOSFC

	Errors are negative, no error is indicated by 0. Errors higher than
	HTEUSERDEFINED but less than 0 are available for use with custom
	HypCTransport derived classes.
*/
typedef enum HypETransportError
{
	/// No error
	HTENOERROR = 0,
	/// Request canceled
	HTECANCELIO = -32768,
	/// Communication error
	HTECOMM,	
	/// Timeout
	HTETIMEOUT,
	/// The parameter is unknown or invalid
	HTEINVAL,
	/// Operation in progress
	HTEINPROGRESS,
	/// Data size too large
	HTEMSGSIZE,
	/// Already connected. Error is returned by transport on connect
	/// request if transport is already connected. Application may use
	/// transport the same way as HTENOERROR is returned.
	HTEISCONN,
	/// Not connected. Usually returned on read/write request attempts to
	/// non-connected transport.
	HTENOTCONN,
	/// Device not initialized
	HTENOTINIT,
	/// Unexpected error
	HTEUNEXPECTED,
	/// Parity error
	HTEPARITY,
	/// Bad LRC or CRC (unused).
	HTECRC,
	/// Lost communications
	HTELOSTCOMMS,
	/// LOST CARRIER for modem (same code as HTELOSTCOMMS)
	HTELOSTCARRIER = HTELOSTCOMMS,
	/// Chat execution was failed (response not matched)
	HTECHATFAILED,
	/// Chat script with errors
	HTEBADCHATSCRIPT,
	/// No dial tone in line
	HTENODIALTONE,
	/// No answer
	HTENOANSWER,
	/// Number is busy
	HTEBUSY,
	/// Line in use
	HTELINEINUSE,
	/// Host message have been received in response to header, parameter or
	/// detail records (unused).
	HTEUNEXPHSTRSP,
	/// Not enough resources to complete request
	HTENORESOURCES, 
	/// Error on underlying level (unused).
	HTEUNDERL,
	/// Bad protocol. Remote host doesn't support protocol (unused).
	HTEBADPROTO,
	/// Wrong read/write calling sequence (unused).
	HTEWRONGSQ,
	/// Not ready: the read or write operation already in progress
	/// or task synchronization delay occured. In last case the operation
	/// should be retried later after small delay or relinquish (unused).
	HTENOTREADY,
	/// Buffer overflow (unused).
	HTEOVERFLOW,
	/// Retry attemts exceeded (unused).
	HTERETRYEXCEED,
	/// Start of user define errors.
	HTEUSERDEFINED
} HypETransportError;

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // _HYPEERRORS_H_
