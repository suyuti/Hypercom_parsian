/*******************************************************************************
*                   ----------------------------------------
*                    Copyright TEVERO AS, Norway, 2000-2003
*                                www.tevero.no
*                   ----------------------------------------
* This source code is the property of Tevero AS, Norway. The source/compiled
* code may not be used or modified without a written consent from Tevero AS.
* The consent shall specify under which conditions the code may be used.
*
* This copyright notice may not be removed from the source code nor changed.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Tevero AS has no obligation to
* support this Software. Tevero AS is providing the Software "AS IS", with no
* express or implied warranties of any kind, including, but not limited to, any
* implied warranties of merchantability or fitness for any particular purpose
* or warranties against infringement of any proprietary rights of a third party.
* Tevero AS will not be liable for any consequential, incidental, or special
* damages, or any other relief, or for any claim by any third party, arising
* from your use of this Software.
*
*******************************************************************************/

/*******************************************************************************
********************************************************************************
********************************************************************************
*
* File:     tffsconf.h
* Version:  2.6.2
* Descr.:   Transaction Flash File System Configuration include file used in
*           order to customize TFFS target/user specific definitions. This file
*           is typically modified per target in order to make TFFS work in the
*           desired environment.
*
* Author:   Tevero AS
* Date:     2000-05-24
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef TFFSCONF_H
#define TFFSCONF_H

/*******************************************************************************
                             TARGET INCLUDE FILES
 *******************************************************************************
 Add include files and external functions here if it is needed due to customized
 defines in this file or for possible other reasons. These include files will be
 included in all TFFS source code when compiling.
*******************************************************************************/

void falTffsFatal(int id);



/*******************************************************************************
                             DEBUGGING
 ******************************************************************************/

/*******************************************************************************
 -----------------
 TFFS_DEBUG_LEVEL:
 -----------------
 Define whether to include code for more extensive debugging for development
 purposes. Set this to 0 for normal usage, in order to avoid inclusion of
 debug code. Inclusion of debug code may affect the code size and performance,
 since it adds printout and internal SW checks that should be avoided for
 normal usage. (The amount of internal checks may change with new releases.)
 
 0 - Do not include debug
 1 - Basic debugging included, monitoring for internal programming faults
 2 - Includes level 1 and some major progress printout
 3 - Includes level 2 and detailed progress printout

 Notice that TFFS_DIAG must be enabled if TFFS_DEBUG_LEVEL is enabled.
 All debug printouts are done via tffsPrintf(), which must be defined below
 under the C glue functions.
 ******************************************************************************/

#define TFFS_DEBUG_LEVEL              0       /* [0, 1, 2, 3] */

/*******************************************************************************
 ----------
 TFFS_DIAG:
 ----------
 Define whether to include code to enable runtime control of TFFS diagnostics.
 If set to 1, then diagnostics can be enabled/disabled runtime by the
 application. If set to 0, diagnostics code is not included, reducing the
 total codesize somewhat. Inclusion of diagnostics code hardly affects
 performance if not enabled runtime, since mainly only error situations
 are reported.

 0 - Don't include diagnostics code (thus diagnostics can not be enabled)
 1 - Include diagnostics code
 
 Notice that TFFS_DIAG must be enabled if TFFS_DEBUG_LEVEL is enabled.
 All diagnostics printouts are done via tffsPrintf(), which must be defined below
 under the C glue functions.
*******************************************************************************/

#define TFFS_DIAG                     0       /* [0, 1] */



/*******************************************************************************
                             FATAL HANDLING
 *******************************************************************************
 In some cases, TFFS can detect inconsistencies in internal memory structures.
 In theory, this is only possible by for instance memory corruption (by
 external sources), but deficiencies in the TFFS design could of course trigger 
 such faults.

 Under such circumstances, it may be wise to restart the system or possibly
 unmount and remount the disk. Write operations on an inconsistent TFFS
 disk could corrupt the disk. The TFFS_FATAL(id) macro should either be
 defined to nothing (in this case TFFS will continue execution with error codes
 to the user) or it should be defined to a function with the following prototype:

 void tffsFatal(int id)

 where id is the disk ID of the disk being used. In this case, the prototype
 should also be defined above along with the include files.
*******************************************************************************/

#define TFFS_FATAL(id)                falTffsFatal(id)



/*******************************************************************************
                             RESIZING PARTITIONS
 *******************************************************************************
 Resizing partitions was implemented for very special FDC needs, and is
 rarely used since it requires that the resizing is planned at format time.

 However, since the resize functionality is part of the disk controller API,
 the FDC code to handle this is normally forced included with target code
 due to referring to it when defining the access API. Set the below define
 to 0 in order to avoid including FDC code to handle resizing. If you do intend
 to use resizing flash disks (using tffsResize()), set it to 1.

 Notice that resizing RAM disks is not supported anyway.

 This define is only used to reduce footprint.
*******************************************************************************/

