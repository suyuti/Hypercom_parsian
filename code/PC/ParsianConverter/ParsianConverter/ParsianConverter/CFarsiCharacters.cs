using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;


namespace ParsianConverter
{
    class CFarsiCharacters
    {
        const int TEK   = 0;
        const int BASTA = 1;
        const int ORTADA= 2;
        const int SONDA = 3;

        #region
        public const int _c_Space  = 32; 
        public const int _c_Exclam = 33; // ! 
        public const int _c_Quotes = 34; // "
        public const int _c_Grate  = 35; // #
        public const int _c_Dollar = 36; // $
        public const int _c_Prcnt  = 37; // % 
        public const int _c_At     = 38; // & 
        public const int _c_Apstr  = 39; // '
        public const int _c_Lbrck  = 40; // (
        public const int _c_Rbrck  = 41; // )
        public const int _c_Astr   = 42; // *
        public const int _c_Plus   = 43; // +
        public const int _c_Comma  = 44; // ',
        public const int _c_Minus  = 45; // -
        public const int _c_Dot    = 46; // .
        public const int _c_Slsh   = 47; // /
        public const int _c_0      = 48; 
        public const int _c_1      = 49; 
        public const int _c_2      = 50; 
        public const int _c_3      = 51; 
        public const int _c_4      = 52; 
        public const int _c_5      = 53; 
        public const int _c_6      = 54; 
        public const int _c_7      = 55; 
        public const int _c_8      = 56; 
        public const int _c_9      = 57; 
        public const int _c_Colon  = 58; // :
        public const int _c_Smcln  = 59; // ;
        public const int _c_Less   = 60; // <
        public const int _c_Equal  = 61; // =
        public const int _c_More   = 62; // >
        public const int _c_Quest  = 63; // ?
        public const int _c_Dog    = 64; // @
        public const int _c_A      = 65; 
        public const int _c_B      = 66; 
        public const int _c_C      = 67; 
        public const int _c_D      = 68; 
        public const int _c_E      = 69; 
        public const int _c_F      = 70; 
        public const int _c_G      = 71; 
        public const int _c_H      = 72; 
        public const int _c_I      = 73; 
        public const int _c_J      = 74; 
        public const int _c_K      = 75; 
        public const int _c_L      = 76; 
        public const int _c_M      = 77; 
        public const int _c_N      = 78; 
        public const int _c_O      = 79; 
        public const int _c_P      = 80; 
        public const int _c_Q      = 81; 
        public const int _c_R      = 82; 
        public const int _c_S      = 83; 
        public const int _c_T      = 84; 
        public const int _c_U      = 85; 
        public const int _c_V      = 86; 
        public const int _c_W      = 87; 
        public const int _c_X      = 88; 
        public const int _c_Y      = 89; 
        public const int _c_Z      = 90; 
        public const int _c_LRBrk  = 91; // [
        public const int _c_BkSls  = 92; // \
        public const int _c_RRBrk  = 93; // ]
        public const int _c_Cover  = 94; // ^
        public const int _c_Undln  = 95; // _
        public const int _c_Pfstr  = 96; // `
        public const int _c_a      = 97; 
        public const int _c_b      = 98; 
        public const int _c_c      = 99; 
        public const int _c_d      = 100; 
        public const int _c_e      = 101; 
        public const int _c_f      = 102; 
        public const int _c_g      = 103; 
        public const int _c_h      = 104; 
        public const int _c_i      = 105; 
        public const int _c_j      = 106; 
        public const int _c_k      = 107; 
        public const int _c_l      = 108; 
        public const int _c_m      = 109; 
        public const int _c_n      = 110; 
        public const int _c_o      = 111; 
        public const int _c_p      = 112; 
        public const int _c_q      = 113; 
        public const int _c_r      = 114; 
        public const int _c_s      = 115; 
        public const int _c_t      = 116; 
        public const int _c_u      = 117; 
        public const int _c_v      = 118; 
        public const int _c_w      = 119; 
        public const int _c_x      = 120; 
        public const int _c_y      = 121; 
        public const int _c_z      = 122; 
        public const int _c_LFBrk  = 123; // {
        public const int _c_Stick  = 124; // |
        public const int _c_RFBrk  = 125; // }
        public const int _c_Wave   = 126; // ~

        public const int _f_aA      = 34;
        public const int _f_a       = 39;
        public const int _f_aBe     = 40; 
        public const int _f_aPe     = 126; 
        public const int _f_aTe     = 42; 
        public const int _f_aSe     = 43; 
        public const int _f_aJim    = 44; 
        public const int _f_aChe    = 134; 
        public const int _f_aHe     = 45; 
        public const int _f_aKhe    = 46; 
        public const int _f_aDal    = 47; 
        public const int _f_aZal    = 48; 
        public const int _f_aRe     = 49; 
        public const int _f_aZe     = 50; 
        public const int _f_aZhe    = 152; 
        //public const int _f_pSxx = 0; 
        public const int _f_aSin    = 51; 
        public const int _f_aShin  = 52; 
        public const int _f_aSad   = 53; 
        public const int _f_aZad   = 54; 
        public const int _f_aTa    = 55; 
        public const int _f_aZa    = 56; 
        public const int _f_aEyn   = 57; 
        public const int _f_aQeyn  = 58; 
        public const int _f_aFe    = 65; 
        public const int _f_aQaf   = 66; 
        public const int _f_aKaf   = 169; 
        public const int _f_aGaf   = 175; 
        public const int _f_aLam   = 68; 
        public const int _f_aMim   = 69; 
        public const int _f_aNun   = 70; 
        public const int _f_aVav   = 72; 
        public const int _f_aHe2   = 71; 
        public const int _f_iHe2   = 0; 
        public const int _f_aYe    = 204; 
        public const int _f_Empty  = 0; 
        #endregion
       
