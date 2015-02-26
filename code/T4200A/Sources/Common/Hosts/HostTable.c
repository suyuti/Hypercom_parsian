//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             HostTable.c
//		Tables to simulate "hostdata" region.
//
//=============================================================================
#include "i_host.h"

#include "hyper.h"
#include "reqhyp.h"
#include "rsphyp.h"
#include "demo.h"
#include "visa1.h"
#include "reqvisa1.h"
#include "rspvisa1.h"
#include "visad.h"
#include "reqvisad.h"
#include "rspvisad.h"

//=============================================================================
// External variables / declarations
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
//
// AVAILABLE HOST TABLE
//=============================================================================

//! Host Table
HOSTENTRY const VHDT_HOST_TBL1[] = 
{
	{
		Hyper,						// Hypercom Host Function
		ReqHyper,					// Hypercom Host Request function
		RspHyper,					// Hypercom Host Response function
		"HYPERCOM  "				// Hypercom Host Name
	},

	{
		DemoHost,					// Demo Host Function
		ReqDemo,					// Demo Host Request Function
		RspDemo,					// Demo Host Response Function
		"DEMO      "				// Demo Host Name
	},

	{
		Visa1,						// Visa1 Host Function
		ReqVisa1,					// Visa1 Host Request Function
		RspVisa1,					// Visa1 Host Response Function
		"VISA1     "				// Visa1 Host Name
	 },

	{
		VisaD,						// VisaD Host Function
		ReqVisaD,					// VisaD Host Request Function
		RspVisaD,					// VisaD Host Response Function
		"VISAD     "				// VisaD Host Name
	},
};

//! Dummy value to indicate the END of the Host Table
const int HOST_MARKER1 = 0x22;


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

