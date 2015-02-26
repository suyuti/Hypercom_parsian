//=============================================================================
//                         Hypercom Inc (c) Copyright 2003
//                                    Viking
//=============================================================================
#ifndef _SIMPLEVECTOR_HPP_
#define _SIMPLEVECTOR_HPP_
//
//  SimpleVector.hpp
//  Simple vector implementation. Required as a result of arm compiler does not
//  support stl on the eftos platform.
//  
//=============================================================================

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <new>



template <class T>
class SimpleVector
{
public:
	SimpleVector(size_t sz=0x10) :
	    pArray (NULL),
	    nElemsAllocated(0),
	    nElemsUsed(0),
		nInitAlloc(sz)
	{
		GrowVector(sz);
	}

	~SimpleVector()
	{
		T *p=begin();
		while (p<end())
		{
			p->~T();
			p++;
		}
		free( pArray );
	}

	void push_back( const T &data )
	{
		if( nElemsUsed == nElemsAllocated )
		{
			GrowVector();
		}
		if( nElemsUsed != nElemsAllocated )
		{
	        new ( pArray + nElemsUsed ) T(data);
			nElemsUsed++;
		}
	}

	void push_front( const T &data )
	{
		if( nElemsUsed == nElemsAllocated )
		{
			GrowVector();
		}
		if( nElemsUsed != nElemsAllocated )
		{
			memmove(pArray+1,pArray,nElemsUsed* sizeof( T ));
	        new ( pArray ) T(data);
			nElemsUsed++;
		}
	}

	void erase ( size_t p0 )
	{
		if( p0<nElemsUsed)
		{
			T * p=pArray+p0;
			p->~T();
			nElemsUsed--;
			memmove(p,p+1,(nElemsUsed-p0) * sizeof( T ));
		}
	}
	void erase ( T * p )
	{
		if((begin()<=p) && (p<end()))
		{
			p->~T();
			nElemsUsed--;
			memmove(p,p+1,((char *)end() - (char *)p));
		}
	}


	void clear( void )
	{
		T *p=begin();
		while (p<end())
		{
			p->~T();
			p++;
		}
		nElemsAllocated = 0;
		nElemsUsed = 0;
		GrowVector(nInitAlloc);
	}

	inline T *begin( void ) const
	{
		return pArray;
	}

	T *end( void ) const
	{
		return pArray + nElemsUsed;
	}
	inline size_t size( void ) const
	{
		return nElemsUsed;
	}

	SimpleVector <T>& operator=(const SimpleVector <T>& v)
	{
		clear();
		if (v.nElemsAllocated>nElemsAllocated)
		{
			GrowVector(v.nElemsAllocated);
		}

		T *p=v.begin();
		while (p<v.end())
		{
	        new ( pArray + nElemsUsed ) T(*p);
			nElemsUsed++;
			p++;
		}
		return (*this);
	}


	T& operator[](size_t p0) const
	{
		return *(pArray+p0);
	}


private:
	void GrowVector( size_t sz=0x10 )
	{
		size_t new_size=nElemsUsed + sz;
		if (new_size==nElemsAllocated)
		{
			return;
		}
		T *newArray = (T*) realloc( pArray, new_size * sizeof( T ) );
		if(( newArray != NULL ) || (new_size==0))
		{
			pArray = newArray;
			nElemsAllocated = new_size;
		}
	}

	T *pArray;
	size_t nElemsAllocated;
	size_t nElemsUsed;
	size_t nInitAlloc;

};


#endif // _SIMPLEVECTOR_HPP_
