
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview: AppStart.c
//
//      This module contains functions for starting the application tasks 
//
//=============================================================================

// System includes
#include "stdlib.h"

// HSDK includes
#include "eftos.h"
#include "appstart.h"
#include "ptcs.h"
#include "iocs.h"
#include "osgui.h"
#include "hw.h"
#include "osclib.h"
#include "fixdata.h"
#include "tlib.h"

// Application includes
#include "ProcessTable.h"

// This module external declaration
#include "AppStart.h"

//=============================================================================
// Private defines and typedefs
//=============================================================================
#define NumAppTasks (sizeof(processTable)/sizeof(HDTPROCESS))

//=============================================================================
// Private function declarations
//=============================================================================

//=============================================================================
// Public data definitions
//=============================================================================
PCBDATA	pcb[NumAppTasks];

//=============================================================================
// Private data definitions
//=============================================================================

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  InitApplication
//
//      Initializes applciaiton items and creates main application processes
//
//  Parameters:
//		None
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void InitApplication( void )
{
    // Initialize semaphores	
    HW_SemInitialize();
	
    // Initialize timers
    InitTimers();

    // Initialize IOSC controller
    InitDevices();
	
    // iniatialize fixed application data as required
    SIM_FixdataInit();

    // Create App threads
    CreateApplicationTasks( processTable );
}


//-----------------------------------------------------------------------------
//  StartApplication
//
//      Starts the application processes (tasks )
//
//  Parameters:
//		None
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void StartApplication( void )
{
    // Need to initialize GUI library, otheriwse get exception errors
    GUI_Init();

    // Run the app
    ResumeApplicationTasks( processTable );
}

//-----------------------------------------------------------------------------
//  KillApplication
//
//      Exit application
//
//  Parameters:
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void KillApplication( void )
{
    TerminateApplicationTasks( processTable );
}

//-----------------------------------------------------------------------------
//  CreateApplicationTasks
//
//      This function creates the applicaiton tasks
//
//  Parameters:
//      argc        number of arguments ( not used )
//      argv        pointer to arguments ( not used )
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void CreateApplicationTasks( HDTPROCESS *entryPointTable )
{
    int nProcIndex;

    // Start a task for each application process
    for ( nProcIndex = 0; nProcIndex < NumAppTasks; nProcIndex++ )
    {
        // Save the process name
        pcb[nProcIndex].Name[0] = entryPointTable[nProcIndex].Name[0];
        pcb[nProcIndex].Name[1] = entryPointTable[nProcIndex].Name[1];
        pcb[nProcIndex].Name[2] = entryPointTable[nProcIndex].Name[2];
        pcb[nProcIndex].Name[3] = entryPointTable[nProcIndex].Name[3];
        pcb[nProcIndex].Name[4] = '\0';

        // Create task
        pcb[nProcIndex].NuAppTask = HW_CreateTask( entryPointTable[nProcIndex].pProcess, entryPointTable[nProcIndex].StackSize );
    }
}

//-----------------------------------------------------------------------------
//  ResumeApplicationTasks
//
//      This function resumes (runs) the application tasks
//
//  Parameters:
//      argc        number of arguments ( not used )
//      argv        pointer to arguments ( not used )
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void ResumeApplicationTasks( HDTPROCESS *entryPointTable )
{
    int nProcIndex;

    // Start application tasks 
    for( nProcIndex = 0; nProcIndex < NumAppTasks; nProcIndex++ )
    {
        HW_ResumeTask( pcb[nProcIndex].NuAppTask, NULL );
    }
}


//-----------------------------------------------------------------------------
//  TermninateApplicationTasks
//
//      This function suspends and terminates all the application tasks
//
//  Parameters:
//      argc        number of arguments ( not used )
//      argv        pointer to arguments ( not used )
//
//  Returns:
//      Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
void TerminateApplicationTasks( HDTPROCESS *entryPointTable )
{
    int nProcIndex;

    // Start application tasks 
    for( nProcIndex = 0; nProcIndex < NumAppTasks; nProcIndex++ )
    {
        HW_TerminateTask( pcb[nProcIndex].NuAppTask );
    }
}


//=============================================================================
// Private function definitions
//=============================================================================
