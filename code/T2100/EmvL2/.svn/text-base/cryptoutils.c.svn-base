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
#include <SswDef.h>
#include <cryptolib.h>

/*****************************************************
* Function:     MISC_fetch
* Description:  Fetsch bytes and convert it to a 32 bits value.
* Arguments:    String pointer and length.
* Returns:      A 32 bits value.
******************************************************/
static DWORD Fetch(const void* pAddr, DWORD nLen)
{
    const BYTE* p = (const BYTE*)pAddr;
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
BYTE RSA(BYTE* exponent, BYTE length, BYTE* data, BYTE* result, BYTE* modulus, BYTE* worksp)
{
    DWORD exp = Fetch(&exponent[1], (WORD)exponent[0]);
    C_RsaModExp((BYTE*)data, (DWORD)length, (DWORD)exp, (BYTE*)modulus, (DWORD)length, (BYTE*)result);
    return(0);
}

/*****************************************************
* Function:     SHA
* Description:  Calculate SHA1 digist.
* Arguments:    Data and data length.
* Returns:      String pointer to digist string.
******************************************************/
BYTE* SHA(BYTE* input, WORD len)
{
    static BYTE sha_data[20];
    C_Sha1(input, len, (BYTE*)&sha_data);
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
    char result_encr_str[8];
    C_DesSingleEncryptECB((BYTE*)deskey, (BYTE*)desdata, 8, (BYTE*)result_encr_str);
    memcpy(desdata, result_encr_str, 8);
}

