//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================

#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus

#include <eftos.h>
#include "transp_def.h"
#include "HypCEvent.hpp"
#include "HypCBuffer.hpp"

#endif // __cplusplus

#include "serialtrace.h"

/**
	@addtogroup HPOSFC
	@{
*/

#if defined(__cplusplus) || defined(DOXYGEN)

/**
	@brief Flow contol settings for serial port.
*/
enum ESerialFlowCtrl
{
	/// Hardware flow control:
	/// Hi RTS means that the DTE device (terminal) is ready for receiving data.
	/// Hi CTS means that the DCE device (modem) is ready for receiving data.
	SERIAL_HARDWARE_FC = CH_CHECKCTS | CH_USERTS,
	/// Do not use DTR when open com port, need for GSM modem.
	SERIAL_HARDWARE_NOTERMINAL_FC = CH_CHECKCTS | CH_USERTS | CH_NO_USE_DTR,
	/// Do not use RTS/CTS flow control.
	SERIAL_NONE_FC = 0,
	/// No flow control at all
	SERIAL_NONE_NOTERMINAL_FC = CH_NO_USE_DTR,
	/// Do not use RTS
	SERIAL_TX_FC = CH_CHECKCTS
};

/**
	@brief Serial port leads.
*/
enum ESerialPortLeads
{
	DTR=0x1,        ///< DTR
	RTS=0x2,        ///< RTS
	DSR=0x4,        ///< DSR (can be used only in prompt request)
	CTS=0x8,        ///< CTS (can be used only in prompt request)
	CD=0x10,        ///< DCD (can be used only in prompt request)
	RI=0x20         ///< RI (can be used only in prompt request on RS1)
};

