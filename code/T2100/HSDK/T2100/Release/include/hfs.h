// $Workfile:   hfs.h  $
// $Header:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum Boot Loader/HSDK/include/hfs.h-arc   1.1   May 20 2005 15:54:28   aaganichev  $
// $Log:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum Boot Loader/HSDK/include/hfs.h-arc  $
//
//   Rev 1.1   May 20 2005 15:54:28   aaganichev
//added ffixsize
//
//   Rev 1.0   Feb 16 2004 17:16:30   rzajtsev
//Initial revision.
//
//   Rev 1.0   Jan 05 2004 14:07:00   rzajtsev
//Initial revision.
//
//   Rev 1.0   Aug 12 2003 11:35:00   AAganichev
//Initial revision.
//LOG: 
//LOG:    Rev 1.2   Feb 26 2001 19:16:46   jmunkhof
//LOG: Added new function that returns the amount of free space available in the file system.
//LOG: 
//LOG:    Rev 1.1   Nov 22 2000 16:55:04   jmunkhof
//LOG: The list of pages to use for file storage is now passed to finit()
//LOG: by the application.
//LOG: 
//LOG:    Rev 1.0   Jun 14 2000 11:18:58   jmunkhof
//LOG: Moved the file system from its own PVCS project into the C Library project.
//LOG: 
//LOG:    Rev 1.7   May 26 2000 11:17:24   jmunkhof
//LOG: Add support for findfirst() and findnext().
//LOG: 
//LOG:    Rev 1.6   May 04 2000 11:21:22   jmunkhof
//LOG: Removed THIS_PAGE constant which was no longer needed.
//LOG: 
//LOG:    Rev 1.5   Apr 14 2000 11:47:40   jmunkhof
//LOG: Add various format methods and change return type
//LOG: of finit().
//LOG: 
//LOG:    Rev 1.4   Apr 09 2000 19:39:00   jmunkhof
//LOG: Fix the max number of pages.
//LOG: 
//LOG:    Rev 1.3   Apr 08 2000 17:24:24   jmunkhof
//LOG: Add THIS_PAGE.
//LOG: 
//LOG:    Rev 1.2   Apr 07 2000 19:02:26   jmunkhof
//LOG: Add declarations.
//LOG: 
//LOG:    Rev 1.1   Apr 06 2000 18:18:44   jmunkhof
//LOG: Fix comments.
//LOG: 
//LOG:    Rev 1.0   Apr 06 2000 16:06:20   jmunkhof
//LOG: Initial revision.
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2000
//=============================================================================
#ifndef _HFS_H_
#define _HFS_H_
//=============================================================================
//
// Module overview:
//
//      This file contains public declarations for the Hypercom File System
//      which are not defined in standard C header files.
//
//=============================================================================

//=============================================================================
//=============================================================================
#include <stdio.h>
#include <dirent.h>

//=============================================================================
// Public defines and typedefs
//=============================================================================

//-----------------------------------------------------------------------------
//  HFS_MAX_PAGES
//
//      This constant defines the maximum number of pages that can be
//      allocated to file storage.
//
//      THIS SHOULD BE THE MULTIPLY OF FOUR FOR ARM CPU!!!
//-----------------------------------------------------------------------------
#ifdef MAKE_VIKING
#define HFS_MAX_PAGES   4
#else
#define HFS_MAX_PAGES   64
#endif


//-----------------------------------------------------------------------------
//      These constants define the various methods available for formatting
//      the disk.  See the fformat() function for more information.
//-----------------------------------------------------------------------------
enum
{
    HFS_FORMAT_QUICK,   // Initialize the minimum amount of disk memory
    HFS_FORMAT_FULL     // Initialize all disk memory
};


//-----------------------------------------------------------------------------
//  FFBLK
//
//      This structure is used to search a directory for a matching filename.
//      See findfirst() and findnext() for more information.
//-----------------------------------------------------------------------------
typedef struct
{
    struct dirent dirInfo;      // See dirent.h for a description.

// These elements are used internally by findfirst() and findnext() and
// should not be modified or accessed by an application.
    long reserved;
    char searchString[ FILENAME_MAX+1 ];
} FFBLK;



//=============================================================================
// Public data declarations
//=============================================================================



//=============================================================================
// Public function declarations
//=============================================================================

