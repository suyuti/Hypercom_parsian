// $Workfile:   cryptoutils.h  $
// $Header:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum_x2100_Application/EmvL2/cryptoutils.h-arc   1.0   Aug 25 2005 18:23:02   aasiounk  $
// $Log:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum_x2100_Application/EmvL2/cryptoutils.h-arc  $
//
//   Rev 1.0   Aug 25 2005 18:23:02   aasiounk
//Initial revision.
/*                                                                           */
/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 1998                        *        */
/*  *                                                               *        */
/*  *****************************************************************        */
/*                                                                           */

extern UBYTE  RSA(UBYTE *exponent, UBYTE length, UBYTE *data, UBYTE *result, UBYTE *modulus, UBYTE *worksp);

extern UBYTE* SHA(UBYTE *input, UWORD length);





     
