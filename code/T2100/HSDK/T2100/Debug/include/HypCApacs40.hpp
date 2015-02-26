//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCAPACS40_HPP_
#define _HYPCAPACS40_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/// Maximum supported size of NUA.
#define NUA_SZ 15

/// Default value for timer A.
#define TIMER_A 3000

/// Default value for timer C.
#define TIMER_C 30000

/// Default value for EOT wait timer.
#define WAIT_EOT_TIMER 3000

/**
	@brief APACS 40 transport layer implementation.
*/
class HypCApacs40 : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCApacs40( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCApacs40();

protected:
	/**
		@brief Start timer A.
	*/
	void StartTimerA() { m_Timer.Start( m_T1, TIMEOUT_TA ); }

	/**
		@brief Start timer C.
	*/
	void StartTimerC();
	
	/**
		@brief Start timer C with specific timeout value.

		@param t Timer C timeout.
	*/
	void StartTimerC( size_t t );

	/**
		@brief Stop timer A.
	*/
	void StopTimerA();

	/**
		@brief Stop timer C.
	*/
	void StopTimerC();

	/**
		@brief Transmit address block (A NUA CR).
	*/
	void SendNUA();

	/**
		@brief Disconnect routine.
	*/
	virtual void Discon();

	/**
		@brief Common processor of all states.
	*/
	virtual bool StCommon();

	/**
		@brief Physical connection in progress (PSTN call to TPAD).
	*/
	virtual void StConnecting();

	/**
		@brief Wait for ENQ.
	*/
	virtual void StWaitENQ();

	/**
		@brief Normal processing handler.
	*/
	virtual void StConnected();

	/**
		@brief Wait for acknowledge.
	*/
	virtual void StWaitACK();

	/**
		@brief Disconnect from host.
	*/
	virtual void StDisconnecting();

	/**
		@brief Create expect mask and decode next character.
	*/
	virtual void Expect();

	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );

	/**
		@brief Expect different network events for each state
	*/
	virtual int CreateExpectMask();

private:
	/// Timer C.
	HypCTimer m_Timer2;

	/// Indicates that DLE was received from the line.
	bool m_DLEReceived;

	/// Pointer to NUA string.
	char *m_NUA;
};

/**
	@}
*/

#endif	// _HYPCAPACS40_HPP_
