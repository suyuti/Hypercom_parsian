
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cvtamt.c
//      Convert BCD amount to ASCII
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"
#include "utilfunc.h"

#include "defines.h"
#include "struct.h"
#include "comdata.h"

#include "cvtamt.h"



//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
extern void FormAmt( int cnt, char *pDest, const char *pBuf, char *CrrCode);

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================


/*------------------------------------------------------------------
 *  Name:           CvtAmt (@TUM New variant, optimized)
 *  Description:    Convert BCD Amount to ASCII
 *  Parameters:     pAscAmt - pointer to ASCII Amount (13 bytes)
 *				    pBcdAmt - pointer to BCD Amount (6 bytes).
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void CvtAmt( char *pAscAmt, UBYTE *pBcdAmt )
{
        static char *ptr;        
	static char *ptrend;
	UBYTE  offset, i , j;

	ptr = pAscAmt;
        
	
	/* Blank first position */
	*ptr = ' ';

	/* Convert BCD to ASCII */
	BfAscii( ptr+1, pBcdAmt, 6 ); 
	offset = 13-TCONF.TCDPP;

    /* See if decimal point to place */
	if ( TCONF.TCDPP )
	{
		/* Yes; shift the Amount */
		memcpy( ptr, ptr+1, offset );
		/* Place the decimal point */
		*(ptr+offset) = '.';
	}

	/* Now we need to replace leading 0's with spaces for leading 0
	   plus decimal point */
	
	ptrend = ptr + offset - 1;	// offset-1 = 12-TCONF.TCDPP

	for ( ; ptr < ptrend; ptr++ )
	{
		/* See if character is not a space */
		if ( ' ' != *ptr )
		{
			/* Yes; see if a zero */
			if ( '0' == *ptr )
			{
				/* Yes; overwrite with a space */
				*ptr = ' ';
			}
			else
			{
				/* No; exit the loop */
				break;
			}
		}
	}
        
        ptr[TCONF.TCDPP + 1] = 0x00;

	/* Set Currency char(s) */
        for ( i = 0; ptr[i] != 0x00; ++i);

	for ( j=0,offset = S_TCLEADC; offset; offset-- ,j++)
	{
		if ( TCONF.TCLEADC[offset - 1] > ' ' )
		{
			//if ( (--ptr) >= pAscAmt )
                        if( 1 ) 
			{
                              ptr[i++] = TCONF.TCLEADC[j];
			}
			else
			{
				break;
			}
		}
	}
        ptr[i] = 0x00;
   /*     
        //ptr = temp;
        for ( i = 0 ; i < 3; ++i )
            for ( j = 0; j < 20; ++j ) 
                ptr[j] = ptr[j+1];
          
        memcpy(&ptr[4], TCONF.TCLEADC, 3);*/
} 

/*------------------------------------------------------------------
 *  Name:           CvtAmt (@TUM New variant, optimized)
 *  Description:    Convert BCD Amount to ASCII
 *  Parameters:     pAscAmt - pointer to ASCII Amount (13 bytes)
 *				    pBcdAmt - pointer to BCD Amount (6 bytes).
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/
// @ydeniz -- bc -- ekran görüntüsü -- batch -review için yazýldý . ekran düzenlemeleri yapýlacak DK DK
void CvtAmtYTL( char *pAscAmt, UBYTE *pBcdAmt )
{
	static char *ptr;
	static char *ptrend;
        static char *ptrtemp;
	UBYTE  offset;
        UBYTE i;

	ptr = pAscAmt;
	
	/* Blank first position */
	*ptr = ' ';

	/* Convert BCD to ASCII */
	BfAscii( ptr+1, pBcdAmt, S_TRBASE );

	offset = 13-TCONF.TCDPP;

        // See if decimal point to place 
	if ( TCONF.TCDPP )
	{
		/* Yes; shift the Amount */
		memcpy( ptr, ptr+1, offset );
		/* Place the decimal point */
		*(ptr+offset) = '.';
                ptrtemp = ptr+offset+TCONF.TCDPP;
	}else {
                ptrtemp = ptr+offset;
        }
		    
	ptrend = ptr + offset - 1;	// offset-1 = 12-TCONF.TCDPP

	for ( ; ptr < ptrend; ptr++ )
	{
		// See if character is not a space 
		if ( ' ' != *ptr )
		{			
			if ( '0' == *ptr )
			{                              
				*ptr = ' ';
			}
			else
			{				
				break;
			}
		}
	}
                      
	// Set Currency char(s) 
	for ( i= 0, offset = S_TCLEADC; offset; offset-- ,i++)
	{								
            *ptrtemp = TCONF.TCLEADC[i];
             ptrtemp++;
	}
} 

