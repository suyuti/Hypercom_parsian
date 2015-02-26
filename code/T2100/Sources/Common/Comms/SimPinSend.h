
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             SimPinSend.h
//      Unlock SIM card (send PIN to the card)
//
//=============================================================================

#ifndef _MODEMCFG_H
#define _MODEMCFG_H


#ifdef __cplusplus
extern "C" {
#endif

// Public functions
int SimPinSend( void );
int Get_UnlockStatus ( void );
void Set_UnlockStatus ( int Status );

#ifdef __cplusplus
}
#endif

#endif // _MODEMCFG_H
