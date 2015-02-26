//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdttextinit.h
//
// File Description:
//      HDT text initialization functions
//
//=============================================================================

#ifndef _HDTTEXTINIT_H_
#define _HDTTEXTINIT_H_

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
//! Add HDT and pinpad English messages tables for DML support
extern void HdtTextEn_Init( void );
//! Add HDT and pinpad Germany messages tables for DML support
extern void HdtTextGr_Init( void );
//! Add HDT and pinpad English messages tables for DML support
extern void HdtTextSw_Init( void );
//! Add HDT and pinpad Spanish messages tables for DML support
extern void HdtTextEs_Init( void );
//! Add HDT and pinpad Portuguese messages tables for DML support
extern void HdtTextPt_Init( void );

#ifdef MAKE_FARSI
//! Add HDT and pinpad Farsi messages tables for DML support
extern void HdtTextFa_Init( void );
#endif

//! Initialize dynamic language tables, for general purpose, and PIN use
extern void HdtText_Init( void );


#endif // _HDTTEXTINIT_H_
