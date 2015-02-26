
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             _main.c
//      Initialize and start an application
//
//=============================================================================

// System includes
#include "stdio.h"

// HSDK includes
#include "eftos.h"
#include "osclib.h"
#include "hw.h"
#include "hfs.h"
#include "sdk.h"
#include "osgui.h"
#include "fixdata.h"
#include "sysstruc.inc"
#include <fal.h>
#include <hypffs.h>

// Application includes
#include "AppStart.h"
#include "CommModule.h"
#include "defines.h"
#include "SimPinSend.h"
#include "taginfo.h"
#include "hypffs.h"

// This module external declaration


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define FileSystemBase __FILESYSTEM_segment_start__

extern int FileSystemBase;

#define SRAM_START_ADDRESS		(*((unsigned int *)0x04000000))
#define SRAM_MAXIMUM_ADDRESS	(*((unsigned int *)0x04100000))
#define SRAM_BLOCK_SIZE			((unsigned int)0x00010000)
#define SRAM_TEST_DATA			((unsigned int)0x01010101)

unsigned int HFS_PHYSICAL_PAGE_SIZE;
unsigned int HFS_OS_RESV_SIZE;
int errno;
extern void CheckApplicationExpire ( void );

//=============================================================================
// Private function declarations
//=============================================================================

int main( void )
{
   static unsigned char file_pages[ HFS_MAX_PAGES ] = { FILE_SYSTEM_PAGES };

    // Initialize application tasks
    InitApplication();

    // Start any hardware specific
    HW_Start();

    // Initialize FLASH filesystem
    FFS_Init();

    if( FFS_Mount(FAL_ACCESS_NORMAL) < 0 )
    {
        // The only known reason why FFS cannot be mounted is badly
        // formatted FFS. This should normally not
        // be done by application because of Application Manager will
        // stop operate properly. 
        D_DispStr((const unsigned char *)"FFS not mounted\n");
        // Sleep forever
        for(;;){}
	}

    // Start communication module
    ResetCommModule ();

    // Start trace if need it
    Trace_Start_Mask();

    // Unlock GSM SIM card if required
    SimPinSend ();

    // Initialize filesystem
    HFS_OS_RESV_SIZE = (unsigned int) &FileSystemBase;

    // Prepare the file system for use
    finit( file_pages );

    // Set proper value for HFS_PHYSICAL_PAGE_SIZE (as set by Application Manager)
    ffixsize();

    // Verify the file system
    if( (0 == fverify()) || (-1L == fspaceavail()) )
    {
        D_Clear();
        D_Disp((unsigned char*)"FILE SYSTEM ERROR !!!",21);
        // Sleep forever
        for(;;){}
    }

    // Set Journal file name if we in multiapplication environment,
    // do nothing if we are not.
    ParseCommandFile( cfp_cmd_JournalName );
    
    // Get the number of application available into Number_Of_Applications
    ParseCommandFile( cfp_cmd_ApplicationList );
    
    // Check Application Expire
    //CheckApplicationExpire( );

    // now safe to start application
    StartApplication( );

    // Sleep forever
    for(;;)
    {
        EVT_STRUCT relinquish;
        BOOL nonreadyEvent = 0;

        relinquish.nNoEvt = 1;
        relinquish.EvtPtrArr[ 0 ] = &nonreadyEvent;

        O_Wait( &relinquish );
    }
}


