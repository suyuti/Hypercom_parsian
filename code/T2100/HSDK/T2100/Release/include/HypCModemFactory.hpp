//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCMODEMFACTORY_HPP_
#define _HYPCMODEMFACTORY_HPP_

#include "HypCTransportFactory.hpp"


class HypCModemFactory : public HypCTransportFactory
{
public:
	HypCModemFactory(const HypCConfig *cfg=NULL, int id=0);
	virtual ~HypCModemFactory();
	virtual HypCTransport *CreateTransport(HypCTransport *t);
};


#endif //_HYPCMODEMFACTORY_HPP_
