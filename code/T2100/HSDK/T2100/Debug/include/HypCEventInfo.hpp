//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCEVENTINFO_HPP_
#define _HYPCEVENTINFO_HPP_

/**
	@addtogroup HPOSFC
	@{
*/
        

/**
	@brief Event information container.

	This class provides event information for the application. Once
	application called the Wait method of HypCEvent it will receive the
	event information through this class.
*/
class HypCEventInfo
{
public:
	/**
		@brief Constructor.

		@param reqID Request ID field (optional, defaults to 0).
		@param status Request status (optional, defaults to 0).
		@param device Device ID (optional, defaults to 0).
		@param opt Optional parameter (optional, defaults to 0).
	*/
	HypCEventInfo( int reqID = 0, int status = 0, int device = 0, int opt = 0 ) : m_pDevice( device ), m_RequestID( reqID ), m_Status( status ), m_Opt( opt ) {}

	/**
		@brief Copy constructor.

		@param ei Event info to copy data from.
	*/
	HypCEventInfo( const HypCEventInfo &ei );

	/**
		@brief Destructor.
	*/
	~HypCEventInfo() {}

	/**
		@brief Set all parameters.

		@param reqID Request ID field.
		@param status Request status (optional, defaults to 0).
		@param device Device ID (optional, defaults to 0).
		@param opt Optional parameter (optional, defaults to 0).
	*/
	void Set( int reqID, int status = 0, int device = 0, int opt = 0 ) { m_pDevice = device; m_RequestID = reqID; m_Status = status; m_Opt = opt; }

	/**
		@brief Get device ID caused the event.

		@return Device ID caused the event.
	*/
	inline int GetDevice() const { return m_pDevice; }

	/**
		@brief Get event request ID.

		@return Event request ID.
	*/
	inline int GetRequestID() const { return m_RequestID; }

	/**
		@brief Get event status.

		@return Event status.
	*/
	inline int GetStatus() const { return m_Status; }

	/**
		@brief Get optional parameter.

		@return Optional parameter.
	*/
	inline int GetOpt() const { return m_Opt; }

private:
	/// Pointer to the device.
	int m_pDevice;

	/// Request ID.
	int m_RequestID;

	/// Status.
	int m_Status;

	/// Optional parameter.
	int m_Opt;
};

/**
	@}
*/

#endif // _HYPCEVENTINFO_HPP_
