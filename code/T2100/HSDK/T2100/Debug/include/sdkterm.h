/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdkterm.h 
//!     \brief SDK Term defintions and function prototypes. 
//!     \since 1996
//!

#ifndef SDKTERM
#define SDKTERM

/*
=============================================================================
*       SDK Term defintions and function prototypes.                        *
*       This file should be included in all 'C' source code files.          *
=============================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

// **************** DEFINITIONS AND ENUMERATIONS ******************

// ******************* PROTOTYPE DEFINITIONS ************************

/**
	@addtogroup GroupSDK
	@{
*/

/** 	@brief	Causes the terminal to be reset.

	This function resets the Hypercom terminal. A call to this function does not return.*/

extern void SDK_TerminalReset(void);

/** 	@brief	Reboot the terminal*/

extern void SDK_TerminalReboot(void);

/** 	@brief	Causes the terminal to reset and request a program load.

        This function causes the terminal to reset and initiates the downloading or reloading of the
        application. A call to this function does not return.
*/

extern void SDK_TerminalLoad(void);

/** 	@brief	Initializes terminal data with default values.

        This function initializes the terminal data with default values. The terminal data contains
        parameters as setup in the boot program. Generally, most applications will not use this
        function and it should not be called except in rare cases where the terminal data needs to
        be set to default values.

        The terminal data is declared as TERM. TERM is defined in fixdata.o as supplied with the
        HSDK. TERM is a structure of the type TERMREC.

        This function initializes TERM by copying default data from the boot page into TERM. TERM
        can be accessed at the application level by making the following declaration:

		extern TERMREC TERM; 

	The HSDK-supplied object file fixdata.o must be linked with your application to access TERM.
*/

extern void SDK_TerminalDataInit(void);

/** 	@brief	Validates the terminal data and initializes if invalid.

        This function validates the terminal data and initializes the data if it is found to be
        invalid. The terminal data contains parameters as setup in the boot program. The function
        should be called at the start of an application.

        This function tests the LRC of the data and the format byte. If the LRC is incorrect or the
        format byte is not set to 0x55, then this function initializes the terminal data with
        default data from the boot page.

        The terminal data is declared as TERM. TERM is defined in fixdata.o as supplied with the
        HSDK. TERM is a structure of the type TERMREC.

	TERM can be accessed at the application level by making the following declaration:

		extern TERMREC TERM; 

	The HSDK-supplied object file fixdata.o must be linked with your application to access TERM.

	@return	0       - data is valid\n
		1       - data corrupt, LRC error on data or table not formatted
*/

extern UBYTE SDK_TerminalDataValidate(void);

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif /* SDKTERM */
