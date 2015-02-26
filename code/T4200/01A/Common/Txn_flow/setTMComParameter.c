
#include "setTMComParameter.h"

static struct ipcontab *FindIPCById( UBYTE id );
static struct ipcontab *SetIPTab( void );
static void DisplayFormat( UBYTE *display , int displayLen);
static Bool SaveFormat(UBYTE *tmpBuffer, UBYTE enteredLen, UBYTE len);
static Bool EnabledScreen(Bool Enabled, enum msg_id TitleMid,enum msg_id Line1Mid_Ena, enum msg_id Line1Mid_Dis );
static const ENTRY_STR IPEntry = {
    EnterIPAddress,                // Title
    CustomMsg,                  // 1-st line
    N_NullStr,                  // 2-nd line
};



/*************************************************************************************************
*    Function :  SetTMComParam
*    Parameter:  void
*
*    Return   :  void
*
*               Set TM communication parameters

    { NULL,           IMAGE_EMPTY,        N_IP,         'A'}, 
    { NULL,           IMAGE_EMPTY,        N_GPRS,       'B'},
    { NULL,           IMAGE_EMPTY,        N_HLAN,       'C'},
    { NULL,           IMAGE_EMPTY,        N_DIALBACKUP, 'D'},
    { NULL,           IMAGE_EMPTY,        N_ONLYDIALUP, 'E'},

*
**************************************************************************************************/
extern void SetTMComParam( void )
{
    /* MSD Cagirilmiyor
    Bool retval = False;
    Bool Enabled = False;

    DrawMenu( MENU_COMTYPE );

    do
    {
        // Get Keyboard Entry 
        switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
        {
        case 'A':  
            Enabled = AQTAB[0].AQOPT1 & AQ1_USEIP ? True : False;
            if ( EnabledScreen( Enabled, N_IP, IP_Ena, IP_Dis ) )
            {
                if ( Enabled )
                    AQTAB[0].AQOPT1 &= ~AQ1_USEIP;
                else
                    AQTAB[0].AQOPT1 |= AQ1_USEIP;
                SetAQTabLRC();
            }
            ButtonId = B_PREVIOUS;
            retval = True;
            break;

        case 'B':
            Enabled = AQTAB[0].AQOPT5 & AQ5_USEGPRS ? True : False;
            if ( EnabledScreen( Enabled, N_GPRS, GPRS_Ena, GPRS_Dis ) )
            {
                if ( Enabled )
                    AQTAB[0].AQOPT1 &= ~AQ5_USEGPRS;
                else
                    AQTAB[0].AQOPT1 |= AQ5_USEGPRS;
                SetAQTabLRC();
            }
            retval = True;
            ButtonId = B_PREVIOUS;
            break;

        case 'C':
            Enabled = AQTAB[0].AQOPT5 & AQ5_USEHLAN ? True : False;
            if ( EnabledScreen( Enabled, N_HLAN, HLAN_Ena, HLAN_Dis ) )
            {
                if ( Enabled )
                    AQTAB[0].AQOPT1 &= ~AQ5_USEHLAN;
                else
                    AQTAB[0].AQOPT1 |= AQ5_USEHLAN;
                SetAQTabLRC();
            }
            retval = True;
            ButtonId = B_PREVIOUS;
            break;

        case 'D':
            Enabled = AQTAB[0].AQOPT5 & AQ5_DBPHONE1 ? True : False;
            if ( EnabledScreen( Enabled, N_DialBackUp, DBackup_Ena, DBackup_Dis ) )
            {
                if ( Enabled )
                    AQTAB[0].AQOPT1 &= ~AQ5_DBPHONE1;
                else
                    AQTAB[0].AQOPT1 |= AQ5_DBPHONE1;
                SetAQTabLRC();
            }
            retval = True;
            ButtonId = B_PREVIOUS;
            break;

        case 'E':
            Enabled = ((AQTAB[0].AQOPT5 & AQ5_DBPHONE1) && (!(AQTAB[0].AQOPT1 & AQ1_USEIP))) ? True : False;
            if ( EnabledScreen( Enabled, N_Dial, Dial_Ena, Dial_Dis ) )
            {
                if ( Enabled )
                {
                    AQTAB[0].AQOPT1 |= AQ1_USEIP;                    
                }
                else
                {
                    AQTAB[0].AQOPT1 &= ~AQ1_USEIP;
                    AQTAB[0].AQOPT1 |= AQ5_DBPHONE1;
                }
                SetAQTabLRC();
            }

            ButtonId = B_PREVIOUS;
            retval = True;
            break;


        case B_PREVIOUS:    // Previous opts menu 
            DrawMenu( MENU_PREVIOUS );  // put previou menu back up 
            break;

        case B_NEXT:        // Next opts menu 
            DrawMenu( MENU_NEXT );
            break;

        case B_PREVIOUS_NEW_MENU:   // need to go to new menu 
            return;

        case CANCEL_KY: // Timed out or cancelled             
            return;

        case 0:
            SDK_Beeper( 1 );
            retval = True;
            break;

        default:
            SDK_Beeper( 1 );
            break;
        }

        if ( retval == 'N' )
        {
            DrawMenu( MENU_REDRAW );    // stay in Select Functions 
        }
    }while ( retval == False );

*/
}


