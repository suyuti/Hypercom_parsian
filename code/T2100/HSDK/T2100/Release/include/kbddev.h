/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __kbddev_h
#define __kbddev_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@brief Config mode equates (IOCSMODE) for KBD device.
*/
enum KBD_IOCSMODE {
	KBD_MODE_UNTR = 0x00,	/**< Return untranslated keys.
				                 Bit 7 is set if shift is pressed. */
	KBD_MODE_TRANS1 = 0x01,	/**< Return translated keys.
				                 IOCSDDDA points to 41 key mask
				                 (35+6 no shift key). */
	KBD_MODE_TRANS2 = 0x02,	/**< Return translated keys.
				                 IOCSDDA points to 82 key mask
				                 (35+6, 35+6 shifted) (not supported). */
	KBD_MODE_TRANS3 = 0x03,	/**< Return translated keys.
				                 IOCSDDDA points to a 73 key mask
				                 (35+6+4+26+2 no shift key) (not supported). */
	KBD_MODE_TRANS4 = 0x04,	/**< Return translated keys.
				                 IOCSDDDA points to a 146 key mask
				                 (35+6+4+26+2 *2 shift key) (not supported). */
	KBD_MODE_TYPE = 0x05,	/**< Return keyboard type. */
	KBD_MODE_PF = 0x06,	    /**< Set paperfeed key scan code.
				                 IOCSDDDA contains the scan code (not supported). */
	KBD_MODE_CNTRST = 0x07	/**< Set contrast up & contrast down key scan codes.
				                 IOCSDDDA contains the scan codes, contrast
				                 up first value (not supported). */
};

/**
	@brief Returned keyboard types when KBD_MODE_TYPE command is used.
*/
enum KBD_MODE_TYPES {
    KBD_STD = 0x00,         // Standard 35 key keypad
    KBD_QWERTY = 0x01,      // QWERTY keypad
    KBD_PIN = 0x02,         // PIN Style keypad
};

#if defined(MAKE_OPTIMUM)
/**
	@brief The Symbols for the Raw Key Scan codes.
           T7/T4100 Standard/PIN without shift key.
*/
enum KBD_T7_STD {
    K0 = 0x1F,    // '0' key
    K1 = 0x19,    // '1' key
    K2 = 0x18,    // '2' key
    K3 = 0x17,    // '3' key
    K4 = 0x12,    // '4' key
    K5 = 0x11,    // '5' key
    K6 = 0x10,    // '6' key
    K7 = 0x0B,    // '7' key
    K8 = 0x0A,    // '8' key
    K9 = 0x09,    // '9' key

    KHASH = 0x1E,    // '#' key
    KSTAR = 0x20,    // '*' key

    KCLEAR = 0x16,    // Clear key
    KENTER = 0x1D,    // Enter key
    KCANCEL = 0x01,    // Cancel key
    KBACKSP = 0x0F,    // Backspace key

    KBSP = 0x48,    // Backspace key on QWERTY style T7
    KDEL = 0x0F,    // DEL key on QWERTY style T7

    KSBSP = 0xC8,    // Shift+Backspace key on QWERTY style T7
    KSDEL = 0x8F,    // Shift+DEL key on QWERTY style T7

    KSCLEAR = 0x96,    // Shift+Clear key
    KSENTER = 0x9D,    // Shift+Enter key
    KSCANCEL = 0x81,   // Shift+Cancel key
    KSBACKSP = 0x8F,   // Shift+Backspace key

    KM1 = 0x28,    // Menu key 1
    KM2 = 0x27,    // Menu key 2
    KM3 = 0x26,    // Menu key 3
    KM4 = 0x25,    // Menu key 4
    KM5 = 0x24,    // Menu key 5
    KM6 = 0x23,    // Menu key 6

    KSM1 = 0xA8,   // Shift+Menu key 1
    KSM2 = 0xA7,   // Shift+Menu key 2
    KSM3 = 0xA6,   // Shift+Menu key 3
    KSM4 = 0xA5,   // Shift+Menu key 4
    KSM5 = 0xA4,   // Shift+Menu key 5
    KSM6 = 0xA3,   // Shift+Menu key 6
};

/**
	@brief T7 function keys (both shifted and not).
*/
enum KBD_T7_FUNC {
    KF1 = 0x05,     // Function key 1
    KF2 = 0x0C,     // Function key 2
    KF3 = 0x13,     // Function key 3
    KF4 = 0x1A,     // Function key 4
    KF5 = 0x21,     // Function key 5
    KF6 = 0x08,     // Function key 6
    KF7 = 0x06,     // Function key 7
    KF8 = 0x16,     // Function key 8 ( same as KCLEAR )
    KF9 = 0x04,     // Function key 9
    KF10 = 0x03,    // Function key 10
    KF11 = 0x02,    // Function key 11
    KF12 = 0x01,    // Function key 12 ( same as KCANCEL )

