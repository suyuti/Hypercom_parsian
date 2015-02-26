
#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvaptag.h
//  EMV application processing
//
//=============================================================================

extern UBYTE WriteTermData( void );
extern void WriteSchemeData( void );
extern UBYTE WriteAIDData( UBYTE );
extern void WriteAmountData( void );
extern void WriteFloorInvData( void );
extern UBYTE UpdFieldsPostAppSel( void ); 
extern UBYTE UpdFieldsPostGenAC( void );
extern void UpdFinalCryptogram( void );
extern void UpdFinalResponseCode( void );

#endif // MAKE_EMV
