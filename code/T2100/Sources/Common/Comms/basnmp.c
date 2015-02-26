
#include <net-snmp/net-snmp-config.h>



#if HAVE_IO_H
#include <io.h>
#endif
#include <stdio.h>
#include <errno.h>
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#elif HAVE_WINSOCK_H
#include <winsock.h>
#endif
#if HAVE_NET_IF_H
#include <net/if.h>
#endif
#if HAVE_INET_MIB2_H
#include <inet/mib2.h>
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#if HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include <signal.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#if HAVE_PROCESS_H              /* Win32-getpid */
#include <process.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_PWD_H
#include <pwd.h>
#endif
#if HAVE_GRP_H
#include <grp.h>
#endif
#if defined(MAKE_VIKING) //asa
#include <eftos.h>
#include <hyctypes.h>
#include <sockets.h>
#endif //asa

#ifndef PATH_MAX
# ifdef _POSIX_PATH_MAX
#  define PATH_MAX _POSIX_PATH_MAX
# else
#  define PATH_MAX 255
# endif
#endif

#ifndef FD_SET
typedef long    fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      memset((p), 0, sizeof(*(p)))
#endif


#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <net-snmp/library/fd_event_manager.h>

#include "m2m.h"
#include <net-snmp/agent/mib_module_config.h>

#include "snmpd.h"
#include "mibgroup/struct.h"
#include <net-snmp/agent/mib_modules.h>

#include "mibgroup/util_funcs.h"

#include <net-snmp/agent/agent_trap.h>

#include <net-snmp/agent/table.h>
#include <net-snmp/agent/table_iterator.h>
#include "mib_module_includes.h"

#include "basictyp.h"
#include "basnmp.h"
#include "struct.h"
#include "comdata.h"
#include "fixdata.h"
#include "mibgroup/mibII/vacm_conf.h"
/*
 * Globals.
 */
#ifdef USE_LIBWRAP
#include <tcpd.h>
#endif                          /* USE_LIBWRAP */

#define TIMETICK         500000L

#ifndef LOG_DAEMON
#define LOG_DAEMON (3<<3)
#endif

int             snmp_dump_packet;
int             reconfig = 0;
int             Facility = LOG_DAEMON;

const char     *app_name = "snmpd";

extern int      netsnmp_running;

#define NUM_SOCKETS	32

/*
 * Prototypes.
 */
int             snmp_read_packet(int);
int             snmp_input(int, netsnmp_session *, int, netsnmp_pdu *,
                           void *);
static int      StartListenPort(void);

/*
 * These definitions handle 4.2 systems without additional syslog facilities.
 */
#ifndef LOG_CONS
#define LOG_CONS	0       /* Don't bother if not defined... */
#endif
#ifndef LOG_PID
#define LOG_PID		0       /* Don't bother if not defined... */
#endif
#ifndef LOG_LOCAL0
#define LOG_LOCAL0	0
#endif
#ifndef LOG_LOCAL1
#define LOG_LOCAL1	0
#endif
#ifndef LOG_LOCAL2
#define LOG_LOCAL2	0
#endif
#ifndef LOG_LOCAL3
#define LOG_LOCAL3	0
#endif
#ifndef LOG_LOCAL4
#define LOG_LOCAL4	0
#endif
#ifndef LOG_LOCAL5
#define LOG_LOCAL5	0
#endif
#ifndef LOG_LOCAL6
#define LOG_LOCAL6	0
#endif
#ifndef LOG_LOCAL7
#define LOG_LOCAL7	0
#endif
#ifndef LOG_DAEMON
#define LOG_DAEMON	0
#endif
void SetUDPConnection(void);
void SendTraps(void);

Bool bSNMPAgentStart = False;
Bool bSNMPAgentListen = False;
Bool send_traps = True;

char * trapDest1 = NULL;
char * trapDest2 = NULL;
char * trapPort1 = NULL;
char * trapPort2 = NULL;
static oid      sysPtidNum_oid[] = { 1, 3, 6, 1, 4, 1, 626, 123, 1, 1, 6 };
static oid      sysPtinetAddr_oid[] = { 1, 3, 6, 1, 2, 1, 4, 20, 1, 1 };


