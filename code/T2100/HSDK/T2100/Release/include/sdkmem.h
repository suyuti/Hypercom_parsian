/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdkmem.h
//!     \brief SDK Memory library defintions and function prototypes.  
//!     \since 1996
//!

#ifndef SDKMEM
#define SDKMEM

/*
=============================================================================
*   SDK Memory library defintions and function prototypes.                  *
*   This file should be included in all 'C' source code files.              *
=============================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupSDK
	@{
*/

/** 	@brief	Return the pageid of this page.

	This function returns the Page Number of the page from which the function is called.
	@return	Page number
*/
extern PAGE_NUMBERS SDK_ThisPage( void );

/** 	@brief	Returns the status of a specified page.

        This function returns the page status in terms of PAGE_STATUS, which is defined in SDKOS.H
        as follows:

	typedef enum\n
	{\n
	    psNO_FIT,		// page is not available\n
	    psFREE,		// avail, but not in use\n
	    psACTIVE,		// currently activated\n
	    psINACTIVE,		// deactivated\n
	    psLOADING,		// page is being loaded\n
	    psCORRUPT		// checksum error\n
	} PAGESTATUS;\n

	SDK_PageStatus() is often used to determine the number of pages available.

	@param	PageNum - Logical page number
	@note	Added support for ICE page status. 2-19-99 AW
*/

extern PAGESTATUS SDK_PageStatus( PAGE_NUMBERS PageNum );


/** 	@brief	Register a routine that can be called from other pages. 

        This function registers a routine on a page that can then be called from another page using
        SDK_PerformCrossCall(). Using this function combined with SDK_PerformCrossCall() enables
        application developers to call functions across paged memory boundaries.

        The functions called across pages, however, are not permitted to contain parameters or
        return values. Data, however, can be passed to and from the routine using global variables.
        Due to the constraints of linking on a per-page basis, only routines on the current page can
        be registered. However, they can be called from any page.

	@param	pRoutineId - ptr to 16 byte string used to identify routine.
	@param	page_number - page where routine resides
	@param	pRoutine - address of routine which can be called 
*/

extern OS_RETURN_VALS SDK_RegisterCrossCall( char *pRoutineId,
                                                   PAGE_NUMBERS page_number,
                                                     pFUNCvoid pRoutine );
/**	@example SDK_RegisterCrossCall.c */

/** 	@brief	Call a routine that is on another page.

        This function calls a routine on another page that was previously registered on that page
        using SDK_RegisterCrossCall(). Using this function with SDK_RegisterCrossCall() enables
        application developers to call functions across paged memory boundaries. However, the
        functions called across pages are not permitted to contain parameters or return values.

	See SDK_RegisterCrossCall() for more information. 

	@param	pRoutineId - ptr to 16 byte string used to identify routine.
*/

extern OS_RETURN_VALS SDK_PerformCrossCall( char *pRoutineId );

/** 	@brief	Register a address that can be accessed from other pages. 

        This function registers an address of data on a page that can be retrieved on another page
        using SDK_GetAddress(). Typically, this function is used to register addresses to be used by
        SDK_ReadPage() and SDK_WritePage().

        The address table maintained by the SDK hold 20 addresses. Address can be removed from the
        table by using SDK_UnRegisterAddress().

        SDK_GuiInit() must be called at some point in the application prior to using this function.

	@param	pAddressId - ptr to 16 byte string used to identify routine.
	@param	page_number - page where address resides
	@param	pAddress - address of data which can be accessed
*/

extern OS_RETURN_VALS SDK_RegisterAddress( char *pAddressId,
                                                   PAGE_NUMBERS page_number,
                                                   UBYTE *pAddress );
/**	@example SDK_RegisterAddress.c */

/** 	@brief	Remove an address from address table which was previously added by SDK_RegisterAddress().
	
        This function removes an address which was previously registered by SDK_RegisterAddress().
        The function is useful in cases where an address is only temporarily required. Since the
        address table maintained by the SDK contains only 20 addresses, this function is useful in
        clearing unused addresses.
	@param	pAddressId - ptr to 16 byte string used to identify routine.
*/

extern OS_RETURN_VALS SDK_UnRegisterAddress( char *pAddressId);

/** 	@brief	Learn address of significant data residing on another page. 
	@param	pAddressId - ptr to 16 byte string used to identify routine.
	@param	page_number - page where address resides
	@param	pAddress - where to put address
*/

extern OS_RETURN_VALS SDK_GetAddress( char *pAddressId,
                                              PAGE_NUMBERS * page_number,
                                              UBYTE ** pAddress );

/**	@brief	Read data from another page.

        This function provides a simple mechanism for reading data that resides on another page. Due
        to paged memory, data on one page is not directly accessible to other pages. This function
        is provided to retrieve data from other pages.

        If the address of the data on one page is unknown, you can use SDK_GetAddress() to get the
        address of data on another page as registered by SDK_RegisterAddress().
	@param	pReadData - pointer to buffer where data to be stored.
	@param	len_to_read - number of bytes to read
	@param	page_to_read - page where address resides
	@param	Address - pointer to Address
*/

extern OS_RETURN_VALS SDK_ReadPage( UBYTE * pReadData,
                                            UWORD len_to_read,
                                            PAGE_NUMBERS page_to_read,
                                            UBYTE * Address );
/**	@example SDK_ReadPage.c */

/**	@brief	Write data to another page.

        This function provides a simple mechanism for writing data to another page. Due to paged
        memory, data on a given page is not directly accessible to other pages. Therefore, this
        function is provided to write data across pages.

        If you do not know the address of where the data is to be written, you can use
        SDK_GetAddress() to get the address of data on another page as registered by
        SDK_RegisterAddress().

	@param	pWriteData -  ptr to buffer where data to be stored.
	@param	len_to_write - number of bytes to read
	@param	page_to_write - page where address resides
	@param	Address -  pointer to address
	@return	OS_RETURN_VAL
*/ 

extern OS_RETURN_VALS SDK_WritePage( UBYTE * pWriteData,
                                             UWORD len_to_write,
                                             PAGE_NUMBERS page_to_write,
                                             UBYTE * Address );
/**	@example SDK_WritePage.c */

/**	@brief	Initializes the start address and size for a new malloc area.

        This function initializes data as needed by the malloc routines supplied with the UniWare
        C-library. The UniWare routines include malloc(), free(), and mbrk(). Refer to UniWare
        documentation for information on these functions.

        When you use malloc, you must define two linker regions in the linker specifications for the
        page where the malloc area is to reside. These regions are region mallocvars and region
        malloc.

        Mallocvars is a RAM area that the malloc library routine uses to track the format status,
        start area, and size of the malloc area. This region can reside in upper or lower memory and
        should not be automatically zeroed on startup.

        Malloc is also a RAM area where the memory allocated from this region can reside in upper or
        lower memory and should not be automatically zeroed on startup. SDK_Init_Malloc() does not
        format (zero) the malloc area.
*/

extern void SDK_Init_Malloc(void);

/*
 * Byte used for keeping status of malloc area
 * i.e. Does it need to be formated.
 * User sets this byte.
 */

extern UBYTE SDK_malloc_format;

#ifdef __cplusplus
}
#endif

/**
	@}
*/

#endif          /* ifndef SDKMEM */
