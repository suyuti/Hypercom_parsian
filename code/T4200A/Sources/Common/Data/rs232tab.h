//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
#ifndef _RS232TAB_H_
#define _RS232TAB_H_
//=============================================================================
extern UBYTE RS232CFG( void );
extern void TableInitRS232( void );
extern void SetRSTabLRC( void );
extern void Assign_COM1_COM2( void );

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// RS232 configuration - MUST BE THE SAM AS APPLICATION MANAGER'S DEFINITION !!!
//
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define RSOPT_RSACTIVE	0x80	// USE RS232  
#define RSOPT_SENDESC	0x40	// SEND ESC SYMBOL
#define RSOPT_SENDEOT	0x20	// SEND EOT SYMBOL
#define RSOPT_SENDLEN	0x10	// HEX LENGTH IN HEADER
#define RSOPT_COMNUM	0x08	// 0=COM1/1=COM2 
#define RSOPT_FC		0x04	// FC on\off
#define RSOPT_FC_RXTX	0x02	// 0= FC RX, 1= FC RX\TX
#define RSOPT_PINNUM	0x01	// PIN PORT - 0=COM1/1=COM2 

struct rs232_rec
{
	UBYTE RSCOMPROTOCOL;		// COM PORT PROTOCOL
	UBYTE RSDPS;				// DATA BITS, PARITY, STOP BITS
	UWORD RSSPEED;				// PORT BAUD RATE
	UBYTE RSOPT;				// OPTION BYTE 1
	UBYTE RSPINPROTOCOL;		// PIN PORT PROTOCOL
	UBYTE HUBADDR[6];			// Bluetooth Hub Addr
	UBYTE UUID[2];				// Bluetooth UUID
	UBYTE RSRESV[23];			// RESERVED
} __attribute__((packed));
#define RSSZE  sizeof(struct rs232_rec)

#endif // _RS232TAB_H_
