//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2006
//=============================================================================
//
// Module overview:             collect.h
//      MIB collecting data functionality
//
//=============================================================================

extern void StatisticCollection ();

extern int IsMIBWriteble();

#define SYS_STRING_LEN	256

struct mib_writable_obj
{
	char sysContact[SYS_STRING_LEN];		// System contac 
	char sysName[SYS_STRING_LEN];		// System Name
	char sysLocation[SYS_STRING_LEN];	// System Location
};

#ifdef MAKE_SNMP

extern unsigned char MIBWRITEOBJFMT;
extern struct mib_writable_obj MIBWRITEOBJ;   
extern unsigned char MIBWRITEOBJLRC;
#endif// MAKE_SNMP
