/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdkproc.h
//!     \brief SDK Process library defintions and function prototypes.
//!     \since 1996
//!

#ifndef SDKPROC
#define SDKPROC

/*
=============================================================================
*   SDK Process library defintions and function prototypes.                 *
*   This file should be included in all 'C' source code files.              *
=============================================================================
*/


#if 1 /* obsolete */

#warning Obsolete file included

#else /* obsolete */

/**
	@addtogroup GroupSDK
	@{
*/

/**	@brief	Register a routine entry point for a process
	@param	page_of_routine - page where routine resides
	@param	pRoutine - address of routine to be called 
*/

void RegisterProcess( PAGE_NUMBERS page_of_routine, pFUNCvoid Routine );

/**	@brief	Relinquish rest of process slice back to CPU.*/

void RelinqCPU( void );

#endif /* obsolete */

/**
	@}
*/

#endif  /* ifndef SDKPROC */
