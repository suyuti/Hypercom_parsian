//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:     ProcessTable.h
//      The list of application's processes (tasks)
//
//=============================================================================

#include "disproc.h"
#include "host.h"
#include "hypertcf.h"
#include "comm.h"

// Proces entry points
static HDTPROCESS processTable[] = 
{
//  Type    Handler     Name                 Stack size
	{USER,	INITDISP,	{ 'D','I','S','P' }, 4*4096, DISP_ShutDown },
	{USER,	INITHOST,	{ 'H','O','S','T' }, 4*4096, NULL },
	{USER,	INITTCFW,	{ 'T','C','F','W' }, 4*8192, NULL },
	{USER,	INITCOMM,	{ 'C','O','M','M' }, 4*4096, COM_ShutDown },
};
