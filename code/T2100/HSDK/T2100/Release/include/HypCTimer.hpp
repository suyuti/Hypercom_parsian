//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCTIMER_HPP_
#define _HYPCTIMER_HPP_


//=============================================================================
//
// HypCTimer.hpp
//
// Sends event into specified HypCEvent object
//
//=============================================================================
#include <eftos.h>
#include <hw.h>
#include "HypCEvent.hpp"


//=============================================================================
//
//     HypCTimer
//
// Base Class:
//      
//
// Overview:
//	this class sends event to HypCEvent queue with specified
//  requestID parameter when the timer expires.
//  
//
// Related Documents:
//      None.
//
// See Also:
//		HypCEvent.hpp
//
//=============================================================================

class HypCTimer
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		// Constructor, timer will send event to ev queue with reqID parameter
		HypCTimer( HypCEvent &ev, int reqId = 0 );

		// Destructor
		virtual ~HypCTimer();

		//Function starts timer. When timer is expired, event with specified reqId
		//will be sent to event queue. The time parameter specifies time in milliseconds.
		//HypCEvent queue has been already assigned in constructor.
		void Start( unsigned int time, int reqId )
		{
			m_ReqId = reqId;
			Start( time );
		}
		//Function starts timer. When timer is expired, event will be sent to event queue.
		//reqID and HypCEvent queue have been already assigned in constructor or in Start()
		//The time parameter specifies time in milliseconds.
		void Start( unsigned int time );

		//Stops timer
		void Stop( void );

		//Restart timer
		void Restart( unsigned int time )
		{
			Stop( );
			Start( time );
		}


	private:
		static void CallBack( unsigned int p );
		static void Task( );

//=============================================================================
// Member variables
//=============================================================================
	private:
		int m_ReqId;
		UNSIGNED m_Time;
		HypCEvent &m_Listener;
		unsigned int m_TimerID;
		int m_JobNr;
		EVT_STRUCT m_WaitEvt;
		BOOL m_Flag;
};

#endif // _HYPCTIMER_HPP_
