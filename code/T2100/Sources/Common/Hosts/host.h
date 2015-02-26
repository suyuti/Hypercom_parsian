//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             host.h
//      Host control module
//
//=============================================================================
#include "i_host.h"

extern void INITHOST( void );
extern void HostList( void );

extern UBYTE CurrentAqid;

extern UBYTE TmpBuf[];
extern HOSTENTRY *Host;
extern UBYTE *pRecvData;
