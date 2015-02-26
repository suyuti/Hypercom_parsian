#ifndef EMVUTILS_H_
#define EMVUTILS_H_

//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvutils.h
//  EMV application processing
//
//=============================================================================

/****************** Local Data Types ******************/
typedef struct
			{
    			UWORD Tag;      	// Tag
				UBYTE *pAddr;		// Pointer to destination address
				UBYTE Len;			// Length of data
			} updfieldstab_rec;		// Structure of UPDFIELDS_TAB Entry


extern UBYTE WriteAIDData( UBYTE );	// Write AID related data to the library
extern UBYTE UserSelection( CandTextType *pCandList, UBYTE ListLen );	// Prompt the user to select or confirm an application
extern UBYTE PerformCVMProcessing ( void ); // emvutils.c
extern UBYTE PerformAppSelection ( CandTextType** , UBYTE*);
extern void PrintICCDebug ( UBYTE emv_mode, CandTextType* PrintCand, UBYTE *PrevAmount );
extern Bool InitUserCard ( void );
extern void BuildVerBuff ( UBYTE *buffptr, struct taidrec_rec entry );
extern UBYTE GoOnline( struct iccrec_rec  *pIccEntry );
extern void PerformCardClose ( void );
extern void GetPreviousAmount( UBYTE *Amount );
extern Bool isCardInserted ( void );
extern Bool ForceICCInsertion ( void ); 
extern void EMV_Offline( void );
extern void TransactionVoid( void );

#endif    // end of "ifndef EMVUTILS_H_"

