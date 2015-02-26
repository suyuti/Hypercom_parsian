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
#ifndef _TIMER_H_
#define _TIMER_H_

#include "basictyp.h"

//=============================================================================
// Public functions defined in this module
//=============================================================================

//=============================================================================
// Public data definitions
//=============================================================================

//=============================================================================
// Public function definitions
//=============================================================================
//! Creates a callback timer
void *Timer_CreateCallback( UNSIGNED tid, void (*callback)(UNSIGNED) );
//! Destroys a timer
void Timer_Close( void *timer );
//! Set timer timeout and restart it
void Timer_SetTime( void *timer,unsigned int time, unsigned int rep_time );

#endif // _TIMER_H_

