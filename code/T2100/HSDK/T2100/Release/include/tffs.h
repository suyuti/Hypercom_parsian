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
* File:     tffs.h
* Version:  2.6.2
* Descr.:   Transaction Flash FileSystem include file. This include file
*           contains the global definitions in order to interface to the TFFS
*           system. No other include files are normally required by external SW.
*
* Author:   Tevero AS
* Date:     2000-05-24
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef TFFS_H
#define TFFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Must ensure these files are included, since it may affect contents */
#include "shrdconf.h"
#include "tffsconf.h"

/*******************************************************************************
***********************             DEFINES              ***********************
*******************************************************************************/

/* Error codes from TFFS functions */
#define TFFS_ERROR_USAGE            -1
#define TFFS_ERROR_READ             -2
#define TFFS_ERROR_WRITE            -3
#define TFFS_ERROR_MEMORY           -4
#define TFFS_ERROR_NODISK           -5
#define TFFS_ERROR_DISK_FULL        -6
#define TFFS_ERROR_INCONSISTENCY    -7
#define TFFS_ERROR_NONEXISTING      -8
#define TFFS_ERROR_ACCESS           -9
#define TFFS_ERROR_LAST             (TFFS_ERROR_ACCESS)         

/* Diagnostics modes */
#define TFFS_DIAG_DISABLE           0
#define TFFS_DIAG_ERRORS            1
#define TFFS_DIAG_ALL               2

/* Disk item attribute codes */
#define TFFS_READ                   0x10
#define TFFS_WRITE                  0x20
#define TFFS_HIDDEN                 0x40
#define TFFS_DIR                    0x80

/* Access defines */
#define TFFS_ACCESS_NORMAL          0x00
#define TFFS_ACCESS_HIDDEN          0x40
#define TFFS_ACCESS_SUPER           0x80

/* Max file/directory name length, excluding 0x00 termination */
#define TFFS_FILENAME_MAX           28

/* End of file value */
#define TFFS_EOF                    (-1)

/* fseek start points */
#define TFFS_SEEK_SET               0
#define TFFS_SEEK_CUR               1
#define TFFS_SEEK_END               2

/*******************************************************************************
***********************             TYPEDEFS             ***********************
*******************************************************************************/

/* Disk controller access functions and info. Used by TFFS in order */
/* to be able to use different types of disk controller, i.e. for */
/* instance also support RAM partitions. */
typedef struct
{
   /************************************************/
   /* Mandatory info, must be defined              */
   /************************************************/

   /* Underlying partition type in use */
   U8 type;

   /************************************************/
   /* Mandatory function pointers, must be defined */
   /************************************************/

   /* Get size of block in partition */
   int (*getBlockSize)(int id, U8 *size);

   /* Get ptr to info string for partition */
   const char *(*getInfoString)(int id);

   /* Get number of blocks in partition that are available for use */
   int (*getNumberOfBlocks)(int id, U32 *number);

   int (*read)(int id, U32 block, U8 *to, U32 length, U32 offset);

   int (*write)(int id, U32 block, const U8 *from, U32 length, U8 category);

   /* Write directly into block without updating status */
   int (*writeTransparent)(int id,
                           U32 block,
                           const U8 *from,
                           U32 length,
                           U32 offset,
                           U8 category);

   /*********************************************************/
   /* Optional function pointers, not used if volatile mode */
   /*********************************************************/

   /* Mark a block unused, typically required for flash controllers */
   int (*blockDelete)(int id, U32 block);

   /* Is a block used (e.g. has not been deleted after use) */
   int (*isUsed)(int id, U32 block);

   /************************************************/
   /* Optional function pointers, not used if NULL */
   /************************************************/

   /* Get value the media erases to, 0 or 1 */
   int (*getEraseValue)(int id);

   int (*getNumberOfObsoleteBlocks)(int id, U32 *number);

   int (*reclaim)(int id, U8 percent);

#if TFFS_RESIZE
   int (*resize)(int *id, U16 ezones, U8 front, U8 expand);
#endif
} TFFS_DISK_CTRL;


/* File information structure used on TFFS files which are always opened */
/* in buffered mode. */
typedef struct tffs_file
{
   /* Pointer to area holding cached block */
   U8 *block;

   /* Pointer to disk info structure for file which file is located */
   /* in. Use a void pointer since the disk info structure is not */
   /* available in this include file, and should be hidden for the */
   /* user. */
   void *di;

#if TFFS_MONITOR
   /* Ptr to first monitor entry applicable for this stream */
   /* for an item at opening time. */
   void *firstMonitorEntry;

   /* Number of monitor entries applicable for this stream */
   U32 monitorEntries;
#endif

#if (TFFS_PROTECT)
   /* Previous entry in double linked list of opened streams */
   struct tffs_file *prev;

   /* Next entry in double linked list of opened streams */
   struct tffs_file *next;
#endif

   /* Logical block number for block that is cached. Set to 0 */
   /* if stream is positioned within new last block not yet stored */
   /* to partition (and thus no logical block number assigned). */
   /* Thus, by definition an empty file will have this field set */
   /* to 0, since it is pointing into the start of the first */
   /* block to be stored. (Block 0 is never a valid file block */
   /* in TFFS.) */
   U32 cachedBlock;

   /* Offset in file to start of cached block. */
   U32 cachedBlockPosition;

   /* Logical block number for first block of file chain (0 if empty file) */
   U32 firstBlock;

   /* Logical block number for last block of file chain (0 if empty file) */
   U32 lastBlock;

   /* Logical block number for first block of parent directory file chain */
   U32 parentDir;

   /* Current position in file in bytes from start */
   U32 position;

   /* Actual size of file in bytes */
   U32 size;

   /* Stream flags */
   U16 flags;

   /* File name */
   char name[TFFS_FILENAME_MAX + 1];
} TFFS_FILE;