extern void SetTMPhoneNumber( void )
{
    static const ENTRY_STR TelEntry = {
        EnterPhoneNumber,   // enterphonenumber
        Currently,          // custommsg
        CustomMsg,
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE Tel[S_AQPSTEL*2+1] = {0x00};
    int Status;

    tempDial = (SetTMParameterVariables & SELECT_TRX_HOST_INFO) ? &(AQTAB[0].conn.dial.trans) : &(AQTAB[0].conn.dial.settle);
    tempPhone = (SetTMParameterVariables & SELECT_PRIMARY_INFO) ? &(tempDial->primary) : &(tempDial->secondary);


    do
    {
        // Telefon Numarasýnýn var olan deðerini göster                 
        BfAscii( Tel, tempPhone->phoneNum, S_AQPSTEL );
        DisplayFormat( Tel, sizeof(Tel)/sizeof(UBYTE) );

        // yeni telefon numarasý deðeri alýnýr.        
        switch ( AlphaNumEntry( 1, S_AQPSTEL, 0, &TelEntry ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Tel, Dspbuf, length );

            if ( !SaveFormat(Tel, length, 24) )
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( tempPhone->phoneNum, Tel, S_AQPSTEL );
            SetAQTabLRC();
            ButtonId = B_PREVIOUS;

            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );
}

extern void SetTMConnectTime( void )
{
#define CONTIMELEN          2
    static const ENTRY_STR EntryMsg = {
        EnterConnectTime,   // enterphonenumber
        Currently,
        CustomMsg,          // custommsg        
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE tmpDisp[CONTIMELEN+1] = {0x00};
    int Status;
    UBYTE Temp[3];

    tempDial = (SetTMParameterVariables & SELECT_TRX_HOST_INFO) ? &(AQTAB[0].conn.dial.trans) : &(AQTAB[0].conn.dial.settle);
    tempPhone = (SetTMParameterVariables & SELECT_PRIMARY_INFO) ? &(tempDial->primary) : &(tempDial->secondary);


    do
    {
        // Telefon Numarasýnýn var olan deðerini göster                 
        BfAscii( tmpDisp, &(tempPhone->timeout), 1 );
        DisplayFormat(tmpDisp,sizeof(tmpDisp)/sizeof(UBYTE) );


        // yeni telefon numarasý deðeri alýnýr.        
        switch ( NumEntry( 1, CONTIMELEN, 1, &EntryMsg ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp, length, 3) )
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( &(tempPhone->timeout), Temp, 1 );
            SetAQTabLRC();

            ButtonId = B_PREVIOUS;

            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );
}

extern void SetTMDialAttemp( void )
{
#define DIALATTEMLEN    2
    static const ENTRY_STR EntryMsg = {
        EnterDialAttemp,   // enterphonenumber
        Currently,
        CustomMsg,          // custommsg        
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE tmpDisp[DIALATTEMLEN+1] = {0x00};
    int Status;
    UBYTE Temp[3]= {0x00};

    tempDial = (SetTMParameterVariables & SELECT_TRX_HOST_INFO) ? &(AQTAB[0].conn.dial.trans) : &(AQTAB[0].conn.dial.settle);
    tempPhone = (SetTMParameterVariables & SELECT_PRIMARY_INFO) ? &(tempDial->primary) : &(tempDial->secondary);


    do
    {
        // Telefon Numarasýnýn var olan deðerini göster                 
        BfAscii( tmpDisp, &(tempPhone->trys), 1 );
        DisplayFormat(tmpDisp, sizeof(tmpDisp)/sizeof(UBYTE));

        switch ( NumEntry( 1, DIALATTEMLEN, 1, &EntryMsg ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp, length, 3 ))
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( &(tempPhone->trys), Temp, 1 );
            SetAQTabLRC();


            ButtonId = B_PREVIOUS;

            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );

}

extern void SetTMPPPMode( void )
{
#define PPPMODELEN  2
    static const ENTRY_STR TelEntry = {
        EnterPPPMode,   // enterphonenumber
        Currently,        
        CustomMsg,          // custommsg        
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE tmpDisp[PPPMODELEN+1] = {0x00};
    int Status;
    UBYTE Temp[3]= {0x00};
    UBYTE *tmpPPPMode;

    if ( SetTMParameterVariables & SELECT_TRX_HOST_INFO )
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            tmpPPPMode = &(AQTAB[0].AQPPPMODE1);
        else
            tmpPPPMode = &(AQTAB[0].AQPPPMODE2);

    }
    else
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            tmpPPPMode = &(AQTAB[0].AQSPPPMODE1);
        else
            tmpPPPMode = &(AQTAB[0].AQSPPPMODE2);
    }

    do
    {
        // Telefon Numarasýnýn var olan deðerini göster                 
        BfAscii( tmpDisp, tmpPPPMode, 1 );
        DisplayFormat(tmpDisp, sizeof(tmpDisp)/sizeof(UBYTE));

        switch ( NumEntry( 1, PPPMODELEN, 1, &TelEntry ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp, length, 3))
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( tmpPPPMode, Temp, 1 );
            SetAQTabLRC();

            ButtonId = B_PREVIOUS;

            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );
}

extern void SetTMModemMode( void )
{
    static const ENTRY_STR TelEntry = {
        EnterPPPMode,   // enterphonenumber
        Currently,   // 
        CustomMsg,          // custommsg      
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE tmpDisp[3] = {0x00};
    int Status;
    UBYTE Temp[3]= {0x00};
    UBYTE *tmpModemMode;

    if ( SetTMParameterVariables & SELECT_TRX_HOST_INFO )
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            tmpModemMode = &(AQTAB[0].conn.dial.aqModem);
        else
            tmpModemMode = &(AQTAB[0].AQMODEM2);

    }
    else
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            tmpModemMode = &(AQTAB[0].AQSMODEM);
        else
            tmpModemMode = &(AQTAB[0].AQSMODEM2);
    }

    do
    {
        // Telefon Numarasýnýn var olan deðerini göster                 
        BfAscii( tmpDisp, tmpModemMode, 1 );
        DisplayFormat(tmpDisp, sizeof(tmpDisp)/sizeof(UBYTE));

        switch ( NumEntry( 1, 2, 1, &TelEntry ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp, length, 3))
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( tmpModemMode, Temp, 1 );
            SetAQTabLRC();
            ButtonId = B_PREVIOUS;
            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );

}


extern void SetTMIpAddress( void )
{
    struct ipcontab* pIPC;
    UBYTE retval;
    char ipAddress[16];
    size_t lenMsg, lenIp;

    pIPC = SetIPTab();
    if ( pIPC == NULL )
        return;

    ShowWarningMsg(UsePoint, BlankMsg );    

    memset( ipAddress, 0, sizeof(ipAddress) );
    memset( CSTMSG, 0, sizeof(CSTMSG) );

    GetMsg( CurrentIP, CSTMSG);
    lenMsg = strlen( CSTMSG );
    lenIp = strlen((char *)pIPC->ipaddr);
    memcpy( &CSTMSG[lenMsg], ( UBYTE * )pIPC->ipaddr , lenIp ); 

    retval = IpNumEntry(1, 15, 1, &IPEntry );
    if ( retval != 1 )
        return;

    memcpy(pIPC->ipaddr, Dspbuf, strlen(Dspbuf)); 
    SetIPCTabLRC();
    ButtonId = B_PREVIOUS;

}


extern void SetTMIpPort( void )
{
#define IPPORTLEN       5
    static const ENTRY_STR EntryMsg = {
        EnterIPPort,   // enterphonenumber
        Currently,
        CustomMsg,          // custommsg
    };

    int length,i;    
    UBYTE tmpDisp[IPPORTLEN+1] = {0x00};
    int Status;
    UBYTE Temp[3]= {0x00};
    UBYTE *tmpPort;
    struct ipcontab* pIPC;    

    pIPC = SetIPTab();
    if ( pIPC == NULL )
        return;

    tmpPort = pIPC->port;

    memcpy( tmpDisp, tmpPort, IPPORTLEN );

    // Move NMS Telephone Number to global display buffer 
    memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
    MoveTillZero( CSTMSG, tmpDisp, sizeof( CSTMSG ) );

    // Set clear key 
    GlobalDsp = CLRKEYON;

    // yeni telefon numarasý deðeri alýnýr.        
    switch ( NumEntry( 1, IPPORTLEN, 1, &EntryMsg ))
    {
    case 1:         // Okay; get length of input
        length = strlen(Dspbuf); 
        memset( tmpPort, 0x20, IPPORTLEN);
        memcpy( tmpPort, Dspbuf, length );
        SetIPCTabLRC();

        ButtonId = B_PREVIOUS;
        break;
    case 3:         // Go back to previous entry 
        //Status = -1;
        ButtonId = B_PREVIOUS;
        break;

    default:            // Entry cancelled or timed-out 
        //Status = 0;
        break;
    }


}

extern void SetTMIpNII( void )
{
#define NIILEN     2 
    static const ENTRY_STR TelEntry = {
        EnterNII,   // enterphonenumber
        Currently,
        CustomMsg,          // custommsg
    };

    int length,i;    
    UBYTE tmpDisp[6] = {0x00};
    int Status;
    UBYTE Temp[6]= {0x00};
    UBYTE *tmpNII;
    struct ipcontab* pIPC;


    pIPC = SetIPTab();
    if ( pIPC == NULL )
        return;

    tmpNII = pIPC->nii;

    do
    {
        BfAscii( tmpDisp, tmpNII, NIILEN );
        DisplayFormat(tmpDisp, sizeof(tmpDisp)/sizeof(UBYTE));

        switch ( NumEntry( 1, 4, 1, &TelEntry ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp,length, 6 ))
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( tmpNII, Temp, 2 );
            SetIPCTabLRC();
            ButtonId = B_PREVIOUS;
            break;

        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );

}

extern void HexLenInHeaderEnabled( void )
{
    static ENTRY_STR YNEntry;
    UBYTE keyval;   
    Bool Enabled = False;
    struct ipcontab *pIPC;

    pIPC = SetIPTab();
    if ( pIPC == NULL )
        return;

    Enabled = pIPC->opt1 & IPC_LENGTH;

    if ( Enabled )
    {
        YNEntry.TitleMid = N_HexLenEnable;
        YNEntry.Line1Mid = HexLen_Ena;
        YNEntry.Line2Mid = DisablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES  
        {
            pIPC->opt1 &= ~IPC_LENGTH;            
            SetIPCTabLRC();
        }
    }
    else
    {
        YNEntry.TitleMid = N_HexLenEnable;
        YNEntry.Line1Mid = HexLen_Dis;
        YNEntry.Line2Mid = EnablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES 
        {
            pIPC->opt1 |= IPC_LENGTH;
            SetIPCTabLRC();
        }
    }
}

extern void PosTPDUInHeaderEnabled( void )
{
    static ENTRY_STR YNEntry;
    UBYTE keyval;   
    Bool Enabled = False;

    struct ipcontab *pIPC;

    pIPC = SetIPTab();
    if ( pIPC == NULL )
        return;

    Enabled = pIPC->opt1 & IPC_TPDU ? True : False;

    if ( Enabled )
    {
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PosTPDU_Ena;
        YNEntry.Line2Mid = DisablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES  
        {
            pIPC->opt1 &= ~IPC_TPDU; 
            SetIPCTabLRC();
        }
    }
    else
    {
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PosTPDU_Dis;
        YNEntry.Line2Mid = EnablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES 
        {
            pIPC->opt1 |= IPC_TPDU; 
            SetIPCTabLRC();
        }
    }


}

extern void SetTMNII( void )
{
    static const ENTRY_STR TelEntry = {
        EnterNII,   // enterphonenumber
        Currently,
        CustomMsg,          // custommsg
    };

    PS_DIAL_STRUCT *tempDial;
    PHONE_STRUCT  *tempPhone;
    int length,i;    
    UBYTE tmpDisp[5] = {0x00};
    int Status;
    UBYTE Temp[5]= {0x00};
    UBYTE *tmpNII;

    tmpNII = AQTAB[0].AQNII;

    do
    {
        BfAscii( tmpDisp, tmpNII, 2 );
        DisplayFormat(tmpDisp, sizeof(tmpDisp)/sizeof(UBYTE) );

        switch ( NumEntry( 1, 4, 1, &TelEntry ))
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Temp, Dspbuf, length );

            if ( !SaveFormat(Temp, length, 5))
                Status = 99;

            // Don't save if illegal char was input.
            if ( 99 == Status )
                break;

            AscHex( tmpNII, Temp, 2 );
            SetAQTabLRC();

            ButtonId = B_PREVIOUS;
            break;
        case 3:         // Go back to previous entry 
            //Status = -1;
            ButtonId = B_PREVIOUS;
            break;

        default:            // Entry cancelled or timed-out 
            //Status = 0;
            break;
        }
    }while ( Status == 99 );
}

