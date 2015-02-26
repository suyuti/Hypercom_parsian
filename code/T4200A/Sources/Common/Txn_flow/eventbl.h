
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _EVENTBL_H_
#define _EVENTBL_H_
//=============================================================================
//
// Module overview:             eventbl.h
//      Event table data
//
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================
extern const struct event_entry EVENT_TABLE[];


//=============================================================================
// Public function declarations
//=============================================================================
//! Returns the number of entries defined in EVENT_TABLE.
extern UWORD EventTableSize( void );
//! Verify the number of entried in EVENT_TABLE against the enumerations in TRANSDEF.h.
extern void VerifyEventTableSize( void );


#endif // _EVENTBL_H_
