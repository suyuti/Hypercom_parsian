/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _PINATTCK_H_
#define _PINATTCK_H_

//=============================================================================
// Module overview:
//	Public interface to PIN Attack Delay routines
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupVTL
	@{
*/

//-----------------------------------------------------------------------------
//  PinAttackDelay 		Setup the pin attack delay using 
//						PIN attack algorithm described in document 
//						Infogard-Design.DOC Version 1.00 (dated January 30, 2003)
//  Parameters:
//		pPan	- pointer to PAN to be checked. 
//		pPin	- pointer to encrypted PIN block to be checked. 
//		PhysPage - Physical page number 
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
/**
	@brief Setup the pin attack delay.

	A PIN attack delay is used to deter a brute force attack for finding a
	PIN for a given card number and PIN encryption key by sending the same
	PAN data to the PIN pad and trying every possible PIN combination,
	comparing the PIN pad output to the correct output. The delay algorithm
	is made up of three parts.<ol>
	<li> Every master / session PIN entry command requires at least 2.5
	     seconds to process. If a PIN is entered faster than 2.5 seconds,
	     the PIN pad delays sending the response until the 2.5 seconds has
	     expired.</li>
	<li> A circular PAN velocity queue has 40 entries. Each entry in the
	     queue keeps a "hit" count, primary account number signature, and
	     encrypted PIN block signature. For every master / session PIN
	     entry command, the queue is checked. If an entry in the queue has
	     a matching signature, and the PIN block signature differs -
	     indicating a different PIN was entered -- the corresponding hit
	     count is incremented. When the "hit" count reaches 5, the hit
	     count is decremented and a 50 second delay occurs before the PIN
	     block is sent back.</li>
	<li> When the same PIN is entered for the same PAN, an exception is made
	     to the delay algorithm. The PIN pad compares the signature the
	     current PIN block with the signature PIN block in the queue for
	     this PAN. If they match the following occurs.<ol>
	     <li>The count for same PIN block for this PAN is incremented.</li>
	     <li>If the same PIN block count equals 20, it is reset to 0 and
	         the PIN "hit" count is decremented by 1. (The purpose of this
	         logic is to erase the results of a mis-keyed PIN. After 20
	         correct PIN entries, the result of the mis-key is negated.)
	         </li></ol></ol>

	The following flow chart illustrates the PIN attack delay:
	@image html pinattack.png

	@param pPan Pointer to PAN to be checked.
	@param pPin Pointer to encrypted PIN block to be checked.
	@param PhysPage Physical page number (not used).
*/
extern void PinAttackDelay( UBYTE * pPan, UBYTE * pPin, UBYTE PhysPage );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif	// _PINATTCK_H_