void INITSNMP( void )
{
	const char     *app_name = "snmpd";
    int             ret, quit;
	

 
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, 
			   NETSNMP_DS_AGENT_NO_ROOT_ACCESS, 1);

    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, 
			   NETSNMP_DS_AGENT_AGENTX_MASTER, 0);
    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                       NETSNMP_DS_AGENT_AGENTX_TIMEOUT, -1);
    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                       NETSNMP_DS_AGENT_AGENTX_RETRIES, -1);

    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                       NETSNMP_DS_AGENT_CACHE_TIMEOUT, 5);

    snmp_log_syslogname(app_name);
    netsnmp_ds_set_string(NETSNMP_DS_LIBRARY_ID,
                          NETSNMP_DS_LIB_APPTYPE, app_name);

    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, 
			   NETSNMP_DS_AGENT_ROLE, -1);


    quit = ! netsnmp_ds_get_boolean(NETSNMP_DS_APPLICATION_ID,
                                    NETSNMP_DS_AGENT_QUIT_IMMEDIATELY);
    ret = netsnmp_daemonize(quit, snmp_stderrlog_status());

    if(ret != 0) 
	{
		while (True)
		{
			SDK_RelinqCPU(  );
		}
                     /*  Exit logs exit val for us  */ //ERROR
    }

    init_agent(app_name);        /* do what we need to do first. */
    init_mib_modules();

    init_snmp(app_name);

    system_parse_config_sysServices("sysServices", "79");
    system_parse_config_sysObjectID("sysObjectID", "1.3.6.1.4.1.626");
	vacm_parse_view("view", "all included .1");
	config_community();

	
	while (True)
	{
		// Wait for enabled flag to Start SNMP agent
		while ( bSNMPAgentStart != True)
		{
			SDK_RelinqCPU(  );
		}

		if ( init_master_agent() != 0) 
		{
			/* Some error opening one of the specified agent transports. */
			// Stop Agent forever
			while (True)
			{
				SDK_RelinqCPU(  );
			}
		}
		
		snmp_store(app_name);
		
		// if Traps Are not Send yet send it 
		if ( send_traps == True)
		{
			// Call this function just for not lose traps ... It send UDP message to dest adress
			SetUDPConnection();
			// Send Cold boot traps
			SendTraps();
			// Set flag traps was send
			send_traps = False;
		}
		// Wait for enabled flag to Start SNMP agent
		while ( bSNMPAgentListen != True)
		{
			SDK_RelinqCPU(  );
		}

		// Start Listen port It should exit only if Socket connection was broken
		StartListenPort	();

		bSNMPAgentStart = False;
		/* these attempt to free all known memory, but result in double frees */
		shutdown_master_agent();
	}
}



/*******************************************************************-o-******
 * StartListenPort(void)
 *
 * Parameters:
 *      
 * Returns:
 *	0	On success.
 *	-1	System error.
 *
 * Infinite while-loop which monitors incoming messges for the agent.
 * Invoke the established message handlers for incoming messages on a per
 * port basis.  Handle timeouts.
 */
