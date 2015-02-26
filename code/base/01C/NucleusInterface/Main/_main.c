//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hyperware Base Application
//      (c) Copyright 2006
//
// File Name:
//      _main.c
//
// File Contents:
//  This is the starting module for the Base Application
//  As usual, execution starts at main() which in turn calls Base_App
//  The main application objects are instantiated and start execution.
//
//=============================================================================
// System includes
#include "stdio.h"

// HSDK includes
#include "osclib.h"
#include "hw.h"
#include "hfs.h"
#include "sdk.h"
#include "osgui.h"
#include "fixdata.h"
#include "sysstruc.inc"

// Application includes
#include "AppStart.h"
#include "defines.h"
#include "DbgTrace.h"

//=============================================================================
// External references
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

//! Application name
char _APPNAME[8]= "BaseApp";
//! Pointer to application name (Needed for HwOptimum.lib)
const char *AppName4MSRArbitrator= _APPNAME;

//=============================================================================
// Public data definitions
//=============================================================================

//! Stack size
const unsigned int _STACKSIZE= APPLICATION_STACK_SIZE;
//! Dynamic memory size
const unsigned int _DYNAMIC_MEMORY_SIZE = DYNAMIC_MEMORY_SIZE;
//! Application stack (needed for AppMan)
char ApplicationStack[APPLICATION_STACK_SIZE];
//! Dynamic memory pool (needed for AppMan)
char DynamicMemoryPool[DYNAMIC_MEMORY_SIZE];

//! This info is populated in _sbrk
//! Heap size
unsigned int HeapSize;
//! Max heap in using
unsigned int HeapInUseMax;
//! Logging library support
DBGTRACE_DECLARE_LOGGER
const char* TYPE_SPOS32 = _APPNAME;
//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

//=============================================================================
// Public function definitions
//=============================================================================

/*
void alloc_break( void )
{
    ;
}
*/

//-----------------------------------------------------------------------------
//!
//!      An application entry point
//!
//! \param
//!      argc        number of passed arguments
//! \param
//!		argv        argument's list
//!
//! \return
//!      Return value
//!
//! \note
//!
int main( int argc,char** argv )
{
    (void)argc;
    (void)argv;

    DBGTRACE_INIT_FROM_FILE ( _APPNAME );
    //SetBreakAtAlloc( 110, alloc_break );

#if !defined MAKE_M2
	// Setting mode of the screen output
	GUI_SetScreenMode( SCREEN_OUTPUT_OR );
#endif
    // Start any hardware specific
    HW_Init();

    // Initialize application tasks
    InitApplication();

	// create the flash queue for fast file writing
	StartFlashQueue();

    // now safe to start application
    StartApplication();

    // Sleep forever or until 'Exit' menu item is pressed (debug build only!)
    do
    {
        SDK_RelinqCPU();
        FlashWriteTask();
    }
    while( !AppCompleted );
    KillApplication();
	EndFlashQueue();
    HW_End();

    DBGTRACE_CLOSE();
}

//-----------------------------------------------------------------------------
//!
//!      Decreases the heap size
//!
//! \param
//!      incr		how much to decrease the heap size
//!
//!  Global Inputs:
//!		heap_end
//!
//! \return
//!      heap end
//!
//! \note
//!		This function belongs to newlib. It is here only as a temporary solution
//!		while newlib itself is modified.
//!
caddr_t _sbrk( int incr )
{
    extern char   end asm ("end");	// Defined by the linker.
    extern char   heapend asm ("heapend");
    static char  *heap_end = 0;
    char         *prev_heap_end;

    if (heap_end == NU_NULL)
    {
        heap_end = & end;
        HeapSize = &heapend - &end;
        HeapInUseMax = 0;
    }

    prev_heap_end = heap_end;
    if (heap_end + incr > &heapend)
	{
        // Some of the libstdc++-v3 tests rely upon detecting
        //out of memory errors, so do not abort here.
        return (caddr_t) -1;
	}
    heap_end += incr;
  // Saves highest heap usage
  if( HeapInUseMax < ( HeapSize - ( &heapend - heap_end ) ) )
  {
	  HeapInUseMax = HeapSize - ( &heapend - heap_end );
  }
    return (caddr_t) prev_heap_end;
}

