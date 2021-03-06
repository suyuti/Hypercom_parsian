
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             schedule.c
//      Schedule Functions
//
//=============================================================================

// System includes
#include "string.h"

// HSDK includes
#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"
#include "fixdata.h"

// Application includes
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"
#include "hypertcf.h"

// This module external declaration
extern void Schedule( void );

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static Bool ProcessDLL( void );
static Bool ProcessINIT( void );
static void ProcDLLNow( void );
static void ProcINITNow( void );
static Bool ProcDLLStatus( void );
static Bool ProcINITStatus( void );
static void ClrUsedEntry( void );
static Bool IsTimeXXMins( UBYTE mins );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static const UBYTE LOADNOW[6] = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99 };

static struct schedule_rec *pSchedulePtr;	// Pointer to SCHTAB entry      


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Downline Loads and Initializes Schedule.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Schedule( void )
{
	Bool ClearFlag = False;

	// Search schedule table for the entry used 
	for ( pSchedulePtr = SCHTAB; pSchedulePtr < &SCHTAB[SCHMAX];
		  pSchedulePtr++ )
	{
		// Look at every entry to see if this entry is used. 
		if ( pSchedulePtr->SCHFLAG & SCH_USED )
		{
			// Yes, process schedule entry information 
			// Check the schedule message type 
			if ( SM_DLL == pSchedulePtr->SMTYPE )
			{
				// Process Downline Load Schedule 
				if ( !ProcessDLL(  ) )
				{
					ClearFlag = True;
				}
			}
			else if ( SM_INIT == pSchedulePtr->SMTYPE )
			{
				// Process Initialize Schedule 
				if ( !ProcessINIT(  ) )
				{
					ClearFlag = True;
				}
			}

			if ( ClearFlag )
			{
				// Clear this entry used 
				ClrUsedEntry(  );
			}
		}
	}

	// Wait for one second 
	SDK_Wait( ONESECOND );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Process Downline Load Schedule Entry.
//!
//! \return
//!     True if download successful otherwise False
//-----------------------------------------------------------------------------
static Bool ProcessDLL( void )
{
	static Bool retval;

	retval = True;

	// See if schedule flag's bit 2 is set 
	if ( pSchedulePtr->SCHFLAG & SCH_REQ )
	{
		// Yes, This event has already been started 
		// Check result of program load and restore parameters changed 
		if ( !ProcDLLStatus(  ) )
		{
			// return false so event will be cleared from schedule 
			retval = False;
		}
	}
	else
	{
		// See if a permanent update 
		if ( SMTEL_PERM == pSchedulePtr->SMTELTYPE )
		{
			// Initialize the TERM NMS phone number field (12) 
			memset( (UBYTE *)TERM.TERMNmsTel, 0xFF, sizeof( TERM.TERMNmsTel ) );

			memcpy( (UBYTE *)TERM.TERMNmsTel, pSchedulePtr->SMTELNO, S_SMTELNO );

			// Recalculate the LRC 
			SetTERMTabLRC(  );
		}

		// See if scheduled time is null 
		if ( NullComp( ( char * ) pSchedulePtr->SMWHEN, S_SMWHEN ) )
		{
			// Null, Exit with false, so clear event from schedule 
			retval = False;
		}
		else
		{
			// See if scheduled for load now,                               
			// or current time same as scheduled time ( exception seconds ) 
			// or current time is equal to scheduled time plus 30 minutes,  
			// or current time is equal to scheduled time plus 60 minutes,  
			if (   !memcmp( pSchedulePtr->SMWHEN, ( UBYTE * ) LOADNOW, S_SMWHEN ) 
				|| IsTimeXXMins( 0x00 ) || IsTimeXXMins( 0x30 )
				|| IsTimeXXMins( 0x60 ) )
			{
				// Yes, Initiate the program load now 
				ProcDLLNow(  );
			}
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     See if current time is XX minutes after the scheduled time.
//!
//! \param
//!		Mins	XX minutes.
//!
//! \return
//!     True if XX minutes have been elapsed after the scheduled time
//!     otherwise False
//-----------------------------------------------------------------------------
static Bool IsTimeXXMins( UBYTE Mins )
{
	static UBYTE retval;
	static UBYTE RtcBuf[RTCLEN];	// RTC Read Buffer             
	static UBYTE RtcBcd[5];		// Current YYMMDDHHMM in BCD.  
	static UBYTE SchBcd[5];		// Copy of scheduled time      

	// Default return value to false 
	retval = False;

	// Read the real time clock.  
	SDK_RtcRead( RtcBuf );

	// Convert current YYMMDDHHMM to BCD 
	AscHex( RtcBcd, ( char * ) &RtcBuf[0], ( UBYTE ) ( S_SMWHEN - 1 ) );

	// Get the scheduled time from schedule event 
	memcpy( SchBcd, pSchedulePtr->SMWHEN, ( UWORD ) ( S_SMWHEN - 1 ) );

	if ( 0x00 == Mins )
	{
		if ( !memcmp( SchBcd, RtcBcd, ( UWORD ) ( S_SMWHEN - 1 ) ) )
		{
			// Current time same as scheduled time ( exception seconds ) 
			retval = True;
		}
	}
	else
	{
		if ( 0x30 == Mins )
		{
			// Add 30 minutes to the scheduled time 
			SchBcd[4] += 0x30;

			// See if minutes more than 60 minutes 
			if ( SchBcd[4] >= 0x60 )
			{
				// Add one to hours 
				SchBcd[3] += 0x01;

				// get the minutes 
				SchBcd[4] -= 0x60;
			}
		}
		else if ( 0x60 == Mins )
		{
			// Add one hour to the scheduled time 
			SchBcd[3] += 0x01;
		}

		// See if passed the 24 hours 
		if ( SchBcd[3] >= 0x24 )
		{
			// Add one day to the scheduled time 
			SchBcd[2] += 0x01;

			// Get the hours 
			SchBcd[3] -= 0x24;
		}

		// See if current date/time is 30 or 
		// 60 minutes after scheduled date/time 
		if ( !memcmp( &RtcBcd[0], &SchBcd[0], ( UWORD ) 5 ) )
		{
			// Yes, return true 
			retval = True;
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Clear this entry used.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ClrUsedEntry( void )
{
	// Clear this entry used 
	pSchedulePtr->SCHFLAG = 0;

	// Recalculate the LRC 
	SetSCHTabLRC(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Process Initialize Schedule	Entry.
//!
//! \return
//!     True if Successful, False event will be cleared from schedule.
//-----------------------------------------------------------------------------
static Bool ProcessINIT( void )
{
	static Bool retval;

	retval = True;

	// See if schedule flag's bit 2 is set 
	if ( pSchedulePtr->SCHFLAG & SCH_REQ )
	{
		// Yes, This event has already been started 
		// Check result of initialize load and restore parameters changed 
		if ( !ProcINITStatus(  ) )
		{
			// return false so event will be cleared from schedule 
			retval = False;
		}
	}
	else
	{
		// See if a permanent update 
		if ( SMTEL_PERM == pSchedulePtr->SMTELTYPE )
		{
			// Initialize the TERM Init phone number field (12) 
			memset( (UBYTE *)TERM.TERMInitTel, 0xFF, sizeof( TERM.TERMInitTel ) );

			memcpy( (UBYTE *)TERM.TERMInitTel, pSchedulePtr->SMTELNO, S_SMTELNO );

			// Recalculate the LRC 
			SetTERMTabLRC(  );
		}

		// See if scheduled time is null 
		if ( NullComp( ( char * ) pSchedulePtr->SMWHEN, S_SMWHEN ) )
		{
			// Null, Exit with false, so clear event from schedule 
			retval = False;
		}
		else
		{
			// See if scheduled for intialize now,                          
			// or current time same as scheduled time ( exception seconds ) 
			// or current time is equal to scheduled time plus 30 minutes,  
			// or current time is equal to scheduled time plus 60 minutes,  
			if (   !memcmp( ( UBYTE * ) pSchedulePtr->SMWHEN, ( UBYTE * ) LOADNOW, S_SMWHEN ) 
				|| IsTimeXXMins( 0x00 ) 
				|| IsTimeXXMins( 0x30 )
				|| IsTimeXXMins( 0x60 ) )
			{
				// Yes, Initiate the intialize now 
				ProcINITNow(  );
			}
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Initiate the program load now.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcDLLNow( void )
{
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry  

	// See if a temporary update 
	if ( SMTEL_TEMP == pSchedulePtr->SMTELTYPE )
	{
		// Yes, Save current NMS telephone number 
		memcpy( pSchedulePtr->SCHOLDTEL, (UBYTE *)TERM.TERMNmsTel, S_SCHOLDTEL );

		// Initialize the TERMMaster NMS phone number field (12) 
		memset( (UBYTE *)TERM.TERMNmsTel, 0xFF, sizeof( TERM.TERMNmsTel ) );

		memcpy( (UBYTE *)TERM.TERMNmsTel, pSchedulePtr->SMTELNO, S_SMTELNO );

		// Recalculate the LRC 
		SetTERMTabLRC(  );
	}
	else
	{
		// See if use acquirer 1 primary 
		if ( SMTEL_AQ1P == pSchedulePtr->SMTELTYPE )
		{
			// Yes, Save current NMS telephone number 
			memcpy( pSchedulePtr->SCHOLDTEL, (UBYTE *)TERM.TERMNmsTel, S_SCHOLDTEL );

			// Initialize the TERMMaster NMS phone number field (12) 
			memset( (UBYTE *)TERM.TERMNmsTel, 0xFF, sizeof( TERM.TERMNmsTel ) );

			pAcquirerPtr = FindFirstAcquirer(  );

			// If acquirer exists 
			if ( NULL != pAcquirerPtr )
			{
				MoveItAq( (UBYTE *)TERM.TERMNmsTel, pAcquirerPtr->conn.dial.trans.primary.phoneNum,
						sizeof( TERM.TERMNmsTel ) );

				// Recalculate the LRC 
				SetTERMTabLRC(  );
			}
		}
	}

	// Save current software name 
	memcpy( pSchedulePtr->SCHOLDSWARE, VERSION, S_SCHOLDSWARE );

	// Set required for program load 
	pSchedulePtr->SCHFLAG |= SCH_REQ;

	// Recalculate the LRC 
	SetSCHTabLRC(  );

	// Wait 5 seconds for other process to finish 
	SDK_Wait( ONESECOND * 5 );

	// Restart Terminal and request Program Load 
	SDK_TerminalLoad(  );

	// Loop till Terminal Restarted 
	while ( True )
	{
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Initiate the terminal initialize now.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcINITNow( void )
{
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry  

	// See if a temporary update 
	if ( SMTEL_TEMP == pSchedulePtr->SMTELTYPE )
	{
		// Yes, Save current Init telephone number 
		memcpy( pSchedulePtr->SCHOLDTEL, (UBYTE *)TERM.TERMInitTel, S_SCHOLDTEL );

		// Initialize the TERMMaster Init phone number field (12) 
		memset( (UBYTE *)TERM.TERMInitTel, 0xFF, sizeof( TERM.TERMInitTel ) );

		memcpy( (UBYTE *)TERM.TERMInitTel, pSchedulePtr->SMTELNO, S_SMTELNO );

		// Recalculate the LRC 
		SetTERMTabLRC(  );
	}
	else
	{
		// See if use acquirer 1 primary telephone number 
		if ( SMTEL_AQ1P == pSchedulePtr->SMTELTYPE )
		{
			// Yes, Save current Init telephone number 
			memcpy( pSchedulePtr->SCHOLDTEL, (UBYTE *)TERM.TERMInitTel, S_SCHOLDTEL );

			// Initialize the TERMMaster Init phone number field (12) 
			memset( (UBYTE *)TERM.TERMInitTel, 0xFF, sizeof( TERM.TERMInitTel ) );

			pAcquirerPtr = FindFirstAcquirer(  );

			// If acquirer exists 
			if ( NULL != pAcquirerPtr )
			{
				MoveItAq( (UBYTE *)TERM.TERMInitTel, pAcquirerPtr->conn.dial.trans.primary.phoneNum,
						sizeof( TERM.TERMInitTel ) );

				// Recalculate the LRC 
				SetTERMTabLRC(  );
			}
		}
	}

	// Save current software name 
	memcpy( pSchedulePtr->SCHOLDSWARE, VERSION, S_SCHOLDSWARE );

	// Set required for initialize 
	pSchedulePtr->SCHFLAG |= SCH_REQ;

	// Recalculate the LRC 
	SetSCHTabLRC(  );

	// Initiate the initialize 
	TERM.TERMInit = 0x0FE;

	SetTERMTabLRC(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Check result of program load and restore parameters changed.
//!
//! \return
//!     True if Event won't be cleared from schedule else False if Scheduled
//!     Download Program was successful event will be cleared from schedule.
//-----------------------------------------------------------------------------
static Bool ProcDLLStatus( void )
{
	static Bool retval;

	retval = True;

	// See if a temporary update 
	if ( (SMTEL_TEMP == pSchedulePtr->SMTELTYPE) ||
	 	(SMTEL_AQ1P == pSchedulePtr->SMTELTYPE))
	{
		// Yes, Save current NMS telephone number 
		memcpy( (UBYTE *)TERM.TERMNmsTel, pSchedulePtr->SCHOLDTEL,
				sizeof( TERM.TERMNmsTel ) );

		// Recalculate the LRC 
		SetTERMTabLRC(  );
	}

	// See if time scheduled for load now 
	if ( memcmp( pSchedulePtr->SMWHEN, ( UBYTE * ) LOADNOW, S_SMWHEN ) )
	{
		// Not same, exit with false so event will be cleared from schedule 
		retval = False;
	}
	else
	{
		// See if software version has been changed 
		if ( !memcmp( pSchedulePtr->SCHOLDSWARE, VERSION, S_SCHOLDSWARE ) )
		{
			// Not changed 
			retval = False;
		}
		else
		{
			// Secheduled Download wasn't successful, Reset schedule flag's bit 2 
			pSchedulePtr->SCHFLAG &= ~SCH_REQ;

			// Recalculate the LRC 
			SetSCHTabLRC(  );
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Check result of initialize and restore parameters changed.
//!
//! \return
//!     True if Event won't be cleared from schedule else False if Scheduled
//!     Initialize was successful event will be cleared from schedule.
//-----------------------------------------------------------------------------
static Bool ProcINITStatus( void )
{
	static Bool retval;

	retval = True;

	// See if initialization is not in progress 
	if ( !( TFTAB.TFIND1 & TF1_DOING_INIT ) )
	{
		// See if a temporary update 
		if ( (SMTEL_TEMP == pSchedulePtr->SMTELTYPE) ||
	 		(SMTEL_AQ1P == pSchedulePtr->SMTELTYPE))
		{
			// Yes, Save current NMS telephone number 
			memcpy( (UBYTE *)TERM.TERMNmsTel, pSchedulePtr->SCHOLDTEL,
					sizeof( TERM.TERMNmsTel ) );

			// Recalculate the LRC 
			SetTERMTabLRC(  );
		}

		// See if initialization done 
		if ( 0x0FF == TERM.TERMInit )
		{
			// Scheduled initialization was successful 
			// exit with false so event will be cleared from schedule 
			retval = False;
		}
		else
		{
			// Scheduled initialization wasn't successful 
			// Reset schedule flag's bit 2 
			pSchedulePtr->SCHFLAG &= ~SCH_REQ;

			// Recalculate the LRC 
			SetSCHTabLRC(  );
		}
	}

	return retval;
}
