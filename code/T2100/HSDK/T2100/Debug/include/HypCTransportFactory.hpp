//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCPROTOFACTORY_HPP_
#define _HYPCPROTOFACTORY_HPP_


#include "transports.h"
#include "HypCConfig.hpp"
#include "HypCTransport.hpp"

class HypCTransportFactory
{
public:
	HypCTransportFactory(const HypCConfig *cfg=NULL, int id=0);
	virtual ~HypCTransportFactory();

	void SetConfig();
	void SetConfig(HypCConfig &cfg);
	virtual HypCTransport *CreateTransport(HypCTransport *t)=0;
	void DestroyTransport();

	inline int GetDeviceID() const
		{ return m_Id; }
	inline HypCTransport *GetTransport() const
		{ return m_Transport; }
	inline HypETransports GetTransportType() const
		{ return m_TransportType; }
	inline HypCConfig* GetDefaultConfig()
	{ return m_DefaultCfg; }
	void SetDefaultConfig(const HypCConfig * cfg);



protected:
	int m_Id;
	HypCTransport *m_Transport;
	HypETransports m_TransportType;
	HypCConfig * m_DefaultCfg;
};


template <class T, HypETransports P>
class HypCFactoryT : public HypCTransportFactory
{
public:
	HypCFactoryT(const HypCConfig *cfg=NULL, int id=0) :
	  HypCTransportFactory (cfg, id)
	{
#ifdef __ARMCC_VERSION
		(T *)NULL;
#endif
		m_TransportType=P;
	}
	virtual ~HypCFactoryT()
	{
	}
	virtual HypCTransport *CreateTransport(HypCTransport *t)
	{
		if (m_Transport==NULL)
		{
			if (t!=NULL)
			{
				m_Transport=new T(t, m_Id);
			}
		}
		return m_Transport;
	}
};

#endif