        #region
        public static CCharDefinition c_Space   = new CCharDefinition('\x20',  10,   0x00);
        public static CCharDefinition c_Exclam  = new CCharDefinition('\x21',8,   0x00 );
        public static CCharDefinition c_Quotes  = new CCharDefinition('\x22',12,       0x00 );
        public static CCharDefinition c_Grate   = new CCharDefinition('\x23',12,   0x00 );
        public static CCharDefinition c_Dollar  = new CCharDefinition('\x24',12,   0x00 );
        public static CCharDefinition c_Prcnt   = new CCharDefinition('\x25',12,   0x00 );
        public static CCharDefinition c_At      = new CCharDefinition('\x26',12,   0x00 );
        public static CCharDefinition c_Apstr   = new CCharDefinition('\x27',8,   0x00 );
        public static CCharDefinition c_Lbrck   = new CCharDefinition('\x28',    9,       0x00 );
        public static CCharDefinition c_Rbrck   = new CCharDefinition('\x29',    9,       0x00 );
        public static CCharDefinition c_Astr    = new CCharDefinition('\x2A',    12,       0x00 );
        public static CCharDefinition c_Plus    = new CCharDefinition('\x2B',    12,       0x00 );
        public static CCharDefinition c_Comma   = new CCharDefinition('\x2C',    7,       0x00   );
        public static CCharDefinition c_Minus   = new CCharDefinition('\x2D',    12,       0x00 );
        public static CCharDefinition c_Dot     = new CCharDefinition('\x2E',    7,       0x00  );
        public static CCharDefinition c_Slsh    = new CCharDefinition('\x2F',    12,       0x00   );
        public static CCharDefinition c_0       = new CCharDefinition('\x30',     7,       0x00   );
        public static CCharDefinition c_1       = new CCharDefinition('\x31',      9,       0x00 );
        public static CCharDefinition c_2       = new CCharDefinition('\x32',    7,       0x00 );
        public static CCharDefinition c_3       = new CCharDefinition('\x33',    7,       0x00  );
        public static CCharDefinition c_4       = new CCharDefinition('\x34',     7,       0x00   );
        public static CCharDefinition c_5       = new CCharDefinition('\x35',    7,       0x00   );
        public static CCharDefinition c_6       = new CCharDefinition('\x36',    7,       0x00  );
        public static CCharDefinition c_7       = new CCharDefinition('\x37',    7,       0x00 );
        public static CCharDefinition c_8       = new CCharDefinition('\x38',    7,       0x00   );
        public static CCharDefinition c_9       = new CCharDefinition('\x39',    7,       0x00   );
        public static CCharDefinition c_Colon   = new CCharDefinition('\x3A',    7,       0x00   );
        public static CCharDefinition c_Smcln   = new CCharDefinition('\x3B',    7,       0x00  );
        public static CCharDefinition c_Less    = new CCharDefinition('\x3C',    12,       0x00    );
        public static CCharDefinition c_Equal   = new CCharDefinition('\x3D',    12,       0x00   );
        public static CCharDefinition c_More    = new CCharDefinition('\x3E',    12,       0x00   );
        public static CCharDefinition c_Quest   = new CCharDefinition('\x3F',    12,       0x00    );
        public static CCharDefinition c_Dog     = new CCharDefinition('\x40',    7,      0x00  );
        public static CCharDefinition c_A       = new CCharDefinition('\x41',     12,       0x00    );
        public static CCharDefinition c_B       = new CCharDefinition('\x42',    12,       0x00 );
        public static CCharDefinition c_C       = new CCharDefinition('\x43',    12,       0x00    );
        public static CCharDefinition c_D       = new CCharDefinition('\x44',    12,       0x00    );
        public static CCharDefinition c_E       = new CCharDefinition('\x45',    12,       0x00    );
        public static CCharDefinition c_F       = new CCharDefinition('\x46',    12,       0x00   );
        public static CCharDefinition c_G       = new CCharDefinition('\x47',    12,       0x00    );
        public static CCharDefinition c_H       = new CCharDefinition('\x48',     12,       0x00    );
        public static CCharDefinition c_I       = new CCharDefinition('\x49',     7,       0x00    );
        public static CCharDefinition c_J       = new CCharDefinition('\x4A',    12,       0x00    );
        public static CCharDefinition c_K       = new CCharDefinition('\x4B',     12,       0x00    );
        public static CCharDefinition c_L       = new CCharDefinition('\x4C',     12,       0x00    );
        public static CCharDefinition c_M       = new CCharDefinition('\x4D',     7,       0x00    );
        public static CCharDefinition c_N       = new CCharDefinition('\x4E',     12,       0x00    );
        public static CCharDefinition c_O       = new CCharDefinition('\x4F',    12,       0x00    );
        public static CCharDefinition c_P       = new CCharDefinition('\x50',    12,       0x00   );
        public static CCharDefinition c_Q       = new CCharDefinition('\x51',    12,       0x00     );
        public static CCharDefinition c_R       = new CCharDefinition('\x52',    12,       0x00    );
        public static CCharDefinition c_S       = new CCharDefinition('\x53',    12,       0x00    );
        public static CCharDefinition c_T       = new CCharDefinition('\x54',     12,       0x00 );
        public static CCharDefinition c_U       = new CCharDefinition('\x55',    12,       0x00  );
        public static CCharDefinition c_V       = new CCharDefinition('\x56',    12,       0x00  );
        public static CCharDefinition c_W       = new CCharDefinition('\x57',    7,       0x00  );
        public static CCharDefinition c_X       = new CCharDefinition('\x58',    12,       0x00   );
        public static CCharDefinition c_Y       = new CCharDefinition('\x59',    12,       0x00     );
        public static CCharDefinition c_Z       = new CCharDefinition('\x5A',     12,       0x00   );
        public static CCharDefinition c_LRBrk   = new CCharDefinition('\x5B',    12,       0x00   );
        public static CCharDefinition c_BkSls   = new CCharDefinition('\x5C',    12,       0x00   );
        public static CCharDefinition c_RRBrk   = new CCharDefinition('\x5D',    12,       0x00   );
        public static CCharDefinition c_Cover   = new CCharDefinition('\x5E',    12,       0x00    );
        public static CCharDefinition c_Undln   = new CCharDefinition('\x5F',    12,       0x00    );
        public static CCharDefinition c_Pfstr   = new CCharDefinition('\x60',    12,       0x00    );
        public static CCharDefinition c_a       = new CCharDefinition('\x61',     12,       0x00   );
        public static CCharDefinition c_b       = new CCharDefinition('\x62',     12,       0x00    );
        public static CCharDefinition c_c       = new CCharDefinition('\x63',     12,       0x00    );
        public static CCharDefinition c_d       = new CCharDefinition('\x64',     12,       0x00    );
        public static CCharDefinition c_e       = new CCharDefinition('\x65',     12,       0x00   );
        public static CCharDefinition c_f       = new CCharDefinition('\x66',     12,       0x00   );
        public static CCharDefinition c_g       = new CCharDefinition('\x67',     12,       0x00    );
        public static CCharDefinition c_h       = new CCharDefinition('\x68',     12,       0x00    );
        public static CCharDefinition c_i       = new CCharDefinition('\x69',     7,      0x00    );
        public static CCharDefinition c_j       = new CCharDefinition('\x6A',     12,       0x00   );
        public static CCharDefinition c_k       = new CCharDefinition('\x6B',     12,      0x00    );
        public static CCharDefinition c_l       = new CCharDefinition('\x6C',     7,       0x00     );
        public static CCharDefinition c_m       = new CCharDefinition('\x6D',     7,       0x00  );
        public static CCharDefinition c_n       = new CCharDefinition('\x6E',     12,       0x00  );
        public static CCharDefinition c_o       = new CCharDefinition('\x6F',     12,       0x00   );
        public static CCharDefinition c_p       = new CCharDefinition('\x70',     12,       0x00   );
        public static CCharDefinition c_q       = new CCharDefinition('\x71',     12,       0x00   );
        public static CCharDefinition c_r       = new CCharDefinition('\x72',     7,       0x00  );
        public static CCharDefinition c_s       = new CCharDefinition('\x73',     12,       0x00   );
        public static CCharDefinition c_t       = new CCharDefinition('\x74',     12,       0x00  );
        public static CCharDefinition c_u       = new CCharDefinition('\x75',     12,       0x00  );
        public static CCharDefinition c_v       = new CCharDefinition('\x76',     12,       0x00 );
        public static CCharDefinition c_w       = new CCharDefinition('\x77',     7,       0x00 );
        public static CCharDefinition c_x       = new CCharDefinition('\x78',     12,       0x00   );
        public static CCharDefinition c_y       = new CCharDefinition('\x79',     12,       0x00 );
        public static CCharDefinition c_z       = new CCharDefinition('\x7A',    12,       0x00  );
        public static CCharDefinition c_LFBrk   = new CCharDefinition('\x7B',    12,       0x00 );
        public static CCharDefinition c_Stick   = new CCharDefinition('\x7C',     12,       0x00  );
        public static CCharDefinition c_RFBrk   = new CCharDefinition('\x7D',    12,       0x00   );
        public static CCharDefinition c_Wave    = new CCharDefinition('\x7E',    12,       0x00    );
        public static CCharDefinition c_aAlef   = new CCharDefinition('\x91',       7,       0x00 );
        public static CCharDefinition c_iAlef   = new CCharDefinition('\x92',      9,       0x00   );
        public static CCharDefinition c_mAlef   = new CCharDefinition('\x93',     7,       0x00    );
        public static CCharDefinition c_aBe     = new CCharDefinition('\x94',     7,       0x00   );
        public static CCharDefinition c_iBe     = new CCharDefinition('\x95',     7,       0x00    );
        public static CCharDefinition c_mBe     = new CCharDefinition('\x96',    7,       0x00   );
        public static CCharDefinition c_eBe     = new CCharDefinition('\x97',7,       0x00    );
        public static CCharDefinition c_aPe     = new CCharDefinition('\x98', 7,       0x00  );
        public static CCharDefinition c_iPe     = new CCharDefinition('\x99', 7,       0x00    );
        public static CCharDefinition c_mPe     = new CCharDefinition('\x9A',7,       0x00   );
        public static CCharDefinition c_ePe     = new CCharDefinition('\x9B', 7,       0x00     );
        public static CCharDefinition c_aTe     = new CCharDefinition('\x9C', 7,       0x00    );
        public static CCharDefinition c_iTe     = new CCharDefinition('\x9D',7,       0x00    );
        public static CCharDefinition c_mTe     = new CCharDefinition('\x9E', 7,       0x00   );
        public static CCharDefinition c_eTe     = new CCharDefinition('\x9F', 7,       0x00    );
        public static CCharDefinition c_aSe     = new CCharDefinition('\xA0', 7,       0x00     );
        public static CCharDefinition c_iSe     = new CCharDefinition('\xA1', 7,       0x00 );
        public static CCharDefinition c_mSe     = new CCharDefinition('\xA2', 7,       0x00    );
        public static CCharDefinition c_eSe     = new CCharDefinition('\xA3', 7,       0x00   );
        public static CCharDefinition c_aJim    = new CCharDefinition('\xA4', 7,       0x00   );
        public static CCharDefinition c_iJim    = new CCharDefinition('\xA5', 7,       0x00    );
        public static CCharDefinition c_eJim    = new CCharDefinition('\xA6',7,       0x00   );
        public static CCharDefinition c_aChe    = new CCharDefinition('\xA7',7,       0x00   );
        public static CCharDefinition c_iChe    = new CCharDefinition('\xA8', 7,       0x00  );
        public static CCharDefinition c_aHe     = new CCharDefinition('\xA9', 7,       0x00   );
        public static CCharDefinition c_iHe     = new CCharDefinition('\xAA', 7,       0x00   );
        public static CCharDefinition c_eHe     = new CCharDefinition('\xAB', 7,       0x00    );
        public static CCharDefinition c_aKhe    = new CCharDefinition('\xAC',7,       0x00   );
        public static CCharDefinition c_iKhe    = new CCharDefinition('\xAD',7,       0x00    );
        public static CCharDefinition c_eKhe    = new CCharDefinition('\xAE',7,       0x00  );
        public static CCharDefinition c_aDal    = new CCharDefinition('\xAF', 7,        0x00   );
        public static CCharDefinition c_aZal    = new CCharDefinition('\xB0',7,        0x00 );
        public static CCharDefinition c_aRe     = new CCharDefinition('\xB1', 12,      0x00   );
        public static CCharDefinition c_aZe     = new CCharDefinition('\xB2', 12,       0x00  );
        public static CCharDefinition c_aZhe    = new CCharDefinition('\xB3',12,       0x00  );
        public static CCharDefinition c_pSxx    = new CCharDefinition('\xB4', 12,       0x00  );
        public static CCharDefinition c_aSin    = new CCharDefinition('\xB5',9,        0x00     );
        public static CCharDefinition c_iSin    = new CCharDefinition('\xB6', 7,       0x00     );
        public static CCharDefinition c_mSin    = new CCharDefinition('\xB7', 7,       0x00    );
        public static CCharDefinition c_eSin    = new CCharDefinition('\xB8', 7,       0x00    );
        public static CCharDefinition c_aShin   = new CCharDefinition('\xB9', 9,        0x00     );
        public static CCharDefinition c_iShin   = new CCharDefinition('\xBA',7,       0x00    );
        public static CCharDefinition c_mShin   = new CCharDefinition('\xBB', 7,       0x00   );
        public static CCharDefinition c_eShin   = new CCharDefinition('\xBC', 7,       0x00   );
        public static CCharDefinition c_aSad    = new CCharDefinition('\xBD',12,       0x00     );
        public static CCharDefinition c_iSad    = new CCharDefinition('\xBE', 7,       0x00  );
        public static CCharDefinition c_mSad    = new CCharDefinition('\xBF', 7,       0x00   );
        public static CCharDefinition c_eSad    = new CCharDefinition('\xC0',7,       0x00   );
        public static CCharDefinition c_aZad    = new CCharDefinition('\xC1', 12,       0x00   );
        public static CCharDefinition c_iZad    = new CCharDefinition('\xC2', 7,       0x00   );
        public static CCharDefinition c_mZad    = new CCharDefinition('\xC3', 7,       0x00    );
        public static CCharDefinition c_eZad    = new CCharDefinition('\xC4', 7,       0x00    );
        public static CCharDefinition c_aTa     = new CCharDefinition('\xC5', 7,       0x00   );
        public static CCharDefinition c_mTa     = new CCharDefinition('\xC6', 7,       0x00   );
        public static CCharDefinition c_aZa     = new CCharDefinition('\xC7', 7,       0x00   );
        public static CCharDefinition c_mZa     = new CCharDefinition('\xC8', 7,       0x00    );
        public static CCharDefinition c_aEyn    = new CCharDefinition('\xC9', 12,       0x00    );
        public static CCharDefinition c_iEyn    = new CCharDefinition('\xCA', 12,       0x00       );
        public static CCharDefinition c_mEyn    = new CCharDefinition('\xCB',12,       0x00   );
        public static CCharDefinition c_eEyn    = new CCharDefinition('\xCC', 12,       0x00     );
        public static CCharDefinition c_aQeyn   = new CCharDefinition('\xCD', 12,       0x00  );
        public static CCharDefinition c_iQeyn   = new CCharDefinition('\xCE', 12,       0x00  );
        public static CCharDefinition c_mQeyn   = new CCharDefinition('\xCF', 12,       0x00   );
        public static CCharDefinition c_eQeyn   = new CCharDefinition('\xD0',  12,       0x00   );
        public static CCharDefinition c_aFe     = new CCharDefinition('\xD1',   7,       0x00    );
        public static CCharDefinition c_iFe     = new CCharDefinition('\xD2',   7,       0x00    );
        public static CCharDefinition c_mFe     = new CCharDefinition('\xD3',   7,       0x00   );
        public static CCharDefinition c_eFe     = new CCharDefinition('\xD4',   7,       0x00   );
        public static CCharDefinition c_aQaf    = new CCharDefinition('\xD5',   7,  0x00  );
        public static CCharDefinition c_iQaf    = new CCharDefinition('\xD6',   7,  0x00  );
        public static CCharDefinition c_mQaf    = new CCharDefinition('\xD7',   7,  0x00    );
        public static CCharDefinition c_eQaf    = new CCharDefinition('\xD8',   7,  0x00  );
        public static CCharDefinition c_aKaf    = new CCharDefinition('\xD9',   7,  0x00    );
        public static CCharDefinition c_iKaf    = new CCharDefinition('\xDA',   7,  0x00    );
        public static CCharDefinition c_mKaf    = new CCharDefinition('\xDB',   7,  0x00   );
        public static CCharDefinition c_eKaf    = new CCharDefinition('\xDC',   7,  0x00   );
        public static CCharDefinition c_aGaf    = new CCharDefinition('\xDD',   7,  0x00    );
        public static CCharDefinition c_iGaf    = new CCharDefinition('\xDE',   7,  0x00   );
        public static CCharDefinition c_mGaf    = new CCharDefinition('\xDF',   7,  0x00    );
        public static CCharDefinition c_eGaf    = new CCharDefinition('\xE0',   7,  0x00    );
        public static CCharDefinition c_aLam    = new CCharDefinition('\xE1',   7,  0x00    );
        public static CCharDefinition c_iLam    = new CCharDefinition('\xE2',   7,  0x00   );
        public static CCharDefinition c_mLam    = new CCharDefinition('\xE3',   7,  0x00    );
        public static CCharDefinition c_eLam    = new CCharDefinition('\xE4',   7,  0x00  );
        public static CCharDefinition c_aMim    = new CCharDefinition('\xE5',   7,  0x00  );
        public static CCharDefinition c_iMim    = new CCharDefinition('\xE6',   7,  0x00  );
        public static CCharDefinition c_mMim    = new CCharDefinition('\xE7',   7,  0x00  );
        public static CCharDefinition c_eMim    = new CCharDefinition('\xE8',   7,  0x00  );
        public static CCharDefinition c_aNun    = new CCharDefinition('\xE9',   7,  0x00  );
        public static CCharDefinition c_iNun    = new CCharDefinition('\xEA',   7,  0x00  );
        public static CCharDefinition c_mNun    = new CCharDefinition('\xEB',   7,  0x00 );
        public static CCharDefinition c_eNun    = new CCharDefinition('\xEC',   7,  0x00   );
        public static CCharDefinition c_aVav    = new CCharDefinition('\xED',   7,  0x00  );
        public static CCharDefinition c_aHe2    = new CCharDefinition('\xEE',   9,  0x00   );
        public static CCharDefinition c_iHe2    = new CCharDefinition('\xEF',   7,  0x00  );
        public static CCharDefinition c_mHe2    = new CCharDefinition('\xF0',   7,  0x00);
        public static CCharDefinition c_eHe2    = new CCharDefinition('\xF1',   7,  0x00  );
        public static CCharDefinition c_aYe     = new CCharDefinition('\xF2',   7,  0x00  );
        public static CCharDefinition c_iYe     = new CCharDefinition('\xF3',   7,  0x00  );
        public static CCharDefinition c_mYe     = new CCharDefinition('\xF4',   7,  0x00   );
        public static CCharDefinition c_eYe     = new CCharDefinition('\xF5',   7,  0x00   );
        public static CCharDefinition c_Empty   = new CCharDefinition('\xFF',   7,  0x00 );
#endregion
        public String RevStr(string val) 
        {
            StringBuilder sb = new StringBuilder();
            for (int i = val.Length - 1; i >= 0; --i) {
                sb.Append(val[i]);
            }
            return sb.ToString();
        }
        public string GetFarisiString(string val)
        {
            return "";
            /*
            StringBuilder sb = new StringBuilder();
            ArrayList res = new ArrayList();
            byte[] bytes = null;
            int harfSayisi = 0;
            int harf = 0;
            int pos = 0;
            int ch = 0;
            string[] vals = val.Split(" ".ToCharArray());
            int kelimePos = 0;
            foreach (string kelime in vals) {
                harfSayisi = kelime.Length;
                harf = 0;
                bytes = Encoding.Unicode.GetBytes(kelime);
                if (bytes.Length > 0 && bytes[1] == 0)
                {
                    // farsca degilse ters cevir
                    bytes = Encoding.Unicode.GetBytes(RevStr(kelime));
                }

                if (kelimePos != 0) {
                    res.Add("\\x20");
                }
                for (int i = 0; i < bytes.Length; ++i) {
                    if ((i % 2) == 0) {
                        continue;
                    }
                    if (harf == 0) {
                        pos = BASTA;
                    }
                    else if (harf == harfSayisi - 1) {
                        pos = SONDA;
                    }
                    else {
                        pos = ORTADA;
                    }
                    if (ch == (int)c_mAlef.CharCode ||
                        ch == (int)c_aAlef.CharCode ||
                        ch == (int)c_aRe.CharCode   ||
                        ch == (int)c_aZe.CharCode   ||
                        ch == (int)c_aZhe.CharCode  ||
                        ch == (int)c_aDal.CharCode  ||
                        ch == (int)c_aZal.CharCode  ||
                        ch == (int)c_aVav.CharCode) {
                        // bu harflerden sonra gelenler m, e olamaz.
                            if (pos == SONDA) {
                                pos = TEK;
                            }
                            else {
                                pos = BASTA;
                            }
                    }
                    if (harf < harfSayisi - 1) {
                        if (bytes[i + 2] == 0) {
                            // eger kelime icindeki bir sonraki harf ASCII ise bu harf SONDA olur
                            pos = SONDA;
                        }
                    }
                    if (harf > 0) {
                        if (bytes[i - 2] == 0) { 
                            // eger kelime icindeki bir onceki harf ASCII ise bu harf BASTA olur
                            pos = BASTA;
                        }
                    }
                    ch = (int)GetMappedChar(bytes[i - 1], bytes[i], pos);
                    res.Add(String.Format("\\x{0:X2}", ch));
                    if (
                        ch == (int)c_aShin.CharCode ||
                        ch == (int)c_iShin.CharCode ||
                        ch == (int)c_mShin.CharCode ||
                        ch == (int)c_eShin.CharCode ||
                        ch == (int)c_aSin.CharCode ||
                        ch == (int)c_iSin.CharCode ||
                        ch == (int)c_mSin.CharCode ||
                        ch == (int)c_eSin.CharCode ||
                        ch == (int)c_aSad.CharCode ||
                        ch == (int)c_iSad.CharCode ||
                        ch == (int)c_mSad.CharCode ||
                        ch == (int)c_eSad.CharCode ||
                        ch == (int)c_aZad.CharCode ||
                        ch == (int)c_iZad.CharCode ||
                        ch == (int)c_mZad.CharCode ||
                        ch == (int)c_eZad.CharCode )
                    {
                        if (pos == SONDA || pos == TEK) { 
                            res.Add(String.Format("\\x{0:X2}", (int)c_pSxx.CharCode));
                        }
                    }
                    ++harf;
                }
                ++kelimePos;
            }
            for (int i = res.Count - 1; i >= 0; --i)
                sb.Append(res[i]);
            return sb.ToString();
             * */
        }
        public string GetFarisiString_(string val)
        {
            return "";
            /*
            ArrayList res = new ArrayList();
            StringBuilder sb = new StringBuilder();
            //StringBuilder sx = new StringBuilder();
            //for (int i = val.Length - 1; i >= 0; --i) 
            //{
            //    sx.Append(val[i]);
            //}

            //byte[] bytes = Encoding.Unicode.GetBytes(sx.ToString());
            byte[] bytes = Encoding.Unicode.GetBytes(val);
            int pos = 0;
            int ch = 0;
            for (int i = 0; i < bytes.Length; ++i)
            {
                if (i == 0 || i == 1)
                {
                    pos = BASTA;
                }
                else if (i == bytes.Length || i == bytes.Length - 1)
                {
                    pos = SONDA;
                }
                else
                {
                    pos = ORTADA;
                }

                if ((i % 2) != 0)
                {
                    if (ch == (int)c_mAlef.CharCode || ch == (int)c_aAlef.CharCode ||
                        ch == (int)c_aRe.CharCode ||
                        ch == (int)c_aZe.CharCode ||
                        ch == (int)c_aZhe.CharCode ||
                        ch == (int)c_aDal.CharCode ||
                        ch == (int)c_aZal.CharCode ||
                        ch == (int)c_aVav.CharCode) {
                            if (pos == SONDA) {
                                pos = TEK;
                            }
                            else {
                                pos = BASTA;
                            }
                    }

                    ch = (int)GetMappedChar(bytes[i - 1], bytes[i], pos);
                    res.Add(String.Format("\\x{0:X2}", ch));

                    if (ch == (int)c_aShin.CharCode || ch == (int)c_aSin.CharCode ||ch == (int)c_aSad.CharCode ||ch == (int)c_aZad.CharCode) {
                        if (i == (bytes.Length - 1) || (i == bytes.Length)) {
                            res.Add(String.Format("\\x{0:X2}", (int)c_pSxx.CharCode));
                        }
                    }
                }
            }
            for (int i = res.Count - 1; i >= 0; --i)
                    sb.Append(res[i]);
            return sb.ToString();
             */ 
        }
        public char[] GetFarisiCharArray(String val)
        {
            char[] retVal = new char[val.Length];
            int j = retVal.Length - 1;

            String[] strs = val.Split(" ".ToCharArray());

            int x = 0;
            foreach(String str in strs) {
                byte[] bytes = Encoding.Unicode.GetBytes(str);
                int pos = 0;
                bool baskaKarakterVar = true;
                for (int i = 0; i < bytes.Length; ++i) {
                    if (i == 0 || i == 1) {
                        pos = BASTA;
                    }
                    else if (i == bytes.Length || i == bytes.Length - 1)
                    {
                        pos = SONDA;
                    }
                    else {
                        pos = ORTADA;
                    }
                    if ((i % 2) != 0) {
                        if (i >= bytes.Length - 1) {
                            baskaKarakterVar = false;
                        }
                        retVal[j] = GetMappedChar(bytes[i - 1], bytes[i], pos, baskaKarakterVar);
                        --j;
                    }
                }
                //retVal[j] = ' ';
                x++;
                if (x < strs.Length)
                {
                    retVal[j] = ' ';
                    --j;
                }
            }
            return retVal;
        }

