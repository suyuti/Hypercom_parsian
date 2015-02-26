/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef UTLINC
#define UTLINC

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEPRECATED
#ifdef MAKE_VIKING
#define DEPRECATED __attribute__((deprecated))
#else
#define DEPRECATED
#endif // MAKE_VIKING
#endif // DEPRECATED	

/**
	@defgroup GroupUTL Utility Library Public Interface
	@{
*/

/**
	@brief Converts a one digit ASCII value, 0-9 or A-F, to a binary value
	       0-F.

	@param AsciiChar Contains ASCII value.

	@return	0: '0' or invalid ASCII character.<br>
	        Non-zero: Binary value.

	@see Same as Binary().
*/
extern UBYTE A2hex( char AsciiChar );

/**
	@brief Adds two 6-byte BCD numbers together.

	@param pResult Points to a 6 byte area where the sum is to be placed.
	@param pAmt1 First numbers to be added together.
	@param pAmt2 Second numbers to be added together.
	@return	0: Overflow did not occur.<br>
	        1: Overflow occurred.
*/
extern UBYTE AddAmt( UBYTE *pResult, const UBYTE *pAmt1, const UBYTE *pAmt2 );

/**
	@brief Adds two 1-byte BCD numbers together.

	@param pResult Points to 1 byte where the sum is to be placed.
	@param pAmt1 First numbers to be added together.
	@param pAmt2 Second numbers to be added together.
	@return	0: Overflow did not occur.<br>
	        1: Overflow occurred.
*/
extern UBYTE AddBcdByte( UBYTE *pResult, const UBYTE *pAmt1, const UBYTE *pAmt2 );

/**
	@brief Adds two 2-byte BCD numbers together.

	@param pResult Points to a 2 byte area where the sum is to be placed.
	@param pAmt1 First numbers to be added together.
	@param pAmt2 Second numbers to be added together.
	@return	0: Overflow did not occur.<br>
	        1: Overflow occurred.
*/
extern UBYTE AddCnt( UBYTE *pResult, const UBYTE* pAmt1, const UBYTE *pAmt2 );

/**
	@brief Returns TRUE.

	@return	TRUE
*/
extern UBYTE Always( void );

/**
	@brief Converts ASCII into BCD or binary.

	Converts ASCII format numbers (decimal or hexadecimal) into BCD or HEX.

	@param pBuffer The address of the destination field.
	@param pAscii The address of the ASCII source field.
	@param NumBytes Length of the destination field. The source field is
	                twice as long.
*/
extern void AscHex( UBYTE *pBuffer, const char *pAscii, UBYTE NumBytes );

/**
	@brief Converts one digit binary or BCD to ASCII.

	Converts a one digit binary or BCD value to a one byte ASCII character.

	@param Value Binary value.
	@return ASCII value.
*/
extern char Ascii( UBYTE Value );

/**
	@brief Returns FALSE.
	@return FALSE
*/
extern UBYTE Bad( void );

/**
	@brief Increments by one, a two-byte, 4-digit, BCD number. 
*   @param pBcd2 Points to a two-byte BCD number.
*                There is no validity check done on the original
*                BCD number.
*/
extern void Bcd2( UBYTE *pBcd2 );

/**
	@brief Increments by one, a three-byte, 6-digit, BCD number.

*   @param pBcd3 Points to a three-byte BCD number.
*                There is no validity check done on the original
*                BCD number.
*/
extern void Bcd3( UBYTE* pBcd3 );

/**
	@brief Increments by one, a N-byte, 2N-digit, BCD number.

*   @param pBcdNumber Points to the first byte of a
*                     n-byte BCD number. There is no validity
*                     check done on the original BCD number.
	@param NumBytes The number of bytes in the BCD number.
*/
extern void BcdN( UBYTE *pBcdNumber, UBYTE NumBytes );

