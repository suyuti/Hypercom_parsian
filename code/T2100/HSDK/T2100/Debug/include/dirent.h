// $Workfile:   dirent.h  $
// $Header:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum Boot Loader/HSDK/include/dirent.h-arc   1.0   Feb 16 2004 17:16:28   rzajtsev  $
// $Log:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum Boot Loader/HSDK/include/dirent.h-arc  $
//
//   Rev 1.0   Feb 16 2004 17:16:28   rzajtsev
//Initial revision.
//
//   Rev 1.0   Jan 05 2004 14:06:54   rzajtsev
//Initial revision.
//
//   Rev 1.0   Aug 12 2003 11:34:52   AAganichev
//Initial revision.
//LOG: 
//LOG:    Rev 1.1   Dec 21 2000 13:20:08   jmunkhof
//LOG: Add the d_modified field to the dirent structure so the file's
//LOG: timestamp is accessible by an application via the
//LOG: findfirst() and findnext() functions.
//LOG: 
//LOG:    Rev 1.0   Jun 14 2000 11:18:08   jmunkhof
//LOG: Moved the file system from its own PVCS project into the C Library project.
//LOG: 
//LOG:    Rev 1.6   May 26 2000 11:16:54   jmunkhof
//LOG: Add mode and filesize elements to make them available
//LOG: to application code.
//LOG: 
//LOG:    Rev 1.5   May 16 2000 18:00:30   jmunkhof
//LOG: Change the definitions of the directory structures
//LOG: to differentiate between the data stored on disk and
//LOG: the data returned to the application.
//LOG: 
//LOG:    Rev 1.4   May 03 2000 21:54:08   jmunkhof
//LOG: Change file storage from fixed-size sectors to
//LOG: variable-sized memory blocks.
//LOG: 
//LOG:    Rev 1.3   Apr 13 2000 11:46:30   jmunkhof
//LOG: Pad directory entry so it occupies an entire sector.
//LOG: 
//LOG:    Rev 1.2   Apr 09 2000 01:54:52   jmunkhof
//LOG: Comment out unused elements for now to ensure
//LOG: directory entry is less than sector size.
//LOG: 
//LOG:    Rev 1.1   Apr 08 2000 20:58:36   jmunkhof
//LOG: Change cluster to sector.
//LOG: 
//LOG:    Rev 1.0   Apr 07 2000 22:24:02   jmunkhof
//LOG: Initial revision.
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2000
//=============================================================================
#ifndef __DIRENT_H
#define __DIRENT_H
//=============================================================================
//
// Module overview:
//
//
//=============================================================================

//=============================================================================
//=============================================================================
#include <stdio.h>
#include <time.h>
#include <sys\types.h>

//=============================================================================
// Public defines and typedefs
//=============================================================================

struct dirent
{
    char    d_name[FILENAME_MAX+1];

// These elements are not POSIX-compliant.  Any code that accesses them
// is not portable!
    off_t   d_filesize;
    mode_t  d_mode;
    time_t  d_modified;     // Date and time file was last modified.
};


//-----------------------------------------------------------------------------
//  Hide the implementation of the DIR type since the structures are
//  not accessible by the application code due to the paged memory
//  architecture of the Z-80 platform.
//-----------------------------------------------------------------------------
#ifdef __HFS_BUILD_LIBRARY__
#include "dirdef.h"
#else
typedef void DIR;
#endif


//=============================================================================
// Public data declarations
//=============================================================================



//=============================================================================
// Public function declarations
//=============================================================================

//-----------------------------------------------------------------------------
//  opendir
//
//      This function opens a directory stream corresponding to the directory
//      named in the 'dirname' parameter.  The stream is positioned to the
//      first entry.
//
//  Parameters:
//      dirname     Pointer to the name of the directory to open.
//
//  Returns:
//      A pointer to be used with readdir(), closedir(), and rewinddir().
//      NULL is returned if an error occurred.
//-----------------------------------------------------------------------------
DIR * opendir( const char * dirname );

//-----------------------------------------------------------------------------
//  readdir
//
//      This function returns a pointer to a dirent structure representing
//      the next directory entry from the directory stream pointed to by
//      'pDir'.  On end-of-file (i.e. end of the directory), NULL is returned.
//
//      Entries are returned for the current (".") and parent ("..")
//      directories
//
//      The data pointed to by readdir() will be overwritten by another call
//      to readdir() for the same directory stream.  It will not be overwritten
//      by a call for another directory stream.
//
//  Parameters:
//      pDir        Pointer returned by opendir().
//
//  Returns:
//      A pointer to the dirent structure, or NULL if the end of the directory
//      was reached or an error occurred.
//-----------------------------------------------------------------------------
struct dirent * readdir( DIR *pDir );

//-----------------------------------------------------------------------------
//  closedir
//
//      This function ends a directory read operation.  The directory stream
//      associated with 'pDir' is closed.
//
//  Parameters:
//      pDir        Pointer returned by opendir().
//
//  Returns:
//      Zero if successful, -1 if an error occurred.
//-----------------------------------------------------------------------------
int closedir( DIR *pDir );

//-----------------------------------------------------------------------------
//  rewinddir
//
//      This function resets the position associated with the directory
//      stream pointed to by 'pDir'.  It also causes the directory stream
//      to refer to the current state of the directory.
//
//  Parameters:
//      pDir        Pointer returned by opendir().
//
//  Returns:
//      Nothing
//-----------------------------------------------------------------------------
void rewinddir( DIR *pDir );


#endif  // __DIRENT_H