#define TFFS_RESIZE                   0       /* [0, 1] */



/*******************************************************************************
                          MONITORING FILE SYSTEM ACTIONS
 *******************************************************************************
 In some situations, there is a need to monitor file system actions. For
 instance a special action shall maybe be triggered when a user stores
 a file using FTP. For FTP, it is just a normal store operation. The file
 system can then be configured to support monitoring by using callbacks.

 If TFFS_MONITOR is 1, monitoring functionality is included. If TFFS_MONITOR
 is 0, then monitoring support is not included in order to reduce the footprint.

 When monitoring is enabled, an additional protection is needed when accessing
 the monitor entry list. The protection scheme API is similar to what is used
 for a TFFS disk, except no input/output parameter is used. This means that
 reading the list can be done by multiple users, but modifying the list (adding
 or removing a monitor entry) requires exclusive write access. If used in
 a non-preemptive environment, then simply make the defines empty. Also, if
 monitor entries are only created during system startup, and never removed,
 the protection defines may be left empty.

 Please refer to monitor API to see what kind of file system actions that can
 be monitored.
*******************************************************************************/

#define TFFS_MONITOR                  0       /* [0, 1] */

#if (TFFS_MONITOR)
/* If defining these functions, prototypes for the functions should also */
/* be defined at the top of this file. All functions of type 'void x(void)' */
#define tffsMonitorWaiSemRead()
#define tffsMonitorSigSemRead()
#define tffsMonitorWaiSemWrite()
#define tffsMonitorSigSemWrite()
#endif



/*******************************************************************************
                            PROTECTING FILE ACCESS
 *******************************************************************************
 The file system by default has no protection on multiple access on the same
 file. Multiple read of an file is not a problem, but simultaneously read and
 write access by two different threads may cause problems. Also, deleting a
 file that some other thread has opened will cause problems.

 An example is in a system with a WEB server. For instance if a page is
 showing a picture that is updated regularly by some thread, then this
 will cause problems if the browser refreshes its page for instance every 5
 seconds. Every now and then, the thread will rewrite a new picture while
 the WEB server is reading the picture file. In effect, the file is cleared
 while the WEB server is in the middle of reading.

 In this case, file access protection must be provided somehow by the file
 system.

 If TFFS_PROTECT is 1, protection functionality is included. This means that
 a file can only be opened once in exclusive write mode. A file opened can
 not be deleted. If TFFS_PROTECT is 0, then protection support is not included
 in order to reduce the footprint.

 In order to support protection, each stream is added to a double linked list.
 This requires two additional pointers for each stream.

 When protection is enabled, an additional protection is needed when accessing
 the opened stream list. The protection scheme API is similar to what is used
 for a TFFS disk, except no input/output parameter is used. This means that
 reading the list can be done by multiple users, but modifying the list (adding
 or removing a stream entry) requires exclusive write access. If used in
 a non-preemptive environment, then simply make the defines empty.

 Please refer to protection API to see what kind of file system protection
 can be defined.

 Notice that a user with SUPER access rights is not stopped by the protection
 mechanisms!
*******************************************************************************/

#define TFFS_PROTECT                  1       /* [0, 1] */

#if (TFFS_PROTECT)
/* If defining these functions, prototypes for the functions should also */
/* be defined at the top of this file. All functions of type 'void x(void)' */
void tffsProtectWaiSemRead(void);
void tffsProtectSigSemRead(void);
void tffsProtectWaiSemWrite(void);
void tffsProtectSigSemWrite(void);
#endif


/*******************************************************************************
                             C GLUE FUNCTIONS
 *******************************************************************************
 In some rare cases, some of the standard C functions shall be overrided for
 the TFFS SW only. The defines below can be used to achieve this. If such
 overriding for TFFS is not needed, then leave the below defines unchanged.

 The below functions are defined to C library functions, appropriate
 standard C include files may need to be added to the top of this file.
 This depends on the environment.

 All the below glue functions MUST be reentrant if running in a preemptive
 environment!
*******************************************************************************/

/* Prototype: void free(void *memblock); */
#define tffsFree                      shrdFree

/* Prototype: void *malloc(size_t size); */
#define tffsMalloc                    shrdMalloc

/* Prototype: int printf(const char *format [, argument]...); */
#define tffsPrintf(x)                 shrdPrintf(x)



#endif /* TFFSCONF_H */
