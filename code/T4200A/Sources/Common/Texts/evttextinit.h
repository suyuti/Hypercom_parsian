//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evttextinit.h
//
// File Description:
//      Event text initialization functions
//
//=============================================================================
#ifndef _EVTTEXTINIT_H_
#define _EVTTEXTINIT_H_


//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
//! Add English Event table for DML support
extern void EvtTextEn_Init( void );
//! Add German Event table for DML support
extern void EvtTextGr_Init( void );
//! Add Swedish Event table for DML support
extern void EvtTextSw_Init( void );
//! Add Spanish Event messages tables for DML support
extern void EvtTextEs_Init( void );
//! Add Portuguese Event messages tables for DML support
extern void EvtTextPt_Init( void );

#ifdef MAKE_FARSI
//! Add Farsi Event messages tables for DML support
extern void EvtTextFa_Init( void );
#endif

//! Initialize dynamic language tables for EvtText tables
extern void EvtText_Init( void );


#endif // _EVTTEXTINIT_H_