int StartListenPort(void)
{
    int             numfds;
    fd_set          readfds, writefds, exceptfds;
    struct timeval  timeout, *tvp = &timeout;
    int             count, block, i;


    netsnmp_addrcache_initialise();

    /*
     * ignore early sighup during startup
     */
    reconfig = 0;

    /*
     * Loop-forever: execute message handlers for sockets with data
     */
    while (netsnmp_running) {
        if (reconfig) {
            break;
//            reconfig = 0;
//            snmp_log(LOG_INFO, "Reconfiguring daemon\n");
	    /*  Stop and restart logging.  This allows logfiles to be
		rotated etc.  */
//	    netsnmp_logging_restart();
//	    snmp_log(LOG_INFO, "NET-SNMP version %s restarted\n",
//		     netsnmp_get_version());
//            update_config();
//            send_easy_trap(SNMP_TRAP_ENTERPRISESPECIFIC, 3);
        }

        for (i = 0; i < NUM_EXTERNAL_SIGS; i++) {
            if (external_signal_scheduled[i]) {
                external_signal_scheduled[i]--;
                external_signal_handler[i](i);
            }
        }

        /*
         * default to sleeping for a really long time. INT_MAX
         * should be sufficient (eg we don't care if time_t is
         * a long that's bigger than an int).
         */
        tvp = &timeout;
        tvp->tv_sec = INT_MAX;
        tvp->tv_usec = 0;

        numfds = 0;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        block = 0;
        snmp_select_info(&numfds, &readfds, tvp, &block);
        if (block == 1) {
            tvp = NULL;         /* block without timeout */
	}

        netsnmp_external_event_info(&numfds, &readfds, &writefds, &exceptfds);

    reselect:
        DEBUGMSGTL(("snmpd/select", "select( numfds=%d, ..., tvp=%p)\n",
                    numfds, tvp));
        if(tvp)
            DEBUGMSGTL(("timer", "tvp %d.%d\n", tvp->tv_sec, tvp->tv_usec));
        count = select(numfds, &readfds, &writefds, &exceptfds, tvp);
        DEBUGMSGTL(("snmpd/select", "returned, count = %d\n", count));

        if (count > 0) {
            netsnmp_dispatch_external_events(&count, &readfds,
                                           &writefds, &exceptfds);
            /* If there are still events leftover, process them */
            if (count > 0) {
              snmp_read(&readfds);
            }
        } else
            switch (count) {
            case 0:
                snmp_timeout();
                break;
            case -1:
                DEBUGMSGTL(("snmpd/select", "  errno = %d\n", errno));
                if (errno == EINTR) {
                    /*
                     * likely that we got a signal. Check our special signal
                     * flags before retrying select.
                     */
		    if (netsnmp_running && !reconfig) {
                        goto reselect;
		    }
                    continue;
                } else {
                    snmp_log_perror("select");
                }
                return -1;
            default:
                snmp_log(LOG_ERR, "select returned %d\n", count);
                return -1;
            }                   /* endif -- count>0 */

        /*
         * run requested alarms 
         */
        run_alarms();

        netsnmp_check_outstanding_agent_requests();

    }                           /* endwhile */

    snmp_log(LOG_INFO, "Received TERM or STOP signal...  shutting down...\n");
    return 0;

}                               /* end receive() */



/*******************************************************************-o-******
 * snmp_input
 *
 * Parameters:
 *	 op
 *	*session
 *	 requid
 *	*pdu
 *	*magic
 *      
 * Returns:
 *	1		On success	-OR-
 *	Passes through	Return from alarmGetResponse() when 
 *	  		  USING_V2PARTY_ALARM_MODULE is defined.
 *
 * Call-back function to manage responses to traps (informs) and alarms.
 * Not used by the agent to process other Response PDUs.
 */
int
snmp_input(int op,
           netsnmp_session * session,
           int reqid, netsnmp_pdu *pdu, void *magic)
{
    struct get_req_state *state = (struct get_req_state *) magic;

    if (op == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
        if (pdu->command == SNMP_MSG_GET) {
            if (state->type == EVENT_GET_REQ) {
                /*
                 * this is just the ack to our inform pdu 
                 */
                return 1;
            }
        }
    } else if (op == NETSNMP_CALLBACK_OP_TIMED_OUT) {
        if (state->type == ALARM_GET_REQ) {
            /*
             * Need a mechanism to replace obsolete SNMPv2p alarm 
             */
        }
    }
    return 1;

}                               /* end snmp_input() */

extern void clear_community(void);