/**
	@brief Converts multibyte (up to 32 bytes) BCD value into multibyte
	       binary value.

	@param pDest Points to the beginning of destination buffer.
	@param pSource Points to the beginning of source (BCD) value.
	@param Len Contains the length of BCD value (in bytes).
*/
void Bcd2Bin( UBYTE *pDest, const UBYTE *pSource, UBYTE Len );

/**
	@brief Converts a two-byte, 3-digit, BCD number into a binary value.

	@param pBcdBin Points to the two-byte BCD number. The most significant
	               digit of the 4-digit BCD number is ignored by the
	               routine. There is no validity check done on the original
	               BCD number.
	@return	Binary equivalent.
*/
extern UWORD BcdBin( UBYTE *pBcdBin );
	
/**
	@brief Converts several bytes of binary or BCD digits into ASCII
	       characters.

	@param pAscii Points to the address the ASCII characters are to be
	              placed.
	@param pBuffer Points to the address where the binary or BCD bytes are
	               located.
	@param NumBytes Contains the number of input bytes.

	@see Same as UnPack().
*/
void BfAscii( char *pAscii, const UBYTE *pBuffer, UBYTE NumBytes );

/**
	@brief Converts an ASCII character 0-9 or A-F to the binary equivalent. 

	@param AsciiChar Contains ASCII value.

	@return	0: '0' or invalid ASCII character.<br>
	        Non-zero: Binary value.

	@see Same as A2hex().
*/
extern UBYTE Binary( char AsciiChar );

/**
	@brief Converts one byte of binary or BCD digits into two ASCII
	       characters.

	@param pAscii Points to the address the ASCII characters are to be
                      placed.
	@param pBuffer Points to the address where the binary or BCD byte is
                       located.
*/
void BufAsc( char *pAscii, const UBYTE *pBuffer );

/**
	@brief Caluculates the checksum (LRC) value for specified block of
	       memory.

	@param pBuffer Points to start of the data.
	@param NumBytes Length of the data.
	@return	Calculated checksum (LRC) value.
*/
extern UBYTE CalcLrc( const UBYTE *pBuffer, UWORD NumBytes );

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

 	@brief Sets each byte of a block of memory to a selected value.

	@param pBuffer Pointer to the first byte of a block of memory.
	@param Pattern Pattern to store in each byte of the memory block.
	@param NumBytes Length of the memory block in bytes.

	@see Same as FillIt().
*/
extern void ChrIt( UBYTE *pBuffer, UBYTE Pattern, UWORD NumBytes ) DEPRECATED;

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

 	@brief Sets a block of memory to binary zeros.

	@param pBuffer  - pointer to the first byte of a block of memory.
	@param NumBytes - length of the memory block in bytes.
*/
extern void ClrIt( UBYTE *pBuffer, UWORD NumBytes ) DEPRECATED;

/**
	@deprecated It is recommended to use standard C function memcmp()
	            instead.

	@brief Compares two memory blocks.

	@param pBuffer1 Pointer to the first byte of a block of memory.
	@param pBuffer2 Pointer to the first byte of a block of memory.
	@param NumBytes Length of the memory blocks in bytes.
	@return	0: Buffer1 = Buffer2.<br>
		-1: Buffer1 @< Buffer2.<br>
		+1: Buffer1 @> Buffer2.
*/
extern BYTE CmpBuf( const UBYTE *pBuffer1, const UBYTE *pBuffer2, UWORD	NumBytes ) DEPRECATED;

/**
	@deprecated Simple mathematics will do the job.

 	@brief Compares two integer (2-byte) values.

	@param int1 2-byte value.
	@param int2 2-byte value.

	@return	0: int1 = int2.<br>
		-1: int1 @< int2.<br>
		+1: int1 @> int2.
*/
extern BYTE CmpWord( UWORD int1, UWORD int2 ) DEPRECATED;

