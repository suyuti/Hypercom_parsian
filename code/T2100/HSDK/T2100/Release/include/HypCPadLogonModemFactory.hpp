//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCPADLOGONMODEMFACTORY_HPP_
#define _HYPCPADLOGONMODEMFACTORY_HPP_
// Factory creates simple HypCModem for chat script processing on ext com ports

#include "HypCTransportFactory.hpp"


class HypCPadLogonModemFactory : public HypCTransportFactory
{
public:
	HypCPadLogonModemFactory(const HypCConfig *cfg=NULL, int id=0);
	virtual ~HypCPadLogonModemFactory();
	virtual HypCTransport *CreateTransport(HypCTransport *t);
};


#endif //_HYPCPADLOGONMODEMFACTORY_HPP_