        private static bool sonrakiSagdanBirlesir = false;
        char GetMappedChar(int firstByte, int secondByte, int pos, bool baskaKarakterVar)
        {
            char retVal = '\xFF';
            if (secondByte == 0) {
                // Ascii character
                switch (firstByte) {
                    case _c_Space  : retVal = c_Space.CharCode; break;
                    case _c_Exclam : retVal = c_Exclam.CharCode;break;
                    case _c_Quotes : retVal = c_Quotes.CharCode;break;
                    case _c_Grate  : retVal = c_Grate.CharCode;break;
                    case _c_Dollar : retVal = c_Dollar.CharCode;break;
                    case _c_Prcnt  : retVal = c_Prcnt.CharCode;break;
                    case _c_At     : retVal = c_At.CharCode;break;
                    case _c_Apstr  : retVal = c_Apstr.CharCode;break;
                    case _c_Lbrck  : retVal = c_Lbrck.CharCode;break;
                    case _c_Rbrck  : retVal = c_Rbrck.CharCode;break;
                    case _c_Astr   : retVal = c_Astr.CharCode;break;
                    case _c_Plus   : retVal = c_Plus.CharCode;break;
                    case _c_Comma  : retVal = c_Comma.CharCode;break;
                    case _c_Minus  : retVal = c_Minus.CharCode;break;
                    case _c_Dot    : retVal = c_Dot.CharCode;break;
                    case _c_Slsh   : retVal = c_Slsh.CharCode;break;
                    case _c_0      : retVal = c_0.CharCode;break;
                    case _c_1      : retVal = c_1.CharCode;break;
                    case _c_2      : retVal = c_2.CharCode;break;
                    case _c_3      : retVal = c_3.CharCode;break;
                    case _c_4      : retVal = c_4.CharCode;break;
                    case _c_5      : retVal = c_5.CharCode;break;
                    case _c_6      : retVal = c_6.CharCode;break;
                    case _c_7      : retVal = c_7.CharCode;break;
                    case _c_8      : retVal = c_8.CharCode;break;
                    case _c_9      : retVal = c_9.CharCode;break;
                    case _c_Colon  : retVal = c_Colon.CharCode;break;
                    case _c_Smcln  : retVal = c_Smcln.CharCode;break;
                    case _c_Less   : retVal = c_Less.CharCode;break;
                    case _c_Equal  : retVal = c_Equal.CharCode;break;
                    case _c_More   : retVal = c_More.CharCode;break;
                    case _c_Quest  : retVal = c_Quest.CharCode;break;
                    case _c_Dog    : retVal = c_Dog.CharCode;break;
                    case _c_A      : retVal = c_A.CharCode;break;
                    case _c_B      : retVal = c_B    .CharCode;break;
                    case _c_C      : retVal = c_C    .CharCode;break;
                    case _c_D      : retVal = c_D    .CharCode;break;
                    case _c_E      : retVal = c_E    .CharCode;break;
                    case _c_F      : retVal = c_F    .CharCode;break;
                    case _c_G      : retVal = c_G    .CharCode;break;
                    case _c_H      : retVal = c_H    .CharCode;break;
                    case _c_I      : retVal = c_I    .CharCode;break;
                    case _c_J      : retVal = c_J    .CharCode;break;
                    case _c_K      : retVal = c_K    .CharCode;break;
                    case _c_L      : retVal = c_L    .CharCode;break;
                    case _c_M      : retVal = c_M    .CharCode;break;
                    case _c_N      : retVal = c_N    .CharCode;break;
                    case _c_O      : retVal = c_O    .CharCode;break;
                    case _c_P      : retVal = c_P    .CharCode;break;
                    case _c_Q      : retVal = c_Q    .CharCode;break;
                    case _c_R      : retVal = c_R    .CharCode;break;
                    case _c_S      : retVal = c_S    .CharCode;break;
                    case _c_T      : retVal = c_T    .CharCode;break;
                    case _c_U      : retVal = c_U    .CharCode;break;
                    case _c_V      : retVal = c_V    .CharCode;break;
                    case _c_W      : retVal = c_W    .CharCode;break;
                    case _c_X      : retVal = c_X    .CharCode;break;
                    case _c_Y      : retVal = c_Y    .CharCode;break;
                    case _c_Z      : retVal = c_Z    .CharCode;break;
                    case _c_LRBrk  : retVal = c_LRBrk    .CharCode;break;
                    case _c_BkSls  : retVal = c_BkSls    .CharCode;break;
                    case _c_RRBrk  : retVal = c_RRBrk    .CharCode;break;
                    case _c_Cover  : retVal = c_Cover    .CharCode;break;
                    case _c_Undln  : retVal = c_Undln    .CharCode;break;
                    case _c_Pfstr  : retVal = c_Pfstr    .CharCode;break;
                    case _c_a      : retVal = c_a    .CharCode;break;
                    case _c_b      : retVal = c_b    .CharCode;break;
                    case _c_c      : retVal = c_c    .CharCode;break;
                    case _c_d      : retVal = c_d    .CharCode;break;
                    case _c_e      : retVal = c_e    .CharCode;break;
                    case _c_f      : retVal = c_f    .CharCode;break;
                    case _c_g      : retVal = c_g    .CharCode;break;
                    case _c_h      : retVal = c_h    .CharCode;break;
                    case _c_i      : retVal = c_i    .CharCode;break;
                    case _c_j      : retVal = c_j    .CharCode;break;
                    case _c_k      : retVal = c_k    .CharCode;break;
                    case _c_l      : retVal = c_l    .CharCode;break;
                    case _c_m      : retVal = c_m    .CharCode;break;
                    case _c_n      : retVal = c_n    .CharCode;break;
                    case _c_o      : retVal = c_o    .CharCode;break;
                    case _c_p      : retVal = c_p    .CharCode;break;
                    case _c_q      : retVal = c_q    .CharCode;break;
                    case _c_r      : retVal = c_r    .CharCode;break;
                    case _c_s      : retVal = c_s    .CharCode;break;
                    case _c_t      : retVal = c_t    .CharCode;break;
                    case _c_u      : retVal = c_u    .CharCode;break;
                    case _c_v      : retVal = c_v    .CharCode;break;
                    case _c_w      : retVal = c_w    .CharCode;break;
                    case _c_x      : retVal = c_x    .CharCode;break;
                    case _c_y      : retVal = c_y    .CharCode;break;
                    case _c_z      : retVal = c_z    .CharCode;break;
                    case _c_LFBrk  : retVal = c_LFBrk    .CharCode;break;
                    case _c_Stick  : retVal = c_Stick    .CharCode;break;
                    case _c_RFBrk  : retVal = c_RFBrk    .CharCode;break;
                    case _c_Wave   : retVal = c_Wave    .CharCode;break;
                }
            }
            else if (secondByte == 6) {
                // Farisi Character
                if (sonrakiSagdanBirlesir == false && (pos == ORTADA || pos == SONDA) && baskaKarakterVar == true) {
                    pos = BASTA;
                }
                if (sonrakiSagdanBirlesir == false && baskaKarakterVar == false) { 
                    // en sondadır.
                    pos = TEK;
                }
                switch (firstByte) {
                    case _f_aA: { if (pos == TEK) { retVal = c_iAlef.CharCode; } else if (pos == BASTA) { retVal = c_iAlef.CharCode; } else if (pos == ORTADA) { retVal = c_mAlef.CharCode; } else if (pos == SONDA) { retVal = c_mAlef.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_a: { if (pos == TEK) { retVal = c_aAlef.CharCode; } else if (pos == BASTA) { retVal = c_aAlef.CharCode; } else if (pos == ORTADA) { retVal = c_mAlef.CharCode; } else if (pos == SONDA) { retVal = c_mAlef.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_aBe: { if (pos == TEK) { retVal = c_aBe.CharCode; } else if (pos == BASTA) { retVal = c_iBe.CharCode; } else if (pos == ORTADA) { retVal = c_mBe.CharCode; } else if (pos == SONDA) { retVal = c_eBe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aPe: { if (pos == TEK) { retVal = c_aPe.CharCode; } else if (pos == BASTA) { retVal = c_iPe.CharCode; } else if (pos == ORTADA) { retVal = c_mPe.CharCode; } else if (pos == SONDA) { retVal = c_ePe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aTe: { if (pos == TEK) { retVal = c_aTe.CharCode; } else if (pos == BASTA) { retVal = c_iTe.CharCode; } else if (pos == ORTADA) { retVal = c_mTe.CharCode; } else if (pos == SONDA) { retVal = c_eTe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aSe: { if (pos == TEK) { retVal = c_aSe.CharCode; } else if (pos == BASTA) { retVal = c_iSe.CharCode; } else if (pos == ORTADA) { retVal = c_mSe.CharCode; } else if (pos == SONDA) { retVal = c_eSe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aJim: { if (pos == TEK) { retVal = c_aJim.CharCode; } else if (pos == BASTA) { retVal = c_iJim.CharCode; } else if (pos == ORTADA) { retVal = c_iJim.CharCode; } else if (pos == SONDA) { retVal = c_eJim.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aChe: { if (pos == TEK) { retVal = c_aChe.CharCode; } else if (pos == BASTA) { retVal = c_iChe.CharCode; } else if (pos == ORTADA) { retVal = c_iChe.CharCode; } else if (pos == SONDA) { retVal = c_aChe.CharCode; } sonrakiSagdanBirlesir = true; } break; //?
                    case _f_aHe: { if (pos == TEK) { retVal = c_aHe.CharCode; } else if (pos == BASTA) { retVal = c_iHe.CharCode; } else if (pos == ORTADA) { retVal = c_iHe.CharCode; } else if (pos == SONDA) { retVal = c_eHe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aKhe: { if (pos == TEK) { retVal = c_aKhe.CharCode; } else if (pos == BASTA) { retVal = c_iKhe.CharCode; } else if (pos == ORTADA) { retVal = c_iKhe.CharCode; } else if (pos == SONDA) { retVal = c_eKhe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aDal: { if (pos == TEK) { retVal = c_aDal.CharCode; } else if (pos == BASTA) { retVal = c_aDal.CharCode; } else if (pos == ORTADA) { retVal = c_aDal.CharCode; } else if (pos == SONDA) { retVal = c_aDal.CharCode; } sonrakiSagdanBirlesir = false; } break; //?
                    case _f_aZal: { if (pos == TEK) { retVal = c_aZal.CharCode; } else if (pos == BASTA) { retVal = c_aZal.CharCode; } else if (pos == ORTADA) { retVal = c_aZal.CharCode; } else if (pos == SONDA) { retVal = c_aZal.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_aRe: { if (pos == TEK) { retVal = c_aRe.CharCode; } else if (pos == BASTA) { retVal = c_aRe.CharCode; } else if (pos == ORTADA) { retVal = c_aRe.CharCode; } else if (pos == SONDA) { retVal = c_aRe.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_aZe: { if (pos == TEK) { retVal = c_aZe.CharCode; } else if (pos == BASTA) { retVal = c_aZe.CharCode; } else if (pos == ORTADA) { retVal = c_aZe.CharCode; } else if (pos == SONDA) { retVal = c_aZe.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_aZhe: { if (pos == TEK) { retVal = c_aZhe.CharCode; } else if (pos == BASTA) { retVal = c_aZhe.CharCode; } else if (pos == ORTADA) { retVal = c_aZhe.CharCode; } else if (pos == SONDA) { retVal = c_aZhe.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    //case _f_pSxx : {if (pos == TEK) {retVal = c_ .CharCode;}else if (pos == BASTA){retVal = c_.CharCode;}else if (pos == ORTADA){retVal = c_.CharCode;}else if (pos == SONDA){retVal = c_.CharCode;}} break;
                    case _f_aSin: { if (pos == TEK) { retVal = c_aSin.CharCode; } else if (pos == BASTA) { retVal = c_iSin.CharCode; } else if (pos == ORTADA) { retVal = c_mSin.CharCode; } else if (pos == SONDA) { retVal = c_eSin.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aShin: { if (pos == TEK) { retVal = c_aShin.CharCode; } else if (pos == BASTA) { retVal = c_iShin.CharCode; } else if (pos == ORTADA) { retVal = c_mShin.CharCode; } else if (pos == SONDA) { retVal = c_eShin.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aSad: { if (pos == TEK) { retVal = c_aSad.CharCode; } else if (pos == BASTA) { retVal = c_iSad.CharCode; } else if (pos == ORTADA) { retVal = c_mSad.CharCode; } else if (pos == SONDA) { retVal = c_eSad.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aZad: { if (pos == TEK) { retVal = c_aZad.CharCode; } else if (pos == BASTA) { retVal = c_iZad.CharCode; } else if (pos == ORTADA) { retVal = c_mZad.CharCode; } else if (pos == SONDA) { retVal = c_eZad.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aTa: { if (pos == TEK) { retVal = c_aTa.CharCode; } else if (pos == BASTA) { retVal = c_aTa.CharCode; } else if (pos == ORTADA) { retVal = c_mTa.CharCode; } else if (pos == SONDA) { retVal = c_mTa.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aZa: { if (pos == TEK) { retVal = c_aZa.CharCode; } else if (pos == BASTA) { retVal = c_aZa.CharCode; } else if (pos == ORTADA) { retVal = c_mZa.CharCode; } else if (pos == SONDA) { retVal = c_mZa.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aEyn: { if (pos == TEK) { retVal = c_aEyn.CharCode; } else if (pos == BASTA) { retVal = c_iEyn.CharCode; } else if (pos == ORTADA) { retVal = c_mEyn.CharCode; } else if (pos == SONDA) { retVal = c_eEyn.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aQeyn: { if (pos == TEK) { retVal = c_aQeyn.CharCode; } else if (pos == BASTA) { retVal = c_iQeyn.CharCode; } else if (pos == ORTADA) { retVal = c_mQeyn.CharCode; } else if (pos == SONDA) { retVal = c_eQeyn.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aFe: { if (pos == TEK) { retVal = c_aFe.CharCode; } else if (pos == BASTA) { retVal = c_iFe.CharCode; } else if (pos == ORTADA) { retVal = c_mFe.CharCode; } else if (pos == SONDA) { retVal = c_eFe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aQaf: { if (pos == TEK) { retVal = c_aQaf.CharCode; } else if (pos == BASTA) { retVal = c_iQaf.CharCode; } else if (pos == ORTADA) { retVal = c_mQaf.CharCode; } else if (pos == SONDA) { retVal = c_eQaf.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aKaf: { if (pos == TEK) { retVal = c_aKaf.CharCode; } else if (pos == BASTA) { retVal = c_iKaf.CharCode; } else if (pos == ORTADA) { retVal = c_mKaf.CharCode; } else if (pos == SONDA) { retVal = c_eKaf.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aGaf: { if (pos == TEK) { retVal = c_aGaf.CharCode; } else if (pos == BASTA) { retVal = c_iGaf.CharCode; } else if (pos == ORTADA) { retVal = c_mGaf.CharCode; } else if (pos == SONDA) { retVal = c_eGaf.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aLam: { if (pos == TEK) { retVal = c_aLam.CharCode; } else if (pos == BASTA) { retVal = c_iLam.CharCode; } else if (pos == ORTADA) { retVal = c_mLam.CharCode; } else if (pos == SONDA) { retVal = c_eLam.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aMim: { if (pos == TEK) { retVal = c_aMim.CharCode; } else if (pos == BASTA) { retVal = c_iMim.CharCode; } else if (pos == ORTADA) { retVal = c_mMim.CharCode; } else if (pos == SONDA) { retVal = c_eMim.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aNun: { if (pos == TEK) { retVal = c_aNun.CharCode; } else if (pos == BASTA) { retVal = c_iNun.CharCode; } else if (pos == ORTADA) { retVal = c_mNun.CharCode; } else if (pos == SONDA) { retVal = c_eNun.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    case _f_aVav: { if (pos == TEK) { retVal = c_aVav.CharCode; } else if (pos == BASTA) { retVal = c_aVav.CharCode; } else if (pos == ORTADA) { retVal = c_aVav.CharCode; } else if (pos == SONDA) { retVal = c_aVav.CharCode; } sonrakiSagdanBirlesir = false; } break;
                    case _f_aHe2: { if (pos == TEK) { retVal = c_aHe2.CharCode; } else if (pos == BASTA) { retVal = c_iHe2.CharCode; } else if (pos == ORTADA) { retVal = c_mHe2.CharCode; } else if (pos == SONDA) { retVal = c_eHe2.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    //case _f_iHe2   : {if (pos == TEK) {retVal = c_ .CharCode;}else if (pos == BASTA){retVal = c_.CharCode;}else if (pos == ORTADA){retVal = c_.CharCode;}else if (pos == SONDA){retVal = c_.CharCode;}} break;
                    case _f_aYe: { if (pos == TEK) { retVal = c_aYe.CharCode; } else if (pos == BASTA) { retVal = c_iYe.CharCode; } else if (pos == ORTADA) { retVal = c_mYe.CharCode; } else if (pos == SONDA) { retVal = c_eYe.CharCode; } sonrakiSagdanBirlesir = true; } break;
                    //case _f_Empty  : {if (pos == TEK) {retVal = c_Empty.CharCode;   }else if (pos == BASTA){retVal = c_Empty.CharCode;}else if (pos == ORTADA){retVal = c_Empty.CharCode;}else if (pos == SONDA){retVal = c_Empty.CharCode;}} break;
                }
            }
            return retVal;
        }
     
        public static char GetUnicodeChar(String hex) 
        {
            if (!HexEncoding.InHexFormat(hex))
            {
                return '\0';
            }
            int val = Int32.Parse(hex, System.Globalization.NumberStyles.HexNumber);
            return GetUnicodeChar(val);
        }
        public static char GetUnicodeChar(int hex)
        {
            Char ret = ' ';
            #region
            CHexUnicode[] hexUnicode = new CHexUnicode[] {
                new CHexUnicode(0x0, '\u0621'),
                new CHexUnicode(0x0, '\u0622'),
                new CHexUnicode(0x0, '\u0623'),
                new CHexUnicode(0x0, '\u0624'),
                new CHexUnicode(0x0, '\u0625'),
                new CHexUnicode(0x0, '\u0626'),

                // elif
                new CHexUnicode(0x91, '\u0627'),
                new CHexUnicode(0x92, '\u0627'),
                new CHexUnicode(0x93, '\u0627'),
                
                // be
                new CHexUnicode(0x94, '\u0628'),
                new CHexUnicode(0x95, '\u0628'),
                new CHexUnicode(0x96, '\u0628'),
                new CHexUnicode(0x97, '\u0628'),

                // pe
                new CHexUnicode(0x98, '\u067E'),
                new CHexUnicode(0x99, '\u067E'),
                new CHexUnicode(0x9A, '\u067E'),
                new CHexUnicode(0x9B, '\u067E'),

                // te
                new CHexUnicode(0x9C, '\u062A'),
                new CHexUnicode(0x9D, '\u062A'),
                new CHexUnicode(0x9E, '\u062A'),
                new CHexUnicode(0x9F, '\u062A'),

                // se
                new CHexUnicode(0xA0, '\u062B'),
                new CHexUnicode(0xA1, '\u062B'),
                new CHexUnicode(0xA2, '\u062B'),
                new CHexUnicode(0xA3, '\u062B'),

                // jim
                new CHexUnicode(0xA4, '\u062C'),
                new CHexUnicode(0xA5, '\u062C'),
                new CHexUnicode(0xA6, '\u062C'),

                // Che
                new CHexUnicode(0xA7, '\u0686'),
                new CHexUnicode(0xA8, '\u0686'),

                // He-1
                new CHexUnicode(0xA9, '\u062D'),
                new CHexUnicode(0xAA, '\u062D'),
                new CHexUnicode(0xAB, '\u062D'),

                // Khe
                new CHexUnicode(0xAC, '\u062E'),
                new CHexUnicode(0xAD, '\u062E'),
                new CHexUnicode(0xAE, '\u062E'),

                // dal
                new CHexUnicode(0xAF, '\u062F'),
                
                // zal
                new CHexUnicode(0xB0, '\u0630'),
                
                // re
                new CHexUnicode(0xB1, '\u0631'),
                
                // ze
                new CHexUnicode(0xB2, '\u0632'),

                // zhe
                new CHexUnicode(0xB3, '\u0698'),

                // Sin
                new CHexUnicode(0xB5, '\u0633'),
                new CHexUnicode(0xB6, '\u0633'),
                new CHexUnicode(0xB7, '\u0633'),
                new CHexUnicode(0xB8, '\u0633'),

                // shin
                new CHexUnicode(0xB9, '\u0634'),
                new CHexUnicode(0xBA, '\u0634'),
                new CHexUnicode(0xBB, '\u0634'),
                new CHexUnicode(0xBC, '\u0634'),

                // Sad
                new CHexUnicode(0xBD, '\u0635'),
                new CHexUnicode(0xBE, '\u0635'),
                new CHexUnicode(0xBF, '\u0635'),
                new CHexUnicode(0xC0, '\u0635'),

                // Zad
                new CHexUnicode(0xC1, '\u0636'),
                new CHexUnicode(0xC2, '\u0636'),
                new CHexUnicode(0xC3, '\u0636'),
                new CHexUnicode(0xC4, '\u0636'),

                // Ta
                new CHexUnicode(0xC5, '\u0637'),
                new CHexUnicode(0xC6, '\u0637'),

                // Za
                new CHexUnicode(0xC7, '\u0638'),
                new CHexUnicode(0xC8, '\u0638'),

                // Eyn
                new CHexUnicode(0xC9, '\u0639'),
                new CHexUnicode(0xCA, '\u0639'),
                new CHexUnicode(0xCB, '\u0639'),
                new CHexUnicode(0xCC, '\u0639'),

                // Qeyn
                new CHexUnicode(0xCD, '\u063A'),
                new CHexUnicode(0xCE, '\u063A'),
                new CHexUnicode(0xCF, '\u063A'),
                new CHexUnicode(0xD0, '\u063A'),

                // fe
                new CHexUnicode(0xD1, '\u0641'),
                new CHexUnicode(0xD2, '\u0641'),
                new CHexUnicode(0xD3, '\u0641'),
                new CHexUnicode(0xD4, '\u0641'),

                // Qaf
                new CHexUnicode(0xD5, '\u0642'),
                new CHexUnicode(0xD6, '\u0642'),
                new CHexUnicode(0xD7, '\u0642'),
                new CHexUnicode(0xD8, '\u0642'),

                // Kaf
                new CHexUnicode(0xD9, '\u0643'),
                new CHexUnicode(0xDA, '\u0643'),
                new CHexUnicode(0xDB, '\u0643'),
                new CHexUnicode(0xDC, '\u0643'),

                // Gaf-2
                new CHexUnicode(0xDD, '\u06AF'),
                new CHexUnicode(0xDE, '\u06AF'),
                new CHexUnicode(0xDF, '\u06AF'),
                new CHexUnicode(0xE0, '\u06AF'),
                
                // Lam
                new CHexUnicode(0xE1, '\u0644'),
                new CHexUnicode(0xE2, '\u0644'),
                new CHexUnicode(0xE3, '\u0644'),
                new CHexUnicode(0xE4, '\u0644'),

                // mim
                new CHexUnicode(0xE5, '\u0645'),
                new CHexUnicode(0xE6, '\u0645'),
                new CHexUnicode(0xE7, '\u0645'),
                new CHexUnicode(0xE8, '\u0645'),

                // nun
                new CHexUnicode(0xE9, '\u0646'),
                new CHexUnicode(0xEA, '\u0646'),
                new CHexUnicode(0xEB, '\u0646'),
                new CHexUnicode(0xEC, '\u0646'),

                // He-2
                new CHexUnicode(0xEE, '\u0647'),
                new CHexUnicode(0xEF, '\u0647'),
                new CHexUnicode(0xF0, '\u0647'),
                new CHexUnicode(0xF1, '\u0647'),

                // vav
                new CHexUnicode(0xED, '\u0648'),
                                
                // Ye
                new CHexUnicode(0xF2, '\u06CC'),
                new CHexUnicode(0xF3, '\u06CC'),
                new CHexUnicode(0xF4, '\u06CC'),
                new CHexUnicode(0xF5, '\u06CC'),

                // rakamlar
                new CHexUnicode(0x80, '\u06F0'),
                new CHexUnicode(0x81, '\u06F1'),
                new CHexUnicode(0x82, '\u06F2'),
                new CHexUnicode(0x83, '\u06F3'),
                new CHexUnicode(0x84, '\u06F4'),
                new CHexUnicode(0x85, '\u06F5'),
                new CHexUnicode(0x86, '\u06F6'),
                new CHexUnicode(0x87, '\u06F7'),
                new CHexUnicode(0x88, '\u06F8'),
                new CHexUnicode(0x89, '\u06F9')
            };
            #endregion

            for (int i = 0; i < hexUnicode.Length; ++i) {
                if (hex == hexUnicode[i].m_hexVal) {
                    return hexUnicode[i].m_unicodeVal;
                }
            }
            return ret;
        }
    }
}