// $header:$
/*                                                                           */
/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  EMV Level 2 Library in C                        *        */
/*  *                                                               *        */
/*  *  COMPONENT :  Additional Tag Store                            *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv_at.h                                        *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  31 JAN 02     Ian Smith     1st Draft                        *        */
/*  *                                                               *        */
/*  *****************************************************************        */
/*                                                                           */

#if !defined( __ARMCC_VERSION ) && !defined( __packed )
#define __packed
#endif

#if !defined( __GNUC__ ) && !defined( __attribute__ )
#define __attribute__(A)
#endif


//Data Source Constants
#define EMVL2_ICC       0x00  // Data originates from ICC
#define EMVL2_TERM      0x80  // Data originates from Terminal

#define  EMVL2_ADT_SIZE  2500  // size of additional tag store (in bytes)


typedef __packed struct _CustTagStoreType
{
    UBYTE Tag;
    UBYTE Control;
    UBYTE MinLength;
    UBYTE MaxLength;
    UBYTE Length;
    UBYTE *Data;
} __attribute__(( packed )) CustTagStoreType;



void  EMVAT_InitialiseTagStore(UBYTE RecID);
Bool  EMVAT_IsTagPresent(UWORD tag);
Bool  EMVAT_ReadTagData (UBYTE *data, UBYTE *length, UWORD tag);
UBYTE EMVAT_WriteTagData (UBYTE *data, UBYTE length, UWORD parent, UWORD tag);
UBYTE EMVAT_BuildSDATag(UWORD tag);
void  EMVAT_ReadFormattedTagData (UBYTE *data, UBYTE length, UWORD tag);
void  EMVAT_DeleteTagData (UWORD tag);
UWORD EMVAT_GetParentBitmap (UWORD tag);