/*******************************************************************************
***********************              MACROS              ***********************
*******************************************************************************/

/*******************************************************************************
*
* Macro:    tffsRewind
*
* Descr.:   Rewind file to start and clear error indicator for a stream.
*
*           EOF is cleared if set.
*
* Reentr.:  Reentrant
* 
* Input:    stream      Pointer to file structure for file to rewind
*
* Output:   void
*
* Author:   Tevero AS
* Date:     2002-03-15
* Tested:   

*******************************************************************************/

#define tffsRewind(stream) tffsClearerr(stream); tffsFseek(stream, 0, TFFS_SEEK_SET)


/*******************************************************************************
***********************            PROTOTYPES            ***********************
*******************************************************************************/

/******************************************/
/* Basic Transaction Flash FileSystem API */
/******************************************/

void tffsClearerr(TFFS_FILE *stream);

int tffsDiagMode(int id, U8 mode);

int tffsFclose(TFFS_FILE *stream);

int tffsFeof(TFFS_FILE *stream);

int tffsFerror(TFFS_FILE *stream);

int tffsFflush(TFFS_FILE *stream);

TFFS_FILE *tffsFopen(int id,
                     U32 currentDir,
                     U8 attrib,
                     const char *filename,
                     const char *mode);

int tffsFormat(int prtId,
               U32 maxBlocks,
               TFFS_DISK_CTRL *dapi,
               void *userPointer,
               int (*wai_semWrite)(void *userPointer),
               void (*sig_semWrite)(void *userPointer),
               U8 diagMode);

U32 tffsFread(void *buffer, U32 size, U32 count, TFFS_FILE *stream);

int tffsFseek(TFFS_FILE *stream, long offset, int origin);

int tffsFtruncate(TFFS_FILE *stream, U32 length);

U32 tffsFwrite(const void *buffer, U32 size, U32 count, TFFS_FILE *stream);

U32 tffsGetRoot(int id);

void tffsInit(U32 max);

int tffsMount(int prtId,
              TFFS_DISK_CTRL *dapi,
              void *userPointer,
              int (*wai_semRead)(void *userPointer),
              void (*sig_semRead)(void *userPointer),
              int (*wai_semWrite)(void *userPointer),
              void (*sig_semWrite)(void *userPointer),
              U8 (*getAccess)(void *userPointer),
              U8 diagMode);



/*********************************************/
/* Optional Transaction Flash FileSystem API */
/*********************************************/

int tffsChange(int id,
               U32 currentDir,
               const char *item,
               U8 *attrib,
               const char *name);

int tffsChdir(int id, U32 currentDir, const char *dir, U32 *newDir);

int tffsDelete(int id, U32 currentDir, const char *item);

int tffsExists(int id, U32 currentDir, const char *item, U8 *attrib);

int tffsGetBlockInfo(int id, U32 *totalBlocks, U32 *usedBlocks, U32 *blockSize);

int tffsGetPath(int id, U32 dir, char *path, U32 max);

int tffsLsdir(int id,
              U32 currentDir,
              const char *dir,
              U32 entry,
              char *name,
              U8 *attr,
              U32 *size);

int tffsMkdir(int id, U32 currentDir, const char *dir, U8 attrib);

void *tffsMonitorAdd(int id,
                     const char *item,
                     unsigned long actions,
                     void (*callback)(void *callbackPtr, unsigned long action),
                     void *callbackPtr);
                        
void tffsMonitorRemove(void *entry);

int tffsReclaim(int id, U8 level);

int tffsResize(int *tffsId,
               int *prtId,
               U16 ezones,
               U8 front,
               U8 expand);

int tffsUnmount(int id);



/************************************************/
/* Diagnostics Transaction Flash FileSystem API */
/************************************************/

void tffsDiagDiskInfo(int id);



/**************************************/
/* Disk controller API init functions */
/**************************************/

void tffsFdcApiInit(TFFS_DISK_CTRL *ctrlInfo);
void tffsRdcApiInit(TFFS_DISK_CTRL *ctrlInfo, U8 nonvol);



#ifdef __cplusplus
}
#endif

#endif /* TFFS_H */
