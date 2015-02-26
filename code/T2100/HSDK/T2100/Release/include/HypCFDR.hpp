//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                   (c) Copyright 2003-2006
//=============================================================================
#ifndef _HYPCFDR_HPP_
#define _HYPCFDR_HPP_

#include <eftos.h>
#include "HypCVisaCommon.hpp"
#include "HypCTransport.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/// Default value for T1 timer (waiting ENQ)
#define FDR_T1_TIMER		15000

/// Default value for waiting host responce
#define FDR_T2_TIMER		30000

/// Default value for T3 timer (waiting poll character)
#define FDR_T3_TIMER		4000

/**
	@brief FDR transport layer implementation.

	This class provides FDR authorization link level protocol. The FDR
	batch upload should be handled via VISA Capture protocol.

	Following parameters are used for configure protocol:
	- HC_TimerT1
	- HC_TimerT2
	- HC_TimerT3

	Default mode: T1=FDR_T1_TIMER, T2=FDR_T2_TIMER, T3=FDR_T3_TIMER.

	The EOT from terminal to FDR host is sent by application. Following
	chart describes interface to protocol module: @include FDR.txt

	Related documents:
	Electronic Ticket Capture "Plus",
	Integrated Point-Of-Sale Specifications
	First Data Merchant Services
	Omaha Platform (Fdms - Omaha)
	Version 1.0 July 2001

	@see HypCTransport, HypCVisaCommon, HypCVisaCapture
*/
class HypCFDR : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCFDR( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCFDR();

protected:
	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );

	/**
		@brief Build transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Expect different network events for each state
	*/
	virtual int CreateExpectMask();

	/**
		@brief Create expect mask and decode next character.
	*/
	void Expect();

	/**
		@brief State machine.
	*/
	virtual void StateMachine();

	/**
		@brief Common processor of all states.
	*/
	virtual bool StCommon();

	/**
		@brief Wait for ENQ.
	*/
	virtual void StWaitENQ();

	/**
		@brief Send online message.

		The online message is the first message in the flow and
		responded with ACK from the host.
	*/
	void StSendOnlineReq();

	/**
		@brief Send offline messages.

		Offline messages are sent without confirmation from the host.
		When there's no more offline messages application should send
		EOT to the host.
	*/
	void StSendOfflineReq();

	/**
		@brief Wait for acknowledge or response message.
	*/
	virtual void StWaitACK();

	/**
		@brief Wait for EOT from the host.
	*/
	virtual void StWaitEOT();

	/**
		@brief Send frame to next layer.
	*/
	virtual void SendFrame();

private:
	/// Counter of NAKs received.
	int m_NakCount;

	/// EOT sent flag.
	bool m_EOTsent;
};

/**
	@}
*/

#endif // _HYPCFDR_HPP_