/**
	@deprecated It is recommended to use standard C function memcmp()
	            instead.

 	@brief Compares two memory blocks
	@param pBuffer1 Pointer to the first byte of a block of memory.
	@param pBuffer2 Pointer to the first byte of a block of memory.
	@param NumBytes Length of the memory blocks in bytes.
	@return 0: memory blocks are equal.<br>
	        1: memory blocks are different.
*/
extern BYTE Compar( const UBYTE *pBuffer1, const UBYTE *pBuffer2, UWORD NumBytes ) DEPRECATED;

/**
	@brief Converts a two byte binary number into a two byte BCD number.

	@param Number Two byte binary number to be converted.
	@return	The converted number in BCD format.

	@note This function does not check for overflow.
*/
extern UWORD CvtBcd( UWORD Number );

/**
	@brief Converts a BCD number into a binary number.

	@param Number BCD number to be converted to binary.
	@return The converted binary number.

	@note There is no validity check for the BCD value.
*/
extern UBYTE CvtBin( UBYTE Number );

/**
	@brief Converts a 4-digit BCD number into a binary number.

	@param Number The 4-digit BCD number to be converted to binary.
	@return	The converted binary number.

	@note There is no validity check for the BCD value.
*/
extern UWORD CvtBinW( UWORD Number );

/**
	@brief Used to divide a 6-byte BCD number by another 6-byte BCD number
	       giving a 6-byte BCD quotient and a 6-byte BCD remainder.

	@param pQuotient Points to a 6-byte area where the quotient is to be
	                 placed.
	@param pRemainder Points to a 6-byte area where the remainder is to be
	                  placed.
	@param pDividend Points to a 6-byte area where the dividend is located.
	@param pDivisor Points to a 6-byte area where the dividend is located.
	@return	FALSE: If a divide by zero was attempted.<br>
	        TRUE: If division was correct.
*/
extern UBYTE DivAmt( UBYTE *pQuotient, UBYTE *pRemainder, const UBYTE *pDividend, const UBYTE *pDivisor );

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

	@brief Sets a block of memory to all ones.

	@param pBuffer Pointer to the first byte of a block of memory.
	@param NumBytes Length of the memory block in bytes.
*/
extern void FFIt( const	UBYTE *pBuffer, UWORD NumBytes ) DEPRECATED;

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

 	@brief Sets each byte of a block of memory to a selected value.

	@param pBuffer Pointer to the first byte of a block of memory.
	@param NumBytes Length of the memory block in bytes.
	@param Pattern Pattern to store in each byte of the memory block.

	@see Same as ChrIt().
*/
extern void FillIt( UBYTE *pBuffer, UWORD NumBytes, UBYTE Pattern ) DEPRECATED;

/**
	@brief Get the specified semaphore.

	If the semaphore is already in use, wait. This is not a real semaphore,
	as it cannot be used for process synchronization, just mutual exclusion
	(it only has two states: available and unavailable).

	@param pSemaphore Pointer to the semaphore to get.

	@see RelSem()
*/
extern void GetSem( UBYTE* pSemaphore );

/**
	@brief Returns TRUE.

	@return	TRUE
*/
extern UBYTE Good( void );

/**
	@brief Increments by one, a 1-byte, 2-digit, BCD number.

	@param BcdNumber - a 1-byte BCD number.
	@return Incremented BCD number.

	@note There is no validity check for the BCD value.
*/
extern UBYTE IncBcdByte( UBYTE BcdNumber );

/**
	@brief Increments by one, a N-byte, 2N-digit, BCD number.

	@param pBcdNumber Points to a N byte area where the BCD number is.
	@param NumBytes Number of bytes in the BCD number.
*/
extern void IncBcdLen( UBYTE *pBcdNumber, UBYTE NumBytes );

/**
	@brief Clears leading zeros ("0") to spaces (" ").

	@param pNumber Points to the start of the number.
	@param MaxZeros Maximum number of leading zeros to clear.
*/
extern void LeadSpace( char *pNumber, UBYTE MaxZeros );

