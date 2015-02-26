//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCPAIR_HPP_
#define _HYPCPAIR_HPP_


//=============================================================================
//
// HypCPair.hpp
//
// Base Class:
//      None
//
// Overview:
//		The HypCPair class stores pair of elements. First element (key) is
//		integer value, it is used as key in HypCMap class.
//		Second elemet (value) can be integer or null terminated string.
//
// See Also:
//		HypCMap
//=============================================================================
#include "stddef.h" //NULL definition for ads compiler


class HypCPair
{
public:
	HypCPair();
	HypCPair(const HypCPair &e);
	HypCPair(int key, const char *s);
	HypCPair(int key, int i);

	~HypCPair();

	inline bool IsString() const
	{
		return m_Stype;
	}
	inline int Key() const
	{
		return m_Key;
	}
	int& Value();
	char*& StringValue();

	HypCPair& operator=(const HypCPair& s);
	HypCPair& operator++();    // Prefix increment operator.
	HypCPair& operator++(int); // Postfix increment operator, the same as prefix increment
	HypCPair& operator--();    // Prefix decrement operator.
	HypCPair& operator--(int); // Postfix decrement operator, the same as prefix decrement
	HypCPair& operator-=(int x);
	HypCPair& operator+=(int x);
	
private:
	bool m_Stype;
	int m_Key;
	union _val {
		int i;
		char *pC;
		_val() { i = 0; pC = NULL; }
		_val(int v) { i = v; }
		_val(char *v) { pC = v; }
	} m_Val;
};
#endif //_HYPCPAIR_HPP_

