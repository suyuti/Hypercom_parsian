
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             CommModule.c
//      Do reset communication module at start up
//
//=============================================================================

#include "hw.h"
#include "basictyp.h"
#include "sysstruc.inc"
#include "siodev.inc"
#include "prtdev.inc"
#include "fixdata.h"
#include "sdk.h"
#include "sdkio.h"
#include "hwconexant.h"

#include "struct.h"
#include "rs232tab.h"
#include "comdata.h"
#include "commmodule.h"

//=============================================================================
// Public function declarations
//=============================================================================


//-----------------------------------------------------------------------------
//  ResetCommModule - reset and config specified hardware comms module
//
//
//  Parameters:
//
//  Returns:
//      Nothing
//-----------------------------------------------------------------------------
extern void ResetCommModule ( void )
{
    LOCALIPCONF IpConf;
	LOCALPOSLANCONF poslancfg;
	COMCONFIGDATA config;

    // Start any hardware specific COMM module
    switch ( TERM.TERMOpt3 & TM3_HWTYPE_MASK )
    {
        // V22 phone modem
        case TM3_V22:
            if (TERM.TERMOpt2 & TM2_9600NF)
            {
                // perform hardware reset and load patch
                HW_ResetCommModule(HW_MODEMV22);
            }
            else
            {
                // perform hardware reset only
                HW_ResetConexantModem();
            }
            break;

        // V90 phone modem
        case TM3_V90:
            if (TERM.TERMOpt2 & TM2_9600NF)
            {
                // perform hardware reset, load patch and set fixed baud rate
                HW_ResetCommModule(HW_MODEMV90);
            }
            else
            {
                // perform hardware reset only
                HW_ResetConexantModem();
		HW_SetFixedBaudRate();
            }
            break;

        // GSM modem
        case TM3_GSM:
            HW_ResetCommModule(HW_GSMMODEM);
            break;

        // GSM modem (old version)
        case TM3_GSM_OLD:
            HW_ResetCommModule(HW_GSMMODEM_OLD);
            break;

        // Ethernret module
        case TM3_ETHERNET:
            HW_ResetCommModule(HW_ETHERNET);
	
            IpConf.Address = TERM.TERMIPAddress;
            IpConf.Mask = TERM.TERMSubnetIP;
            IpConf.Gateway = TERM.TERMGatewayIP;
	
            IpConf.Parameters = 0;
            ((UBYTE*)&IpConf.Parameters)[0] = ((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET) ? (TERM.TERMOpt3 & TM3_HEMSTRANSP) ? 1 : 2 : 3;
            ((UBYTE*)&IpConf.Parameters)[1] = TERM.TERMPDMode;
            ((UBYTE*)&IpConf.Parameters)[2] = (TERM.TERMOpt3 & TM3_SSL);

            config.Ptr=(const void *)&IpConf;
            SDK_ComConfig( cc_LOCALIPCONF, config );
            
            break;

        // POS LAN module
        case TM3_LAN:
            HW_ResetCommModule(HW_POSLAN);

            // set CU address
            poslancfg.Address = TERM.TERMCuAd;
             
            // set baud rate
            if ( SIO_B4800 == RSTAB.RSSPEED )
                poslancfg.BaudRate = COM_POS_4800;
            else
                poslancfg.BaudRate = COM_POS_19200;

            // set timeout
            poslancfg.Timeout = 30; // SNRM timeout

            // config it
            config.Ptr = &poslancfg;
            SDK_ComConfig(cc_LOCALPOSLANCONF, config);

            break;

        // Bluetooth module
        case TM3_BLUETOOTH:
            HW_ResetCommModule(HW_BLUETOOTH);
            break;

        // ISDN module - do nothing 
        case TM3_ISDN:
            HW_ResetCommModule(HW_MODEMISDN);
            break;
	// V90, GSM/GPRS  Dial back up module // @AAMELIN
        case TM3_DIALBACKUP:
		if (TERM.TERMOpt2 & TM2_9600NF)
                {
                  HW_ResetCommModule(HW_DIALBACKUP);

                }else
                {
                  HW_InitCommModuleType(HW_DIALBACKUP);
                  HW_ResetConexantModem();
                  HW_SetFixedBaudRate();
                  HW_DBSwitchToGSM();
                }
		break;

    }

    // Force transsparent protocol for communication through COM1/COM2
    RSTAB.RSCOMPROTOCOL = PRTP_TRANS;
    SetRSTabLRC( );

}