/**
	@brief Multiplies two 6-byte BCD numbers together.

	@param pResult Points to a 6 byte area where the product is to be placed.
	@param pAmt1 First number to be multiplied together.
	@param pAmt2 Second number to be multiplied together.

	@return	FALSE: Overflow occurred.<br>
	        TRUE: Overflow did not occur.
*/
extern UBYTE MltAmt( UBYTE *pResult, const UBYTE* pAmt1, const UBYTE* pAmt2 );

/**
	@brief Convert a BCD amount to an ASCII string.

	Supresses leading zeros in the ASCII buffer unless leading zeros are
	needed to generate the number of digits specified in the MinDigits
	argument. A decimal point is placed in the ASCII buffer.

	@param pBuffer Points to the buffer to store amount string.
	@param pBcdAmt Points to the BCD amount.
	@param Length Length of the BCD amount in bytes.
	@param MinDigits Minimum digits (must be less than or equal to the
	                 number of BCD digits).
	@param DecPtPos Decimal point position. This defines the character
	                position of the decimal point from the right end of
	                the string. ie. 3 specifies --@> 7654.21
	@return	Length of the ASCII amount field including the decimal point.
*/
extern UWORD MovAmt( char *pBuffer, const UBYTE *pBcdAmt, UBYTE Length, UBYTE MinDigits, UBYTE DecPtPos );

/**
	@brief Move an ASCII string terminated by a NULL.

	Fill the remainder of the destination buffer with the fill character.

	@param pBuffer Points to destination buffer.
	@param pString NULL terminated string.
	@param Length Length of destination field.
	@param FillChar Fill character.

	@see MovAscZFillChr()
*/
extern void MovAscZFill( char *pBuffer, const char *pString, UBYTE Length, char FillChar );

/**
	@brief Move an ASCII string terminated by a specified character.

	Fill the remainder of the destination buffer with the fill character.

	@param pBuffer Points to destination buffer.
	@param pString Specified character terminated string (TermChar).
	@param Length Length of destination field.
	@param FillChar Fill character.
	@param TermChar Terminating character.

	@see MovAscZFill()
*/
extern void MovAscZFillChr( char *pBuffer, const char *pString, UBYTE Length, char FillChar, char TermChar );

/**
	@deprecated It is recommended to use standard C function memcpy()
	            instead.

 	@brief Perform a move from source to destination for number of bytes
	       specified.

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param Bytes Number of bytes to move.
*/
extern void MoveIt( void *pDest, const void *pSource, UWORD Bytes ) DEPRECATED;

/**
	@brief Perform a move from source to destination replacing NULLs (0x00)
	       of the source with spaces (0x20) in the destination.

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param NumBytes Number of bytes to move.
*/
extern void MoveOrSpace( char *pDest, const char *pSource, UWORD NumBytes );

/**
	@brief Perform a move from source to destination until the source
	       equals a specified stop character.

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param MaxBytes Maximum number of bytes to move.
	@param StopChar The stop character.
	@return	The number of bytes moved.
*/
extern UWORD MoveTillChar( char *pDest, const char *pSource, UWORD MaxBytes, char StopChar );

/**
	@brief Perform a move from source to destination until the destination
	       contains a non-blank character.

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param MaxBytes Maximum number of bytes to move.
	@return	The number of bytes moved.
*/
extern UWORD MoveTillNonBlank( char *pDest, const char *pSource, UWORD MaxBytes );

/**
	@brief Perform a move from source to destination until the source
	       buffer contains a non-numeric character.

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param MaxBytes Maximum number of bytes to move.
	@return	The number of bytes moved.
*/
extern UWORD MoveTillNonNum( char *pDest, const char *pSource, UWORD MaxBytes );

