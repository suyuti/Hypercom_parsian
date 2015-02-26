//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCEVENT_HPP_
#define _HYPCEVENT_HPP_

#include <eftos.h>
#include <nu_compat.h>
#include "SimpleVector.hpp"
#include <hyputils.h>
#include <HypCEventInfo.hpp>

/**
	@addtogroup HPOSFC
	@{
*/
        
/**
	@brief Event manipulation class.

	Event queue class. It responsible for syncronization tasks (threads)
	via delivery messages (events) between tasks. Event is 4th element
	message, each element has unsigned int type. The elements have
	following names: Request ID, Status, Source Address and Optional
	Parameter.

	HypCEvent object is a queue for events. Any task can send events to
	this queue, and single task can get event from queue. If no events are
	available in queue, task can block on receiving event from queue (wait
	function) until other task send event to queue.

	All elements of event may be used for passing any information, however
	usually Request ID is used as type of request (e.g. read request, write
	request and etc.: see definitions in file transp_def.h). The Status is
	used for return result of request (see HypEErrors.h). The Source
	Address is used for return ID of the object which have performed the
	request. The Optional Parameter during read request is used to return
	pointer to HypCBuffer object with read data.

	@note: Only single task can be blocked on wait function at one time.
*/
class HypCEvent
{
public:
	/**
		@brief Constructor.

		@param qsize Queue size (optional, defaults to 8).

		@note Queue is not increased automatically.
	*/
	HypCEvent( int qsize = 8 );

	/**
		@brief Destructor.
	*/
	virtual ~HypCEvent() { Reset(); NU_Delete_Queue( &m_Queue ); delete [] m_QueueBuf; }

	/**
		@brief Remove all events from queue.
	*/
	void Reset();

	/**
		@brief Wait for events (event info version).

		@param eventInfo Reference to event info container.
		@param timeout If equals to zero the call is non-blocking,
		               if equals to -1 casted to unsigned the call is
		               blocking (optional, defaults to blocking).

		@return true if eventInfo is changed, false otherwise.
	*/
	bool Wait( HypCEventInfo &eventInfo, unsigned int timeout = ( unsigned int )-1 );
		
	/**
		@brief Wait for events (4 parameters version).

		Call is blocked until receving event.

		@param reqId Reference to Request ID.
		@param status Reference to Status.
		@param srcAddr Reference to Source Address.
		@param opt Reference to Optional Parameter.
	*/
	void Wait( int &reqId, int &status, int &srcAddr, int &opt );
		
	/**
		@brief Wait for events (3 parameters version).

		Call is blocked until receving event.

		@param reqId Reference to Request ID.
		@param status Reference to Status.
		@param srcAddr Reference to Source Address.
	*/
	void Wait( int &reqId, int &status, int &srcAddr );
		
	/**
		@brief Wait for events (2 parameters version).

		Call is blocked until receving event.

		@param reqId Reference to Request ID.
		@param status Reference to Status.
	*/
	void Wait( int &reqId, int &status );
		
	/**
		@brief Wait for events (1 parameters version).

		Call is blocked until receving event.

		@param reqId Reference to Request ID.
	*/
	void Wait( int &reqId );

	/**
		@brief Send event to queue.

		@param reqId Reference to Request ID.
		@param status Reference to Status (optional, defaults to 0).
		@param srcAddr Reference to Source Address (optional, defaults
		               to 0).
		@param opt Reference to Optional Parameter (optional, defaults
		           to 0).
	*/
	void Send( int reqId, int status = 0, int srcAddr = 0, int opt = 0 );

	/**
		@brief Send event to front of queue.

		@param reqId Reference to Request ID.
		@param status Reference to Status (optional, defaults to 0).
		@param srcAddr Reference to Source Address (optional, defaults
		               to 0).
		@param opt Reference to Optional Parameter (optional, defaults
		           to 0).
	*/
	void SendToFront( int reqId, int status = 0, int srcAddr = 0, int opt = 0 );

	/**
		@brief Remove specified event from queue.

		@param reqId Request ID to find and remove.
	*/
	void RemoveMsg( int reqId );
		
	/**
		@brief Wait for specific events.

		Call is blocked until receving one of two specified events. All
		other events in queue are preserved.

		@param eventInfo Reference to event info container.
		@param req1 1st request to wait.
		@param req2 2nd request to wait (optional, defaults to -1).
	*/
	void Wait2( HypCEventInfo &eventInfo, int req1, int req2 = -1 );

private:
	/**
		@brief Wait from Nucleus-style queue.

		@param eventInfo Reference to event info container.
		@param timeout If equals to zero the call is non-blocking,
		               if equals to -1 casted to unsigned the call is
		               blocking (optional, defaults to blocking).

		@return true if eventInfo is changed, false otherwise.
	*/
	bool WaitFromNU_Queue( HypCEventInfo &eventInfo, unsigned int timeout = (unsigned int)-1 );

private:
	/// Nucleus-style queue, used for event exchange.
	NU_QUEUE m_Queue;

	/// Queue container.
	uint8_t *m_QueueBuf;

	/// Queue size.
	unsigned int m_QSize;

	/// Data structure stored in queue.
	struct _queueElem
	{
		int req;                ///< Request ID.
		int status;             ///< Status.
		int id;                 ///< Source Address.
		int opt;                ///< Optional Parameter.
	};

	/// Type definition for the array of events.
	typedef SimpleVector<class HypCEventInfo> HypCEventQ;

	/// Temporary array of events.
	HypCEventQ *m_pEventQ;
};

/**
	@brief Remove all events from queue.

	@note If event has Request ID set to READ_REQUEST and its Status is set
	      to HTENOERROR and Optional Parameter is not NULL, this functions
	      interprets Optional Parameter as pointer on HypCBuffer and
	      deletes it.
*/
extern void ClearEvent( HypCEvent *pEvent );

/**
	@}
*/

#endif // _HYPCEVENT_HPP_
