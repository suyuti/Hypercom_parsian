//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCBUFFER_HPP_
#define _HYPCBUFFER_HPP_
#include <stddef.h>
#include <string.h>

/**
	@addtogroup HPOSFC
	@{
*/

/// Default initial buffer size of HypCBuffer.
#define DEFAULT_BUFFER_SIZE 2048

/// Default initial gap before data in HypCBuffer.
#define DEFAULT_ROOM_BEFORE 64

/**
	@brief Varying-length sequence of elements of type char.

	Buffer structure: @include BufferFormat.txt

	Copying of data contained in buffer on the insert/erase operation
	occures only if it is necessary. E.g. if data is erased from the
	beginnig, data is not copied,  only m_DataPtr is increased.

	Buffer is automatically reallocated if data does not fit in current
	buffer. HypCBuffer object holds 2 user defined values: UserPtr and
	DataType. Application may use it for store any data associated with
	data in the buffer.
*/
class HypCBuffer
{
public:
	/// The constant is the largest representable value
	static const size_t npos = (size_t)(-1);

	/**
		@brief Constructor.

		Creates empty buffer.

		@param initialSize Initial buffer size (optional, defaults to
		                   DEFAULT_BUFFER_SIZE).
		@param roomBeforeData Initial gap before data (optional,
		                      defaults to DEFAULT_ROOM_BEFORE).

		@note The summary size of the buffer is initialSize plus
		      roomBeforeData.
	*/
	HypCBuffer( size_t initialSize = DEFAULT_BUFFER_SIZE,
	            size_t roomBeforeData = DEFAULT_ROOM_BEFORE );

	/**
		@brief Constructor.

		Creates buffer and initializes it with data from pData.

		@param pData Pointer to the data to initialize buffer from.
		@param dataLen Size of the data to copy from the pointer.
		@param initialSize Initial buffer size (optional, defaults to
		                   0). If initialSize is lower than dataLen it
		                   will be increased to dataLen and overall
		                   reserved size will be rounded to the upper
		                   64 bytes boundary.
		@param roomBeforeData Initial gap before data (optional,
		                      defaults to DEFAULT_ROOM_BEFORE).

		@note The summary size of the buffer is initialSize plus
		      roomBeforeData.
	*/
	HypCBuffer( const void *pData, size_t dataLen, size_t initialSize = 0,
	            size_t roomBeforeData = DEFAULT_ROOM_BEFORE);

	/**
		@brief Copy constructor.

		Creates buffer and initializes it with data from another buffer.

		@param buf Reference to the buffer to get data from.
	*/
	HypCBuffer( const HypCBuffer &buf );

	/**
		@brief Constructor.

		Creates buffer and initializes it with data from NULL
		terminated string. There's no free room before and after data
		allocated.

		@param pStr Pointer to the NULL terminated string.
	*/
	HypCBuffer( const char *pStr );

	/**
		@brief Destructor.
	*/
	virtual ~HypCBuffer();

public:
	/**
		@brief Copy operator.

		Deletes all data contained in the buffer and saves one byte of
		data.

		@param x New value of the buffer.

		@return Reference to this buffer.
	*/
	HypCBuffer &operator =( const char x );

	/**
		@brief Copy operator.

		Deletes all data contained in the buffer, then data is being
		copied from the specified buffer.

		@param buf Buffer to copy data from.

		@return Reference to this buffer.
	*/
	HypCBuffer &operator =( const HypCBuffer &buf );

	/**
		@brief Index operator.

		Returns a referense to the element of the buffer at given
		position. If that position is invalid, then '@\0' charactes are
		appended to the buffer up to required size.

		@param pos Position within buffer.

		@return Reference to the element at specified position.
	*/
	char &operator []( size_t pos );

	/**
		@brief Constant index operator.

		Returns a constant referense to the element of the buffer at
		given position. If that position is invalid, then result is
		unspecified.

		@param pos Position within buffer.

		@return Constant reference to the element at specified position.
	*/
	inline const char &operator []( size_t pos ) const { return m_pBuffer[ pos + m_DataPtr ]; }

	/**
		@brief Get element by index.

		Returns a value of the buffer element at given position. If
		that position is invalid, then result is unspecified.

		@param pos Position within buffer.

		@return Value of the element at specified position.
	*/
	inline char at( size_t pos ) const { return m_pBuffer[ pos + m_DataPtr ]; }

	/**
		@brief Get data size.

		@return The length of the data in the buffer.
	*/
	inline size_t size() const { return m_DataLen; }

	/**
		@brief Get data size.

		@return The length of the data in the buffer.
	*/
	inline size_t length() const { return size(); }
	
	/**
		@brief Get pointer to the data.

		@return Pointer to the first element of the buffer.

		@note Calling any non-const member function for *this can
		      invalidate the pointer.
	*/
	inline char *data() const { return m_pBuffer + m_DataPtr; }

	/**
		@brief Get C string.

		Returns a pointer to a C string constructed by adding a
		terminating NULL character to the end of buffer.

		@return Pointer to C sting.

		@note Calling any non-const member function for *this can
		      invalidate the pointer.
	*/
	char *c_str();