/**
	@deprecated It is recommended to use standard C function strcpy()
	            instead.

 	@brief Perform a move from source to destination until the source
	       buffer contains a NULL (0x00).

	@param pDest Points to destination buffer.
	@param pSource Points to source buffer.
	@param MaxBytes Maximum number of bytes to move.
	@return	The number of bytes moved.
*/
extern UWORD MoveTillZero( char *pDest, const char *pSource, UWORD MaxBytes );

/**
	@brief Converts packed PAN to ASCII.

	PAN is terminated by 0xFF.

	@param pBuffer Pointer to buffer to store ASCII PAN.
	@param pPan  Points to PAN.
	@param MaxBytes Safety count of destination.
	@return	Remaining bytes in destination.
*/
extern UBYTE MovPanAsc( char *pBuffer, const UBYTE *pPan, UBYTE MaxBytes );

/**
	@brief Move an ASCII string and trim trailing spaces and NULLs.

	@param pBuffer Points to destination buffer.
	@param pString Points to source buffer.
	@param MaxBytes Maximum length of string.
	@return	0: Data is moved.<br>
	        1: No data is moved (the field is blank).
*/
extern UBYTE MovTrim( char *pBuffer, const char *pString, UWORD MaxBytes );

/**
	@brief Returns FALSE.

	@return	FALSE
*/
extern UBYTE Never( void );

/**
	@brief Increments by one, a two-byte, 4-digit, BCD number.

	Incrementing stops at 9999 (it will not roll over).

	@param pBcdNumber Points to the two-byte BCD number. There is no
	                  validity check done on the original BCD number.
*/
extern void Nms2( UBYTE* pBcdNumber );

/**
	@brief Increments by one, a three-byte, 6-digit, BCD number.

	Incrementing stops at 999999 (it will not roll over).

	@param pBcdNumber Points to the three-byte BCD number. There is no
	                  validity check done on the original BCD number.
*/
extern void Nms3( UBYTE* pBcdNumber );

/**
	@brief Checks if each byte in a buffer is NULL (0x00).

	@param pBuffer Points to the buffer.
	@param NumBytes The number of bytes to check.
	@return	FALSE: Not all NULLs.<br>
	        TRUE: All NULLs.
*/
extern UBYTE NullComp( const char *pBuffer, UWORD NumBytes );

/**
	@deprecated Simple mathematics will do the job.

	@brief Used determine the address of a particular table entry.

	@param pTable Points to the start of the table.
	@param EntryLength The entry length of the table (the table has fixed
	                   length entries).
	@param EntryNumber The entry number desired (1 = first entry).
	@return	Pointer to the desired table entry.
*/
extern UBYTE* Offset( const UBYTE *pTable, UWORD EntryLength, UBYTE EntryNumber ) DEPRECATED;

/**
	@brief Removes parity (clear bit 7 from each byte) from a data buffer.

	@param pDataBuffer Points to the data buffer that is to have the parity
	                   bits removed.                       
	@param DataLength Length of the data in the data buffer.
*/
extern void Parity( UBYTE *pDataBuffer, UWORD DataLength );      	                       

/**
	@brief Release the specified semaphore.

	@param pSemaphore Pointer to the semaphore to be released.

	@see GetSem()
*/
extern void RelSem( UBYTE* pSemaphore);

/**
	@brief Compress using RLE (Run-Length Encoding).

	@param pOutput Points to the address where compressed data will be put.
	@param pInput Points to the starting address of the data to be
	              compressed.
	@param NumBytes Contains the number of input bytes.
	@return	Size of compressed data.
*/
extern UWORD RleCompress( UBYTE *pOutput, const UBYTE *pInput, UWORD NumBytes );

/**
	@brief Decompress RLE (Run-Length Encoding).

	@param pOutput Points to the address where uncompressed data will be
	               put. 
	@param pInput Points to the starting address of the data to be
	              uncompressed.
	@param NumBytes Contains the number of input bytes.
	@return	Size of uncompressed data.	
*/
extern UWORD RleDeCompress( UBYTE *pOutput, const UBYTE *pInput, UWORD NumBytes );

