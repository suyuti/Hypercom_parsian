//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCCOMMMODE_HPP_
#define _HYPCCOMMMODE_HPP_

#include "SimpleVector.hpp"
#include "HypCTransportFactory.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Communication mode definition.

	Holds modem mode and steps required to create protocol layers hierarchy.
*/
class HypCCommMode
{
public:
	/**
		@brief Constructor.

		@param modeNo Modem mode number.

		@note Initializes work variables.
	*/
	HypCCommMode( size_t modeNo );

	/**
		@brief Destructor.
	*/
	~HypCCommMode();

	/**
		@brief Add new layer to the top of hierarchy.

		@param factory Transport factory to add to the new layer.
	*/
	void AddLayer( HypCTransportFactory *factory );

	/**
		@brief Get factory from given layer.

		@param l The layer to get factory for.

		@return Transport factory for given layer.
	*/
	HypCTransportFactory *GetLayer( size_t l );

	/**
		@brief Get number of layers.

		@return Number of layers.
	*/
	size_t Size() const;

	/**
		@brief Get modem mode number.

		@return Modem mode number.
	*/
	inline size_t GetCommModeNo() const { return m_CommModeNo; }

	/**
		@brief Get pointer to the first factory in the stack.

		@return Pointer to the first factory in the stack.
	*/
	inline HypCTransportFactory **Begin() const { return m_Stack.begin(); }

	/**
		@brief Get pointer to the last factory in the stack.

		@return Pointer to the last factory in the stack.
	*/
	inline HypCTransportFactory **End() const { return m_Stack.end(); }

private:
	/// Modem mode number.
	size_t m_CommModeNo;

	/// Stack of the transport layers.
	SimpleVector<HypCTransportFactory *> m_Stack;
};

/**
	@}
*/

#endif  //_HYPCCOMMMODE_HPP_
