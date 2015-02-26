
#ifndef _APPMAIN_H_
#define _APPMAIN_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview: AppStart.c
//
// this file defines the IORequest structure used in the HDS simulator IOCS system.
//
//=============================================================================

typedef struct
{
    void (*entry_point)();
    char Name[6];
    DWORD NuAppTask;	
} PCBDATA; 

typedef struct
{
    unsigned char Type;         // type of the process
    void (*pProcess)(void);     // Address of the process
    char Name[4];               // 4 character name
    unsigned int StackSize;     // stack size
} HDTPROCESS;

extern void CreateApplicationTasks( HDTPROCESS *entryPointTable );
extern void ResumeApplicationTasks( HDTPROCESS *entryPointTable );
extern void TerminateApplicationTasks( HDTPROCESS *entryPointTable );
extern void InitApplication( void );
extern void StartApplication( void );
extern void KillApplication( void );

#endif