void CvtAmtPrint( char *pAscAmt, UBYTE *pBcdAmt )
{
    int cnt;
    int len;
    char pTemp[(S_TRBASE * 2) + 1];

    //CvtAmtPrint2(pAscAmt,pBcdAmt);

    memset( pTemp, 0x20, ((S_TRBASE * 2) + 1) );

    // Convert BCD to ASCII 
    BfAscii( pTemp, pBcdAmt, S_TRBASE );

    for ( cnt = 0 ; pTemp[cnt] == '0' ; ++cnt )
        pTemp[cnt] = ' ';

    // Kac basamak var ( Para birimi hariç )
    len = (S_TRBASE * 2)  - cnt;  // Rakamlarý adedini ekle

    //FormAmt( (S_TRBASE * 2) - cnt , pTemp + cnt, pAscAmt, TCONF.TCLEADC, totalLen );
    FormAmt( len , pAscAmt, pTemp + cnt,  TCONF.TCLEADC );

    // Kaç basamak var ( Para birimi dahil )
    len = strlen( pAscAmt );

    // 12 basamak olacak þekilde saga ötele
    memmove( pAscAmt + ((S_TRBASE * 2) + 1) - len, pAscAmt , len );

    // soldakileri ' ' ile doldur
    memset( pAscAmt, ' ', ((S_TRBASE * 2) + 1) - len );
}

// slip için yazýlan 
void CvtAmtPrintXXX( char *pAscAmt, UBYTE *pBcdAmt )
{
    static char *ptr;
	static char *ptrend;
	UBYTE  offset,i, j;

	memset(pAscAmt, 0x20, 40);

	ptr = pAscAmt;
	
	/* Blank first position */
	*ptr = ' ';

	/* Convert BCD to ASCII */
	BfAscii( ptr+1, pBcdAmt, S_TRBASE );

	offset = 13-TCONF.TCDPP;

    /* See if decimal point to place */
	if ( TCONF.TCDPP )
	{
		/* Yes; shift the Amount */
		memcpy( ptr, ptr+1, offset );
		/* Place the decimal point */
		*(ptr+offset) = '.';
	}

	/* Now we need to replace leading 0's with spaces for leading 0
	   plus decimal point */
	
	ptrend = ptr + offset - 1;	// offset-1 = 12-TCONF.TCDPP

	for ( ; ptr < ptrend; ptr++ )
	{
		/* See if character is not a space */
		if ( ' ' != *ptr )
		{
			/* Yes; see if a zero */
			if ( '0' == *ptr )
			{
				/* Yes; overwrite with a space */
				*ptr = ' ';
			}
			else
			{
				/* No; exit the loop */
				break;
			}
		}
	}
        
       for ( i = 0; ptr[i] != 0x20; ++i);

	for ( j=0,offset = S_TCLEADC; offset; offset-- ,j++)
	{
		if ( TCONF.TCLEADC[offset - 1] > ' ' )
		{
			//if ( (--ptr) >= pAscAmt )
                        if( 1 ) 
			{
                              ptr[i++] = TCONF.TCLEADC[j];
			}
			else
			{
				break;
			}
		}
	}
        
        for ( i = 0; i< 3; ++i ) 
          for ( j = 0; j < 12; j++ )  
             pAscAmt[j] = pAscAmt[j+1];
        
        for( i = 0 ; i < 3; ++i )
          pAscAmt[10+i] = TCONF.TCLEADC[i];

        /*ptr[i] = 0x20;

	//Set Currency char(s) 
	for ( offset = S_TCLEADC; offset; offset-- )
	{
		if ( TCONF.TCLEADC[offset - 1] > ' ' )
		{
			if ( (--ptr) >= pAscAmt )
			{
				*ptr = TCONF.TCLEADC[offset - 1];
			}
			else
			{
				break;
			}
		}
	}*/

}
