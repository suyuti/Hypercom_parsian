using System;
using System.Collections.Generic;
using System.Text;

namespace ParsianConverter
{
    class CCharDefinition
    {
        public char CharCode;
        public int CharWdth;
        public int CharOptions;

        public CCharDefinition(char code, int w, int opt)
        {
            CharCode    = code;
            CharWdth    = w;
            CharOptions = opt;
        }
    }
    class CHexUnicode 
    {
        public int m_hexVal;
        public char m_unicodeVal;
        public CHexUnicode(int h, Char u) {
            m_hexVal            = h;
            m_unicodeVal        = u;
        }
    };
}
