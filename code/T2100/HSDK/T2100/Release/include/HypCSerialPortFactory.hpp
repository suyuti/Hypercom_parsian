//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCSERIALPORTFACTORY_HPP_
#define _HYPCSERIALPORTFACTORY_HPP_

#include "HypCTransportFactory.hpp"


class HypCSerialPortFactory : public HypCTransportFactory
{
public:
	HypCSerialPortFactory(const HypCConfig *cfg=NULL, int id=0);
	virtual ~HypCSerialPortFactory();
	virtual HypCTransport *CreateTransport(HypCTransport *t);
};


#endif //_HYPCSERIALPORTFACTORY_HPP_
