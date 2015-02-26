#ifndef __hypffs_h
#define __hypffs_h

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@defgroup GroupFFS Hypercom Helpers For Flash File System
	@{
*/

/**
	@brief Initialize FFS.

	This function must be called once before any use of FFS.
*/
void FFS_Init( void );

/**
	@brief Reclaim obsolete blocks on the FFS.

	@note This function must be called only if write time on subsequent
	      writes is very critical (i.e. during communication). Calling
	      this function may cause to the quicker wear of the flash.
*/
void FFS_ReclaimBlocks(void);

/**
	@brief Get size of free space left on the FFS.

	@return Size of free space left on the FFS.
*/
size_t FFS_GetFreeSpace(void);

/**
	@brief Mount FFS partition.

	Mounts FFS partition to /flash directory.

	@param permission Sets permission level. Applications should use
	                  FAL_ACCESS_NORMAL.

	@return 0: OK.<br>
	        -1: FDC layer failed to mount.<br>
	        -2: TFFS layer failed to mount.

	@note Every user process must call falRegister() before use of FFS
	      after calling this function.
*/
int FFS_Mount( int permission );

/**
	@brief Unmount FFS partition.

	Unmounts FFS partition mounted to /flash directory with FFS_Mount().

	@return 0: OK.<br>
	        -1: FDC layer failed to mount.<br>
	        -2: TFFS layer failed to mount.

	@note Every user process that registered to use FFS must call
	      falUnregister() before calling this function.
*/
int FFS_Unmount( void );

/**
	@brief Format FFS.

	@return 0: OK<br>
	        -1: FDC layer format failed.<br>
	        -2: TFFS layer format failed.

	@note Filesystem should be unmounted when calling this function.
*/
int FFS_Format( void );

/**
	@example FFStest.c
	This is an example of how to use the Hypercom helpers for flash file
	system.
*/

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __hypffs_h
