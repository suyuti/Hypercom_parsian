/*****************************************************
*
* File:         cryptoutils.c
* Part of:      EMV
* Author:       Allan Ekman
* Description:  Crypto utilities.
*
* Revision history:
* 020318    AE  Original.
*
******************************************************/

#include <string.h>
#ifdef MAKE_OPTIMUM
#include "basictyp.h"
#else
#include <SswDef.h>
#endif
#include <defines.h>

/*****************************************************
* Function:     MISC_fetch
* Description:  Fetsch bytes and convert it to a 32 bits value.
* Arguments:    String pointer and length.
* Returns:      A 32 bits value.
******************************************************/
static DWORD Fetch(const void* pAddr, DWORD nLen)
{
    const UBYTE* p = (const UBYTE*)pAddr;
    DWORD u = 0;

    while (nLen--)
        u = (u << 8) + *p++;

    return(u);
}

/*****************************************************
* Function:     RSA
* Description:  RSA encrypt.
* Arguments:    Exponent (4 bytes where the first byte is the length byte).
*               Modulus length, (also data length). Data to encrypt, result 
*               buffer and modulus key.
* Returns:      True or false.
******************************************************/
UBYTE RSA(UBYTE* exponent, UBYTE length, UBYTE* data, UBYTE* result, UBYTE* modulus, UBYTE* worksp)
{
    DWORD exp = Fetch(&exponent[1], (WORD)exponent[0]);
    C_RsaModExp((UBYTE*)data, (DWORD)length, (DWORD)exp, (UBYTE*)modulus, (DWORD)length, (UBYTE*)result);
    return(0);
}

/*****************************************************
* Function:     SHA
* Description:  Calculate SHA1 digist.
* Arguments:    Data and data length.
* Returns:      String pointer to digist string.
******************************************************/
UBYTE* SHA(UBYTE* input, DWORD len)
{
    static UBYTE sha_data[20];
    C_Sha1(input, len, (UBYTE*)&sha_data);
    return(sha_data);
}
/*****************************************************
* Function:     EMVL2_DESENC
* Description:  Encrypt.
* Arguments:    Work area, key and data to encrypt.
* Returns:      None.
******************************************************/
void EMVL2_DESENC(void *wrkarea, void *deskey, void *desdata)
{
//    char result_encr_str[8];
//    C_DesSingleEncryptECB((UBYTE*)deskey, (UBYTE*)desdata, 8, (UBYTE*)result_encr_str);
//    memcpy(desdata, result_encr_str, 8);
}
