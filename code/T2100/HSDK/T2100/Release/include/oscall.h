/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file oscall.h 
//!     \brief SDK function prototypes for OS calls.
//!     \since 2002
//!

#ifndef OSCALLH
#define OSCALLH

#include "sdkos.h"
#include "ddtm.h"			// contains port numbers

/**
	@addtogroup GroupSDK
	@{
*/

//-----------------------------------------------------------------------------
//! \brief Performs O/S open command
//!
//! \param[in]  devicePort - device port id
//!
//! \return OS_RETURN_VALS
//-----------------------------------------------------------------------------
OS_RETURN_VALS SDK_OSOpen( PORT devicePort );

/**	@example SDK_OSOpen.c */

//-----------------------------------------------------------------------------
//! \brief Performs O/S close command
//!
//! \param[in]  devicePort - device port id
//!
//! \return OS_RETURN_VALS
//!
//! \note Direct call to operating system (O/S) to close specified device
//-----------------------------------------------------------------------------
OS_RETURN_VALS SDK_OSClose( PORT devicePort );

//-----------------------------------------------------------------------------
//! \brief Performs O/S rewind command
//!
//! \param[in]  devicePort - device port id
//!
//! \return OS_RETURN_VALS
//!
//! \note Direct call to operating system (O/S) to rewind specified device
//-----------------------------------------------------------------------------
OS_RETURN_VALS SDK_OSRewind( PORT devicePort );

//-----------------------------------------------------------------------------
//! \brief Performs O/S configuration command
//!
//! If pIocsBlock is null, then block will be put 
//! on stack and a wait till complete IOCS call
//! will be performed.
//! If an IOCS block is specified then error
//! status upon flag will be in 
//! pIocsBlock->iocs_stati
//!
//! \param[in]  devicePort device port id
//! \param[in]  mode configuration mode
//! \param[in]  data data as required by mode
//! \param[in]  pIocsBlock pointer to caller's IOCS block
//! \param[in]  pAux pointer to caller's flag or returned data storage - 
//!                  dependent upon mode and devicePort
//!
//! \return OS_RETURN_VALS
//-----------------------------------------------------------------------------
#ifdef MAKE_VHDT
OS_RETURN_VALS SDK_OSConfig( PORT devicePort, UBYTE mode, IOCS_DDDA_DATA *data,
                                    IOCS_BLK *pIocsBlock, UBYTE *pAux );
#else
OS_RETURN_VALS SDK_OSConfig( PORT devicePort, UBYTE mode, UWORD data,
                                    IOCS_BLK *pIocsBlock, UBYTE *pAux );
#endif

//-----------------------------------------------------------------------------
//! \brief Performs O/S read command direct
//!
//! Error status upon completion will be in 
//! pIocsBlock->iocs_stati and read length will be
//! in pIocsBlock->iocs_var.iocs_xfer.iocs_tlenx
//! 
//! Completion is determined by *pFlag set
//! to non-zero. This function clears *pFlag
//! at start.
//! 
//! User does not need to fill in IOCS_BLK values,
//! just needs to declare for storage
//!
//! \param[in]  devicePort device port id
//! \param[in]  pBuffer pointer to data storage area
//! \param[in]  pBuflen pointer to buf length
//! \param[in]  pIocsBlock pointer to caller's IOCS block
//! \param[in]  pFlag pointer to caller's flag
//!
//! \return OS_RETURN_VALS
//-----------------------------------------------------------------------------
OS_RETURN_VALS SDK_OSRead( PORT devicePort, void *pBuffer, UWORD *pBuflen,
                                    IOCS_BLK *pIocsBlock, UBYTE *pFlag );

//-----------------------------------------------------------------------------
//! \brief Performs O/S write command direct to O/S
//!
//! Error status upon completion will be in 
//! pIocsBlock->iocs_stati and write length will be
//! in pIocsBlock->iocs_var.iocs_xfer.iocs_tlenx
//! 
//! Completion is determined by *pFlag set
//! to non-zero. This function clears *pFlag
//! at start.
//! 
//! User does not need to fill in IOCS_BLK values,
//! just needs to declare for storage
//!
//! \param[in]  devicePort device port id
//! \param[in]  pBuffer data to write
//! \param[in]  Buflen pointer to buf length
//! \param[in]  pIocsBlock pointer to caller's IOCS block
//! \param[in]  pFlag pointer to caller's flag
//!
//! \return OS_RETURN_VALS
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_OSWrite( PORT devicePort, void *pBuffer, UWORD Buflen,
                                   IOCS_BLK *pIocsBlock, UBYTE *pFlag );

#endif

/**
	@}
*/
