#ifdef MAKE_EMV
/* Include directives */
#include "basictyp.h"
#include "osclib.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "sdl.h"
#include "fixdata.h"

#include "defines.h"
#include "utllib.h"
#include "util.h"
#include "util2.h"
#include "emv_l2.h"
#include "emvdata.h"
#include "scard.h"
#include "string.h"
#include "sdktime.h"
#include "emv2_pk.h"
#include "emvtabs2.h"
#include "emvaptag.h"
#include "emvutils.h"
#include "autotest.h"

#define MAX_TAG 5
#define AUTO_LENGTH 30

#define CFG_NONE 0x00
#define CFG_RAND 0x01
#define CFG_TACS 0x02
#define CFG_DDOL 0x03
#define CFG_TDOL 0x04

//! Chip card communication buffer
UBYTE TxnData[AUTO_LENGTH];
//! APDU Response SWitches buffer
UBYTE SC_SW12[2];
//! First application flag
Bool  First;

// External functions
extern UBYTE DOL_TRANTYPE( UBYTE * destptr, UBYTE entry );

// Internal functions
void AddTagToBuffer(UWORD TagID, UBYTE TagLen, UBYTE *TagData, UBYTE *Buffer );


//-----------------------------------------------------------------------------
//! \brief
//!     Adds TLV data to buffer and increments overall length
//!
//! \param
//!		[in]TagID			Tag
//!		[in]TagLen			Tag length
//!		[in]TagData			Tag data
//!		[in/out]Buffer		Pointer to Length of TLV
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void AddTagToBuffer(UWORD TagID, UBYTE TagLen, UBYTE *TagData, UBYTE *Buffer )
{
    // Note First byte of buffer contains current data length
	UBYTE *WritePtr = Buffer + *Buffer + 1;

	// Write the tag ID
	if (TagID > 0xFF)  // Incoming TagID may be big or little endian
	{
		*(WritePtr++) = (UBYTE) (TagID >> 8); 	// First write the most significant byte
		(*Buffer)++;
	}
    *(WritePtr++) = (UBYTE) (TagID & 0x00FF);  //  Now write the least significant byte
	
	// Write the tag length
    *(WritePtr++) = TagLen;
	(*Buffer) += 2;

	// Finally write the tag data
	memcpy(WritePtr, TagData, TagLen);
	*Buffer += TagLen;
}


#endif

