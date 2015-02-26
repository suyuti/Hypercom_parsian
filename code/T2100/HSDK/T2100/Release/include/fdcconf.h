/*******************************************************************************
*                   ----------------------------------------
*                    Copyright TEVERO AS, Norway, 2000-2005
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
* File:     fdcconf.h
* Version:  1.3.2
* Descr.:   Flash Disk Controller Configuration include file used in order to
*           customize FDC target/user specific definitions. This file is
*           typically modified per target in order to make FDC work in the
*           desired environment.
*
* Author:   Tevero AS
* Date:     2000-04-12
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef FDCCONF_H
#define FDCCONF_H

/*******************************************************************************
                             TARGET INCLUDE FILES AND EXTERNALS
 *******************************************************************************
 Add include files and external functions here if it is needed due to customized
 defines in this file or for possible other reasons. These include files will be
 included in all FDC source code when compiling.
*******************************************************************************/

void falFdcFatal(int id);



/*******************************************************************************
                             DEBUGGING
 ******************************************************************************/

/*******************************************************************************
 ----------------
 FDC_DEBUG_LEVEL:
 ----------------
 Define whether to include code for more extensive debugging for development
 purposes. Set this to 0 for normal usage, in order to avoid inclusion of
 debug code. Inclusion of debug code may affect the code size and performance,
 since it adds printout and internal SW checks that should be avoided for
 normal usage. (The amount of internal checks may change with new releases.)
 
 0 - Do not include debug
 1 - Basic debugging included, monitoring for internal programming faults
 2 - Includes level 1 and some major progress printout
 3 - Includes level 2 and detailed progress printout

 Notice that FDC_DIAG must be enabled if FDC_DEBUG_LEVEL is enabled.
 All debug printouts are done via fdcPrintf(), which must be defined below
 under the C glue functions.
 ******************************************************************************/

#define FDC_DEBUG_LEVEL               0       /* [0, 1, 2, 3] */

/*******************************************************************************
 ---------
 FDC_DIAG:
 ---------
 Define whether to include code to enable runtime control of FDC diagnostics.
 If set to 1, then diagnostics can be enabled/disabled runtime by the
 application. If set to 0, diagnostics code is not included, reducing the
 total codesize somewhat. Inclusion of diagnostics code hardly affects
 performance if not enabled runtime, since mainly only error situations
 are reported.

 0 - Don't include diagnostics code (thus diagnostics can not be enabled)
 1 - Include diagnostics code
 
 Notice that FDC_DIAG must be enabled if FDC_DEBUG_LEVEL is enabled.
 All diagnostics printouts are done via fdcPrintf(), which must be defined below
 under the C glue functions.
*******************************************************************************/

#define FDC_DIAG                      0       /* [0, 1] */



/*******************************************************************************
                             FATAL HANDLING
 *******************************************************************************
 In some cases, FDC can detect inconsistencies in internal memory structures.
 In theory, this is only possible by for instance memory corruption (by
 external sources), but deficiencies in the FDC design could of course trigger 
 such faults.

 Under such circumstances, it may be wise to restart the system or possibly
 close and reopen the partition. Write operations on an inconsistent FDC
 partition could corrupt the partition. The FDC_FATAL(id) macro should either be
 defined to nothing (in this case FDC will continue execution with error codes
 to the user) or it should be defined to a function with the following prototype:

 void fdcFatal(int id)

 where id is the partition ID of the partition being used. In this case, the
 prototype should also be defined above along with the include files.
*******************************************************************************/

#define FDC_FATAL(id)                 falFdcFatal(id)



/*******************************************************************************
 ----------------------------------
 FDC_200NM_230NM_MIRRORBIT_SUPPORT:
 ----------------------------------
 
 Applies only to Spansion/AMD devices: Am29LV-M, SG29GL-M, S29GL-A
 
 Ignore (set to 0) if not using any of those devices. If required to support
 above devices AND other types of devices, set to 1 as very little code
 is added to footprint. (Or make it a build dependent define.)
 
 Spansion (former AMD) announced late 2004 (a year and a half after shipping
 devices!) a change in specification with respect to bit field programming for
 200 and 230nm MirrorBit process technology based devices. This covers the
 above mentioned device families only (at this time of writing). Spansion
 has announced that new 110nm (eg SG29GL-N) process based devices will not
 have this limitation.
 
 Bit field programming is the act of programming bytes multiple times without
 erasing, as long as no bits are changed from 0 to 1. In the case of these
 devices, it is also the act of only programming one or a few bytes at a time
 within small memory ranges (32 byte aligned areas).
 
 FDC relies on this feature, and due to the limitations announced by Spansion,
 a workaround for use with these devices has been added. The workaround requires
 a different flash partition layout.
 
 Please refer to Spansion appl. note for details on limitations:
 
 "Bit Field Programming for Flash Products Based on 230 nm and 200 nm MirrorBit
 Technology", Rev. A, Amendment 0, Oct 26 2004.
 
 NOTICE:
 If using any of the above devices, it is strongly recommended to investigate
 the driver to ensure that unneeded repeated programming of write buffers does
 not occur, as well as to ensure that the driver does NOT use the single byte/word
 write command feature (which is slow anyway). Even reprogramming a byte to
 same value without any bit change will contribute to this effect. So, when
 programming using the write buffer mechanism, it may be an idea to check if
 programming is actually required. One should also notice the requirement of
 a 4 usec poll delay limitation after issuing write, in case polling is used
 by the driver.
 
 0 - Don't include support for special layout change for above mentioned devices.
     FDC can be used for above devices, but it will use a disk layout format that
     is potentially prone to a small unknown chance of bit errors during readback
     due to device limitation and physical disturbance on complementary bits
     during repeated write. Other devices do not have this limitation, and
     we stick to original layout.
     
 1 - Include support for using disk partitions with special workaround layout
     for above mentioned devices. FDC will continue using the layout format
     of a formatted partition. When formatting partitons, the original layout
     will be used. Use the fdcForceMirrorBitWorkaroundLayoutSet(1) function
     prior to formatting in order to use special workaround layout.
     Remember to use fdcForceMirrorBitWorkaroundLayoutSet(0) after format to disable
     feature if formatting other devices not prone to this limitation.
     
     The fdcIsMirrorBitWorkaroundLayout() function can be used to query if an
     opened partition is of the special layout or not (in order to possibly
     trigger reformat if not special layout).
 
*******************************************************************************/

#define FDC_200NM_230NM_MIRRORBIT_SUPPORT     0       /* [0, 1] */



/*******************************************************************************
                             C GLUE FUNCTIONS
 *******************************************************************************
 In some rare cases, some of the standard C functions shall be overrided for
 the FDC SW only. The defines below can be used to achieve this. If such
 overriding for FDC is not needed, then leave the below defines unchanged.

 The below functions are defined to C library functions, appropriate
 standard C include files may need to be added to the top of this file.
 This depends on the environment.

 All the below glue functions MUST be reentrant if running in a preemptive
 environment!
*******************************************************************************/

/* Prototype: void free(void *memblock); */
#define fdcFree                       shrdFree
/* Prototype: void *malloc(size_t size); */
/* Requirement: Allocated memory must start at a 32 bit aligned address */
#define fdcMalloc                     shrdMalloc
/* Prototype: int printf(const char *format [, argument]...); */
#define fdcPrintf(x)                  shrdPrintf(x)



#endif /* FDCCONF_H */
