//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCCONFIGSET_HPP_
#define _HYPCCONFIGSET_HPP_

#include "HypCConfig.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Configuration set container.

	An object of the HypCConfigSet is used to store a set of HypCConfig
	objects.
*/
class HypCConfigSet
{
public:
	/**
		@brief Constructor.

		@param sz Size of an empty initial controlled sequence. Memory
		          is allocated for store up to sz elements and will
		          dynamically grow as needed. Optional, defaults to 2.

		@note Initializes work variables.
	*/
	HypCConfigSet( size_t sz = 2 ) : m_ConfigSet( sz ) {}

	/**
		@brief Destructor.
	*/
	~HypCConfigSet() {}

	/**
		@brief Append configuration.

		Appends the given configuration to the set.

		@param p Configuration to append.
	*/
	void append( const HypCConfig &p );

	/**
		@brief Get set size.

		@return The number of the active elements in the set.
	*/
	inline size_t size() { return m_ConfigSet.size(); }

	/**
		@brief Get iterator to the first defined configuration.

		@return Iterator to the first defined configuration.
	*/
	inline HypCConfig *begin() const { return m_ConfigSet.begin(); }

	/**
		@brief Get iterator to the last defined configuration.

		@return Iterator to the last defined configuration.
	*/
	inline HypCConfig *end() const { return m_ConfigSet.end(); }

	/**
		@brief Erase configuration from the set by index.

		Erases configuration from the given position.

		@param p0 Position to erase configuration on.
	*/
	void erase( size_t p0 );

	/**
		@brief Erase configuration from the set by iterator.

		Erases configuration by the given iterator.

		@param p Iterator to erase configuration on.
	*/
	void erase( HypCConfig *p );

	/**
		@brief Remove all configurations from the set.
	*/
	void clear();

	/**
		@brief Copy operator.

		Deletes all configurations contained in the set, then copy all
		configurations from the specified buffer.

		@param addr Set to copy configurations from.

		@return Reference to this buffer.
	*/
	HypCConfigSet &operator =( const HypCConfigSet &addr );

	/**
		@brief Index operator.

		Returns a referense to the configuration in the set at given
		position. Does not check for invalid position.

		@param p0 Position within set.

		@return Reference to the configuration at specified position.
	*/
	HypCConfig &operator []( size_t p0 );

	/**
		@brief Constant index operator.

		Returns a constant referense to the configuration in the set
		at given position. Does not check for invalid position.

		@param p0 Position within set.

		@return Constant reference to the configuration at specified
		        position.
	*/
	const HypCConfig &operator []( size_t p0 ) const;

private:
	/// Vector that holds configurations.
	SimpleVector< HypCConfig > m_ConfigSet;
};

/**
	@}
*/

#endif
