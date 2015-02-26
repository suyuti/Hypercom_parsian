
#ifndef _CONFRPT_H_
#define _CONFRPT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2000
//=============================================================================
//
// Module overview: perfutil.h
//
//=============================================================================

extern void Valrptab ( void );
extern void InitRptTable ( void );
extern void IncrementCnt ( void );
extern void PrintPerformRpt(void); // To print the Performance Report
extern void DoConfigRpt( void );
extern void UpdateStatistic( void );


//=============================================================================
// Public defines and typedefs
//=============================================================================
//  typedef enum 
typedef enum 
{
  APPROVALS  ,  // Transactions approved
  DENIALS    ,  // Transactions denials
  MANUAL     , 	// Transaction input mode manual entry
  SWIPECARD  ,	// Transaction input mode swipe card
  TOTALTRANS ,	// Transaction totals 
  REDIALS    ,  // transaction redials
  TTIMEOUT   ,  // transactions Timeout
  RETRANSMITS   // transaction retransmits
}PERFORMRPT;


// PRINT FIELD TABLE 

#define F_SZE sizeof(struct FLDDEF)	// DEFINE STRUCTURE SIZE

// Type of Text fields Table to print
typedef enum
{
  TX_DEFAULT = 0,
  TX_EXTENDED,
  TX_SERVUS
}ENTXTTYPE;

// Type of fields Definition Tables to print  
typedef enum
{
  FX_DEFAULT = 0,
  FX_EXTENDED,
  FX_SERVUS
}TABTYPE;

// Select FLDTAB structure 
typedef struct
{
  TABTYPE Type;
  const struct FLDDEF *ptable1;
  const struct FLDDEF *ptable2;
}STRFLDTAB;


// Get Current element 
typedef struct
{
  STRFLDTAB currentTab;

}CURRENTELEM;

//=============================================================================
// Public data declarations
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================

#endif
