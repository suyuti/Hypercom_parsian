//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCV29_HPP_
#define _HYPCV29_HPP_

//=============================================================================
//
// HypCV29.hpp
//
// V29 turn around protocol. see doc: conexant V.23 and V.29 Half-Duplex 
//		Turn-Arround Application Note
//
//=============================================================================
#include <eftos.h>
#include "HypCTransport.hpp"
#include "HypCSerialPort.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "HypCTransparentCommon.hpp"

//=============================================================================
//
// HypCV29
//
// Base Class:
//      HypCTransparentCommon
//
// Overview:
//      This class provides controlling routines for implementation
// Half Duplex TurnArround protocol.
//
// Related Documents:
//      Half Duplex TurnArround App Note 070102_PRELIMINARY.pdf
//
// See Also:
//      HypCModem, HypCModemV90, HypCTransparentCommon
//
//=============================================================================
class HypCV29 : virtual public HypCTransparentCommon
{
	protected:
	//specific V29 evets:
	enum V29Events
	{
		DCD_OFF=HypCTransparentCommon::LAST_EV_TAG,
		DCD_ON,
		CTS_ON,
		TIMEOUT_TXDATA,
		TIMEOUT_DCDTORST
	};

	public:
		// Constructor
		HypCV29( HypCSerialPort *serial, int deviceId );

		// Destructor
		virtual ~HypCV29();

	protected:
		void StartWaitTxDataTimer();
		void StopWaitTxDataTimer();

		//V29 state machine
		void StV29Connected();
		//preparation for start V29 state machine
		void V29Start();
		void V29WaitEvent();
		//set RTS signal
		void Rts(bool state);
		bool Pin(HypESerialPortLeads p);

//=============================================================================
// Member variables
//=============================================================================
	private:
		enum {
			RX,
			RX_TO_TX,
			TX,
			TX_TO_RX
		} m_Mode;
		//Timer value (default 500 ms), if data to sent is not available in 500 ms
		//turn V29 to receive data
		size_t m_TimeoutTxData; 
		HypCTimer m_V29Timer;
		HypCTimer m_DCDtoRSTTimer;
		//Pointer to the serial port
		HypCSerialPort *m_pSerialPort;
		//Indicator of first transmission from terminal side
		bool m_FirstTx;
};


#endif // _HYPCV29_HPP_
