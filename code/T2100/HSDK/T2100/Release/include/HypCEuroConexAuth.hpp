//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCEUROCONEXAUTH_HPP_
#define _HYPCEUROCONEXAUTH_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/
        
/**
        @brief EuroConex authorization protocol

	EuroConex is VISA-based protocol. See EuroConex POS Message
	Specifications Version 2.16.

	The class accepts following parametes:
	 - HC_Timer1 defines Wait first ENQ timer.
	 - HC_Timer2 defines timeout for waiting response from host.
	 - HC_Timer4 defines Wait EOT or ENQ timer after receiving response.

	@see HypCVisaCommon
*/
class HypCEuroConexAuth : public HypCVisaCommon
{
private:
	/// Default value (ms) for T1 timer (awating ENQ).
	static const int EC_WAIT_ENQ_TIMER = 15000;

	/// Default value for T4 timer (awating EOT or next ENQ).
	static const int EC_WAIT_EOT_TIMER = 5000;

	/// Default value for T2 timer (awating responce from host).
	static const int EC_WAIT_ACK_TIMER = 30000;

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCEuroConexAuth( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCEuroConexAuth();

protected:
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
		@brief Handles connected state.
	*/
	virtual void StConnected();

	/**
		@brief Wait for response from host.
	*/
	virtual void StWaitResponse();

	/**
		@brief Wait for EOT, ENQ, or write from application level.
	*/
	virtual void StWaitEOT();

	/**
		@brief Expect different network events for each state
	*/
	virtual int CreateExpectMask();

	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig (const HypCConfig &cfg);
};

/**
	@}
*/

#endif	// _HYPCEUROCONEXAUTH_HPP_
