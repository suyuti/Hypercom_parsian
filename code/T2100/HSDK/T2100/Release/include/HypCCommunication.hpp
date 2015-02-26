//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCCOMMUNICATION_HPP_
#define _HYPCCOMMUNICATION_HPP_

#include "HypCConfig.hpp"
#include "HypCConfigSet.hpp"
#include "HypCCommModesTable.hpp"

#include "HypCTransparentCommon.hpp"
#include "HypCSerialPort.hpp"
#include "HypCModem.hpp"

#include "transports.h"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Communications control implementation.

	This class is responsible for handling modem modes, reconnect
	attempts, collect statistics, etc. It works on top of the other
	communication layers, providing easy mechanism to handle communication
	stuff.

	@note For interaction with D2100 software derived class
	      HypCACSCommunication must be used instead.
*/
class HypCCommunication : public HypCTransparentCommon
{
public:
	/**
		@brief Constructor.

		@param nDeviceID Not used.
		@param module Module type installed. HW_DIALBACKUP should not
		              be used here. Selected module (HW_GSMMODEM or
		              HW_MODEMV90) should be used instead.
		@param proxyModule Proxy module type. Currently, only two modes
		                   are supported: HW_NOMODULE when working with
		                   module directly or HW_BLUETOOTH when
		                   terminal is attached to the hardware via
		                   BlueTooth link (i.e., for GSM phone
		                   attached via BlueTooth the parameter above
		                   should be set to HW_GSMMODEM and this
		                   parameter should be set to HW_BLUETOOTH).

		@note Initializes work variables and modem modes table.
	*/
	HypCCommunication( int nDeviceID, enum HypEModuleType module, enum HypEModuleType proxyModule = HW_NOMODULE );

	/**
		@brief Destructor.
	*/
	virtual ~HypCCommunication();

	/**
		@brief Set destination.

		@param addr Set of destination addresses.
	*/
	void SetDestination( const HypCConfigSet& addr );

	/**
		@brief Get status of the modem.

		@return Status of modem connection progress.
	*/
	virtual HypCTransport::HypETransportStatus GetModemStatus();

	/**
		@brief Get current statistics.

		@return Statistics for the calls done from the class creation
		        time.
	*/
	virtual HypCStatistics *GetStatistics() { return &m_Stat; }

	/**
		@brief Clear collected statistics.
	*/
	void ClearStatistics() { m_Stat.Clear(); }

	/**
		@brief Get line status from module.

		@return -1 if line status is not available or not applicable
		        for installed module.

		@see HypCModem::GetLineStatus() or
		     HypCHemsCommon::GetLineStatus() for return values.

		@note This function works only while the object is in
		      disconnected state.
	*/
	virtual int GetLineStatus();

	/**
		@brief Get signal level.

		@return -1 if signal level is not available or not applicable
		        for installed module.

		@see HypCModem::GetSignalLevel() for return values.

		@note This function works only while the object is in
		      disconnected state.
	*/
	virtual int GetSignalLevel();

	/**
		@brief Get connection number currently processing
		@return Active connection number
	*/
	size_t GetActiveConnectionNo() { return m_CurrentSet; }

	/**
		@brief Modem preinit.

		@param cfg Configuration options for preinitialization (modem
		           specific).

		@note Configuration should contain the following options:

		@li For GSM modem - HC_FlowControl.
		@li For V90 modem - HC_LoadConexantPatch, HC_ConexantPatchPtr,
		                    HC_ConexantPatchLength.
		@li For V90 and V22 modems - HC_CountryCode.

		Not used for other types at this moment.
	*/
	bool PreInit( HypCConfig *cfg );

private:
	/**
		@brief Set configuration.

		@param cfg Configuration options for next call.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );

	/**
		@brief Initialize communication modes table.

		@note The table is defined in separate file CommConfig.cpp
		      distributed in source code with HSDK. The intension for
		      that is to allow customers to customize modem modes
		      without need to rebuild HSDK. The file could be appended
		      to the project and modified at wish.
	*/
	void InitCommModeTable();

protected:
	/**
		@brief Check configuration validity.

		@return true if configuration is valid, false otherwise.
	*/
	virtual bool CheckConfig();

	/**
		@brief Handles connecting state.
	*/
	virtual void StConnecting();

	/**
		@brief Handles disconnecting of communications.
	*/
	virtual void Discon();

	/**
		@brief Get factory for the modem.

		@brief Factory for the modem for current modem mode.
	*/
	HypCTransportFactory *GetModemFactory();

	/**
		@brief Disconnect stack of layers.
	*/
	void DisconnectStack();

	/**
		@brief Destroy stack of layers.
	*/
	void DestroyStack();

	/**
		@brief Get transport factory by transport ID.

		@param t Transport ID.

		@return Pointer to the transport factory or NULL if it not
		        applicable to the current modem mode.
	*/
	HypCTransportFactory *GetFactory( HypETransports t );

private:
	/**
		@brief State machine.
	*/
	virtual void StateMachine();

	/**
		@brief Handles connected state.
	*/
	void StConnected();

	/**
		@brief Get modem protocol.

		@return Modem protocol used for currentcomm mode, or -1 if
		        modem is not used or modem protocol is not defined.
	*/
	int GetModemProto();

	/**
		@brief Create serial port class.

		@return Pointer to the class configured for the current
		        communication module.
	*/
	HypCSerialPort *CreateSP();

	/**
		@brief Create modem class.

		@param com Pointer to the serial port to create modem class
		           over.

		@return Pointer to the modem class created over the given
		        serial port.
	*/
	HypCModem *CreateModem( HypCSerialPort *com );

protected:
	/// Array of destination addresses.
	HypCConfigSet m_AddressSet;

	/// Pointer to the configuration for the current attempt.
	HypCConfig *m_CurrentConfig;

	/// Index of the configuration for the current attempt.
	size_t m_CurrentSet;

	/// Attempt number within set.
	size_t m_ConnAttempt;

	/// Attempt within connection.
	size_t m_CurrentConnAttempt;

	/// Pointer to the current modem mode.
	HypCCommMode *m_CurrentMode;

	/// Contains serial port configuration for initializing modem.
	HypCTransport *m_TopLayer;

	/// Communication modes table.
	HypCCommModesTable m_CommModesTable;

	/// Statistic collection.
	HypCStatistics m_Stat;

	/// Type of proxy module.
	enum HypEModuleType m_proxyModuleType;
};

/**
	@}
*/

#endif // _HYPCCOMMUNICATION_HPP_
