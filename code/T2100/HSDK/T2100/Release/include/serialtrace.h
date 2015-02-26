//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2005
//=============================================================================

#ifndef _SERIALTRACE_H_
#define _SERIALTRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Start serial port trace (circular buffer).

	@param nCh Port number.
	@param param Size of the trace buffer (not used if trace was previously
	             suspended).

	@return 0: Trace not started due to error.<br>
	        1: Trace started successfully.

*/
extern int SerialTraceStart( unsigned int nCh, int param );

/**
	@brief Start serial port trace (non-circular buffer).

	@param nCh Port number.
	@param param Size of the trace buffer (not used if trace was previously
	             suspended).

	@return 0: Trace not started due to error.<br>
	        1: Trace started successfully.
*/
extern int SerialTraceStartOnePass( unsigned int nCh, int param );

/**
	@brief Stop serial port trace.

	@param nCh Port number.
*/
extern void SerialTraceStop( unsigned int nCh );

/**
	@brief Suspend serial port trace.

	The tracing can be resumed by calling ScardTraceStart() or
	ScardTraceStartOnePass(). The mode of tracing (circular or not) will
	not be changed on resuming with either call.

	@param nCh Port number.
*/
extern void SerialTraceSuspend( unsigned int nCh );

/**
	@brief Get pointer to the serial port trace buffer.

	@param nCh Port number.

	@return Pointer to the serial port trace buffer.
*/
extern char * SerialTraceGetBuf( unsigned int nCh );

/**
	@brief Get used length of the serial port trace buffer.

	@param nCh Port number.

	@return Used length of the serial port trace buffer.
*/
extern unsigned int SerialTraceGetBufLen( unsigned int nCh );

/**
	@brief Reset serial port trace buffer.

	Moves the buffer pointer to the start of the serial port trace buffer.

	@param nCh Port number.
*/
extern void SerialTraceResetOffset( unsigned int nCh );

/**
	@brief Print user messages in the serial port trace.

	@param nCh Port number.
	@param buf NULL-terminated string to appear in the trace.
*/
extern void SerialTracePrint( unsigned int nCh, const char * buf );

/**
	@brief Reads data from the serial port trace buffer.

	@param nCh Port number.
	@param buf Pointer to the buffer to put trace into.
	@param count Size of the data block to read from trace.

	@return The number of bytes read or zero if buffer end reached.
*/
extern int SerialTraceReadBuf( unsigned int nCh, void *buf, unsigned int count );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif //_SERIALTRACE_H_
