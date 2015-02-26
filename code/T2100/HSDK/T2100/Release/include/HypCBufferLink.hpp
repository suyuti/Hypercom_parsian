//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCBUFFERLINK_HPP_
#define _HYPCBUFFERLINK_HPP_

#include "HypCBuffer.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Single linked list of buffers.
*/
class HypCBufferLink
{
public:
	/**
		@brief Constructor.

		@param thisone Pointer to the buffer to insert as head of list
		               (optional, defaults to NULL).
		@param next Pointer to the next buffer in the list (optional,
		            defaults to NULL).
	*/
	HypCBufferLink( HypCBuffer *thisone = NULL, HypCBufferLink *next = NULL ) : m_this( thisone ), m_next( next ) {}

	/**
		@brief Destructor.

		Deletes all elements of the linked list starting from this.
	*/
	~HypCBufferLink();

	/**
		@brief Get buffer associated to this element.

		@return Reference to the pointer to buffer associated to this
		        element or NULL if no buffer is associated.
	*/
	HypCBuffer* &This() { return m_this; }

	/**
		@brief Get next element of linked list.

		@return Refernce to the pointer to the next element of linked
		        list or NULL if this is last element.
	*/
	HypCBufferLink* &Next() { return m_next; }

protected:
	/// Buffer associated to this element.
	HypCBuffer *m_this;

	/// Pointer to the next element in the linked list.
	HypCBufferLink *m_next;
};

/**
	@brief Append buffer to the linked list.

	@param bl Reference to the linked list.
	@param buf Pointer to the buffer to append to the linked list.
*/
void Append( HypCBufferLink &bl, HypCBuffer *buf );

/**
	@brief Remove element from linked list.

	Removes element from linked list and deletes associated buffer. If
	there is another element linked to the given element the next will be
	reassigned to the given pointer.

	@param bl Reference to the pointer to linked list element to delete.
*/
void RemoveOne( HypCBufferLink *&bl );

/**
	@}
*/

#endif //_HYPCBUFFERLINK_HPP_
