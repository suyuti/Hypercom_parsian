/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2003                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  Hypercom Base Application in C                  *        */
/*  *                                                               *        */
/*  *  COMPONENT :  Key data                                        *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv_keydata.c                                   *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR        COMMENT                         *        */
/*  *  --------     ----------     ----------                       *        */
/*  *  11/11/03     A Shuttleworth  1st Draft                       *        */
/*  *                                                               *        */
/*  *****************************************************************        */

// This file contains buffer declarations which may require modification
// when compiling for non Z80 targets.

#include "emv2_pk.h"

#ifdef __GNUC__
#define __init__(A) A
#else
#define __init__(A)
#ifndef __attribute__
#define __attribute__(A)
#endif
#endif

// ---------- insert target specific pragmas here --------------------------
#ifdef _TARG_Z80
#pragma region("ram=pkdata")
#elif defined( __ARMCC_VERSION )
#pragma arm section zidata="pkdata"
#endif

// 1. Persistant buffers

//! This buffer contains the Certification Authority public keys 
//! (Populated by terminal configuration system).

//! Public Keys Table formatter
__attribute__((section ("pkdata")))
UBYTE PKFMT __init__(=0);
//! Public Keys Table
__attribute__((section ("pkdata")))
struct CAPublicKeyEntry PublicKeyStore[KEYMAX] __init__(={{0}});
//! Public Keys Table LRC
__attribute__((section ("pkdata")))
UBYTE PKLRC __init__(=0);

//! This buffer contains the expiry dates for the CA public keys
//! Populated by the terminal configuration system).

//! Key Expiry Table formatter
__attribute__((section ("pkdata")))
UBYTE KEFMT __init__(=0);
//! Key Expiry Table
__attribute__((section ("pkdata")))
struct KeyExpiryEntry KeyExpiryTable[KEYMAX] __init__(={{0}});
//! Key Expiry Table LRC
__attribute__((section ("pkdata")))
UBYTE KELRC __init__(=0);


//! This buffer holds a lookup table designed reduce the time required to
//! find a particular key. It is not supplied by the terminal configuration 
//! system but after a configuration download, is recalculated, and must
//! then persist.  
__attribute__((section ("pkdata")))
struct PublicKeyLookupEntry KeyLookupTable[KEYMAX] __init__(={{0}});


//! This buffer holds revocation data for Issuer Public key certificates
//! (Populated by the terminal configuration system).
//! Note: only has one entry !!!

//! Revocation Table formatter
__attribute__((section ("pkdata")))
UBYTE RKFMT __init__(=0);
//! Revocation Table
__attribute__((section ("pkdata")))
struct RevocationEntry RevocationTable __init__(={{0}});
//! Revocation Table LRC
__attribute__((section ("pkdata")))
UBYTE RKLRC __init__(=0);


// 2. Dynamic buffers
// none

#ifdef _TARG_Z80
#pragma region("ram=ram")
#endif
// ---------- insert target specific pragmas here --------------------------

