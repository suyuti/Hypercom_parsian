//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCSERIALPORT_HPP_
#define _HYPCSERIALPORT_HPP_

//=============================================================================
//
// HypCSerialPort.hpp
//
// Serial port device wrapper.
//
//=============================================================================
#include <eftos.h>
#include "HypCTransparentCommon.hpp"
#include "serial.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

#define SERIALPORT_STACK_SIZE 4096

/**
	@brief Line mode
*/
enum HypESerialLineMode
{
	HYP_8N1,
	HYP_7E1
};

/**
	@brief Serial port baud rate.
*/
enum HypESerialBaudRate
{
        HYP_SBR_300=S_BAUD300,
        HYP_SBR_600=S_BAUD600,
        HYP_SBR_1200=S_BAUD1200,
        HYP_SBR_2400=S_BAUD2400,
        HYP_SBR_4800=S_BAUD4800,
        HYP_SBR_9600=S_BAUD9600,
        HYP_SBR_19200=S_BAUD19200,
        HYP_SBR_38400=S_BAUD38400,
        HYP_SBR_57600=S_BAUD57600,
        HYP_SBR_115200=S_BAUD115200
};

/**
	@brief Serial port flow control mode.
*/
enum HypESerialFlowCtrl
{
	//hardware flow control:
	//Hi RTS means that the DTE device (terminal) is ready for receiving data
	//Hi CTS means that the DCE device (modem) is ready for receiving data
	HYP_HARDWARE_FC = SERIAL_HARDWARE_FC,
	// Flow control without DTR/DSR
	HYP_HARDWARE_NOTERMINAL_FC = SERIAL_HARDWARE_NOTERMINAL_FC, 
	HYP_NONE_FC=SERIAL_NONE_FC,
	HYP_TX_FC=SERIAL_TX_FC
};

/**
	@brief Serial port stop bit size.
*/
enum HypESerialStop
{
        HYP_STOP_1 = 1,
        HYP_STOP_2 = 2
};

/**
	@brief Serial port parity mode.
*/
enum HypESerialParity
{
        HYP_PARITY_NONE = CH_PARITY_NONE,
        HYP_PARITY_ODD = CH_PARITY_ODD,
        HYP_PARITY_EVEN = CH_PARITY_EVEN
};

/**
	@brief Serial port driver mode for packet assembling.
*/
enum HypESerialDriverMode
{
	HYP_NORMAL=SERIAL_NORMAL,
	HYP_APACS40=SERIAL_APACS40,
	HYP_POSMINI=SERIAL_POSMINI,
	HYP_POSTRANSPAD=SERIAL_POSTRANSPAD,
	HYP_V80=SERIAL_V80,
	HYP_PPP=SERIAL_PPP,
	HYP_HYPVISA=SERIAL_HYPVISA,
	HYP_HEMS=SERIAL_HEMS,
	HYP_GS1=SERIAL_GS1,
	HYP_HFT=SERIAL_HFT,
	HYP_SIBS=SERIAL_SIBS,
	HYP_V80HD=SERIAL_V80_HALFDUPLEX
};

/**
	@brief Serial port leads definition.
*/
enum HypESerialPortLeads
{
	HYP_DTR=DTR,
	HYP_RTS=RTS,
	HYP_DSR=DSR,
	HYP_CTS=CTS,
	HYP_DCD=CD,
	HYP_RI=RI
};

/**
	@brief Serial port data bit size.
*/
enum HypESerialDataBits
{
	HYP_8BIT=8,
	HYP_7BIT=7
};

/**
	@brief Configuration parameters of serial port.
*/
typedef struct HypSSerialPortParameters
{
	enum HypESerialBaudRate BaudRate;
	enum HypESerialFlowCtrl FlowCtrl;
	int  DataBits;
	enum HypESerialParity	Parity;
	enum HypESerialStop		StopBits;

	enum HypESerialDriverMode Mode;

	//Specifies the maximum time, in milliseconds, allowed to elapse between
	//the arrival of two characters on the communications line
	unsigned int ReadIntervalTimeout;  
	int ErrorCharFlag: 1;      // enable error replacement 
	char ErrorChar;            // error replacement character 
}  HypSSerialPortParameters;


/**
	@brief This class provided standard C++ interface to the OS driver.
	@brief It is used as subtransport by the modem class.
*/
class HypCSerialPort : virtual public HypCTransparentCommon
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		// Constructor
		HypCSerialPort( int deviceId );

		// Destructor
		virtual ~HypCSerialPort();

		virtual bool GetPin(enum HypESerialPortLeads pin);
		virtual void SetPin(enum HypESerialPortLeads pin, bool bState);
		virtual void PinReq(HypCEvent &event, int pins);

	protected:
		// StartRead is called to request underlying layer to read data.
		virtual void StartRead();

		// Open the device
		void StConnecting();

		// Connected state handler
		void StConnected();

		// Close the device
		virtual void BeforeDisconnectTransport();

		virtual void DoTrackDisconnectReq( HypCEvent &event);

		virtual void DoSetConfig(const HypCConfig &cfg);

		virtual void ClearReadBuf();

		virtual void StateMachine();

		void SendPinEvent(int status, int opt);

		virtual void DoFlush();

	private:
		// Not implemented on purpose. We do not want to create devices
		// without a type and an ID
		HypCSerialPort();

		//Adjusts settings of opened serial port device.
		bool SetSerialPortParam( void );
		void DefaultMode();

//=============================================================================
// Member variables
//=============================================================================
	private:
		//Serial port config parameters
		int m_PinMask;
		HypCEvent *m_pPinEvent;

};

#endif // _HYPCSERIALPORT_HPP_
