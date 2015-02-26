
///////////////////////////////////////////////////////////////////////////
//
// Module   : MSR
// Part of  : SSW, EFTOS
// Created  : Jan Zimdahl
// Abstract : Header file of module MSR
//
//         Copyright (C) 2002 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 020712  BW      Original version (based on EFTOS7 version 990414)
// 030220  JB      Added bCardDirection to indicate direction of swiped OK card
//                 Removed "DIR" definition
//                 3 Track read capability for H2100 added.
//                 Driver now checks parameter table for present tracks 
//
///////////////////////////////////////////////////////////////////////////

#ifndef MSR_H
#define MSR_H

// Maximum number of tracks to read
#define MAX_TRACKS  3

// Definition of tracks
#define TRACK1      0
#define TRACK2      1
#define TRACK3      2   

// Definition of MSR characters (including parity bit)
#define TRACK1_SS   0x45
#define TRACK2_SS   0x0B
#define TRACK3_SS   0x0B

#define TRACK1_ES   0x1F
#define TRACK2_ES   0x1F
#define TRACK3_ES   0x1F

// Structure used by array holding data specific for each track
typedef struct {
    BOOL    bEnable;    // TRUE to enable reading the track
    LPBYTE  pAD;        // Address (pointer) to application data buffer
    DWORD   nCnt;       // Buffer size (input). Updated to number of read characters.
    DWORD   nRetStat;   // Return status
} MSR_TRACK_DATA;

// Structure holding data for all MSR tracks
typedef struct {
    BOOL volatile bEvt;  // Event flag to indicate when reading is ready
    BOOL bIncludeAll;    // TRUE if SS,ES,LRC will be included in the returned
                         // track data to application buffer
    BOOL bCardDirection;  //Tells which way the card was swiped
                             
    MSR_TRACK_DATA Track[MAX_TRACKS];
} MSR_DATA;


// Array holding data specific for each slave
extern MSR_DATA MSR_Data;

// MSR functions
void MSR_Disable(void);
BOOL MSR_Enable(void);
void MSR_Open(void);

#endif
