/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

//!------------------------------------------------------------------
//!
//! \file  jrndata.h
//!
//! \brief  Header file for common data areas for the journal.
//!
//!------------------------------------------------------------------
 
#ifndef JRNDATA_H_
#define JRNDATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DOXYGEN
#define __attribute__(A)
#endif


//!
//!	\brief Linked list definition.
//!
//!	\note The definition of ll_def must not be changed. The C language
//!	      journal programs depend on ll_def not changing.
//!
struct ll_def
{
	UWORD LLLen;			///< Length of this block
	UBYTE LLThisPg;			///< This page number
	UWORD LLThis;			///< Address of this record (page relative)
	UBYTE LLNextPg;			///< Next page number
	UWORD LLNext;			///< Address to next (page relative)
	UBYTE LLPrevPg;			///< Previous page number
	UWORD LLPrev;			///< Address to previous (page relative)
	UWORD LLIdent;			///< Unique record identifier
	UBYTE LLLRC;			///< LRC of data in record
} __attribute__((packed));

/// Define linked list structure size.
#define LLSZE sizeof(struct ll_def)

//!
//!	\brief Journal page table definition.
//!
//!	\note The definition of jrnarea_rec must not be changed. The C language
//!	      programs depend on jrnarea_rec not changing.  
//!
struct jrnarea_rec
{
	UBYTE jpage;			///< Page area journal resides in
	UWORD jstartaddr;		///< Start of area on this page
	UWORD jendaddr;			///< End of area on this page
};

//!
//!	\brief Journal data definition.
//!
struct jrndata_def
{
	UBYTE JrnFmt;			///< Format indicator
	struct ll_def JrnUsed;		///< Header for used entries in journal
	struct ll_def JrnAvail;		///< Header for free entries in journal
	UWORD JrnIdent;			///< Journal identifier

	int JrnPID;			///< PID of process locking batch
	UBYTE JrnlCnt;			///< Count of times batch is locked

	UWORD JrnFixSize;		///< Size of uncompressed area of journal record
	UWORD JrnCompSize;		///< Size of compressed area of journal record
	UWORD MaxJrnSize;		///< Maximum size of a journal record
	const struct jrnarea_rec *pJrnArea;	///< Pointer to journal areas table
	UBYTE *pJRREC;			///< Pointer to JRREC address
	struct ll_def *pLLBUF;		///< Pointer to LLBUF (in C struct JRBUF).
	UBYTE *pJrnData;		///< Pointer to data (in C struct JRBUF).
	UBYTE *pJrnComp;		///< Pointer to compressed area of journal record
					///< (in C struct JRBUF).
	UWORD TranSize;			///< Uncompressed transaction size.
	struct ll_def *pThis;		///< Address of THIS - used in journal selection.
	int *pJrnRoomCt;		///< Address of JrnRoomCount.

	struct ll_def Next;		///< Next record.
	struct ll_def Prev;		///< Previous record.
	struct ll_def New;		///< New record.
	struct ll_def Remv;		///< Record to remove.
};

//!
//!	\brief Global variable that store information about journal.
//!
//!	\note It should reside in non-volatile memory (section jrndata).
//!
extern struct jrndata_def JournalData;


#ifdef __cplusplus
}
#endif
#endif // __jrndata_h
