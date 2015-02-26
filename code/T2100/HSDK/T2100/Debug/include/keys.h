/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	__keys_h
#define	__keys_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Symbolic definitions for the keys.

	Symbolic definitions for the keys on the numeric keypad and the
	external keyboard. For compatibility, keys on the numeric keypad
	have the same value as similar keys on the external keyboard.
*/
enum KEY_DEFS {
	KEY_BACKSPACE = 0x08,		///< Key Backspace.
	KEY_TAB = 0x09,			///< Key Tab.
	KEY_ENTER = 0x0D,		///< Key Enter.
	KEY_ESCAPE = 0x1B,		///< Key Escape.
	KEY_CANCEL = KEY_ESCAPE,	/**< Key Cancel (for compatibility,
					     have the same value as Escape
					     on external keyboard). */
	KEY_F1 = 0xA1,			///< Key F1.
	KEY_F2 = 0xA2,			///< Key F2.
	KEY_F3 = 0xA3,			///< Key F3.
	KEY_F4 = 0xA4,			///< Key F4.
	KEY_F5 = 0xA5,			///< Key F5.
	KEY_F6 = 0xA6,			///< Key F6.
	KEY_F7 = 0xA7,			///< Key F7.
	KEY_F8 = 0xA8,			///< Key F8.
	KEY_F9 = 0xA9,			///< Key F9.
	KEY_F10 = 0xAA,			///< Key F10.
	KEY_F11 = 0xAB,			///< Key F11.
	KEY_F12 = 0xAC,			///< Key F12.
	KEY_UP_ARROW = 0x95,		///< Key Up.
	KEY_DOWN_ARROW = 0x96,		///< Key Down.
	KEY_LEFT_ARROW = 0x93,		///< Key Left.
	KEY_RIGHT_ARROW = 0x99,		///< Key Right.
	KEY_INSERT = 0x8F,		///< Key Insert.
	KEY_DELETE = 0x7F,		///< Key Delete.
	KEY_HOME = 0x94,		///< Key Home.
	KEY_END = 0x90,			///< Key End.
	KEY_PAGE_UP = 0x97,		///< Key Page Up.
	KEY_PAGE_DOWN = 0x98,		///< Key Page Down.
	KEY_CAPS_LOCK = 0x9B,		///< Key Caps Lock.
	KEY_NUM_LOCK = 0x8B,		///< Key Num Lock.
	KEY_SCROLL_LOCK_ON = 0x84,	///< Key Scroll Lock On.
	KEY_SCROLL_LOCK_OFF = 0x85,	///< Key Scroll Lock Off.
	KEY_WINDOWS = 0x83,		///< Key Windows.
	KEY_MENU = 0x81,		///< Key Menu.
	KEY_APPLICATION = KEY_MENU,	/**< For backward compatibility
					     KEY_APPLICATION has been renamed
					     to KEY_MENU. */
	KEY_PRINT_SCREEN = 0x8C,	///< Key Print Screen.
	KEY_PAUSE = 0x8E,		///< Key Pause.
	KEY_SHIFT_TAB = 0x1E,		///< Key combination of Shift and Tab.
	KEY_SHIFT_F1 = 0xAD,		///< Key combination of Shift and F1.
	KEY_SHIFT_F2 = 0xAE,		///< Key combination of Shift and F2.
	KEY_SHIFT_F3 = 0xAF,		///< Key combination of Shift and F3.
	KEY_SHIFT_F4 = 0xB0,		///< Key combination of Shift and F4.
	KEY_SHIFT_F5 = 0xB1,		///< Key combination of Shift and F5.
	KEY_SHIFT_F6 = 0xB2,		///< Key combination of Shift and F6.
	KEY_SHIFT_F7 = 0xB3,		///< Key combination of Shift and F7.
	KEY_SHIFT_F8 = 0xB4,		///< Key combination of Shift and F8.
	KEY_SHIFT_F9 = 0xB5,		///< Key combination of Shift and F9.
	KEY_SHIFT_F10 = 0xB6,		///< Key combination of Shift and F10.
	KEY_SHIFT_F11 = 0xB7,		///< Key combination of Shift and F11.
	KEY_SHIFT_F12 = 0xB8,		///< Key combination of Shift and F12.
	KEY_CTRL_A = 0x01,		///< Key combination of Ctrl and A.
	KEY_CTRL_B = 0x86,		///< Key combination of Ctrl and B.
	KEY_CTRL_C = 0x87,		///< Key combination of Ctrl and C.
	KEY_CTRL_D = 0x04,		///< Key combination of Ctrl and D.
	KEY_CTRL_E = 0x05,		///< Key combination of Ctrl and E.
	KEY_CTRL_F = 0x06,		///< Key combination of Ctrl and F.
	KEY_CTRL_G = 0x07,		///< Key combination of Ctrl and G.
	KEY_CTRL_H = 0x08,		///< Key combination of Ctrl and H.
	KEY_CTRL_I = 0x09,		///< Key combination of Ctrl and I.
	KEY_CTRL_J = 0x0A,		///< Key combination of Ctrl and J.
	KEY_CTRL_K = 0x0B,		///< Key combination of Ctrl and K.
	KEY_CTRL_L = 0x0C,		///< Key combination of Ctrl and L.
	KEY_CTRL_M = 0x0D,		///< Key combination of Ctrl and M.
	KEY_CTRL_N = 0x0E,		///< Key combination of Ctrl and N.
	KEY_CTRL_O = 0x0F,		///< Key combination of Ctrl and O.
	KEY_CTRL_P = 0x10,		///< Key combination of Ctrl and P.
	KEY_CTRL_Q = 0x11,		///< Key combination of Ctrl and Q.
	KEY_CTRL_R = 0x12,		///< Key combination of Ctrl and R.
	KEY_CTRL_S = 0x13,		///< Key combination of Ctrl and S.
	KEY_CTRL_T = 0x14,		///< Key combination of Ctrl and T.
	KEY_CTRL_U = 0x15,		///< Key combination of Ctrl and U.
	KEY_CTRL_V = 0x16,		///< Key combination of Ctrl and V.
	KEY_CTRL_W = 0x17,		///< Key combination of Ctrl and W.
	KEY_CTRL_X = 0x18,		///< Key combination of Ctrl and X.
	KEY_CTRL_Y = 0x19,		///< Key combination of Ctrl and Y.
	KEY_CTRL_Z = 0x1A,		///< Key combination of Ctrl and Z.
	KEY_CTRL_LEFT_BRACKET = 0x1B,	///< Key combination of Ctrl and [.
	KEY_CTRL_BACKSLASH = 0x1C,	///< Key combination of Ctrl and \.
	KEY_CTRL_RIGHT_BRACKET = 0x1D,	///< Key combination of Ctrl and ].
	KEY_CTRL_TAB = 0x1F,		///< Key combination of Ctrl and Tab.
	KEY_CTRL_F1 = 0xB9,		///< Key combination of Ctrl and F1.
	KEY_CTRL_F2 = 0xBA,		///< Key combination of Ctrl and F2.
	KEY_CTRL_F3 = 0xBB,		///< Key combination of Ctrl and F3.
	KEY_CTRL_F4 = 0xBC,		///< Key combination of Ctrl and F4.
	KEY_CTRL_F5 = 0xBD,		///< Key combination of Ctrl and F5.
	KEY_CTRL_F6 = 0xBE,		///< Key combination of Ctrl and F6.
	KEY_CTRL_F7 = 0xBF,		///< Key combination of Ctrl and F7.
	KEY_CTRL_F8 = 0xC0,		///< Key combination of Ctrl and F8.
	KEY_CTRL_F9 = 0xC1,		///< Key combination of Ctrl and F9.
	KEY_CTRL_F10 = 0xC2,		///< Key combination of Ctrl and F10.
	KEY_CTRL_F11 = 0xC3,		///< Key combination of Ctrl and F11.
	KEY_CTRL_F12 = 0xC4,		///< Key combination of Ctrl and F12.
	KEY_CTRL_BREAK = 0x82,		///< Key combination of Ctrl and Break.
	KEY_ALT_F1 = 0xC5,		///< Key combination of Alt and F1 (converted in OS_CheckUserInput()).
	KEY_ALT_F2 = 0xC6,		///< Key combination of Alt and F2 (converted in OS_CheckUserInput()).
	KEY_ALT_F3 = 0xC7,		///< Key combination of Alt and F3 (converted in OS_CheckUserInput()).
	KEY_ALT_F4 = 0xC8,		///< Key combination of Alt and F4 (converted in OS_CheckUserInput()).
	KEY_ALT_F5 = 0xC9,		///< Key combination of Alt and F5 (converted in OS_CheckUserInput()).
	KEY_ALT_F6 = 0xCA,		///< Key combination of Alt and F6 (converted in OS_CheckUserInput()).
	KEY_ALT_F7 = 0xCB,		///< Key combination of Alt and F7 (converted in OS_CheckUserInput()).
	KEY_ALT_F8 = 0xCC,		///< Key combination of Alt and F8 (converted in OS_CheckUserInput()).
	KEY_ALT_F9 = 0xCD,		///< Key combination of Alt and F9 (converted in OS_CheckUserInput()).
	KEY_ALT_F10 = 0xCE,		///< Key combination of Alt and F10 (converted in OS_CheckUserInput()).
	KEY_ALT_F11 = 0xCF,		///< Key combination of Alt and F11 (converted in OS_CheckUserInput()).
	KEY_ALT_F12 = 0xD0,		///< Key combination of Alt and F12 (converted in OS_CheckUserInput()).
	KEY_CTRL_BACKSPACE = 0xD1,	///< Key combination of Ctrl and Backspace (converted in OS_CheckUserInput()).
	KEY_CLEAR = KEY_CTRL_BACKSPACE,	/**< Key Clear (for compatibility,
					     have the same value as combination
					     of Ctrl and Backspace on external
					     keyboard). */
	KEY_CTRL_ENTER = 0xD2,		///< Key combination of Ctrl and Enter (converted in OS_CheckUserInput()).
	KEY_CTRL_HOME = 0xD3,		///< Key combination of Ctrl and Home (converted in OS_CheckUserInput()).
	KEY_CTRL_END = 0xD4,		///< Key combination of Ctrl and End (converted in OS_CheckUserInput()).
	KEY_CTRL_PAGE_UP = 0xD5,	///< Key combination of Ctrl and Page Up (converted in OS_CheckUserInput()).
	KEY_CTRL_PAGE_DOWN = 0xD6	///< Key combination of Ctrl and Page Down (converted in OS_CheckUserInput()).
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif  // __keys_h
