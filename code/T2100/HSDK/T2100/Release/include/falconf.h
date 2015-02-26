/*******************************************************************************
*                   ----------------------------------------
*                    Copyright TEVERO AS, Norway, 2001-2003
*                                www.tevero.no
*                   ----------------------------------------
* This source code is the property of Tevero AS, Norway. The source/compiled
* code and derivatives may be used and distributed freely as long as this
* copyright notice is not removed.
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
* File:     falconf.h
* Version:  1.7.1
* Descr.:   File Abstraction Layer configuration for TFFS file abstraction
*           This file is used to configure FAL for a specific target.
*
* Author:   Tevero AS
* Date:     2001-01-12
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef FALCONF_H
#define FALCONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
                             TARGET INCLUDE FILES AND EXTERNALS
 *******************************************************************************
 Add include files and external functions here if it is needed due to customized
 defines in this file or for possible other reasons. These include files will be
 included in all FAL source code when compiling. (Notice that <stdio.h> is
 always included by fal.h, so it is not required here.)
*******************************************************************************/

void *falTestGetStatePtr(void);
void falTestSetStatePtr(void *);


/*******************************************************************************
                             GLUE FUNCTIONS
 *******************************************************************************
 Glue functions that must be made available.
 
 All the below glue functions MUST be reentrant if running in a preemptive
 environment!
*******************************************************************************/

/* In many OSes it is possible to store some user variables or pointer to */
/* some user specific area related to a process/thread. If there is no */
/* normal OS running, it may be sufficient just to store the pointer in */
/* a variable. The main thing is that the pointer points to an area holding */
/* information on disk location and other issues. It should only be used by */
/* one user, i.e. changing the directory will change the information pointed */
/* to by the pointer. */

/* Prototype: void *falGetStatePtr(); */
#define falGetStatePtr                falTestGetStatePtr

/* Prototype: void falSetStatePtr(void *info); */
#define falSetStatePtr                falTestSetStatePtr


/*******************************************************************************
                             C GLUE FUNCTIONS
 *******************************************************************************
 In some rare cases, some of the standard C functions shall be overrided for
 the FAL SW only. The defines below can be used to achieve this. If such
 overriding for FAL is not needed, then leave the below defines unchanged.

 The below functions are defined to C library functions, appropriate
 standard C include files may need to be added to the top of this file.
 This depends on the environment.

 All the below glue functions MUST be reentrant if running in a preemptive
 environment!
********************************************************************************/

/* Prototype: void free(void *memblock); */
#define falFree                       shrdFree

/* Prototype: void *malloc(size_t size); */
#define falMalloc                     shrdMalloc

#ifdef __cplusplus
}
#endif

#endif /* FALCONF_H */
