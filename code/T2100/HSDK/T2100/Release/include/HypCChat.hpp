//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCCHAT_HPP_
#define _HYPCCHAT_HPP_

#include <eftos.h>
#include "HypCTransport.hpp"
#include "HypCTransparentCommon.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "SimpleVector.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Chat script interpreter.

	This class provides controlling routines for modem and used by the
	HypCModem derived classes to establish control modem.

	See @ref chat "chat script" section for description of the language.
*/
class HypCChat : virtual public HypCTransparentCommon
{
private:
	/**
		@brief Internal representation of parsed script tokens.
	*/
	enum HypEChatToken 
	{
		subseq = 0x01,          ///< Sub-expression (i.e. second level).
		expect = 0x02,          ///< Expect string.
		add_cr = 0x04,          ///< Add carriage return to the line.
		command = 0x100,        ///< Commands bias.
		command_timeout = command, ///< Timeout.
		command_pause,          ///< Pause.
		command_flush,          ///< Flush.
		command_mode            ///< Mode.
	};

	/**
		@brief Precompiled mode values.
	*/
	enum HypEChatMode
	{
		MODE_8N1,               ///< 8 bits, no parity, 1 stop bit.
		MODE_7E1,               ///< 7 bits, even parity, 1 stop bit.
		MODE_7O1                ///< 7 bits, odd parity, 1 stop bit.
	};

protected:
	/**
		@brief Internal events generated during chat script processing.
	*/
	enum ChatEvents
	{
		CHAT_DONE = HypCTransparentCommon::LAST_EV_TAG, ///< Successfully done.
		CHAT_ERROR,             ///< Done with error.
		CHAT_TIMER_EXP,         ///< Timer expired.
		CHAT_TOKEN_MATCHED,     ///< Token successfully matched.
		CHAT_TOKEN_NOT_MATCHED, ///< Token not matched.
		CHAT_NEED_MORE_DATA,    ///< More data required to make design.
		CHAT_COMMAND_OK,        ///< Command successfully processed.
		LAST_EV_TAG             ///< Last event used by HypCChat.
	};

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCChat( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCChat();

protected:
	/**
		@brief Executes script.

		@param s1 First part of script.
		@param s2 Second part of script (optional, defaults to NULL).

		@note Each part of script should be self-complete. The mode
		      for each part of the script is reset to 8N1 before
		      parsing.
	*/
	void ChatStateMachine( const char *s1, const char *s2 = NULL );

private:
	/**
		@brief Tokenize script.

		@param p Script to parse.
	*/
	void Tokenize( const char *p );

	/**
		@brief Parse single token.

		@param p Reference to the pointer to token. On exit will point
		         to the next token.
	*/
	void ParseToken( const char *&p );

	/**
		@brief Parse number.

		@param p Reference to the pointer to number. On exit will point
		         to the data after number.
	*/
	unsigned long ReadNumber( const char * &p );

	/**
		@brief Save number in the HypCBuffer.

		@param m Number to store.
		@param res Reference to the buffer to store number.
	*/
	void SaveNumber( size_t m, HypCBuffer &res );

	/**
		@brief Restore number from HypCBuffer.

		@param b Reference to the buffer to restore number from.

		@return Restored number.
	*/
	size_t RestoreNumber( HypCBuffer &b ) const;

	/**
		@brief Read single word from the string.

		@brief p Reference to the pointer to the string to parse. On
		         exit will point to the data after word.
		@prief res Reference to the buffer to store word in.

		@note The word is the either value defined in plain or defined
		      in quotes.
	*/
	void ReadWord( const char *&p, HypCBuffer &res );

	/**
		@brief Handler for expect state.
	*/
	void StChatExpect();

	/**
		@brief Handler for send state.
	*/
	void StChatSend();

	/**
		@brief Stop chat script execution.
	*/
	void ChatStop();

	/**
		@brief Interpret single command.
	*/
	void InterpretCommand();

	/**
		@brief Compare expect data with the stream.

		Compare expect portion of the string with the data currently
		in the buffer.
	*/
	void Compare();

	/**
		@brief Check if abort string is present in the buffer.
	*/
	bool CheckAbort();

	/**
		@brief Start read from the underlying level.
	*/
	void ChatStartRead();

	/**
		@brief Wait for event.
	*/
	void ChatWaitEvent();

private:
	/// Parsed script.
	SimpleVector< HypCBuffer > m_Script;

	/// Parsed abort strings.
	SimpleVector< HypCBuffer > m_Abort;

	/// Pointer to the currently processed script token.
	HypCBuffer *m_Iter;

	/// Pointer to the position in token.
	size_t m_TokenPtr;

	/// Pointer to the position in read buffer.
	size_t m_ReadPtr;

	/// Current timeout value
	unsigned int m_Timeout;

	/// Execution timer.
	HypCTimer m_Timer;

	/// Encoding mode.
	HypEChatMode m_Mode;

protected:
	/// Index of matched abort string.
	int m_AbortIdx;
};

/**
	@}
*/

/**
	@page chat Chat Script

	This documentation is derived from the public domain man page for chat
	program.

	@section description Description

	The script defines a conversational exchange between the terminal
	application and the modem. Its primary purpose is to establish the
	connection to the host. It is designed for both regular dial and logon
	script implementation.

	@section script Chat Script

	The chat script defines the communications.

	A script consists of one or more "expect-send" pairs of strings,
	separated by spaces, with an optional "subexpect-subsend" string pair,
	separated by a dash as in the following example:

	@verbatim
ogin:-BREAK-ogin: ppp ssword: hello2u2@endverbatim

	This line indicates that the chat program should expect the string
	"ogin:". If it fails to receive a login prompt within the time interval
	allotted, it is to send a break sequence to the remote and then expect
	the string "ogin:". If the first "ogin:" is received then the break
	sequence is not generated.

	Once it received the login prompt the chat program will send the string
	ppp and then expect the prompt "ssword:". When it receives the prompt
	for the password, it will send the password hello2u2.

	A carriage return is normally sent following the reply string. It is
	not expected in the "expect" string unless it is specifically requested
	by using the @\@\r character sequence.

	The expect sequence should contain only what is needed to identify the
	string. Since it is normally stored on a disk file or in precompiled
	binary, it should not contain variable information. It is generally not
	acceptable to look for time strings, network identification strings, or
	other variable pieces of data as an expect string.

	To help correct for characters which may be corrupted during the
	initial sequence, look for the string "ogin:" rather than "login:". It
	is possible that the leading "l" character may be received in error and
	you may never find the string even though it was sent by the system.
	For this reason, scripts look for "ogin:" rather than "login:" and
	"ssword:" rather than "password:".

	A very simple script might look like this: 
	@verbatim
ogin: ppp ssword: hello2u2@endverbatim

	In other words, expect ....ogin:, send ppp, expect ...ssword:, send
	hello2u2. 

	In actual practice, simple scripts are rare. At the vary least, you
	should include sub-expect sequences should the original string not be
	received. For example, consider the following script:

	@verbatim
ogin:--ogin: ppp ssword: hello2u2@endverbatim

	This would be a better script than the simple one used earlier. This
	would look for the same login: prompt, however, if one was not
	received, a single return sequence is sent and then it will look for
	login: again. Should line noise obscure the first login prompt then
	sending the empty line will usually generate a login prompt again.

	@section abort Abort Strings

	Many modems will report the status of the call as a string. These
	strings may be CONNECTED or NO CARRIER or BUSY. It is often desirable
	to terminate the script should the modem fail to connect to the remote.
	The difficulty is that a script would not know exactly which modem
	string it may receive. On one attempt, it may receive BUSY while the
	next time it may receive NO CARRIER.

	These "abort" strings may be specified in the script using the ABORT
	sequence. It is written in the script as in the following example:

	@verbatim
ABORT BUSY ABORT 'NO CARRIER' '' ATZ OK ATDT5551212 CONNECT@endverbatim

	This sequence will expect nothing; and then send the string ATZ. The
	expected response to this is the string OK. When it receives OK, the
	string ATDT5551212 to dial the telephone. The expected string is
	CONNECT. If the string CONNECT is received the remainder of the script
	is executed. However, should the modem find a busy telephone, it will
	send the string BUSY. This will cause the string to match the abort
	character sequence. The script will then fail because it found a match
	to the abort string. If it received the string NO CARRIER, it will
	abort for the same reason. Either string may be received. Either string
	will terminate the chat script.

	@section timeout TIMEOUT

	The initial timeout value is 45 seconds.

	To change the timeout value for the next expect string, the following
	example may be used:

	@verbatim
ATZ OK ATDT5551212 CONNECT TIMEOUT 10 ogin:--ogin: ppp TIMEOUT 5 assword: hello2u2@endverbatim

	This will change the timeout to 10 seconds when it expects the login:
	prompt. The timeout is then changed to 5 seconds when it looks for the
	password prompt.

	The timeout, once changed, remains in effect until it is changed again.

	@section mode MODE

	Changes serial data encoding mode. Could be one of the following: 8N1,
	7E1, 7O1.

	@section flush FLUSH

	Bypass and ignore all data currently in receive buffer. Usually used in
	the beginning of the script to discard pending data that might contain
	abort strings.

	@section delay DELAY, PAUSE

	To set the delay in the script, the following example may be used:

	@verbatim
ATZ OK ATDT5551212 CONNECT ogin:--ogin: DELAY 1 ppp TIMEOUT 5 assword: PAUSE 500 hello2u2@endverbatim

	This will delay 1 second before sending login information and 500 ms
	before sending password information.

	@section break Generating Break (Currently Ignored)

	The special reply string of BREAK will cause a break condition to be
	sent. The break is a special signal on the transmitter. The normal
	processing on the receiver is to change the transmission rate. It may
	be used to cycle through the available transmission rates on the remote
	until you are able to receive a valid login prompt. The break sequence
	may be embedded into the send string using the @\K sequence.

	@section escape Escape Sequences

	The expect and reply strings may contain escape sequences. All of the
	sequences are legal in the reply string. Many are legal in the expect.
	Those which are not valid in the expect sequence are so indicated.

	@li <tt>''</tt> or <tt>""</tt> - Expects or sends a null string. If you
	    send a null string then it will still send the return character.
	    This sequence may either be a pair of apostrophe or quote
	    characters.
	@li <tt>@\b</tt> - Represents a backspace character.
	@li <tt>@\c</tt> or <tt>@\C</tt> - Suppresses the newline at the end
	    of the reply string. This is the only method to send a string
	    without a trailing return character. It must be at the end of the
	    send string. For example, the sequence hello@\c will simply send
	    the characters h, e, l, l, o (not valid in expect).
	@li <tt>@\K</tt> - Insert a BREAK (not valid in expect, currently sent
	    as 0 to modem).
	@li <tt>@\n</tt> or <tt>@\N</tt> - Send a newline or linefeed character.
	@li <tt>@\P</tt> - Replaced with the password from addressbook.
	@li <tt>@\r</tt> or <tt>@\R</tt> - Send or expect a carriage return.
	@li <tt>@\s</tt> - Represents a space character in the string. This may
	    be used when it is not desirable to quote the strings which
	    contains spaces. The sequence 'HI TIM' and HI@\sTIM are the same.
	@li <tt>@\t</tt> - Send or expect a tab character.
	@li <tt>@\T</tt> - Replaced with the telephone number from addressbook.
	@li <tt>@\U</tt> - Replaced with the username from addressbook.
	@li <tt>@\xNN</tt> or <tt>@\XNN</tt> - Substitute the sequence with the
	    character represented by hexadecimal value NN.
	@li <tt>@\@\</tt> - Send or expect a backslash character.

	@section terminate Termination Codes

	The chat program will terminate with the following completion codes. 
	@li <tt>0</tt> - The normal exit. This indicates that the script was
	    executed without error to the normal conclusion. 
	@li <tt>-1</tt> - The script execution is unsuccessful.

	@section notes Notes

	The NUL character is reserved for sending BREAK command (currently
	passed as is).

	PAD logon script should start from "send" string, for example:

	@verbatim
"" : TEST Open \C \R\N@endverbatim

	(send carriage  return, expect ":", send "TEST", expect "Open", send
	nothing, expect bytes 0x0d 0x0a).
*/

#endif // _HYPCCHAT_HPP_
