//=============================================================================
//                         Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
#ifndef _SPRITE_H_
#define _SPRITE_H_
//=============================================================================
//
// Module overview:             sprite.h
//      Sprite identifiers
//
//=============================================================================


//=============================================================================
// Public defines and typedefs
//=============================================================================
//.Sprite Identifiers

typedef enum
{
	SPRITE_SWIPE_CARD,			// moving card image on idle screen
	SPRITE_ALTERNATING_MENU,	// alternating menu on idle screen
	SPRITE_GSMMETER,            // GSM signal meter
	SPRITE_BATTARY,             // Battary charge indicator
	SPRITE_FLASH,               // Flash current position by alpha-num entry
    NUM_OF_SPRITES
}
SPRITE_ID;



//=============================================================================
// Public data declarations
//=============================================================================
extern Bool Battery_available;

//=============================================================================
// Public function declarations
//=============================================================================
extern void DoSprites(void);
extern void AddSprite(SPRITE_ID spriteID);
extern void DeleteAllSprites(void);
extern void InitSprites(void);
extern void GetSpriteSem(void);
extern void ReleaseSpriteSem(void);

#endif	// _SPRITE_H_

