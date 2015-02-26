//=============================================================================
//
// \file
//		sprite.h
//	\brief
//		Sprite identifiers
//
//=============================================================================
#ifndef _SPRITE_H_
#define _SPRITE_H_


//=============================================================================
// Public defines and typedefs
//=============================================================================
//! Sprite Identifiers
typedef enum
{
	//! moving card image on idle screen
	SPRITE_SWIPE_CARD,
	//! alternating menu on idle screen
	SPRITE_ALTERNATING_MENU,	
	//! Flash current position by alpha-num entry
	SPRITE_FLASH,        
	//! number of sprites
    NUM_OF_SPRITES
}
SPRITE_ID;



//=============================================================================
// Public data declarations
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================
//! Draw enabled sprites
extern void DoSprites(void);
//! Set active the sprite flag for later adding
extern void AddSprite(SPRITE_ID spriteID);
//! Clear sprite queue
extern void DeleteAllSprites(void);
//! Perform any initialization required for sprites
extern void InitSprites(void);
//! X-Callable functions to get sprite semaphore
extern void GetSpriteSem(void);
//! X-Callable functions to release sprite semaphore
extern void ReleaseSpriteSem(void);

#endif	// _SPRITE_H_