void config_community(void)
{
	char			temp_string[256] = {0};

	// Clear SNMP community first (need after init for example)
	netsnmp_udp_com2SecList_free();
//	vacm_free_view();
	vacm_free_access();
	vacm_free_group();

	// Set up new values according with settings

	strncat( temp_string, TCONF.TCCOMMUNITY, S_TCCOMMUNITY);
	snmpd_parse_config_trapcommunity("sysCommunity", temp_string);

	memset( temp_string, 0,sizeof(temp_string));
	strncat( temp_string, TCONF.TCCOMMUNITY, S_TCCOMMUNITY);
	strcat( temp_string, " default ");
	strncat( temp_string, TCONF.TCCOMMUNITY, S_TCCOMMUNITY);
	netsnmp_udp_parse_security("com2sec", temp_string);
	memset( temp_string, 0,sizeof(temp_string));
	strcpy( temp_string, "RR v2c ");
	strncat( temp_string, TCONF.TCCOMMUNITY,S_TCCOMMUNITY);
	vacm_parse_group("group", temp_string );

	memset( temp_string, 0,sizeof(temp_string));
	strcpy( temp_string, "RR \"\" any noauth exact ");

	strcat( temp_string, "all ");
	if (TCONF.TCSNMPOPT & TCSNMP_ACCESS)
	{
		strcat( temp_string, "all none");
	}
	else
	{
		strcat( temp_string, "none none");
	}

	vacm_parse_access("access", temp_string);

}                              /* end snmp_community() */

void SetUDPConnection(void)
{
	struct sockaddr_in destAddr1,destAddr2;
	SOCKET sock_udp = INVALID_SOCKET;
	sock_udp = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( sock_udp  == INVALID_SOCKET )
	{
		return;
	}
	if ( (*trapDest1) != 0 )
	{
		destAddr1.sin_family = AF_INET;
		destAddr1.sin_addr.s_addr = inet_addr( trapDest1 );
		destAddr1.sin_port = htons(7);
		sendto( sock_udp, "ECHO", 4, 0, (struct sockaddr *) &destAddr1, sizeof(struct sockaddr));
	}
	if ( (*trapDest2) != 0 )
	{
		destAddr2.sin_family = AF_INET;
		destAddr2.sin_addr.s_addr = inet_addr( trapDest2 );
		destAddr2.sin_port = htons(7);
		sendto( sock_udp, "ECHO", 4, 0, (struct sockaddr *) &destAddr2, sizeof(struct sockaddr));
	}
	
	closesocket(sock_udp);
	SDK_Wait(ONESECOND * 2);
}

void SendTraps()
{
	netsnmp_variable_list * varlist = NULL;
	char temp_string[256] = {0};
	struct in_addr localip;

	ioctlsocket( 0, SIOMYIP, &localip);

	// Set Second trap destination adress
	if ( (*trapDest2) != 0 )
	{
		strcpy(temp_string,trapDest2);
		if (strchr(temp_string, ' ') !=0)
			*strchr(temp_string, ' ') = 0;
		strcat(temp_string,":");
		strcat(temp_string,trapPort2);

		snmpd_parse_config_trap2sink("trap2Sink", temp_string);
	}

	// Set First trap destination adress
	if ( (*trapDest1) != 0 )
	{
		strcpy(temp_string,trapDest1);
		if (strchr(temp_string, ' ') !=0)
			*strchr(temp_string, ' ') = 0;
		strcat(temp_string,":");
		strcat(temp_string,trapPort1);

		snmpd_parse_config_trap2sink("trap2Sink", temp_string);
	}
	memset (temp_string, 0 , sizeof (temp_string));

	snmp_varlist_add_variable(&varlist,  &sysPtidNum_oid, OID_LENGTH(sysPtidNum_oid), ASN_OCTET_STR, &TERM.TERMSer, 8);
	snmp_varlist_add_variable(&varlist,  &sysPtinetAddr_oid, OID_LENGTH(sysPtinetAddr_oid), ASN_OCTET_STR, inet_ntoa(localip), strlen(inet_ntoa(localip)));

	/* Send coldstart trap if possible. */
	send_trap_vars(SNMP_TRAP_ENTERPRISESPECIFIC, 1, varlist);

}
