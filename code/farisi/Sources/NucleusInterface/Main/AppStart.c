//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hyperware Base Application
//      (c) Copyright 2006
//
// File Name:
//      AppStart.c
//
// File Contents:
//  This module contains functions for starting the application tasks 
//
//=============================================================================
// System includes
#include <stdlib.h>
#include <hycddl.h>

// HSDK includes
#include "appstart.h"
#include "ptcs.h"
#include "iocs.h"
#include "osgui.h"
#include "hw.h"
#include "osclib.h"
#include "tlib.h"
// Application includes
#include "ProcessTable.h"
#include "DbgTrace.h"

//=============================================================================
// Private defines and typedefs
//=============================================================================
//! Number of tasks
#define NumAppTasks (sizeof(processTable)/sizeof(processTable[0]))
BOOLEAN AppCompleted = False;
//=============================================================================
// Private function declarations
//=============================================================================
//! Starts thread tasks
static void RunTask(UNSIGNED argc, void *argv);

//=============================================================================
// Public data definitions
//=============================================================================
//! Process control blocks array
PCBDATA	pcb[NumAppTasks];

//! Is transport thread running?
extern Bool TRANSPORT_IS_RUNNING;

//=============================================================================
// Private data definitions
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================
//! Init persistence data
extern void InitPersistentData( void );

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Initializes applciaiton items and creates main application processes
//!
//! \param
//!
//! \return
//!      None
//!
//! \note
//!
void InitApplication( void )
{
    // Initialize timers
    InitTimers();

    // Initialize IOSC controller
    InitDevices();
	
    // Create App threads
    CreateApplicationTasks( processTable );
}

//-----------------------------------------------------------------------------
//!
//!      Starts the application processes (tasks )
//!
//! \param
//!
//! \return
//!      None
//!
//! \note
//!
void StartApplication( void )
{
    // Need to initialize GUI library, otheriwse get exception errors
    GUI_Init();

    // need to open COM port , since it is open by real terminal O/S
    OS_DeviceOpen( COM );

	// Each task must register as a File User user
	NU_Become_File_User();

	// Open the disk for access
	NU_Open_Disk("C:");

	// Set default drive for future access
	NU_Set_Default_Drive(2);

	// initialize any persistent data
	InitPersistentData();

    // Run the app
    ResumeApplicationTasks( processTable );
}

//-----------------------------------------------------------------------------
//!
//!      Exit application
//!
//! \param
//!
//! \return
//!      None
//!
//! \note
//!
void KillApplication( void )
{
    TerminateApplicationTasks( processTable );
}

//-----------------------------------------------------------------------------
//!
//!      Set Application Exit flag
//!
//! \param
//!
//! \return
//!      None
//!
//! \note
//!
void ExitApplication( void )
{   
    AppCompleted = True;
}

//-----------------------------------------------------------------------------
//!
//!      This function creates the applicaiton tasks
//!
//! \param
//!      entryPointTable        process table pointer
//!
//! \return
//!      None
//!
//! \note
//!
void CreateApplicationTasks( HDTPROCESS *entryPointTable )
{
    int     nProcIndex;
    PCBDATA	*pPCB;
    STATUS  nustatus;
    VOID    *pMemory;

    // Start a task for each application process
    for ( pPCB = pcb,nProcIndex = 0,nustatus = NU_SUCCESS;
          nProcIndex < (int)NumAppTasks && NU_SUCCESS == nustatus;
          nProcIndex++,pPCB++,entryPointTable++ )
    {
        // Save the process entry point.
        pPCB->entry_point = entryPointTable->pProcess;

        // Save the process name
        pPCB->Name[0] = entryPointTable->Name[0];
        pPCB->Name[1] = entryPointTable->Name[1];
        pPCB->Name[2] = entryPointTable->Name[2];
        pPCB->Name[3] = entryPointTable->Name[3];
        pPCB->Name[4] = '\0';

        // clear the process io pending flag. 
        pPCB->IOPendingFlag = 0x00; // false

        // Create task 
        // really need to free this memory - but do shortcut for now
        // also can get by with much smaller stacks!
        nustatus = NU_Allocate_Memory( Dynamic_Memory, &pMemory, 
                                        entryPointTable->StackSize, NU_NO_SUSPEND );

        // Create task
        if ( NU_SUCCESS == nustatus )
        {
            nustatus = Hyc_AllocControlBlock( CB_NU_TASK, (void **)&pPCB->NuAppTask );
            if ( NU_SUCCESS == nustatus )
            {
                //DBGTRACE( LOG_DEBUG, TYPE_SPOS32, "%s: Creating task \"%s\"\n", __FUNCTION__, pPCB->Name );
                nustatus = NU_Create_Task(	pPCB->NuAppTask, pPCB->Name,
                                            RunTask, nProcIndex, NU_NULL, 
                                            pMemory, entryPointTable->StackSize,
                                            DEFAULT_PRIORITY, DEFAULT_TIMESLICE,
                                            NU_PREEMPT, NU_NO_START );
                if ( NU_SUCCESS != nustatus )
                {
                    Hyc_FreeControlBlock( pPCB->NuAppTask );
                    NU_Deallocate_Memory( pMemory );
                }
            }
            else
            {
                NU_Deallocate_Memory( pMemory );
            }
        }
    }
}

