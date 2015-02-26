/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

//!
//! \file error.h
//!     \brief returns the error enumeration
//!     \since 1996
//!


//-----------------------------------------------------------------------------
//! \brief Converts OS error value to enumeration
//!
//! \param[in]  OSErrorValue    value returned from OS
//!
//! \return Converted SDK error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS OSError( UBYTE OSErrorValue );
