
#ifndef __taginfo_h
#define __taginfo_h
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             taginfo.h
//      Command file parser defintions
//
//=============================================================================


extern int ParseCommandFile( UBYTE );


// Define Command File name - must be the same as in Application Manager
// (this file used for swapping between Application and Application Manager)
#define COMMAND_FILE "shell.cmd"


// Define input arguments of command file parser
#define cfp_cmd_JournalName       1
#define cfp_cmd_ApplicationList   2


// Define output of command file parser
#define cfp_retval_Completed      0
#define cfp_retval_NoCommandFile  1
#define cfp_retval_FormatError    2
#define cfp_retval_TooLongData    3


// Define max application number
#define APPMAX 7


#endif // __taginfo_h