//-----------------------------------------------------------------------------
//  finit
//
//      This function initializes the resources used by the Hypercom File
//      System.  It must be called after a system reset and before any other
//      function in the HFS library.
//
//      This function should only be called during system start-up since it
//      initializes all of the structures used to manage files.  If this
//      function is called while files are open, data could be lost.  To
//      verify the integrity of the file system without initializing, use
//      the fverify() function.
//
//      The caller must specify the memory pages to use for file storage
//      in the character array pointed to by pPageList.  Each entry in
//      the array must be a valid memory page number and the list must
//      be terminated with a null character.  If the page list changes
//      in any way, the file system must be re-formatted.
//      
//  Parameters:
//      pPageList   Pointer to a list of page numbers to use for file
//                  storage.  The list must be null terminated.
//
//  Returns:
//      Nothing
//-----------------------------------------------------------------------------
extern void finit( const unsigned char * pPageList );

//-----------------------------------------------------------------------------
//  ffixsize
//
//      This function sets the proper value for HFS_PHYSICAL_PAGE_SIZE
//	from internal structures, so the application will use the same value as
//	set by application manager.
//
//	This function need to be called by application after finit call.
//
//  Parameters:
//      None.
//
//  Returns:
//      Nothing
//-----------------------------------------------------------------------------
void ffixsize(void);

//-----------------------------------------------------------------------------
//  fformat
//
//      This function initializes the memory pages used by the Hypercom
//      File System.  It is analogous to formatting a disk on a PC.
//
//      There are two methods of formatting: quick and full.  The desired
//      method is specified as a parameter to this function using the
//      constants HFS_FORMAT_QUICK and HFS_FORMAT_FULL, respectively.
//
//      The full method initializes every byte on each page of file storage
//      (except for the area reserved by the OS) by filling the memory with
//      0xFF.  After the fill is complete, a quick format is performed to
//      initialize the disk.
//
//      The quick method of formatting initializes the minimum amount of
//      memory necessary to make the disk usable (e.g. the File Allocation
//      Table and the root directory).  Any files that existed on the disk
//      prior to the format become inaccessible, even though the file data
//      is not erased.
//
//  Parameters:
//      method      The desired formatting method.
//
//  Returns:
//      Non-zero if the file system was formatted successfully, zero if there
//      was an error.
//-----------------------------------------------------------------------------
extern int fformat( int method );

//-----------------------------------------------------------------------------
//  fverify
//
//      This function verifies the integrity of the Hypercom File System.
//      The state of the file system is not changed so this function may
//      be called at any time, even if files are open.
//
//  Parameters:
//      None
//
//  Returns:
//      Non-zero if the file system is usable, zero if it must be re-formatted.
//-----------------------------------------------------------------------------
extern int fverify( void );

//-----------------------------------------------------------------------------
//  findfirst
//
//      This function searches a directory for the first file or subdirectory
//      matching the given filename.  The filename may contain wildcard
//      characters but the path, if specified, may not.  The specified FFBLK
//      structure will be filled in with the filename of the first matching
//      file plus data needed to continue the search with findnext().
//
//      The permitted wildcard characters are asterisks ('*') and question
//      marks ('?').  An asterisk matches any string of zero or more
//      characters while a question mark matches any single character.
//
//      Examples of wildcard matching:
//          "*" matches everything.
//          "*.*" matches "a.dat", "abc.dat", "abc.", and ".abc" but not "abc"
//          "he*lo" matches "helo" ,"hello", "helllo", "he.lo", etc.
//          "?ell?" matches "hello" and "jello" but not "bell" or "bellow".
//
//  Parameters:
//      filename    Pointer to search string
//      pFFBlk      Pointer to search buffer
//
//  Returns:
//      Non-zero if a matching file was found, zero otherwise.
//-----------------------------------------------------------------------------
extern int findfirst( const char * filename, FFBLK * pFFBlk );

//-----------------------------------------------------------------------------
//  findnext
//
//      This function continues a directory search which was initiated by
//      a call to findfirst().  The next file in the directory which matches
//      the original search string is returned.
//
//  Parameters:
//      pFFBlk      Pointer to a search buffer specifed in a previous call
//                  to findfirst().
//  Returns:
//      Non-zero if a matching file was found, zero otherwise.
//-----------------------------------------------------------------------------
extern int findnext( FFBLK * pFFBlk );

//-----------------------------------------------------------------------------
//  fspaceavail
//
//      This function returns the amount of free space available in
//      the file system.
//
//  Parameters:
//      None
//
//  Returns:
//      Amount of space available (in bytes) or -1L if there is an
//      error.
//
//  Notes:
//      Some of the space reported as available by this function
//      may not be usable for file storage due to the way memory
//      is allocated.
//-----------------------------------------------------------------------------
extern long fspaceavail( void );


#endif /* _HFS_H_ */

