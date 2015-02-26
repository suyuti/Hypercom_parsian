//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCDUKPT_HPP_
#define _HYPCDUKPT_HPP_

#include "HypCCat33.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief DUKPT transport layer implementation.

	Same as terminal side CAT33, except of not waiting EOT at the end of
	session.
*/
class HypCDukpt : public HypCCat33
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCDukpt( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCDukpt();
};

/**
	@}
*/

#endif //_HYPCDUKPT_HPP_
