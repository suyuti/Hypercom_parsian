#include "superVariables.h"

extern void SetSuperTabLRC( void )
{
	SUPERLRC = CalcLrc( ( UBYTE * ) & SUPER_VARIABLES_TAB, SUPERSZE );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&SUPER_VARIABLES_TAB );
#endif
}