extern void PPPModeEnabled( void )
{
    static ENTRY_STR YNEntry;
    UBYTE keyval;   
    Bool Enabled = False;

    if ( SetTMParameterVariables & SELECT_TRX_HOST_INFO )
    {
        Enabled = AQTAB[0].AQOPT5 & AQ5_TRANS_PPP ? True : False;           
    }
    else
    {
        Enabled = AQTAB[0].AQOPT5 & AQ5_SETTLE_PPP ? True : False;       
    }

    if ( Enabled )
    {
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PPP_Ena;
        YNEntry.Line2Mid = DisablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES  
        {
            if ( SetTMParameterVariables && SELECT_TRX_HOST_INFO )
                AQTAB[0].AQOPT5 &= ~AQ5_TRANS_PPP;
            else
                AQTAB[0].AQOPT5 &= ~AQ5_SETTLE_PPP;

            SetAQTabLRC();
        }
    }
    else
    {
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PPP_Dis;
        YNEntry.Line2Mid = EnablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES 
        {
            if ( SetTMParameterVariables & SELECT_TRX_HOST_INFO )
                AQTAB[0].AQOPT5 |= AQ5_TRANS_PPP;
            else
                AQTAB[0].AQOPT5 |= AQ5_SETTLE_PPP;

            SetAQTabLRC();
        }
    }
}

