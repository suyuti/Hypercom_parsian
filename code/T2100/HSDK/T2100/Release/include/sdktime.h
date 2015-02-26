/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdktime.h 
//!     \brief SDK Timing library defintions and function prototypes.
//!     \since 1996
//!

#ifndef SDKTIME
#define SDKTIME

/*
=============================================================================
*       SDK Timing library defintions and function prototypes.              *
*       This file should be included in all 'C' source code files.          *
=============================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
	@brief Timer values used in RegisterTimerRoutine
*/
typedef enum
{
    rt_ONE_SEC = 100,		/**< match timer values in execdef */
    rt_ONE_MIN = 6000
} REG_TIMES;

/**
	@addtogroup GroupSDK
	@{
*/

/** 	@brief	Relinquish the time slice of this process back to the O/S.

        This function relinquishes the time slice of the current process, allowing other processes
        to run. When a process is waiting for a flag to be set or for some other event, using this
        function avoids wasting processing time. Other processes can use the extra time.
*/

extern void SDK_RelinqCPU( void );

//-----------------------------------------------------------------------------
//! \brief Wait for some period of time
//!
//! doesn't return until time passes
//!
//! \param[in]  WaitDuration		timeout value in ticks
//!
//! \return none
//-----------------------------------------------------------------------------
extern void SDK_Wait( OS_TIMES WaitDuration );

/** 	@brief	Starts a timer and sets a flag when the timer expires.

        This function sets a flag after a given time expires. The function automatically clears the
        flag pointed to by the pflag parameter. The function sets the flag to a nonzero value when
        the time specified by WaitDuration expires. This function returns a timer ID that can be
        used to cancel the time before it expires with SDK_CancelTimer( ).

	@param	WaitDuration - time to wait in units of OS_TIMES
	@param	pFlag - pointer to the flag that is set when the time expires
	@return	timer id, use SDK_CancelTimer to cancel this timer
	@note Multiple timers can be initiated with SDK_WaitAndFlag( ).
*/ 
extern UBYTE SDK_WaitAndFlag( OS_TIMES WaitDuration, UBYTE * pFlag );

/**	@example SDK_WaitAndFlag.c */

/** 	@brief	Cancels a timer previously initiated by SDK_WaitAndFlag().

        This function cancels a timer that SDK_WaitAndFlag() initiated. This function needs to be
        used only to cancel a timer before it expires. If the timer is canceled, the flag specified
        in SDK_WaitAndFlag() is not set.

	@param	TimerId - id of timer retured by SDK_WaitAndFlag() 
	@note If the timer ID specified does not exist, SDK_CancelTimer() has no effect.
*/ 

extern void SDK_CancelTimer( UBYTE TimerId );

/**	@example SDK_CancelTimer.c */

/** 	@brief	Register a routine to be called at a specified time interval.

        This function registers a routine to be called at a specified time interval. The interval is
        of the type REG_TIMES, which presently allows only two values: rt_ONE_SEC and rt_ONE_MIN.
        Timer routines should do little and be completed quickly. System performance degradation
        occurs if timer routines are inefficient.
	@param	time_interval - allowable register time increments in units of REG_TIMES as defined in sdktime.h
	@param	page_number - page number where the routine resides
	@param	pRoutine - address of routine which can be called 
*/

extern OS_RETURN_VALS SDK_RegisterTimerRoutine( REG_TIMES time_interval,
					           PAGE_NUMBERS page_number,
						        pFUNCvoid pRoutine );
/**	@example SDK_RegisterTimerRoutine.c */

/** 	@brief	Wake up a process which has been previously put to sleep using SDK_Sleep().

        This function wakes up either the current process or the named process. The name of a
        process is that as defined in the main file for an application page.

        When a process is asleep, interrupts such as those set up by SDK_RegisterTimerRoutine() are
        still called. For that reason, the current process can wake itself up or put itself to sleep.

	@param	pName -  if NULL, then current process is awaken, else pName points to
		 	4 character name of process to be awaken
	@return	orvOK or orvPROCESSNOTFOUND
*/ 

extern OS_RETURN_VALS SDK_Wake( char *pName );

/** 	@brief Puts a process to sleep.

        This function puts the current process to sleep or the named process. The name of a process
        is that as defined in the main file for an application page.

	For example, the name of the process for the process definition below is "TEST".\n
	static const PROCESS Test[] = \n
	{\n
		USER,INITTEST,'T', 'E','S','T'\n
	};\n
	A process can be awaken using SDK_Wake().

        When a process is asleep, interrupts such as those set up by SDK_RegisterTimerRoutine() are
        still called. For that reason, the current process can wake itself up or put itself to
        sleep.

 	@param  pName -  if NULL, then current process is put to sleep, else pName points to
 			4 character name of process to be put to sleep
  	@return orvOK or orvPROCESSNOTFOUND
*/

OS_RETURN_VALS SDK_Sleep( char *pName );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif		/* ifndef SDKTIME */
