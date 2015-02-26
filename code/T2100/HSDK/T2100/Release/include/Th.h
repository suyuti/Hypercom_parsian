
///////////////////////////////////////////////////////////////////////////
//
// Module   : TH
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module TH
//
//         Copyright (C) 2003 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 020712  BW      Original version (based on EFTOS7 version 020114)
// 021004  BW      Adapted for HFT824 with new timer (LTIM_MOTOR)
// 030409  BW      Adapted for ARM compiler
// 030611  BW      Added dedicated timers for all channels in CH module
//                 Changed to only handle timers for RS0 and RS1.
//                 Network level and TH Disable variables removed.
// 031008  BW      Added dedicated short timers for CH module.
// 031106  JZ      STIM_APPL added as short application timer.
// 040923  RZ      Implemented dynamic timers support.
// 050112  RZ      Removed ADC staff.
// 050214  RZ      Changed DSP Refresh period
// 050427  RZ      Added additional timer state
//
///////////////////////////////////////////////////////////////////////////

#ifndef TH_H
#define TH_H

// Define overall number of timers
#define TH_MAX_TIMER	64

// Number of background tasks (processes)
#define NO_BKGND_TASK   8



// Define timer interval using different type of timer
#define TH_1MS_TIMER	1
#define TH_5MS_TIMER	5
#define TH_10MS_TIMER	10
#define TH_20MS_TIMER	20
#define TH_100MS_TIMER	100
#define TH_1S_TIMER		1000

// Define the periodical process timeout (in 10ms units)
#define TH_SEC_SCAN		5		// SEC scan timeout (each 5ms)
#define TH_DSP_REFRESH	5		// Display refresh timeout
#define TH_BLT_SCAN		1		// BackLight switch timeout
#define TH_KBD_EXEC		5		// Execute keyboard process
#define TH_BUZ_DEC		10		// Buzzer counter decrement
#define TH_CUR_BLINK	50		// Cursor blink time


// Define timer control flags
#define TH_TIMER_USED		0x00000001	// Timer in use
#define TH_TIMER_RESTART	0x00000002	// Timer auto restart flag

// Define timer levels
#define TH_CORE_LEVEL		0x00000100	// 1ms units
#define TH_SHELL_LEVEL		0x00000200	// 5ms units
#define TH_SYSTEM_LEVEL		0x00000400	// 10ms units
#define TH_SYSAPPL_LEVEL	0x00000800	// 5ms units
#define TH_APPL_LEVEL		0x00001000	// 100ms units
#define TH_LEVEL_MASK		0x0000FF00	// Timer level mask

// Define timer first start delay
#define TH_DELAY_SHIFT		16
#define TH_DELAY_MASK		0x00FF0000	// Timer delay mask

// Define timer queue actions
#define TH_ADD_TIMER	1
#define TH_DEL_TIMER	0

// Define timer states
typedef enum
{
	TH_CMD_NONE,
	TH_CMD_START,
	TH_CMD_ACTIVE,
	TH_CMD_ELAPSE,
	TH_CMD_DONE,
	TH_CMD_ERASE,
	TH_CMD_STOP,
	TH_CMD_STOPPED
} TH_TIMER_CMD;

// Timer Data Struct
typedef struct
{
	DWORD	nCmd;	// Timer state
	DWORD	nCnt;	// Timer counter
	DWORD	nICnt;	// Initial timer counter
	PROCPTR	pFunc;	// Pointer to function
	DWORD   nCtrl;	// Timer control flags
	BOOL    bEvt;	// Timer flag
	void*	pNext;	// Pointer to next timer
} TIMER_S;

// Type of process state of background jobs
typedef enum {STATE_IDLE,STATE_WAITING,STATE_EXECUTING} PROC_STATE;


// Array of Timers
extern TIMER_S volatile TH_Timer[TH_MAX_TIMER];
// Pointer to first timer
extern TIMER_S* pTH_TIMER;


// Define timer interval using different type of timer (OLD)
#define STIM_5MS     1
#define STIM_100MS   20
#define STIM_1S      200
#define LTIM_100MS   1
#define LTIM_1S      10
#define TIMER_100MS  1
#define TIMER_1S     10
#define TIMER_2S     20

// Define Short Timers
#define FIRST_STIM  0
#define STIM_CH_0   (FIRST_STIM+0)
#define STIM_CH_1   (FIRST_STIM+1)
#define STIM_CH_2   (FIRST_STIM+2)
#define STIM_RS_0   (FIRST_STIM+3)
#define STIM_RS_1   (FIRST_STIM+4)
#define STIM_RS_2   (FIRST_STIM+5)
#define STIM_SPB    (FIRST_STIM+6)
#define LAST_STIM   STIM_SPB
#define NO_STIM     7

// Define Long Timers
#define FIRST_LTIM  LAST_STIM+1
#define LTIM_RS_0   (FIRST_LTIM+0)
#define LTIM_RS_1   (FIRST_LTIM+1)
#define LTIM_RS_2   (FIRST_LTIM+2)
#define LTIM_CH_0   (FIRST_LTIM+3)
#define LTIM_CH_1   (FIRST_LTIM+4)
#define LTIM_CH_2   (FIRST_LTIM+5)
#define LAST_LTIM   LTIM_CH_2
#define NO_LTIM     6


// Define application timers
#define FIRST_APP_TIMER     LAST_LTIM+1
#define STIM_APPL   (FIRST_APP_TIMER+0)
#define TIMER1      (FIRST_APP_TIMER+1)
#define TIMER2      (FIRST_APP_TIMER+2)
#define TIMER3      (FIRST_APP_TIMER+3)
#define TIMER4      (FIRST_APP_TIMER+4)
#define TIMER5      (FIRST_APP_TIMER+5)
#define TIMER6      (FIRST_APP_TIMER+6)
#define TIMER7      (FIRST_APP_TIMER+7)
#define TIMER8      (FIRST_APP_TIMER+8)
#define TIMER9      (FIRST_APP_TIMER+9)
#define TIMER10     (FIRST_APP_TIMER+10)
#define TIMER11     (FIRST_APP_TIMER+11)
#define TIMER12     (FIRST_APP_TIMER+12)
#define TIMER13     (FIRST_APP_TIMER+13)
#define TIMER14     (FIRST_APP_TIMER+14)
#define TIMER15     (FIRST_APP_TIMER+15)
#define TIMER16     (FIRST_APP_TIMER+16)
#define LAST_APP_TIMER      TIMER16
#define NO_TIMER            17
#define VERY_FIRST_TIMER    FIRST_STIM
#define VERY_LAST_TIMER     LAST_APP_TIMER

// Define the number of old predefined timers
#define TH_OLD_TIMER	(NO_STIM+NO_LTIM+NO_TIMER)





// Global Function Prototypes
void  TH_Init(void);
BOOL  TH_IsEnable(void);
BOOL  TH_IsTimerElapsed(const DWORD nIndex);
BOOL  TH_IsTimerEvt(const DWORD nIndex);
DWORD TH_Ret1ms(void);
DWORD TH_Ret100ms(void);
DWORD TH_Ret500ms(void);
void  TH_StartTimer(const DWORD nIndex, const DWORD nTime, const PROCPTR pCallBack);
void  TH_StopTimer(const DWORD nIndex); 
DWORD TH_GetTimer(DWORD nCtrl);
void  TH_FreeTimer(const DWORD nIndex);
DWORD TH_StartBkgndTask(DWORD nCnt, PROCPTR pFunc);
void  TH_StopBkgndTask(const DWORD nID);
void  TH_StateBkgndTask(const DWORD nID, PROC_STATE eState);

#endif
