
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _ICEPAK_H_
#define _ICEPAK_H_
//=============================================================================
//
// Module overview:             icepak.h
//      Definitions and data for ice pak
//
//
//=============================================================================

#include "basictyp.h"
#include "sdkio.h"

#include "msg.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
enum
{
	IMAGE_IDLE1,
	IMAGE_IDLE2,
	IMAGE_IDLE3,
	IMAGE_IDLE4,
	IMAGE_INTERVAL,
	IMAGE_EOT,
	IMAGE_RCPT_HEADER,
	IMAGE_RCPT_FOOTER,
	IMAGE_SAVER,
	NUM_OF_IP_IMAGES
};

struct icepak_images
{
	int id;
	char *pFileName;			// 6 + null
};


//=============================================================================
// Public data declarations
//=============================================================================
extern struct icepak_images const IcePak[NUM_OF_IP_IMAGES];


//=============================================================================
// Public function declarations
//=============================================================================
extern void IcePak_Init( void );
extern void IcePak_RemoveAll( void );
extern void IcePak_NextIdleScreen( void );
extern void IcePak_SaverScreen( void );

extern void ShowIcePak(  enum msg_id Mid, enum msg_id Tid, int image_index );

#endif // _ICEPAK_H_
