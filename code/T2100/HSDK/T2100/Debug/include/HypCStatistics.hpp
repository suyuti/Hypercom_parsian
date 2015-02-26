//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCSTATISTICS_HPP_
#define _HYPCSTATISTICS_HPP_
#include "HypCMap.hpp"

//=============================================================================
//
// HypCStatistics.hpp
//
// Base Class:
//		HypCMap
//
// Overview:
//		An object of the HypCStatistics is used to collect communication
//		statistics.
//
// See Also:
//		HypCPair, HypCMap
//=============================================================================


enum HypEStatisticsKeys
{
	SDLC_SNRM_IN,
	SDLC_SNRM_OUT,
	SDLC_RNR_IN,
	SDLC_RNR_OUT,
	SDLC_TEST_IN,
	SDLC_TEST_OUT,
	SDLC_DM_IN,
	SDLC_DM_OUT,
	SDLC_UA_IN,
	SDLC_UA_OUT,
	SDLC_FRMR_IN,
	SDLC_FRMR_OUT,
	SDLC_RETR,
	SDLC_RXERR,		/* RECEIVE FRAMING ERRORS*/

	STAT_REDIALS,  //Incremented for each dial attempt, excluding the initial dial attempt for a transaction or function.
	STAT_PREDIALS,   /* PRIMARY REDIAL COUNT*/
	STAT_SREDIALS,   /* SECONDARY REDIAL COUNT*/
	STAT_MSG_IN,
	STAT_MSG_OUT,
	STAT_COMMERR  //Incremented for each transaction failure due to exhausting all dial attempts without a connection. "Please Try Again - CE"
};


class HypCStatistics : public HypCMap
{
public:
	//constructors:
	//constructor specifies an empty initial controlled sequence.
	//memory is allocated for store up to sz elemets
	HypCStatistics (size_t sz=10);

	//destructor
	~HypCStatistics ();

	int& operator[](int k);
	void Append (const HypCStatistics &s);
};


#endif

