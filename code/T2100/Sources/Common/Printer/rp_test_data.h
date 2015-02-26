#ifndef RP_TEST_DATA_H
#define RP_TEST_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       TEST REPORTS
//--------------------------------------------------------------------
//
//
//--------------------------------------------------------------------
//       PRINTER TEST
//--------------------------------------------------------------------
//
const UBYTE RP_TEST_DATA[] = {

    IF, OPT1,
        FIRMWARE, LINE, 
        GRPH, 0X50, 0x00,
        IF, CND_ROLL, REPORT, RP_HEAD_SERIAL, ENDIF,
    ENDIF,
    
    IF, OPT2,
        FLD, FLD_HOSTTEXT1,
        FLD, FLD_HOSTTEXT2,
        LINE,
    ELSE,
        MLTCHR, 50, '*', CNTR, ' ', 'N', 'O', 'R', 'M', 'A', 'L', ' ', 'F', 'O', 'N', 'T', ' ', LINE,
        LINE,
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
        RGHT, 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', LINE,
        CNTR, 'U', 'V', 'W', 'X', 'Y', 'Z', LINE,
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
        RGHT, 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', LINE,
        CNTR, 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', LINE,
        LINE,
        CNTR, '!', '"', '#', '$', '%', '$', 0x27, '(', ')', '*', '+', 0x2C, '-',
        '.', '/', ':', '/', '/', '<', '=', '>', '?', '@', LINE,
        CNTR, '[', '\\', ']', '^', '_', '`', '{', '|', '}', 0x7E, 0x7F, LINE,
        LINE,
        'N', 'O', 'R', 'M', 'A', 'L', DOUBLE, RGHT, 'D', 'O', 'U', 'B', 'L', 'E', LINE,
        LINE,

        MLTCHR, 50, '*', CNTR, ' ', 'C', 'H', 'A', 'N', 'G', 'E', ' ', 'F', 'O', 'N', 'T', ' ', LINE,
        FONT, SMALL_FONT, 'S', 'M', 'A', 'L', 'L', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, LOW_FONT, 'L', 'O', 'W', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, NORM_FONT, 'N', 'O', 'R', 'M', 'A', 'L', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, DOUB_FONT, 'D', 'O', 'U', 'B', 'L', 'E', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, WIDE_FONT, 'W', 'I', 'D', 'E', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, HIGH_FONT, 'H', 'I', 'G', 'H', ' ', 'F', 'O', 'N', 'T', LINE,
        FONT, LARGE_FONT, 'L', 'A', 'R', 'G', 'E', ' ', 'F', 'O', 'N', 'T', LINE,

        MLTCHR, 50, '*', CNTR, ' ', 'F', 'O', 'N', 'T', ' ', 'S', 'T', 'Y', 'L', 'E', 'S', ' ', LINE,
        FONT, REVERSE_ON, FONT, BOLD_ON, 'R', 'E', 'V', 'E', 'R', 'S', 'E', ' ', 'F', 'O', 'N', 'T', FONT, BOLD_OFF, FONT, REVERSE_OFF, LINE,
        FONT, UNDERLINE_ON, 'U', 'N', 'D', 'E', 'R', 'L', 'I', 'N', 'E', ' ', 'F', 'O', 'N', 'T', FONT, UNDERLINE_OFF, LINE,
        FONT, BOLD_ON, 'B', 'O', 'L', 'D', ' ', 'F', 'O', 'N', 'T', FONT, BOLD_OFF, LINE,
        FONT, ITALIC_ON, 'I', 'T', 'A', 'L', 'I', 'C', ' ', 'F', 'O', 'N', 'T', FONT, ITALIC_OFF, LINE,


        MLTCHR, 50, '*', CNTR, ' ', 'B', 'A', 'R', 'C', 'O', 'D', 'E', ' ', 'E', 'X', 'A', 'M', 'P', 'L', 'E', ' ', LINE,
        FONT, BARCODE_ON, BARCODE_START, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', BARCODE_END, LINE,
        MLTCHR, 50, '*', CNTR, ' ', 'T', 'H', 'E', ' ', 'E', 'N', 'D', ' ', LINE,
    ENDIF,

    IF, OPT3,
        GRPH, 0x60, 0x00,
        FORM,
    ENDIF

};

#endif
