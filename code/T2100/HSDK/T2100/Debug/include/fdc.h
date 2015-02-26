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
* File:     fdc.h
* Version:  1.3.2
* Descr.:   Flash Disk Controller include file. This include file contains the
*           global definitions in order to interface to the FDC system. No
*           other include files are normally required by external SW.
*
* Author:   Tevero AS
* Date:     2000-04-12
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef FDC_H
#define FDC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Must ensure these files are included, since it may affect contents */
#include "shrdconf.h"
#include "fdcconf.h"

/*******************************************************************************
***********************             DEFINES              ***********************
*******************************************************************************/

/* Error codes from FDC functions */
#define FDC_ERROR_USAGE             -1
#define FDC_ERROR_READ              -2
#define FDC_ERROR_WRITE             -3
#define FDC_ERROR_MEMORY            -4
#define FDC_ERROR_NO_PARTITION      -5
#define FDC_ERROR_PARTITION_FULL    -6
#define FDC_ERROR_MAPPING_TABLE     -7
#define FDC_ERROR_NON_EXISTING      -8
#define FDC_ERROR_BAD_BLOCKS        -9
#define FDC_ERROR_BAD_EZONE         -10

#define FDC_ERROR_LAST              (FDC_ERROR_BAD_EZONE)

/* Partition info layout field defines */
#define FDC_DEVICES_INTERLEAVED_1   0x00
#define FDC_DEVICES_INTERLEAVED_2   0x10
#define FDC_DEVICES_INTERLEAVED_4   0x30
#define FDC_DEVICES_INTERLEAVED_8   0x70

/* Partition info bit width for a flash device */
#define FDC_DEVICE_BITWIDTH_8       0x00
#define FDC_DEVICE_BITWIDTH_16      0x01
#define FDC_DEVICE_BITWIDTH_32      0x03
#define FDC_DEVICE_BITWIDTH_64      0x07

/* Maximum number of characters in partition info (excluding 0x00 termination) */
#define FDC_MAX_INFO                15

/* Diagnostics modes */
#define FDC_DIAG_DISABLE            0
#define FDC_DIAG_ERRORS             1
#define FDC_DIAG_ALL                2

/* Block data category, used to group blocks in erase zones */
#define FDC_CATEGORY0               0
#define FDC_CATEGORY1               1

/*******************************************************************************
***********************            PROTOTYPES            ***********************
*******************************************************************************/

/* Basic Flash Disk Controller API */

int fdcClose(int id);

int fdcDelete(int id, U32 block);

int fdcDiagMode(int id, U8 mode);

#if (FDC_200NM_230NM_MIRRORBIT_SUPPORT == 1)
U8 fdcForceMirrorBitWorkaroundLayoutGet(void);
void fdcForceMirrorBitWorkaroundLayoutSet(U8 enable);
int fdcIsMirrorBitWorkaroundLayout(int id);
#endif

int fdcFormat(char *info,
              U32 startOffset,
              U32 partitionSize,
              U32 partitionMaxSize,
              U8 deviceSize,
              U8 ezoneSize,
              U8 blockSize,
              U8 layout,
              U8 reclaimEzones,
              U8 reclaimBlocks,
              void *userPointer,
              int (*flashErase)(void *userPointer, U32 offset),
              int (*flashRead)(void *userPointer, void *to, unsigned int length, U32 offset),
              int (*flashWrite)(void *userPointer, const void *buffer, unsigned int length, U32 offset),
              int (*wai_semWrite)(void *userPointer),
              void (*sig_semWrite)(void *userPointer),
              U8 diagMode);

int fdcGetBlockSize(int id, U8 *size);

const char *fdcGetInfoString(int id);

int fdcGetNumberOfBlocks(int id, U32 *number);

int fdcGetNumberOfFreeBlocks(int id, U32 *number);

int fdcGetNumberOfObsoleteBlocks(int id, U32 *number);

int fdcGetStartOffset(int id, U32 *offset);

void fdcInit(U32 max);

int fdcInvalidate(int id);

int fdcIsUsed(int id, U32 block);

int fdcOpen(U32 startOffset,
            U32 range,
            U8 step,
            void *userPointer,
            int (*flashErase)(void *userPointer, U32 offset),
            int (*flashRead)(void *userPointer, void *to, unsigned int length, U32 offset),
            int (*flashWrite)(void *userPointer, const void *buffer, unsigned int length, U32 offset),
            int (*wai_semRead)(void *userPointer),
            void (*sig_semRead)(void *userPointer),
            int (*wai_semWrite)(void *userPointer),
            void (*sig_semWrite)(void *userPointer),
            U8 diagMode);

int fdcRead(int id,
            U32 block,
            U8 *to,
            U32 length,
            U32 offset);

int fdcReclaim(int id, U8 level);

int fdcWrite(int id,
             U32 block,
             const U8 *from,
             U32 length,
             U8 category);



/* Optional Flash Disk Controller API */

int fdcGetEraseValue(int id);

int fdcResize(int *id, U16 ezones, U8 front, U8 expand);

U32 fdcSetWearlevelLimit(int id, U32 limit);

int fdcWriteTransparent(int id,
                        U32 block,
                        const U8 *from,
                        U32 length,
                        U32 offset,
                        U8 category);



/* Diagnostics Flash Disk Controller API */

void fdcDiagListEzones(int id);

void fdcDiagListPartitions(void);

void fdcDiagPartitionInfo(int id);

#ifdef __cplusplus
}
#endif

#endif /* FDC_H */
