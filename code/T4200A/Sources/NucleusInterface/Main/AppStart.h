//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hyperware Base Application
//      (c) Copyright 2006
//
// File Name:
//      AppStart.h
//
// File Contents:
//      This file defines the IORequest structure used in the HDS simulator IOCS system.
//
//=============================================================================

#ifndef _APPMAIN_H_
#define _APPMAIN_H_

#include "basictyp.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

//! Description of a process
typedef struct
{
    unsigned char Type;         // type of the process
    void (*pProcess)(void);     // Address of the process
    char Name[4];               // 4 character name
    unsigned int StackSize;     // stack size
    void (*pShutdown)(void);    // Address of the shutdown function
} HDTPROCESS;

//=============================================================================
// Public data declarations
//=============================================================================

//! Pointer to dynamic memory pool
extern NU_MEMORY_POOL *Dynamic_Memory;

extern BOOLEAN AppCompleted;
//=============================================================================
// Public function declarations
//=============================================================================

//! Creates the applicaiton tasks
extern void CreateApplicationTasks( HDTPROCESS *entryPointTable );
//! Resumes (runs) the application tasks
extern void ResumeApplicationTasks( HDTPROCESS *entryPointTable );
//! Suspends and terminates all the application tasks
extern void TerminateApplicationTasks( HDTPROCESS *entryPointTable );
//! Initializes applciaiton items and creates main application processes
extern void InitApplication( void );
//! Starts the application processes (tasks )
extern void StartApplication( void );
//! Exit application
extern void KillApplication( void );

#endif

