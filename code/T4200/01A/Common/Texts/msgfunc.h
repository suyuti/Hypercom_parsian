//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             msgfunc.h
//      Various message functions
//
//=============================================================================
//=============================================================================
//! Returns the number of entries defined in ResponseTab
extern void ResponseTabSize( void );
//! Retrieve a message and store it in MLBuffer
extern void ShowWarningMsg( enum msg_id Mid , enum msg_id Tit);
extern void GETMSG( enum msg_id MSGID );
