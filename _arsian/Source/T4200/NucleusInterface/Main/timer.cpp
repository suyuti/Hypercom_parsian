//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hyperware Base Application
//      (c) Copyright 2006
//
// File Name:
//      MSRArb.c
//
// File Contents:
//  Routines to uses arbitrators 
//
//=============================================================================

#include <HypCTimer.hpp>
extern "C" {
#include "basictyp.h"
#include "timer.h"
#include "DbgTrace.h"
}

//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Public functions defined in this module
//=============================================================================

//=============================================================================
// Public from other modules
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================

//=============================================================================
// Private function declarations
//=============================================================================

//=============================================================================
// Public data definitions
//=============================================================================

//=============================================================================
// Private data definitions
//=============================================================================

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Creates a callback timer
//!
//! \param
//!      tid        32-bit wide value which is stored in timer structure and
//!                 could be used to store pointer to timer object
//!      callback   callback function whick invokes by timer timeout
//! \return
//!      NULL on error else pointer to timer object
//!
//! \note
//!
void *Timer_CreateCallback( UNSIGNED tid, void (*callback)(UNSIGNED) )
{
    HypCTimer *pTmr= new HypCTimer( tid,callback );
    return static_cast<void*>(pTmr);
}

//-----------------------------------------------------------------------------
//!
//!      Destroys a timer
//!
//! \param
//!      timer      pointer to timer object to be destroyed.
//! \return
//!      None
//!
//! \note
//!
void Timer_Close( void *timer )
{
    if( timer )
        delete static_cast<HypCTimer*>(timer);
}

//-----------------------------------------------------------------------------
//!
//!      Set timer timeout and restart it
//!
//! \param
//!      timer      pointer to timer object to be set up
//!      time       prime timer's timeout
//!      rep_time   timer's repeat timeout
//!
//! \return
//!		None
//!
//! \note
//!
void Timer_SetTime( void *timer, unsigned int time, unsigned int rep_time )
{
    HypCTimer *pTmr= static_cast<HypCTimer*>(timer);
    pTmr->Start( time, rep_time );
}

//=============================================================================
// Private function definitions
//=============================================================================

