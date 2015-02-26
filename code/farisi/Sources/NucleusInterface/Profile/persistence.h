//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_
//=============================================================================
//
// Module overview:
//
//      This header file is contains functions that are use for 
//      saving persistent data areas
//
//=============================================================================

//=============================================================================
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================

//! should be called prior to app startup
void InitPersistentData( void );

//! called to store a table or tables(s) to file
void StoreTable( UBYTE * pTab );

//! called to get rid of corresponding persistence file
void PurgeTable( UBYTE * pTab );

#endif // _PESISTENCE_H_