extern void SetTMPosTPDU( void )
{
    /* MSD
    static ENTRY_STR YNEntry;
    UBYTE keyval;   
    Bool Enabled = False;


    Enabled = SUPER_VARIABLES_TAB.dontSendPosTPDU ? False : True;

    if ( Enabled )
    {
        // is now enable
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PosTPDU_Ena;
        YNEntry.Line2Mid = DisablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES  
        {
            SUPER_VARIABLES_TAB.dontSendPosTPDU = True;
            SetSuperTabLRC();
        }
    }
    else
    {
        // is now disable
        YNEntry.TitleMid = N_PPPEnable;
        YNEntry.Line1Mid = PosTPDU_Dis;
        YNEntry.Line2Mid = EnablePP;

        keyval = YesNoEntry( &YNEntry );
        if ( ENTER_KY == keyval )   // If YES 
        {
            SUPER_VARIABLES_TAB.dontSendPosTPDU = False;
            SetSuperTabLRC();
        }
    }
    */
}



//-----------------------------------------------------------------------------
//! \brief
//!     Finds the IP Connect Entry given the Id
//!
//! \param
//!		[in]id				IP Connect Number
//!
//! \return
//!     Pointer to IP Connect Entry or NULL if not found
//-----------------------------------------------------------------------------
static struct ipcontab *FindIPCById( UBYTE id )
{
    UBYTE i;

