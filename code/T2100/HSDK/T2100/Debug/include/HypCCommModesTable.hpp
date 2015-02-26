//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCCOMMMODESTABLE_HPP_
#define _HYPCCOMMMODESTABLE_HPP_

#include "SimpleVector.hpp"
#include "HypCCommMode.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Communication mode table definition.

	Holds modem mode table.
*/
class HypCCommModesTable
{
public:
	/**
		@brief Constructor.

		@note Initializes work variables.
	*/
	HypCCommModesTable();

	/**
		@brief Destructor.
	*/
	~HypCCommModesTable();

	/**
		@brief Add new modem mode to the table.

		@param m Modem mode to add.
	*/
	void AddMode( HypCCommMode *m );

	/**
		@brief Find modem mode by number.

		@param mm_no Modem mode number.

		@return Modem mode array for the corresponding modem mode or
		        NULL if not found.
	*/
	HypCCommMode *FindMode( size_t mm_no );

	/**
		@brief Get iterator to the first defined modem mode.

		@return Iterator to the first defined modem mode.
	*/
	HypCCommMode **Begin();

	/**
		@brief Get iterator to the last defined modem mode.

		@return Iterator to the last defined modem mode.
	*/
	HypCCommMode **End();

private:
	/// Modem modes list.
	SimpleVector< HypCCommMode * > m_Table;
};

/**
	@}
*/

#endif //_HYPCCOMMMODESTABLE_HPP_