/**
	@brief Packet assembling mode on serial port.

	Serial driver ensures that packet will not be splited during read
	operation. For example, when driver operates in APACS40 mode and
	the following data is receiving:<br>
	@<STX@> @<DATA1@> @<ETX@> @<LRC@> @<STX@> @<DATA2@> @<ETX@> @<LRC@>
	@<STX@> @<DATA3@><br>
	Then driver receives read request. In this case driver right away
	returns only whole packets:<br>
	@<STX@> @<DATA1@> @<ETX@> @<LRC@> and @<STX@> @<DATA2@> @<ETX@> @<LRC@>
	and continues receiving end of packet 3.
*/
enum ESerialPortMode
{
	/// @brief NORMAL mode (default).
	/// End of each packet is determinated by intercharacter timeout. 
	/// Intercharacter timeout is defined by ReadIntervalTimeout field in 
	/// the SSerialPortParameters structure. if the ReadIntervalTimeout
	/// is equal to 0, all received data right away is considered as
	/// completed packet.
	SERIAL_NORMAL=0,
	/// @brief APACS40 mode.
	/// Packet format: @<STX@> @<data@> @<ETX@> @<LRC@>.<br>
	/// @<LRC@> is one byte. LRC is not verified.<br>
	/// If STX is not received, all data right away is considered as
	/// completed packet (in that way ACKs, NAKs are received without
	/// intercharacter timeout). After receiving STX driver checks
	/// intercharcter timeout. If timeout occures before receiving end of
	/// frame (LRC), packet will be dropped (acording to APACS40 spec).
	SERIAL_APACS40,
	/// @brief POSMINI mode.
	/// Packet format: @<STX@> @<LL LL@> @<data@> @<ETX@> @<LRC@>.<br>
	/// @<LRC@> is one byte. LRC is not verified.<br>
	/// @<LL LL@> - 2 bytes long BCD length of data in packet, it does not
	/// include @<STX@> @<LL LL@>, @<ETX@>, @<LRC@>. The length must be
	/// less or equal then 2099 (decimal).<br>
	/// If STX is not received, all data right away is considered as
	/// completed packet (in that way ACKs, NAKs are received without
	/// intercharacter timeout). After receiving STX driver checks
	/// intercharcter timeout. if timeout occures before receiving end of
	/// frame (LRC), packet is dropped. If length is more then 2099,
	/// @<STX@> @<LL LL@> will be dropped.
	SERIAL_POSMINI,
	/// @brief POSTRANSPAD mode.
	/// Packet format: @<LL LL@> @<data@>.<br>
	/// @<LL LL@> - 2 bytes long binary length of data in packet, it does
	/// not include length by itself. The length must be less or equal then
	/// 0x9ff (hexadecimal).<br>
	/// If length is more then 0x9ff, @<LL LL@> will be droped.
	/// If intercharacter timeout occures, packet will be dropped.
	SERIAL_POSTRANSPAD,
	/// @brief V80 mode.
	/// Packet formats:<br>
	/// @<data@> @<0x19 0xb1@>,<br>
	/// @<data@> @<0x19 0xb0@>,<br>
	/// @<data@> @<0x19 0xb2@>,<br>
	/// @<0x19 0xbe@> @<xx yy@>,<br>
	/// @<0x19 0xba@>.<br>
	/// @<data@> @<0x19 0xb1@> is a good data packet.<br>
	/// @<data@> @<0x19 0xb0@>, and @<data@> @<0x19 0xb2@> packets are bad
	/// data and dropped by driver.<br>
	/// Intercharcter timeout is not checked.
	SERIAL_V80,
	/// @brief PPP mode.
	/// Returns all data right away after receiving 0x7e flag or after
	/// receiving more then 2000 bytes.
	SERIAL_PPP,
	/// @brief HYPERCOM VISA mode.
	/// Packet format: @<STX@> @<LL LL@> @<data@> @<CRC CRC@>.<br>
	/// @<CRC CRC@> - 2 bytes of CRC. CRC is not verified.<br>
	/// @<LL LL@> - 2 bytes long binary length of data in packet, it does
	/// not include @<STX@>, @<LL LL@>, @<CRC CRC@>. Length must be less
	/// then 4200 (decimal).<br>
	/// If STX is not received, all data right away is considered as
	/// completed packet (in that way ACKs, NAKs are received without
	/// intercharacter timeout). After receiving STX driver checks
	/// intercharcter timeout. If timeout occures before receiving end of
	/// frame (last byte of CRC), packet is dropped. If length is more or
	/// equal then 4200, @<STX@> @<LL LL@> will be dropped.
	SERIAL_HYPVISA,
	/// @brief HEMS mode.
	/// Packet format: @<type@> @<LL LL@> @<data@> @<TERM TERM@>.<br>
	/// @<LL LL@> - 2 bytes long binary length of data in packet.<br>
	/// @<TERM TERM@> - 2 bytes packet terminator (@<0xff 0xff@> or
	/// @<0xff 0xfe@>).<br>
	/// High nibble of @<type@> is verified to match 0xe. All data before
	/// receiving @<type@> is ignored. Returns all data right away after
	/// receiving complete packet. If timeout occures before receiving end
	/// of frame (last byte of terminator), packet is dropped.
	SERIAL_HEMS,
	/// @brief GS1 without LRC.
	/// Packet format: @<STX@> @<data@> @<ETX@>.<br>
	/// The ETX and DLE in the @<data@> are prefixed with additional DLE.
	/// If STX is not received, all data right away is considered as
	/// completed packet (in that way ACKs, NAKs are received without
	/// intercharacter timeout). After receiving STX driver checks
	/// intercharcter timeout. If timeout occures before receiving end of
	/// frame (ETX), packet is dropped.
	SERIAL_GS1,
	/// @brief GS1 with LRC.
	/// Packet format: @<STX@> @<data@> @<ETX@> @<LRC@>.<br>
	/// @<LRC@> is one byte. LRC is not verified.<br>
	/// The ETX and DLE in the @<data@> are prefixed with additional DLE.
	/// If STX is not received, all data right away is considered as
	/// completed packet (in that way ACKs, NAKs are received without
	/// intercharacter timeout). After receiving STX driver checks
	/// intercharcter timeout. If timeout occures before receiving end of
	/// frame (LRC), packet is dropped.
	SERIAL_HFT,
	/// @brief SIBS Key GUN protocol.
	/// Packet format: @<DLE@> @<STX@> @<data@> @<DLE@> @<ETX@> @<LRC@>.<br>
	/// The DLE in the @<data@> is prefixed with additional DLE.
	/// If STX is not received, all data (except DLE) right away is
	/// considered as completed packet (in that way ACKs, NAKs are received
	/// without intercharacter timeout). After receiving initial DLE driver
	/// checks intercharcter timeout. If timeout occures before receiving
	/// end of frame (LRC), packet is dropped.
	SERIAL_SIBS,
	/// @brief V80 forced to half duplex. Modification of V80 mode with
	/// ignorance of incoming data during packet transmission.
	SERIAL_V80_HALFDUPLEX
};