    for ( i = 0; i < IPCMAX; i++ )
    {
        if ( IPCTAB[i].id == id )
        {
            return &IPCTAB[i];
        }
    }
    return NULL;
}                 

static Bool EnabledScreen(Bool Enabled, enum msg_id TitleMid,enum msg_id Line1Mid_Ena, enum msg_id Line1Mid_Dis )
{
    ENTRY_STR YNEntry;
    UBYTE keyval; 

    if ( Enabled )
    {
        YNEntry.TitleMid = TitleMid;
        YNEntry.Line1Mid = Line1Mid_Ena;
        YNEntry.Line2Mid = DisablePP;

        if ( ENTER_KY == YesNoEntry( &YNEntry ) )   // If YES  
        {
            return True;
        }
    }
    else
    {
        YNEntry.TitleMid = TitleMid;
        YNEntry.Line1Mid = Line1Mid_Dis;
        YNEntry.Line2Mid = EnablePP;

        if ( ENTER_KY == YesNoEntry( &YNEntry ) )   // If YES  
        {
            return True;
        }
    }
    return False;
}

static struct ipcontab *SetIPTab( void )
{
    struct ipcontab *pIPC;

    if ( SetTMParameterVariables & SELECT_TRX_HOST_INFO )
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            pIPC = FindIPCById( AQTAB[0].AQ_TRANS_P_IPTAB );
        else
            pIPC = FindIPCById(AQTAB[0].AQ_TRANS_S_IPTAB);
    }
    else
    {
        if ( SetTMParameterVariables & SELECT_PRIMARY_INFO )
            pIPC = FindIPCById( AQTAB[0].AQ_SETTLE_P_IPTAB );
        else
            pIPC = FindIPCById(AQTAB[0].AQ_SETTLE_S_IPTAB);
    }
    return pIPC;
}

