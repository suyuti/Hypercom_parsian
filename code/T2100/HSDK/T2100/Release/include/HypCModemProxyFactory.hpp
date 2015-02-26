//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCMODEMPROXYFACTORY_HPP_
#define _HYPCMODEMPROXYFACTORY_HPP_

#include "HypCTransportFactory.hpp"


class HypCModemProxyFactory : public HypCTransportFactory
{
public:
	HypCModemProxyFactory(const HypCConfig *cfg=NULL, int id=0);
	virtual ~HypCModemProxyFactory();
	virtual HypCTransport *CreateTransport(HypCTransport *t);
};


#endif //_HYPCMODEMPROXYFACTORY_HPP_
