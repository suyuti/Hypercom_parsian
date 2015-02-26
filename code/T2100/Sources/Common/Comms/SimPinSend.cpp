
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             SimPinSend.c
//      Unlock SIM card (send PIN to the card)
//
//=============================================================================

#include "eftos.h"
#include "hw.h"
#include "HypCSerialPort.hpp"
#include "HypCConfig.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "sysstruc.inc"
#include "fixdata.h"
#include "SimPinSend.h"
#include "kbddev.h"


extern "C" {
#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "utllib.h"
}
//=============================================================================
// Public data definitions from other modules
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================

#define TIMER_EVENT (USERDEFINED_REQUEST+1)

//=============================================================================
// Private function declarations
//=============================================================================
HypCSerialPort *SerialPortForModule(void);
static bool SendATCommand( const char *reqString, const char *respString, unsigned int timeout, char *result =0 );
static bool PinEntry ( char * pPIN, int * pinLenght);


//=============================================================================
// Private data definitions
//=============================================================================
int UnlockStatus = 1;

const char ATATReq[] = {"AT\r"};
const char ATOKResp[] = {"OK"};
const char ATCPINReq[] = {"AT+CPIN?\r"};
const char ATCPINRResp[] = {"+CPIN:"};
const char ATSPICReq[] = {"AT^SPIC\r"};
const char ATSPICResp[] = {"^SPIC"};
const char ATSPIC1Resp[] = {"^SPIC: 1"};
char ATPINReq[20] = {"AT+CPIN="};
const char ATReadyResp[] = {"+CPIN: READY"};
const char ATSCKSReq[] = {"AT^SCKS?\r"};
const char ATSCKSResp[] = {"^SCKS: 0"};

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  SimPinSend - Do configure GSM modem (bitrate and flow control) and 
//               unlock GSM SIM card (send PIN)
//
//  Parameters:  NONE
//
//  Returns: 1 - OK processed or does mot required 
//           0 - there is unlock error
//-----------------------------------------------------------------------------
int SimPinSend(void)
{
	BYTE	i;
	bool succeed;
	char * tempbuf;
	int pinLenght;

	UnlockStatus = True;
	succeed = false;

    // Check if GSM modem used (hw1 or hw2)
	if ( ( TM3_GSM_OLD != (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) &&
	     ( TM3_GSM != (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) &&
		 ( TM3_DIALBACKUP != (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) )
    {
       // No this is no GSM modem. Exit with OK
       return 1;
    }

    // Check for Sim PIN unlocking required
	if (!(TERM.TERMOpt3 & TM3_SIMPIN) )
    {
        // OK Sim PIN does not required
        return 1; 
	}


	// Show Starting GSM Modem Nessage
	D_DispStr((unsigned char*) "\fStarting GSM modem\n" );
	for (i=0; i< 10; i++)
	{
		// Wait for 3 second , check every 300 mSec for cancel key
		SDK_Wait(TENMS * 30);
		if ( K_PressedKey()  == KCANCEL )
		{
			D_Disp((unsigned char*) "\fWarning: Procedure   ", 22 );
			D_Disp((unsigned char*) "is cancelled", 12 );
			UnlockStatus = False;
			SDK_Wait(ONESECOND);
			return False;
		}
	}


	for (i=0;	i<15;	i++)
	{
		// Check for AT command responce 15 times 
		if ( SendATCommand(&ATATReq[0],&ATOKResp[0], 1000) )
			break;
	}
	// No responce for 15 times
	if ( i >= 14)
	{
		succeed = false;
		goto unlock_done;		
	}
	tempbuf = new char[30];
	// PIN status request (Send AT+CPIN? request)
	if ( true ==  SendATCommand(&ATCPINReq[0],&ATCPINRResp[0], 3000, tempbuf ) )
	{
		// +CPIN responce recieve
		// Check for PIN already entered
		if ( NULL !=  strstr(tempbuf,ATReadyResp ) )
		{
			delete tempbuf;
			succeed = true;
			goto unlock_done;		
		}
		delete tempbuf;
	}
	else
	{
		// +CPIN responce do not recieve
		delete tempbuf;
		succeed = false;
		goto unlock_done;		
	}

	tempbuf = new char[30];
	// Check for number of try request
	if ( true ==  SendATCommand(&ATSPICReq[0],&ATSPICResp[0], 3000, tempbuf) )
	{
		delete tempbuf;
		// Check  for number of try (if only 1 try exit)
		if ( NULL !=  strstr(tempbuf,ATSPIC1Resp ) )
		{
			succeed = false;
			goto unlock_done;		
		}
	}
	else
	{
		// No correct responce
		succeed = false;
		delete tempbuf;
		goto unlock_done;		
	}
	// Reset counter
	pinLenght = 0;
	// Check For PIN string empty
	if ( (PIN_CODE[3]>= '0') && (PIN_CODE[3]<= '9') )
	{
		strcat(ATPINReq, "\"");
		strncat(ATPINReq, (const char*)(PIN_CODE + 3), 8);
		strcat(ATPINReq, "\"\r");
	}
	else
	{
		tempbuf = new char[8];
		if ( false == PinEntry( tempbuf, &pinLenght) )
		{
			// No correct PIN
			succeed = false;
			delete tempbuf;
			goto unlock_done;		
		}
		strcat(ATPINReq, "\"");
		strncat(ATPINReq, tempbuf, pinLenght);
		strcat(ATPINReq, "\"\r");
	}
	if ( true ==  SendATCommand(&ATPINReq[0],&ATOKResp[0], 30000) )
	{
		succeed = true;
		if ( pinLenght != 0 )
		{
			// Save Pin Data
			//format pin record
			memset(PIN_CODE, 0, sizeof(PIN_CODE));
			PIN_CODE[0] = 'S';
			PIN_CODE[1] = 'C';
			PIN_CODE[2] = '0' + pinLenght;
			memcpy( PIN_CODE + 3, tempbuf, pinLenght );
			// Set Pin code LRC
			PIN_CODELRC = CalcLrc( (UBYTE *)PIN_CODE, ( UWORD ) sizeof( PIN_CODE ) );
		}
	}
	else
	{
		memset( &PIN_CODE, 0, 12);
		succeed = false;
		goto unlock_done;		
	}
	

unlock_done:
	if ( false == succeed )
	{
		UnlockStatus = False;
		if ( true ==  SendATCommand(&ATSCKSReq[0],&ATSCKSResp[0], 3000 ) )
		{
			// no SIM card
			D_DispStr((unsigned char*) "\fError: NO SIM CARD\n" );
		}
		else
		{
			// forbidden SIM status
			D_DispStr((unsigned char*) "\fError: Forbidden SIM Status\n" );
		}
		SDK_Wait( ONESECOND );
	}
	

    if ( succeed )
        return ( 1 );
    else
        return ( 0 );
}




//=============================================================================
// Private function definitions
//=============================================================================
HypCSerialPort *SerialPortForModule(void)
{
	HypCSerialPort *com2=new HypCSerialPort(RS_1);
	HypCConfig cfg;

	// Create configuration for serial line to module
	switch(TERM.TERMOpt3 & TM3_HWTYPE_MASK)
	{

		// GSM modem HW1 require no automatic handling of DSR/DTR
		case TM3_GSM_OLD:
			cfg.Set(HC_BaudRate, HYP_SBR_115200);
			cfg.Set(HC_FlowControl, HYP_HARDWARE_NOTERMINAL_FC);
			break;

		// Allow maximum bitrate and full hardware flow control
		// for other modules
		default:
			cfg.Set(HC_BaudRate, HYP_SBR_115200);
			cfg.Set(HC_FlowControl, HYP_HARDWARE_FC);
			break;
	}

	com2->SetConfig (cfg);

	return com2;
}

//-----------------------------------------------------------------------------
//  SendATCommand - send AT comand to Port and wait for responce
//
//  Parameters: pPort - pointer to port
//              reqString - pointer to request
//					 respString - pointer to responce
//
//  Returns:  	true  - correct responce recieved
//					false - timeout or bad responce
//-----------------------------------------------------------------------------
bool SendATCommand( const char *reqString, const char *respString, unsigned int timeout , char *result )
{
	HypCEvent *event;
	HypCTimer *timer;
	HypCBuffer *buffer;
	int req,status,t;
	HypCBuffer *opt;
	bool bRecieveResp = false;
        req = 0;

	// Create configuration for serial line to module
	HypCSerialPort *com2 = SerialPortForModule();
	com2->Connect(); //modem

	// Create new objects
	event=new HypCEvent();
	timer=new HypCTimer(*event,TIMER_EVENT);
	buffer = new HypCBuffer(reqString, strlen(reqString));

	timer->Start(timeout);
	com2->WriteReq( *event, buffer);

	//wait for the write complete or write timeout
	while ( req != TIMER_EVENT )
	{

		event->Wait(req,status,t);
		if ( req == WRITE_REQUEST)
		{
//			ClearEvent(event);
			break;
		}

		if (req == TIMER_EVENT)
		{
				com2->CancelRequest( WRITE_REQUEST );
				break;
		}
	}

	com2->ReadReq( *event );
	//wait for the write complete or write timeout
	while ( req != TIMER_EVENT )
	{

		event->Wait(req,status,t,(int&)opt);

		if ( req == READ_REQUEST)
		{
			//Process modem answer
				
			if ( NULL != strstr(opt->c_str(), respString) )
			{
				//substring found
				//go to next state
				bRecieveResp = true;
				if ( 0!= result )
					memcpy( result, opt->data() , opt->size() );
				delete opt;
				break;
			}		
			else
			{
//				ClearEvent(event);
				com2->ReadReq( *event );
				delete opt;
			}
		}
		if (req == TIMER_EVENT)
		{
				com2->CancelRequest( READ_REQUEST );
				break;
		}

	}

	timer->Stop();
	delete (timer);
	delete (event);
//	delete (buffer);
	com2->Disconnect();
	delete com2;

	return bRecieveResp;

}

//-----------------------------------------------------------------------------
//  Get_UnlockStatus - get the result of unlock GSM SIM card (send PIN)
//
//  Parameters:  NONE
//
//  Returns: 1 - OK processed or does mot required 
//           0 - there is unlock error
//-----------------------------------------------------------------------------
int Get_UnlockStatus ( void )
{
    return UnlockStatus;
}

//-----------------------------------------------------------------------------
//  Set_UnlockStatus - set the result of unlock GSM SIM card (send PIN) 
//
//  Parameters: 1 - OK processed or does mot required 
//              0 - there is unlock error
//
//  Returns:  NONE
//-----------------------------------------------------------------------------
void Set_UnlockStatus ( int Status )
{
    UnlockStatus = Status;
}

//-----------------------------------------------------------------------------
//  PinEntry - enter Pin number up 10 ten characters.
//
//  Parameters: true - Pin was entered 
//              false - Pin was not entered
//
//  Returns:  NONE
//-----------------------------------------------------------------------------
static bool PinEntry ( char * pPIN, int * pinLenght)
{
	int curNumber;
	int i;
	BYTE currentKey;
	bool bClearWasPressed;
	// Show PinEntry
	bClearWasPressed = True;
	// Max time 30 second
	for (i = 0; i<100; i++)
	{
		SDK_Wait(TENMS*30);
		if (bClearWasPressed)
		{
			bClearWasPressed = false;
			curNumber = 0;
			D_Disp((unsigned char*)"\fEnter PIN: ", 12);
		}
		currentKey = K_PressedKey();
		switch (currentKey)
		{
		case KCANCEL:
			return false;
		case KCLEAR:
			bClearWasPressed = true;
			break;
		case KENTER:
			*pinLenght = curNumber;
			if (curNumber > 0)
				return true;
			break;
		case K0:
		case K1:
		case K2:
		case K3:
		case K4:
		case K5:
		case K6:
		case K7:
		case K8:
		case K9:
			// Check for Max PIN lenght
			if (curNumber < 8)
			{
				D_Disp((unsigned char*)"*",1);
				curNumber++;
			}
			else			
				currentKey = 0; // Clear key value
			break;
		}
		// Save pressed KEY
		switch (currentKey)
		{
		case K0:
			*(pPIN + curNumber -1) = '0';
			break;
		case K1:
			*(pPIN + curNumber -1) = '1';
			break;
		case K2:
			*(pPIN + curNumber -1) = '2';
			break;
		case K3:
			*(pPIN + curNumber -1) = '3';
			break;
		case K4:
			*(pPIN + curNumber -1) = '4';
			break;
		case K5:
			*(pPIN + curNumber -1) = '5';
			break;
		case K6:
			*(pPIN + curNumber -1) = '6';
			break;
		case K7:
			*(pPIN + curNumber -1) = '7';
			break;
		case K8:
			*(pPIN + curNumber -1) = '8';
			break;
		case K9:
			*(pPIN + curNumber -1) = '9';
			break;
		}
	
	}
	// timeout
	return false;
}