static void DisplayFormat( UBYTE *display , int displayLen)
{
    int i;

    // Put Nul character at Terminator position if exists 
    for ( i = 0; i < displayLen; i++ )
    {
        // Check if Terminator 
        if ( display[i] == 'F' )
        {
            // Yes; replace with Nul character 
            display[i] = '\0';

            // Break out of the loop 
            break;
        }
    }

    // Move NMS Telephone Number to global display buffer 
    memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
    MoveTillZero( CSTMSG, display, sizeof( CSTMSG ) );
    GlobalDsp = CLRKEYON;
}

static Bool SaveFormat(UBYTE *tmpBuffer, UBYTE enteredLen, UBYTE tmpBufferlen)
{
    int i;
    // Check if an entry was input 
    if ( enteredLen )
    {
        // Yes; fill out remainder with 'F's 
        memset( ( UBYTE * ) ( tmpBuffer + enteredLen ),
                'F',( UWORD ) ( tmpBufferlen - enteredLen ) );


        // Test for legal ASCII values.  Repeat entry if not 0-9, A-F. 
        for ( i = 0; i < enteredLen; i++ )
        {
            // Only allow values 0-9 and A-F
            if ( !( ( tmpBuffer[i] >= '0' ) &&
                    ( tmpBuffer[i] <= '9' ) ) )
            {
                // Not 0-9, check for A-F
                if ( !
                     ( ( tmpBuffer[i] >= 'A' ) &&
                       ( tmpBuffer[i] <= 'F' ) ) )
                {
                    // Illegal character was input.  Error msg and reprompt.
                    ShowErrMsg( N_Only09AFvalid );
                    return False;
                }
            }
        }
    }

    return True;
}




