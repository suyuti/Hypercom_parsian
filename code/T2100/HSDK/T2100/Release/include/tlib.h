/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef TLIB_H_
#define TLIB_H_

#include "jrndata.h"
#ifdef __cplusplus
extern "C" {
#endif

//!------------------------------------------------------------------
//!
//! \file  tlib.h
//!
//!	GroupVTL TLIB Public Interface
//! 
//!	This library is the collection of routines that was originally
//!	implemented in assembler language for Z80 platform. On historical
//!	reasons they still joined in the library. Most of the library
//!	functionality is related to manipulation with journal.
//!
//!	The journal contains records of all of the transactions that must
//!	be saved during terminal operations. Most transactions, such as sales,
//!	refunds, voids, etc., require transactions to be saved in the journal.
//!	The journal entries can be scanned for specific criteria and reports
//!	displayed or printed. The journal transactions are the basis for
//!	settlement amounts and the individual records may be uploaded to the
//!	settlement host if the host requests the records.
//!
//!	The physicaly journal is located on the HFS in SRAM. The size of the
//!	journal is described in the source of the C program in the Jrn_Areas
//!	table. Typically the journal uses two pages of the terminal memory,
//!	about 64KB. More or less approximately 32KB pages of memory could be
//!	used, depending on individual requirements. Whenever the journal is
//!	first created, or recreated after an error, the journal areas are
//!	formatted. Formatting does destroy all records currently in the
//!	journal. The journal is typically allocated in pages starting at
//!	location 0x0080 and ending at location 0x7FFF. Since the memory in the
//!	Hypercom terminals is battery backed up the journal is unaffected by
//!	powering off the terminal.
//!
//!	The journal consists of two linked lists, a free list and a used list.
//!	When the journal is initially formatted all of the memory is on the
//!	free list. As journal entries are added to the journal the memory is
//!	allocated from the free list and the correct size record is added to
//!	the used list. The linked list entries on both the free list and the
//!	used list are linked forward and backwards. To validate the journal
//!	the links of the lists are verified by traversing forward to the last
//!	entry and then traversing backward to the first entry. If there are
//!	any errors in these traversals then the journal is considered corrupted
//!	and it is then unconditionally formatted, all available memory is
//!	placed on the free list.
//!
//!	Each journal entry consists of an uncompressed section that is not
//!	compressed as entries are put in the journal and an area that is
//!	compressed as it is put into the journal. The uncompressed section
//!	contains data that is used when searching the journal based on
//!	criteria. The compressed section contains data that is not used in
//!	criteria searches. The journal entry is based on the definitions
//!	in the trandata_rec structure. There are three sections of the
//!	trandata_rec for journal purposes. The start of the structure to
//!	TRFIXEDEND is reserved for fields that will go into the journal
//!	uncompressed. Fields in this area will be matched with search criteria
//!	when selecting journal records. From just after TRFIXEDEND to TRJRNEND
//!	will be included in the compression as the journal record is created.
//!	Items within this area are not used for criteria searches but are items
//!	that will be stored in the journal. The fields after TRJRNEND in
//!	trandata_rec will not be stored in the journal.
//!
//!	Each entry in the journal has a unique identifier. This identifier is
//!	used in calls to free an entry, read data for a particular entry, etc.
//!
//!	When a used journal record is freed the free area will be combined with
//!	any adjoining free blocks making a larger free block. If a journal
//!	record is updated and then is too large to fit in the same location,
//!	the old record is freed and a new record of the required larger size is
//!	allocated.
//!
//!	The journal should be manipulated by one process at a time. In order
//!	to insure that two processes do not attempt to update the journal at
//!	the same time, a lock and release process is provided. In most cases
//!	it is up to the caller to acquire a lock on the journal before calling
//!	the journal functions.
//!
//!	The journal C functions prototypes are compatible with z80 cross call
//!	method and can not have parameters or return values. Globals located
//!	in the common area of memory are therefore used to pass needed values
//!	to the functions and to receive completion information.
//!------------------------------------------------------------------

//!
//!	\brief Validate terminal ID checksum.
//!
//!	\param pTermId Points to 9-byte terminal ID.
//!
//!	\return BYTE    TRUE: valid terminal ID checksum.<br>
//!	                FALSE: invalid terminal ID checksum.
//!
extern BYTE ChkTermChk( char *pTermId );

//!
//!	\brief Calculate AMOUNT + REAUTH percentage.
//!
//!	\param pResult Points to 6-byte BCD result amount.
//!	\param pAmount Points to 6-byte BCD auth amount.
//!	\param Percent 1-byte BCD reauth percentage.
//! \return none
extern void CReauth( UBYTE *pResult, UBYTE *pAmount, UBYTE Percent );

//!
//!	\brief Percentage with no digits past decimal point.
//!
//!	\param pAnswer Points to 6-byte area to store the answer
//!	\param pNumber Points to 6-byte BCD number
//!	\param pDivisor Points to 6-byte BCD divisor
//!
//!	\note Number is assumed to be no more than 10 digits. Divisor is
//!	      assumed to be non-zero (a zero answer will be returned if
//!	      divisor is zero). Divisor is assumed to be no more than 6
//!	      digits. Answer is assumed to be no more than 6 digits.
//!
extern void Percent0( UBYTE *pAnswer, UBYTE *pNumber, UBYTE *pDivisor );

//!
//!	\brief Get a random number.
//!
//!	\return UBYTE   Random number.
//!
extern UBYTE GetRandom( void );

//!
//!	\brief Defines file name for the journal.
//!
//!	\param filename File name to use for the journal.
//!
//!	\note If function is not called from the application, default name
//!	      "journal.jrn" will be used.
//!
extern void ForceJournalFileName( const char *filename );

//!
//!	\brief Add an entry to the journal.
//!
//!	\param TotalBytes Size of journal entry requested.
//!
//!	\return UWORD   Zero if journal entry not added successfully. Otherwise return
//!	                is the ident of the new entry.
//!
extern UWORD JournalAllocateW( int TotalBytes );

//!
//!	\brief Add an entry to the journal with a specific journal entry ID.
//!
//!	\param TotalBytes Size of journal entry requested.
//!	\param Ident Ident of the journal record to be allocated.
//!	\return UWORD   Zero if journal entry not added successfully. Otherwise return
//!	        is the ident of the new entry.
//!
//!	\note This function was created to exactly copy records from a Master
//!	      terminal to a Mirror when updating a new Mirror in the MRLAN
//!	      environment.
//!
extern UWORD JournalAllocateIdentW( int TotalBytes, int Ident );

//!
//!	\brief Get the first record in the journal.
//!
//!	If it exists then read the fixed portion into JRBUF.
//!
//!	\return UWORD   Zero if no record found. Otherwise return is the ident of the
//!	                record found.
//!
//!	\note If a journal entry is found then THIS contains the current
//!	      fixed portion of the record.
//!
//!	\see struct jrndata_def
//!
extern UWORD JournalFirstW( void );
extern UWORD JournalFirstR(void);

//!
//!	\brief Unconditionally format the journal.
//!
//!	\note All entires in the journal will be cleared.
//!
extern void JournalFormatW( void );

//!
//!	\brief Free an entry in the journal.
//!
//!	\param Ident Word that specifies ID of journal record.
//!	\return UBYTE   0: journal entry successfully freed and freed space returned
//!	                to free list.<br>
//!	                1: journal entry not successfully freed.
//!
extern UBYTE JournalFreeW( UWORD Ident );
 
//!
//!	\brief Get the journal semaphore.
//!
//!	\return UBYTE   TRUE: if lock is acquired.<br>
//!	                FALSE: if lock is not acquired.
//!
extern UBYTE JournalLockWrapper( void );

//!
//!	\brief Get the next record in the journal.
//!
//!	If it exists then read the fixed portion into JRBUF.
//!
//!	\param Ident Record identifier where search is to be started.
//!	\return UWORD   Zero if no record found. Otherwise is ident of the record
//!	                found.
//!
//!	\see struct jrndata_def
//!
extern UWORD JournalNextW( UWORD Ident );
extern UWORD JournalNextR(UWORD Ident);

//!
//!	\brief Compress transaction.
//!
//!	Compress and move the data from a trandata_rec structure to JHBUF for
//!	writing to the journal.
//!
//!	\param pTranBuff Pointer to trandata_rec structure to be packed.
//!	\return int Length of the record after it was packed.
//!
//!	\see struct jrndata_def
//!
extern int JournalPackW( UBYTE *pTranBuff );

//!
//!	\brief Write fixed portion of journal entry into journal.
//!
extern void JournalPutFixedW( void );

//!
//!	\brief Get the previous record in the journal.
//!
//!	If it exists then read the fixed portion into JRBUF.
//!
//!	\param Ident Record identifier where search is to be started.
//!
//!	\return UWORD   Zero if no record found. Otherwise is ident of the record
//!	                found.
//!
//!	\see struct jrndata_def
//!
extern UWORD JournalPreviousW( UWORD Ident );
extern UWORD JournalPreviousR(UWORD Ident);

//!
//!	\brief Read an entry from the journal.
//!
//!	\param Ident Specifies ID of journal record.
//!	\param UnpackAddr trandata_rec structure to receive data.
//!	\param WholeRecordFlag Byte that specifies read of whole record (1) or
//!	                       fixed header only (0).
//!	\return UBYTE   TRUE: journal record successfully read.<br>
//!	                FALSE: journal entry not read successfully.
//!
extern UBYTE JournalReadW( UWORD Ident, UBYTE *UnpackAddr, UBYTE WholeRecordFlag );

//!
//!	\brief Release the journal semaphore.
//!
//!	\return UBYTE   TRUE: lock released.<br>
//!	                FALSE: release by a process that did not have the lock or the
//!	                journal was not locked at all.
//!
extern UBYTE JournalReleaseWrapper( void );

//!
//!	\brief Count the journal room.
//!
//!	Count the number of maximum sized entries that could still fit in the
//!	journal.
//!
//!	\return int Count of the number of maximum sized entries that could still
//!	            fit in the journal.
//!
extern int JrnRoomCountW( void );

//!
//!	\brief Setup variables from C functions for use by library code.
//!
//!	\param transaction_size Size of trandata_rec.
//!	\param jrn_fixed_size Size of the journal uncompressed header.
//!	\param jrn_compressed_size Size of the compressed area.
//!	\param max_jrn_size Size of the entire journal area uncompressed +
//!	                    compressed + 2 byte compressed size + 32 bytes for
//!	                    expansion.
//!	\param *pJnrAreaAddr Pointer to the table defining pages used by the
//!	                     journal.
//!	\param pJrrecAddr Pointer to the address of JRREC.
//!	\param *pLLbufAddr Pointer to the address of the linked portion of
//!	                   JRBUF.
//!	\param pJhbufAddr Pointer to the address of the fixed portion of JRBUF.
//!	\param *pJrnCompressedAddr Pointer to the address of the compressed
//!	                           portion of JRBUF.
//!	\param pThisAddr Pointer to the address of THIS - used in the selection
//!	                 process.
//!	\param *pJrnRoomCount Pointer to the count of remaining maximum sized
//!	                      records in journal.
//!
//!	\note This function must be called before any other function calls that
//!	      use the journal.
//!
extern void JrnSetupW( UWORD transaction_size, UWORD jrn_fixed_size,
                       UWORD jrn_compressed_size, UWORD max_jrn_size,
                       const struct jrnarea_rec *pJnrAreaAddr,
                       UBYTE * pJrrecAddr, struct ll_def *pLLbufAddr,
                       UBYTE * pJhbufAddr, UBYTE * pJrnCompressedAddr,
                       UBYTE * pThisAddr, int *pJrnRoomCount);

//!
//!	\brief Update an entry in the journal.
//!
//!	\param UpIdent Specifies ID of journal record.
//!	\param TotalBytes Number of bytes in record to be updated.
//!	\return UWORD   Zero if unsuccessful update. Otherwise the new ident.
//!
//!	\note JHBUF must be set with packed record to be updated before this
//!	      call is made.
//!
//!	\see struct jrndata_def
//!
extern UWORD JournalUpdateW( UWORD UpIdent, UWORD TotalBytes );

//!
//!	\brief Verify journal integrity.
//!
//!	Verifies that the journal is formatted and that the journal linked list
//!	is intact.
//!
//!	\return UBYTE   0: journal exists and is intact.<br>
//!	                1: journal did not exist or there was an error in the linked
//!	                list but journal has now been formatted.
//!
//!	\note Caller can display a transaction memory error message if the
//!	      return is non-zero.
//!
extern UBYTE JournalValidateW( void );

//!
//! \deprecated Z80 code compatibility layer. Not recommended
//!   for new design.
//!
//!	\brief Build the version mame.
//!
//!	\note The following external variables need to be defined by the
//!	      application prior calling this function:
//!
//!\code
//!extern UBYTE VERSION[];			// Version name, 13 bytes (destination)
//!extern const UBYTE VER_RELEASE[];	// Software name, 7 bytes (source)
//!extern const UBYTE VER_VERSION[];	// Software version, 3 bytes (source)
//!\endcode
//!
extern void VersionFormat( void );

#ifdef MAKE_OPTIMUM

//!*****************************************************************************
//!  \brief     Generate PIN block - DUKPT
//!  \param		*pan        points to account number
//!  \param     *pinblock   points to entered pin on call and is also
//!					        the location for storage of pin block on return
//!					        pinlength - length of pin entered by user
//!  \param     *pinbuf     points to location for storage of ASCII
//!					        key serial number (KSN)
//!  \retval    UBYTE	TRUE (0xFF) - encryption successful
//!					    FALSE (0x00) - encryption unsuccessful
//!					
//!  \note          To use this function must first load the terminal
//!					with DUKPT keys.
//!
//!*****************************************************************************
extern UBYTE DK_EncryptW( UBYTE *pan, UBYTE *pinblock,
			UBYTE pinlength, UBYTE *pinbuf);

//!*****************************************************************************
//!  \brief    This function initializes the Z8 internal key storage area
//!					so it can be used for internal DES key storage.  Initially
//!					(after the very first power up of the unit) the memory is
//!					allocated for RSA key storage to be backward compatible with
//!					existing application code using earlier firmware versions.
//!					Before Z8 version 1.22 or later can be used for DES key
//!					internal storage this command must be executed.
//!  \param     none
//!  \retval    UBYTE   TRUE (0xFF) - Initialize for DES key storage successful
//!					    FALSE (0x00) -Initialize for DES key storage unsuccessful
//!					
//!  \note          Passwords are set to the factory default, all zeroes.
//!					This function is only for use on Z8 release 1.22 or later.
//!					Master keys and passwords are stored internally within the Z8.
//!
//!*****************************************************************************
extern UBYTE MS_InitDESZ8W( void );

//!*****************************************************************************
//!  \brief    Internal pinpad master session MAC calculation using
//!					master keys stored externally to the Z8 in the key
//!					encrypting key (KEK) table.
//!  \param     masterkeyid binary master session id index
//!  \param		*macblock   points to location to return the MAC block
//!  \param		*mackey     points to the MAC key
//!  \param		*datablock  points to start of data to be MAC'ed
//!  \param		datapage    page where data to be MAC'ed resides
//!  \param		datalength  length of data to be MAC'ed  (2 bytes)
//!  \retval    UBYTE   TRUE (0xFF) - MAC calculation successful
//!					    FALSE (0x00) - MAC calculation unsuccessful
//!					
//!  \note          The terminal must be injected with a master session key
//!					that is stored in the KEK external to the Z8 and the key
//!					tag must be enabled for MAC.
//!					The mac returned is a 4 byte, 8 digit value
//!					This function is for use on Z8 release 1.17 or later.
//!
//!*****************************************************************************
extern UBYTE MS_CalcMACW( UBYTE masterkeyid, UBYTE *macblock,
							UBYTE *mackey, UBYTE *datablock,
							UBYTE datapage, UWORD datalength );

//!*****************************************************************************
//!  \brief    Internal pinpad master session MAC calculation using
//!					master keys stored internally within the Z8.
//!  \param     masterkeyid binary master session id index
//!  \param		*macblock   points to location to return the MAC block
//!  \param		*mackey     points to the MAC key
//!  \param		*datablock  points to start of data to be MAC'ed
//!  \param		datapage    page where data to be MAC'ed resides
//!  \param		datalength  length of data to be MAC'ed  (2 bytes)
//!  \retval    UBYTE   TRUE (0xFF) - MAC calculation successful
//!					    FALSE (0x00) - MAC calculation unsuccessful
//!					
//!  \note          Terminal must have been injected with master session key.
//!					If the pointer to the MAC key(*mackey) passed is NULL,
//!					then the function uses the master key(indicated by masterkeyid)
//!					to perform MAC calculation. This can be used to calculate
//!					the KVC for the master key when a block of 0s is mac'ed.
//!					The mac returned is a 4 byte, 8 digit value
//!					This function is only for use on Z8 release 1.22 or later.
//!					Master keys are stored internally within the Z8.
//!
//!*****************************************************************************
extern UBYTE MS_CalcMACZ8W( UBYTE masterkeyid, UBYTE *macblock,
							UBYTE *mackey, UBYTE *datablock,
							UBYTE datapage, UWORD datalength );
 
//!*****************************************************************************
//!  \brief    Generate PIN block - Master Session	- master key
//!					loaded internally in the Z8.
//!  \param     masterkeyid master key id.
//!  \param		*pan        pointer to primary account number.
//!  \param		*workingkey pointer to working key
//!  \param		*pinblock   pointer to pinblock.  Contains entered PIN
//!			    		    when call made.  After call contains the pinblock.
//!  \param		pinlength   length of the user entered pin.
//!  \retval	UBYTE   TRUE (0xFF) - encryption successful
//!					    FALSE (0x00) - encryption unsuccessful
//!  \note          To use this function must first load the Z8 internally
//!					with keys.  Key tags are not used with this function. 
//!
//*****************************************************************************
extern UBYTE MS_EncryptNoKEKW(UBYTE masterkeyid, UBYTE *pan,
			UBYTE *workingkey, UBYTE *pinblock, UBYTE pinlength);

//!*****************************************************************************
//!  \brief    Generate PIN block - Master Session
//!  \param     masterkeyid binary master session id index
//!  \param		*pan        pointer to primary account number.
//!  \param		*workingkey pointer to working key
//!  \param		*pinblock   pointer to pinblock.  Contains entered PIN
//!					        when call made.  After call contains the pinblock.
//!  \param		pinlength   length of the user entered pin.
//!  \retval    UBYTE	TRUE (0xFF) - encryption successful
//!					    FALSE (0x00) - encryption unsuccessful
//!  \note          To use this function must first load the terminal
//!					with keys and with tag(s) set for pin encryption. 
//!					The keys are stored encrypted external to the Z8.
//!
//!*****************************************************************************
extern UBYTE MS_EncryptW(UBYTE masterkeyid, UBYTE *pan,
			UBYTE  *workingkey, UBYTE *pinblock, UBYTE pinlength);

//*****************************************************************************
//!  \brief    This function loads a master session single length DES
//!					key that is stored internally within the Z8.
//!  \param     masterkeyid binary master session id index
//!  \param		*password   points to a 16 digit (ASCII) password
//!  \param		*masterkey  points to the single length master key
//!  \retval    UBYTE   TRUE (0xFF) - Master Key load successful
//!					    FALSE (0x00) - Master Key load unsuccessful (Failed password)
//!					
//!  \note          The factory default password is sixteen (16) zero's
//!					("0000000000000000").
//!					This function is only for use on Z8 release 1.22 or later.
//!					Master keys and passwords are stored internally within the Z8.
//!
//*****************************************************************************
extern UBYTE MS_LoadMKZ8W(  UBYTE masterkeyid, UBYTE *password, UBYTE *masterkey );


//!*****************************************************************************
//!  \brief    This function updates the Master Session password for a
//!					 master session index.  A Master Session password has 16
//!					(ASCII data) digits. The factory default password is
//!					sixteen(16) zero's ("0000000000000000").
//!  \param     masterkeyid binary master session id index
//!  \param		*oldpwd     points to the old password(16 digits)
//!  \param		*newpwd     points to the new password(16 digits)
//!  \retval    UBYTE   TRUE (0xFF) - Password update successful
//!					    FALSE (0x00) - Old password incorrect, update unsuccessful
//!					
//!  \note          The application may pass in the same pointer for *oldpwd
//!					and *newpwd, as a result the password should not be changed.
//!					This function is only for use on Z8 release 1.22 or later.
//!					Master keys and passwords are stored internally within the Z8.
//!
//*****************************************************************************
extern UBYTE MS_UpdatePasswordZ8W( UBYTE masterkeyid, UBYTE *oldpwd, UBYTE  *newpwd );

//!*****************************************************************************
//!  \brief    Percentage with no digits past decimal point
//!  \param[in]     pNumber     points to 6-byte BCD number
//!  \param[in]		pDivisor    points to 6-byte BCD divisor
//!  \param[out]    pAnswer     points to 6-byte area to store the answer
//!					
//!  \note          Number is assumed to be no more than 10 digits
//!					Divisor is assumed to be non-zero (a zero answer will
//!					  be returned if divisor is zero)
//!					Divisor is assumed to be no more than 6 digits
//!					Answer is assumed to be no more than 6 digits
//!
//*****************************************************************************
extern void Percent0(UBYTE * pAnswer, UBYTE * pNumber, UBYTE * pDivisor);

//!*****************************************************************************
//!  \brief    Return a string with the library version/revision
//!  \param    none
//!  \retval   char*	Static string with the library version/revision
//!  \note          Static string returned is null terminated.
//!
//*****************************************************************************
extern char *TLIB_Version( void );

//*****************************************************************************
//!  \brief    Validates main key encrypting table and shadow table
//!  \param     none
//!  \retval    UBYTE    TRUE (0xFF) - Key encrypting tables are valid
//!					    FALSE (0x00) - Key encrypting tables are invalid
//!
//*****************************************************************************
extern UBYTE ValidateCypherDataW(void);

//*****************************************************************************
//!  \brief    Interogate the Z8 for it's version number
//!  \param    *version     Address of location to store version information.
//!  \retval    UBYTE   TRUE (0xFF) if successful.
//!					    FALSE (0x00) is unsuccessful.
//!					
//!
//*****************************************************************************
extern UBYTE VersionZ8W(UBYTE * version);



// THE FOLLOWING TLIB FUNCTIONS REQUIRE SPECIAL ICE HARDWARE TO IMPLEMENT.
// THEY ARE ONLY HERE FOR COMPLETENESS OF THIS HEADER FILE.
//
//*SPECIAL*********************************************************************
//!  \brief    Starts the read on the Z8 that scans the keypad for input.
//!					The Z8 reads the pin directly from the keypad.
//!  \param     masterkeyid binary master session id index
//!  \param		*pan        pointer to primary account number.
//!  \param		*workingkey pointer to working key
//!  \param		*pinblock   pointer to pinblock.  Contains entered PIN
//!					 when call made.  After call contains the pinblock.
//!  \retval    UBYTE	TRUE (0xFF) - successful start of Z8 read of pin from keyboard
//!					    FALSE (0x00) - error starting Z8 read of pin from keyboard
//!
//!  \note			The Z8 must have been internally loaded with the master session
//!					key.  When the Z8 controls the keypad to get the pin, the Z80
//!					must first close the keypad.  Once the Z8 is started successfully,
//!					MS_ReadPinOnZ8W is used to read the status as each digit of the
//!					pin is entered.  
//!					This function is only for use on Z8 release 1.22 or later.
//!					with keys.  Key tags are not used with this function.
//!					This function works in conjunction with MS_ReadPinOnZ8W
//!					and MS_ResetReadZ8W.
//!*****************************************************************************
extern UBYTE MS_StartReadZ8W( UBYTE masterkeyid, UBYTE *pan,
			UBYTE *workingkey, UBYTE *pinblock );

//*SPECIAL*********************************************************************
//!  \brief    This function reads the status from the Z8 as each digit
//!					is entered from	the terminal keypad that is controlled by
//!					the Z8.  
//!  \param     none
//!
//!  \retval    UBYTE	0x00 -	Pin operation successful, the user pressed ENTER.
//!						 	The Z8 is stopped.  The caller must count keypresses
//!					   		to see if ENTER only or less than the required digits
//!					   		for the pin were entered.
//!					    0x06 -	Digit 0 to 9 pressed.  The Z8 is still running.
//!					    0x07 -	Clear key pressed for user cancel.  The Z8 is stopped.
//!					    0x08 -	Keypress is not the ENTER key after max pin digits of
//!							14 from the Z8.  The Z8 is stopped.
//!					    0xFE -	No keypress from the Z8 to report.  The Z8 is still
//!							running.  The calling program should delay (relinquish
//!							CPU), then call MS_ReadPinOnZ8W again.
//!					    0xFF -	ERROR occurred.  Z8 status is uncertain so reset it with
//!							MS_ResetReadZ8W.
//!
//!  \note			The Z8 must first be started using MS_StartZ8W.
//!					Caller must count digits entered to determine if ENTER only
//!					was pressed or if less than 4 digits or more than 12 digits.
//!					The Z8 will accept up to 14  digits so caller must code for
//!					error at 13th digit. 
//!					MS_ ReadPinOnZ8W will return immediately with a status.  If the
//!					Z8 has no keypress to report then it will return 0xFE status.
//!					This function is only for use on Z8 release 1.22 or later.
//!					Master keys are stored internally within the Z8.
//!					This function works in conjunction with MS_StartReadZ8W
//!					and MS_ResetReadZ8W.
//!					
//!*****************************************************************************
extern UBYTE MS_ReadPinOnZ8W( void );

//*SPECIAL*********************************************************************
//!  \brief    Resets the Z8 that scans the keypad for input.
//!					MS_ResetReadZ8W is required after using the Z8 to
//!					accept the pin directly from the keypad.
//!  \param     none
//!  \retval	none
//!  \note			When the Z8 controls the keypad to get the pin the
//!					Z80 must first close the keypad.
//!					This reset function rewinds and closes the Z8 so
//!					the Z80 can again get control of the keypad.
//!					This function is only for use on Z8 release 1.22
//!					or later.  Master keys are stored internally within
//!					the Z8.
//!					This function works in conjunction with MS_StartReadZ8W
//!					and MS_ReadPinOnZ8W.
//!
//!*****************************************************************************
extern void MS_ResetReadZ8W( void );

#endif 

#ifdef MAKE_VIKING

//!
//!	\deprecated Z80 code compatibility layer. Not recommended for new design.
//!
//!	\brief Cross call compatible function
//!
#define X(Function) Function

//!
//!	\deprecated Z80 code compatibility layer. Not recommended for new design.
//!
//!	\brief Cross call compatible function declaration for enums
//!
//!	\param Function Function to declare.
//!
#define X_DEF(Function) extern void Function(void);

//!
//!	\deprecated Z80 code compatibility layer. Not recommended for new design.
//!
//!	\brief Cross call compatible function declaration for enums with value
//!
//!	\param Function Function to declare.
//!	\param value Enum value (ignored).
//!
#define X_DEFVAL(Function,value) extern void Function(void);

#endif

#if defined( MAKE_VIKING ) || defined( MAKE_OPTIMUM )
//!
//!	\deprecated Z80 code compatibility layer. Not recommended for new design.
//!
//!	\brief Do cross call.
//!
//!	\param Function Page number and function number.
//!
#define APP_PerformCrossCall(Function) (Function)()

//!
//!	\deprecated Z80 code compatibility layer. Not recommended for new design.
//!
//!	\brief Do cross call by reference.
//!
//!	\param Function Page number and function number.
//!
#define APP_PerformCrossCall2(Function) (Function)()
#endif


#ifdef __cplusplus
}
#endif


#endif /* End of #ifndef TLIB_H_ */