/**
	@brief Checks if each byte in a buffer is a space (0x20).

	@param pBuffer Points to the buffer.
	@param NumBytes The number of bytes to check.
	@return	FALSE: Not all spaces.<br>
	        TRUE: All spaces.
*/
extern UBYTE SpaceComp( const char *pBuffer, UWORD NumBytes );

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

 	@brief Sets a block of memory to spaces.

	@param pBuffer Points to the block of memory to space fill.
	@param NumBytes Length of the block of memory.
*/
extern void SpcIt( char * pBuffer, UWORD  NumBytes ) DEPRECATED;

/**
 	@brief Gets length of an ASCII string terminated by a NULL (0x00).

	@param pString Pointer to string terminated by a NULL (0x00).
	@param MaxLength Safety count.
	@return	Length of the string.
*/
extern UBYTE StrLn( const char *pString, UBYTE MaxLength );

/**
	@brief Gets length of an ASCII string terminated by a specified
	       character.

	@param pString Pointer to string terminated by a character which is
	               specified (TermChar).
	@param TermChar Terminator character.
	@param MaxLength Safety count.
	@return	Length of the string.
*/
extern UBYTE StrLnChr( const char *pString, char TermChar, UBYTE MaxLength );

/**
	@brief Subtracts two 6 byte BCD amounts.

	@param pResult Points to a 6 byte area where the difference will be
	               put.
	@param pAmt1 Points to the six-byte data field that contains the number
	             to be subtracted from.
	@param pAmt2 Points to the six-byte data field that contains the number
	             to be subtracted. 
	@return	0: Overflow did not occur.<br>
	        1: Overflow occurred. 
*/
extern UBYTE SubAmt( UBYTE* pResult, const UBYTE* pAmt1, const UBYTE* pAmt2 );

/**
	@brief Subtracts two 1 byte BCD numbers, with borrow.

        This routine is called repetitively to subtract larger BCD fields. The
	borrow is part of the subtraction.

	@param pResult Points to where the difference will be put.
	@param pNum1 Points to the number to be subtracted from.
	@param pNum2 Points to the number to be subtracted out.
	@param PreviousBorrow 0: Borrow not set on previous subtraction.<br>
	                      1: Borrow was set on previous subtraction.
	@return	0: Borrow not needed for next subtraction.<br>
	        1: Borrow needed for next subtraction.
*/
extern UBYTE SubBcdByte( UBYTE *pResult, const UBYTE *pNum1, const UBYTE *pNum2, UBYTE PreviousBorrow );

/**
	@brief Converts several bytes of binary or BCD digits into ASCII
	       characters.

	@param pAscii Pointer to destination (ASCII buffer).
	@param pBuffer Pointer to source (BCD buffer).
	@param NumBytes BCD bytes to process.

	@see Same as BfAscii().
*/
extern void UnPack( char *pAscii, const UBYTE *pBuffer, UBYTE NumBytes );

/**
	@brief Unpacks an odd number of BCD digits into an ASCII buffer.

	The BCD buffer must be right justified (i.e. the first nibble is
	ignored).

	@param pAscii Pointer to destination (ASCII buffer).
	@param pBuffer Pointer to source (BCD buffer).
	@param NumBytes BCD bytes to process.
*/
extern void UnPackOdd( char *pAscii, const UBYTE *pBuffer, UBYTE NumBytes );

/**
	@deprecated It is recommended to use standard C function memset()
	            instead.

	@brief Sets a block of memory to ASCII zeros ('0').

	@param pBuffer Points to the block of memory to zero fill.
	@param NumBytes Length of the block of memory.
*/
extern void ZeroIt( char * pBuffer, UWORD NumBytes ) DEPRECATED;

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif          /* ifndef UTLINC */