/**
	@brief Serial port configuration structure.
*/
typedef struct SSerialPortParameters
{
	DWORD BaudRate; ///< Baud rate.
	DWORD DataBits; ///< Number of data bits.
	DWORD StopBits; ///< Number of stop bits.
	DWORD Parity;   /**< Parity (CH_PARITY_NONE, CH_PARITY_ODD, or
	                     CH_PARITY_EVEN). */
	DWORD FlowCtrl; ///< Flow control (see ESerialFlowCtrl).
	int Mode;       ///< Packet assembling mode (see ESerialPortMode).
	unsigned int ReadIntervalTimeout;
	                /**< Intercharacter timeout. Specifies the maximum
	                     time, in milliseconds, allowed to elapse between
	                     the arrival of two characters on the
	                     communications line. */
	int ErrorCharFlag:1;
	                /**< Enable errorneously received character
	                     replacement. */
	char ErrorChar; ///< Error replacement character.
}
SSerialPortParameters;

extern "C" {
#endif // __cplusplus

/**
	@brief Inititialize serial driver.

	@param nTotalCh Number of served serial ports.

	@return 1: If driver is initialized successfully.<br>
	        0: On error condition.

	@note Should be called before any use of serial port via HPOSFC.
	      Normally called from HW_Start() and not need to be called by
	      application directly.
*/
extern int SerialDriverInit( unsigned int nTotalCh );

/**
	@brief Close serial driver.

	Frees all resources used by the serial driver.
*/
extern void SerialDriverClose( void );

#if defined(__cplusplus) || defined(DOXYGEN)
}

/**
	@brief Open serial port.

	@param nCh Port number.

	@return 1: If driver is initialized successfully.<br>
	        0: On error condition.
*/
extern int SerialOpen( unsigned int nCh );

/**
	@brief Close serial port.

	@param nCh Port number.
*/
extern void SerialClose( unsigned int nCh );

/**
	@brief Request data from serial port.

	@param nCh Port number.
	@param pEv Pointer to HypCEvent object to place event once data is
	           ready.
	@param bWait If bWait is 0 (false) event is sent right away
	             independently of data availability in the receive buffer.
	             If data is absent status field of event is equal to 0.
*/
extern void SerialReadReq( unsigned int nCh, HypCEvent *pEv, int bWait );

/**
	@brief Request write to serial port.

	@param nCh Port number.
	@param pEv Pointer to HypCEvent object to place event once data is
	           completely sent or it will be detected that data cannot be
	           sent.
	@param pBuffer Buffer containing data to transmit.
*/
extern void SerialWriteReq( unsigned int nCh, HypCEvent *pEv, HypCBuffer *pBuffer );

/**
	@brief Cancel read request from serial port.

	@param nCh Port number.

	@note If there is pending request, the event with status equal to
	      HTECANCELIO will be sent.
*/
extern void SerialCancelRead( unsigned int nCh );

/**
	@brief Cancel write request to serial port.

	@param nCh Port number.

	@note If there is pending request, the event with status equal to
	      HTECANCELIO will be sent.
*/
extern void SerialCancelWrite( unsigned int nCh );

/**
	@brief Set serial line parameters.

	@param nCh Port number.
	@param pParam Pointer to SSerialPortParameters structure with desired
	              line parameters.

	@return 1: If line parameters have been set successfully.<br>
	        0: On error condition.
*/
extern int  SerialSetPortParameters( unsigned int nCh, const SSerialPortParameters *pParam );

/**
	@brief Flush receive buffer.

	@param nCh Port number.

	@note SerialFlush does not cancel read requests.
*/
extern void SerialFlush( unsigned int nCh );

/**
	@brief Get serial port lead state.

	@param nCh Port number.
	@param pin Lead to read state from.

	@return Lead state.
*/
extern int SerialGetPin( unsigned int nCh, enum ESerialPortLeads pin );

/**
	@brief Set serial port lead state.

	@param nCh Port number.
	@param pin Lead to set state.
	@param bState State to set lead 
*/
extern void SerialSetPin( unsigned int nCh, enum ESerialPortLeads pin, int bState );

/**
	@brief Request event on serial port leads state change.

	@param nCh Port number.
	@param pEv Pointer to HypCEvent object to place event once lead
	           state is changed.
	@param pins Leads to monitor (OR mask of ESerialPortLeads).
*/
extern void SerialPinReq( unsigned int nCh, HypCEvent *pEv, int pins );

#endif // __cplusplus

/**
	@}
*/

#endif // _SERIAL_H_
