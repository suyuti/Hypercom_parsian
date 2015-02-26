/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _ddtm_H_
#define _ddtm_H_

/**
	Device declarations.
*/
typedef enum {
    NUL  = 0x00,	/**< NUL device. */
    PIOA = 0x01,	/**< PIO A device (not supported). */
    PIOB = 0x02,	/**< PIO B device (not supported). */
    SIOA = 0x03,	/**< SIO A device (not supported). */
    SIOB = 0x04,	/**< SIO B device (not supported). */
    PIOC = 0x05,	/**< PIO C device (not supported). */
    PIOD = 0x06,	/**< PIO D device (not supported). */
    SIOC = 0x07,	/**< SIO C device (not supported). */
    SIOD = 0x08,	/**< SIO D device (not supported). */
    BUZ  = 0x10,	/**< Buzzer device. */
    COM  = 0x11,	/**< Communication device. */
    CRB  = 0x12,	/**< MSR track 2 device. */
    CRD  = 0x13,	/**< MSR track 1 device. */
    CR3  = 0x14,	/**< MSR track 3 device. */
    DSP  = 0x15,	/**< Display device. */
    EEP  = 0x16,	/**< EEPROM device (not supported). */
    ETH  = 0x17,	/**< Ethernet device (not supported). */
    KBD  = 0x18,	/**< Keyboard device. */
    LAN  = 0x19,	/**< POS LAN device (not supported). */
    LPT  = 0x1A,	/**< Printer device. */
    MCD  = 0x1B,	/**< Memory card device (not supported). */
    PAD  = 0x1C,	/**< Touchpad device (not supported). */
    PIN  = 0x1D,	/**< PIN interface device. */
    PRT  = 0x1E,	/**< Serial printer interface device. */
    RTC  = 0x1F,	/**< Real-time clock device. */
    SC1  = 0x20,	/**< SAM 1 device (supported only via Smart Card Library). */
    SC2  = 0x21,	/**< SAM 2 device (supported only via Smart Card Library). */
    SC3  = 0x22,	/**< SAM 3 device (supported only via Smart Card Library). */
    SC4  = 0x23,	/**< SAM 4 device (supported only via Smart Card Library). */
    SCR  = 0x24,	/**< Smart card device (supported only via Smart Card Library). */
    Z8I  = 0x25,	/**< Internal PIN PAD (use Cryposervice Library instead). */
    RS2  = 0x26,	/**< RS232 port 2 (not supported). */
    RS3	 = 0x27,	/**< RS232 port 3 (not supported). */
    BLT  = 0x28		/**< ViperProxy process PCX images (not supported). */
} PORT;

#endif	// _ddtm_H_
