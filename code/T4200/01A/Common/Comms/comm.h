//
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
#ifndef _COMM_H_
#define _COMM_H_

//=============================================================================
// Public function declarations
//=============================================================================

void INITCOMM(void);

extern void SendMessage( void );
extern void RecvMessage( void );
extern void RequestMessage( void );
extern void InitiateDial( void );
extern void CancelDial( void );
extern void CancelComms(void);
extern void Hang_Up_Pin( void );
extern void Hang_Up( void );
extern void Hang_Up_Soon( void );
extern UBYTE GetLineStatus( );
extern UBYTE GetSignalLevel( );
extern void SendTrace(int RS_Port);


extern UDWORD Captured_IPaddr;

#endif // _COMM_H_