    KF13 = 0x0D,    // Function key 13
    KF14 = 0x14,    // Function key 14
    KF15 = 0x1B,    // Function key 15
    KF16 = 0x00,    // Function key 16
    KF17 = 0x07,    // Function key 17
    KF18 = 0x0E,    // Function key 18
    KF19 = 0x15,    // Function key 19
    KF20 = 0x1C,    // Function key 20

    KSF1 = 0x85,    // Shift+Function key 1
    KSF2 = 0x8C,    // Shift+Function key 2
    KSF3 = 0x93,    // Shift+Function key 3
    KSF4 = 0x9A,    // Shift+Function key 4
    KSF5 = 0xA1,    // Shift+Function key 5
    KSF6 = 0x88,    // Shift+Function key 6
    KSF7 = 0x86,    // Shift+Function key 7
    KSF8 = 0x96,    // Shift+Function key 8
    KSF9 = 0x84,    // Shift+Function key 9
    KSF10 = 0x83,   // Shift+Function key 10
    KSF11 = 0x82,   // Shift+Function key 11
    KSF12 = 0x81,   // Shift+Function key 12

    KSF13 = 0x8D,   // Shift+Function key 13
    KSF14 = 0x94,   // Shift+Function key 14
    KSF15 = 0x9B,   // Shift+Function key 15
    KSF16 = 0x80,   // Shift+Function key 16
    KSF17 = 0x87,   // Shift+Function key 17
    KSF18 = 0x8E,   // Shift+Function key 18
    KSF19 = 0x95,   // Shift+Function key 19
    KSF20 = 0x9C,   // Shift+Function key 20
};

/**
	@brief T7 Standard/PIN with shift key.
*/
enum KBD_T7_PIN_SHIFT {
    KSSA = 0x86,    // Shift+'A' key
    KSSB = 0x85,    // Shift+'B' key
    KSSC = 0x84,    // Shift+'C' key
    KSSD = 0x83,    // Shift+'D' key
    KSSE = 0x82,    // Shift+'E' key
    KSSF = 0x80,    // Shift+'F' key
    KSSG = 0x8D,    // Shift+'G' key
    KSSH = 0x8C,    // Shift+'H' key
    KSSI = 0x8B,    // Shift+'I' key
    KSSJ = 0x8A,    // Shift+'J' key
    KSSK = 0x89,    // Shift+'K' key
    KSSL = 0x87,    // Shift+'L' key
    KSSM = 0x94,    // Shift+'M' key
    KSSN = 0x93,    // Shift+'N' key
    KSSO = 0x92,    // Shift+'O' key
    KSSP = 0x91,    // Shift+'P' key
    KSSQ = 0x90,    // Shift+'Q' key
    KSSR = 0x8E,    // Shift+'R' key
    KSSS = 0x9B,    // Shift+'S' key
    KSST = 0x9A,    // Shift+'T' key
    KSSU = 0x99,    // Shift+'U' key
    KSSV = 0x98,    // Shift+'V' key
    KSSW = 0x97,    // Shift+'W' key
    KSSX = 0x95,    // Shift+'X' key
    KSSY = 0xA0,    // Shift+'Y' key
    KSSZ = 0x9F,    // Shift+'Z' key
    KSSPACE = 0x9E,    // Shift+Space key

    KPSI = 0x99,    // Shift+'I' key on PIN style T7Plus
    KPSJ = 0x98,    // Shift+'J' key on PIN style T7Plus
    KPSK = 0x97,    // Shift+'K' key on PIN style T7Plus

    KPSU = 0x8B,    // Shift+'U' key on PIN style T7Plus
    KPSV = 0x8A,    // Shift+'V' key on PIN style T7Plus
    KPSW = 0x89,    // Shift+'W' key on PIN style T7Plus
};

#elif defined(MAKE_VIKING)

/**
	@brief The symbols for the raw key scan codes.
*/
enum KBD_SCAN_CODES {
	KUP = 0x02,		    /**< Up key. */
	KDOWN = 0x03,		/**< Down key. */
	KSELECT = 0x05,		/**< Select key. */
	KCANCEL = 0x04,		/**< Cancel key. */
	K9 = 0x17,		    /**< Key '9'. */
	K8 = 0x18,		    /**< Key '8'. */
	K7 = 0x19,		    /**< Key '7'. */
	K6 = 0x10,		    /**< Key '6'. */
	K5 = 0x11,		    /**< Key '5'. */
	K4 = 0x12,		    /**< Key '4'. */
	K3 = 0x09,		    /**< Key '3'. */
	K2 = 0x0A,		    /**< Key '2'. */
	K1 = 0x0B,		    /**< Key '1'. */
	KENTER = 0x1E,		/**< Enter key. */
	K0 = 0x1F,		    /**< Key '0'. */
	KCLEAR = 0x20		/**< Clear key. */
};
#endif

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif //__kbddev_h