	/**
		@brief Get copy of C string.

		Allocates free memory (by malloc), then copies data into
		allocated memory, appends a terminating NULL character and
		returns a pointer to this memory.

		@return Pointer to C sting copy.

		@note Returned pointer should be deleted by free().
	*/
	char *c_str_dup() const;

	/**
		@brief Reserve space in the buffer.

		Ensures that capacity() henceforth returns at least n.

		@param n Space to reserve (optional, defaults to 0).
	*/
	void reserve( size_t n = 0 );

	/**
		@brief Return size of allocated buffer.

		Returns the storage currently allocated to hold the buffer, a
		value at least as large as size().

		@return Size of allocated buffer.
	*/
	inline size_t capacity() const { return m_BufferLen; }

	/**
		@brief Append operator.

		Appends the given character to the end of the buffer.

		@param c Character to append.

		@return Reference to this buffer.
	*/
	inline HypCBuffer &operator +=( char c ) { return append( c ); }
	
	/**
		@brief Append operator.

		Appends the C string to the end of the buffer.

		@param str C string to append.

		@return Reference to this buffer.
	*/
	inline HypCBuffer &operator +=( const char * str ) { return append( str ); }

	/**
		@brief Append data array.

		Appends the given data array to the end of the buffer.

		@param pData Pointer to the data to append.
		@param dataLen Length of the data.

		@return Reference to this buffer.
	*/
	HypCBuffer &append( const void *pData, size_t dataLen );

	/**
		@brief Append sequence of characters.

		Appends the given number of characters to the end of the buffer.

		@param c Character to append.
		@param n Number of characters to append.

		@return Reference to this buffer.
	*/
	HypCBuffer &append( char c, size_t n );

	/**
		@brief Append single character.

		Appends the given character to the end of the buffer.

		@param c Character to append.

		@return Reference to this buffer.
	*/
	HypCBuffer &append( char c );

	/**
		@brief Append C string.

		Appends the C string to the end of the buffer.

		@param str C string to append.

		@return Reference to this buffer.
	*/
	HypCBuffer &append( const char * str );

	/**
		@brief Append buffer.

		Appends another buffer to the end of this buffer.

		@param b Reference to the buffer to append.

		@return Reference to this buffer.
	*/
	inline HypCBuffer &append( const HypCBuffer &b ) { return append( b.data(), b.size() ); }

	/**
		@brief Erase part of the buffer.

		Erases given number of elements from the given position.

		@param p0 Position to start data erasing (optional, defaults to
		          0).
		@param n Number of elements to erase (optional, defaults to
		         npos).

		@return Reference to this buffer.
	*/
	HypCBuffer &erase( size_t p0 = 0, size_t n = npos );

	/**
		@brief Insert data block.

		Inserts given data block at the specified position.

		@param p0 Position to insert data at.
		@param pData Pointer to the data block to insert.
		@param dataLen Length of the data.

		@return Reference to this buffer.
	*/
	HypCBuffer &insert( size_t p0, const char *pData, size_t dataLen );

	/**
		@brief Insert sequence of characters.

		Inserts given number of characters at the specified position.

		@param p0 Position to insert data at.
		@param c Character to instert.
		@param n Number of characters to insert.

		@return Reference to this buffer.
	*/
	HypCBuffer &insert( size_t p0, char c, size_t n );

	/**
		@brief Insert single character.

		Inserts single character at the specified position.

		@param p0 Position to insert data at.
		@param c Character to insert.

		@return Reference to this buffer.
	*/
	HypCBuffer &insert( size_t p0, char c );
	
	/**
		@brief Replace portion of data in the buffer.

		Replace portion of data with given data.

		@param p0 Position to start data replacing at.
		@param n0 Size of the old data.
		@param s Pointer to the new data.
		@param n Size of the new data.

		@return Reference to this buffer.
	*/
	HypCBuffer &replace( size_t p0, size_t n0, const char *s, size_t n );

	/**
		@brief Get user defined data type.

		@return User defined data type.
	*/
	inline int GetDataType() { return m_DataType; }

	/**
		@brief Set user defined data type.

		@param type User defined data type.
	*/
	inline void SetDataType( int type ) { m_DataType = type; }

	/**
		@brief Get user defined pointer.

		@return User defined pointer.
	*/
	inline size_t GetUserPtr() { return m_UserPtr; }

	/**
		@brief Set user defined pointer.

		@param ptr User defined pointer.
	*/
	inline void SetUserPtr( size_t ptr ) { m_UserPtr = ptr; }

protected:
	/**
		@brief Extend buffer.

		Extends buffer to the given size (the overall buffer size is
		rounded to the upper 64 bytes boundary).

		@param size Size to extend buffer on (optional, defaults to 0).
	*/
	void extend( size_t size = 0 );

	/// Pointer to the allocated buffer.
	char *m_pBuffer;

	/// Overall buffer length.
	size_t m_BufferLen;

	/// Length of data.
	size_t m_DataLen;

	/// Offset of the data in the buffer.
	size_t m_DataPtr;

	/// User defined data type.
	int m_DataType;

	/// User defined pointer.
	size_t m_UserPtr;
};

/**
	@}
*/

#endif //_HYPCBUFFER_HPP_
