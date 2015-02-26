/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __nu_compat_h
#define __nu_compat_h

#include <eftos.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@defgroup NUCLEUS Nucleus compatibility API
	@{

	This layer is not normally used by applications directly. It is
	subject to change without notice. Use HPOSFC wrappers instead.
*/

typedef unsigned int UNSIGNED;
typedef const char CHAR;
typedef void VOID;
typedef int STATUS;
typedef int OPTION;

/// Queue type
typedef struct __nu_queue
{
    CHAR *name;			///< Name of the queue
    VOID *start_address;	///< Start address
    UNSIGNED size;		///< Size of the queue
    OPTION message_type;	///< Message type
    UNSIGNED message_size;	///< Message size
    OPTION suspend_type;	///< Suspend type
    UNSIGNED used_size;		///< Used size
    BOOL not_empty;		///< Queue not empty flag
    BOOL updated;		///< Queue updated flag
} NU_QUEUE;

/// Success
#define NU_SUCCESS           0
/// Invalid operation
#define NU_INVALID_OPERATION -13
/// Invalid size
#define NU_INVALID_SIZE      -22
/// Queue was deleted
#define NU_QUEUE_DELETED     -44
/// Queue is empty
#define NU_QUEUE_EMPTY       -45
/// Queue is full
#define NU_QUEUE_FULL        -46
/// Queue was resetted
#define NU_QUEUE_RESET       -47
/// Timeout
#define NU_TIMEOUT           -50

/// FIFO method for queue
#define NU_FIFO 0

/// Queue have fixed size
#define NU_FIXED_SIZE 0

/// Non-blocking access to the queue
#define NU_NO_SUSPEND 0
/// Blocking access to the queue
#define NU_SUSPEND    ( ( UNSIGNED )-1 )

/**
	@brief Create queue.

	@param queue Pointer to structure to be initialized.
	@param name Name for the queue.
	@param start_address Start address for the queue data.
	@param queue_size Queue size in size of UNSIGNED type.
	@param message_type Message type (not used).
	@param message_size Message size (not used).
	@param suspend_type Suspend type (not used).

	@return Nucleus error codes.
*/
STATUS NU_Create_Queue( NU_QUEUE *queue, CHAR *name, VOID *start_address,
                        UNSIGNED queue_size, OPTION message_type,
                        UNSIGNED message_size, OPTION suspend_type );

/**
	@brief Delete queue.

	@param queue Handle initialized with NU_Create_Queue.

	@return Nucleus error codes.
*/
STATUS NU_Delete_Queue( NU_QUEUE *queue );

/**
	@brief Send to front of queue.

	@param queue Handle initialized with NU_Create_Queue.
	@param message Pointer to the message to post.
	@param size Message size in size of UNSIGNED type.
	@param suspend Suspend method for message posting.

	@return Nucleus error codes.
*/
STATUS NU_Send_To_Front_Of_Queue( NU_QUEUE *queue, VOID *message,
                                  UNSIGNED size, UNSIGNED suspend );

/**
	@brief Send to queue.

	@param queue Handle initialized with NU_Create_Queue.
	@param message Pointer to the message to post.
	@param size Message size in size of UNSIGNED type.
	@param suspend Suspend method for message posting.

	@return Nucleus error codes.
*/
STATUS NU_Send_To_Queue( NU_QUEUE *queue, VOID *message, UNSIGNED size,
                         UNSIGNED suspend );

/**
	@brief Receive from queue.

	@param queue Handle initialized with NU_Create_Queue.
	@param message Pointer to the buffer to read message.
	@param size Message size in size of UNSIGNED type.
	@param actual_size Actual size of data read from the queue in size of
	                   UNSIGNED type.
	@param suspend Suspend method for message reading.

	@return Nucleus error codes.
*/
STATUS NU_Receive_From_Queue( NU_QUEUE *queue, VOID *message, UNSIGNED size,
                              UNSIGNED *actual_size, UNSIGNED suspend );

/**
	@}
*/


#ifdef __cplusplus
}
#endif

#endif // __nu_compat_h
