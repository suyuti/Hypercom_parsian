//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCMAP_HPP_
#define _HYPCMAP_HPP_
#include "SimpleVector.hpp"
#include "HypCPair.hpp"

//=============================================================================
//
// HypCMap.hpp
//
// Base Class:
//		None
//
// Overview:
//		The HypCMap class describes an object that controls a varying-length
//		sequence of elements. Each element is pair of integer Key and its
//		associated value. The value may store either integer or null-terminated
//		string value. The sequence is represented in a way that permits lookup,
//		insertion, and removal of an arbitrary element with a number of
//		operations directly proportional to the number of elements
//		in the sequence.
//
//		string values are processed in the following way:
//		In case of assigment, string is duplicated by strdup() and returnd
//		poiner is being stored. In case of deleting saved pointer released
//		by free().
//
// See Also:
//		HypCPair
//=============================================================================

class HypCMap
{
public:
	//constructors:
	//constructor specifies an empty initial controlled sequence.
	//memory is allocated for store up to sz elemets
	HypCMap (size_t sz=10);
	//initilizes  sequence with elements from addr
	HypCMap (const HypCMap &addr);

	//destructor
	~HypCMap ();

	//function determines whether an element x exists in the sequence whose key matches 'key'.
	//If not, it creates such an element x and initializes it with integer value i.
	//if yes, it deletes old value and assigns new integer value i.
	void Set(int key, int i);
	//function determines whether an element x exists in the sequence whose key matches 'key'.
	//If not, it creates such an element x and initializes it with string value s.
	//if yes, it deletes old value and assigns new string value s(see notes at the top of the file).
	void Set(int key, const char *s);

	void Set(HypCPair &);

	char *GetString(int key) const;
	int Get(int key) const;
	
	HypCPair* GetPair(int key) const;

	bool IsExist(int key) const;
	//function removes element with key=='k' 
	void Remove (int k);

	//function returns the length of the controlled sequence.
	inline int Size() const
	{ return m_Map.size(); }

	//operator deletes all elements from the controlled sequence, then
	//it initilizes  sequence with copies of all elements from addr
	HypCMap  & operator=(const HypCMap  &addr);

	//return reference to value
	//(usage example: my_map[MY_KEY].Value()*=4 or even my_map[MY_KEY]++ or my_map[K2]+=10)
	HypCPair  & operator[](int k);

	//function removes all elements from the controlled sequence
	void Clear();

	//execute fun() for each pair in map
	void Foreach ( void(*fun)(HypCPair &) );

protected:
//	friend bool CopyConfigInt (HypCMap& dst, const HypCMap& src, int key);

	void Add(HypCPair &e);
	SimpleVector <HypCPair> m_Map;
};


#endif
