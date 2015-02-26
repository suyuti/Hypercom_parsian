//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCLAN_HPP_
#define _HYPCLAN_HPP_

#include "HypCHemsCommon6.hpp"

class HypCLan : public HypCHemsCommon6
{
	protected:
		enum HypCLanBaudRate
		{
			HEMS_LAN_LOWSPEED = 0x04,
			HEMS_LAN_HISPEED = 0x19
		};

		enum HypCLanConnections
		{
			HEMS_LAN_PERMANENT = 0x00,
			HEMS_LAN_ONDEMAND
		};

	public:
		HypCLan( HypCTransport *transport, int nDeviceID );
		// Destructor
		virtual ~HypCLan();

	protected:
		// virtual void WaitEvent();

		virtual void TransportConnected();
		virtual void BeforeDisconnectTransport();

		virtual void BuildFrame();

		virtual TransparentCommonEvents ParseApplicationFrame();
		virtual bool TagParser(HypEHemsTags tag, size_t tagsize, size_t posn);

		virtual bool DoSendStart();

		void GenDestinationTag( uint8_t *&p, size_t &len);
		uint8_t *ConvertByte(const uint8_t *str, uint8_t *p);

	protected:
		uint16_t m_SocketNo;
		HypCEvent *m_pLocalRequest;
		HypCLanConnections m_Connection;
};

#endif //_HYPCLAN_HPP_
