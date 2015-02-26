//=============================================================================
//                       Hypercom Inc
//              Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================

#if !defined(_HYP_COMPILER_SPECIFICS_)
#define _HYP_COMPILER_SPECIFICS_

//
//  Define compiler specific behavior to be used by HPOSFC
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned long DWORD;

#define HYP_ASSERT(a) (assert(a))
#define HYP_TRACE(a) TRACE(a)

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif // !FALSE

#endif	// #define _HYP_COMPILER_SPECIFICS_
