//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2006
//=============================================================================
#ifndef CRTTAB_H_
#define CRTTAB_H_
//=============================================================================
//
// Module overview:             crttab.h
//      This include file is used to declare all structures 
//      and functions used by crttab.c
//
//=============================================================================

//=============================================================================
// Defines
//=============================================================================
// X509 Certificate table definitions

#define KEY_DATA_MAX_LENGTH		0x0600	// private key binary data length
#define CERT_DATA_MAX_LENGTH	0x0600	// certificate binary data length
#define MAXCERTSIZE				4096	// Max size of Sertification File

#define CRT_FIXEDKEY			0x01	// use fixed key for decryption
#define S_CERTRESV				9		// number of reserved bytes

struct certificate_rec {
	UBYTE	CERTNUM;						// certificate number
	UBYTE	CERTID;							// certificate id
	UBYTE	PKMKID;							// index of master key for private key decryption
	UBYTE	CRTOPT1;							// option 1
	UBYTE	RESV[S_CERTRESV];				// spare
	UBYTE	CERT[CERT_DATA_MAX_LENGTH];		// certificate binary data
	UWORD	CERTLEN;						// certificate data length
	UBYTE	KEY[KEY_DATA_MAX_LENGTH];		// private key binary data
	UWORD	KEYLEN;							// private key data length
	UWORD	KEYMODULE;						// private key module length	
	UBYTE	KEYFORMAT;						// private key format
};
#define CRTSZE	sizeof(struct certificate_rec)


//=============================================================================
// External  declarations
//=============================================================================
extern void CrtTabPre( void );
extern void CrtTab_LDIR( void );
extern void CrtTab_Post( void );
extern struct certificate_rec * GetCertificate( UBYTE Id );


//=============================================================================
// External variables
//=============================================================================
extern struct certificate_rec CRTTAB;	// X509 Certificate table
extern UBYTE CRTLRC;				// Checksum (LRC)


//=============================================================================
// Defines
//=============================================================================
#define CERTIFICATE_FILE	"client.ck"	// Certificate Data File
#define TEMPCERT_FILE	"temp.ck"	// Temporary Certificate Data File

#ifndef BASIC_TYPE_DEFINED
typedef unsigned int	BOOL;
typedef unsigned long	DWORD;
#endif


#endif // end of "ifndef CRTTAB_H_" 