//-----------------------------------------------------------------------------
//!
//!      This function resumes (runs) the application tasks
//!
//! \param
//!      entryPointTable        process table pointer
//!
//! \return
//!      None
//!
//! \note
//!
void ResumeApplicationTasks( HDTPROCESS *entryPointTable )
{
    PCBDATA	*pPCB;
    int nProcIndex;

    // Start application tasks 
    for( pPCB= pcb,nProcIndex = 0; nProcIndex < (int)NumAppTasks; nProcIndex++,pPCB++ )
    {
        NU_Resume_Task( pPCB->NuAppTask );
    }
}

//-----------------------------------------------------------------------------
//!
//!      This function suspends and terminates all the application tasks
//!
//! \param
//!      entryPointTable        process table pointer
//!
//! \return
//!      None
//!
//! \note
//!
void TerminateApplicationTasks( HDTPROCESS *entryPointTable )
{
    PCBDATA	*pPCB;
    int nProcIndex;
    STATUS  nustatus;

	TRANSPORT_IS_RUNNING = False;
	while( !TRANSPORT_IS_RUNNING )
	{
		SDK_RelinqCPU( );
	}

    CHAR task_name[8]; DATA_ELEMENT task_status; UNSIGNED scheduled_count;
    OPTION priority; OPTION preempt; UNSIGNED time_slice; VOID *pMemory;
    UNSIGNED stack_size; UNSIGNED minimum_stack;

    // Start application tasks 
    for( pPCB= pcb,nProcIndex = 0; nProcIndex < (int)NumAppTasks; nProcIndex++,pPCB++ )
    {
        // Call NU_Task_Information to get pointer to stack memory
		NU_Task_Information( 
			pPCB->NuAppTask, task_name, 
			&task_status,
			&scheduled_count, &priority, &preempt,
			&time_slice, &pMemory,
			&stack_size, &minimum_stack );

        //DBGTRACE( LOG_DEBUG, TYPE_SPOS32, "%s: Terminating task \"%s\"\n", __FUNCTION__, task_name );

        nustatus = NU_Terminate_Task( pPCB->NuAppTask );
        if( nustatus != NU_SUCCESS )
        {
            DBGTRACE( LOG_ERROR, TYPE_SPOS32, "%s: Terminate task error. Task %s\n", __FUNCTION__, task_name );
        }
        NU_Delete_Task( pPCB->NuAppTask );
        Hyc_FreeControlBlock( pPCB->NuAppTask );
        // free stack memory
        nustatus = NU_Deallocate_Memory( pMemory );
        if( nustatus != NU_SUCCESS )
        {
            DBGTRACE( LOG_ERROR, TYPE_SPOS32, "%s: Deallocate memory error. Task %s\n", __FUNCTION__, task_name );
        }
    }
    CloseDevices();
    HW_DisconnectCommPort();
    HW_CloseCommPort();
}

//-----------------------------------------------------------------------------
//!
//!      Returns one byte task id of currently running task (application process)
//!
//! \param
//!
//! \return
//!      unsigned char	One byte task id of currently running task , id = pcb index + 1
//!
//! \note
//!
unsigned char GetPid(void)
{
	unsigned char retval = 0;
	NU_TASK *pTask;
	int i;
	
	// get pointer to current task control block
	pTask = NU_Current_Task_Pointer();

	if(pTask)
	{
		// search for valid task
		for ( i = 0; i < (int)NumAppTasks; i++ )
		{
			if ( pTask == pcb[i].NuAppTask )
			{
				retval = i+1;	// zero is invalid id so add 1
				break;
			}
		}
	}
	
	return retval;
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Entry point for the application processes
//!
//! \param
//!      argc        number of arguments - used for pcb index
//! \param
//!		argv        pointer to arguments ( not used )
//!
//! \return
//!      None
//!
//! \note
//!
static void RunTask( UNSIGNED argc, void *argv )
{
	unsigned int pcbIndex = argc;

	(void)argv;     // This just suppresses the warning

	NU_Become_File_User();

	// Open the disk for access
	NU_Open_Disk("C:");

	// Set default drive for future access
	NU_Set_Default_Drive(2);

	( *(pcb[ pcbIndex ].entry_point) )();
}

