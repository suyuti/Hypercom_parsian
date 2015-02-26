#include "basictyp.h"
#include "fixdata.h"
#include "struct.h"
#include "sdktime.h"

//-----------------------------------------------------------------------------
//  CheckThresLevel     Compare GSM level with Term Master Table and start Timer if less
//
//  Parameters:         cGSMSignalLevel - level of GSM signal
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern Bool COM_GSMLowSignalLevel;
static UBYTE timerID=0;

extern void CheckThresLevel(UBYTE cGSMSignalLevel)
{
	UBYTE cPercLevel;
	// Check for BialBackup comm module connected
	if (TM3_DIALBACKUP != (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )
	{
		COM_GSMLowSignalLevel = False;
		return;
	}
	// Check for settings currently set up
	if ((0 == TCONF.TCTHRESLEVEL) || (0 == TCONF.TCTHRESTIME))
	{
		COM_GSMLowSignalLevel = False;
		return;
	}
	// Calculate Signal level in percent 31 - 100 % , 0 - 0 %
	cPercLevel = cGSMSignalLevel  * (100 / 31);
	// Check for incorrect signal level (usually 99 when no signal)
	if (cGSMSignalLevel > 31 )
		cPercLevel = 0;
	// Compare current level with TM
	if (cPercLevel < (TCONF.TCTHRESLEVEL))
	{
		// if flag already set clear timer ID
		if (COM_GSMLowSignalLevel == True)
		{	
			timerID = 0;
			return;
		}
		// StartTimer when timer finished set up COM_GSMLowSignalLevel flag
		if (timerID == 0 )
			timerID = SDK_WaitAndFlag((ONESECOND * TCONF.TCTHRESTIME),&COM_GSMLowSignalLevel);
	}
	else
	{
		// Clear flag if Signal great then data in TM
		COM_GSMLowSignalLevel = False;
		if (timerID != 0)
		{
			// Stop timer
			SDK_CancelTimer(timerID);
			timerID = 0;
		}
	}	
}
