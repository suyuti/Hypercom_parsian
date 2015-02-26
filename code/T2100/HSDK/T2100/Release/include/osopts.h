/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef osopts_h
#define osopts_h

#define MAKE_EPROM 0	/* Making RAM-based image */

#define INCLBUZ 1		/* 1 if BUZ driver included */

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#define INCLCOM 0		/* 1 if COM driver included */
#define INCLETH 0	   	/* 1 if ETH driver included */
#define INCLLAN 0		/* 1 if LAN driver included */
#else
#define INCLCOM 1		/* 1 if COM driver included */
#define INCLETH 0	   	/* 1 if ETH driver included */
#define INCLLAN 0		/* 1 if LAN driver included */
#endif

#define INCLCRB 1		/* 1 if CRB driver included */
#define INCLCRD 1		/* 1 if CRD driver included */
#define INCLCR3 1		/* 1 if CR3 driver included  */

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#define INCLDSP 0		/* 1 if DSP driver included */
#else
#define INCLDSP 1		/* 1 if DSP driver included */
#endif

#if defined(MAKE_OPTIMUM) || defined(MAKE_M2)
#define INCLPAD 1		/* 1 if PAD driver included */
#else
#define INCLPAD 0		/* 1 if PAD driver included */
#endif
#define	INCLPAD_SIGCAP 0	/* 1 if PAD driver supports Signature Capture */

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#define INCLKBD 0		/* 1 if KBD driver included */
#else
#define INCLKBD 1		/* 1 if KBD driver included */
#endif

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#define	INCLLPT 0		/* 1 if LPT driver included */
#else
#define	INCLLPT 1		/* 1 if LPT driver included */
#endif

#define INCLPIN 1		/* 1 if PIN driver included */
#define INCLPRT 1		/* 1 if PRT driver included */

#define INCLRTC 1		/* 1 if RTC driver included */

#define INCLMCD 0		/* 1 if MCD driver included */
#define INCLSCR 1		/* 1 if SCR driver included */
#define INCLSC1 1		/* 1 if SC1 driver included */
#define INCLSC2 1		/* 1 if SC2 driver included */
#define INCLSC3 1		/* 1 if SC3 driver included */
#define INCLSC4 1		/* 1 if SC4 driver included */
#define INCLSAM (INCLSC1 | INCLSC2 | INCLSC3 | INCLSC4)

#define INCLEEP 0		/* 1 if EEP driver included */
#define INCLTGI 0		/* 1 if TGI driver included */
#define INCLSP1 0		/* 1 if SP1 driver included */
#define INCLATM 0		/* 1 if ATM driver included */
#define INCLRAP 0		/* 1 if RAP driver included */
#define INCLRS2 0		/* 1 if RS2 driver included */
#define INCLRS3 0		/* 1 if RS3 driver included */
#define INCLP9I 0		/* 1 if P9I driver included */
#define INCLZ8I 0		/* 1 if Z8I driver included */

#endif // osopts_h

