using System;
using System.Collections.Generic;
using System.Text;

namespace ParsianConverter
{
    class CConvertEngine
    {
        IConverterForm m_form;
        public CConvertEngine(IConverterForm form)
        {
            m_form = form;
        }

        public void ConvertParsianToHex()
        {
            String parsian = m_form.GetPersianText();
           
            CFarsiCharacters f = new CFarsiCharacters();
            Char[] x = f.GetFarisiCharArray(parsian);
            
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < x.Length; ++i) {
                sb.Append(String.Format("{0:X}", (int)x[i]));
            }
            m_form.SetConvertedText(sb.ToString());
        }

        public void ConvertHexToPersian()
        {
            String persianText;
            StringBuilder sb = new StringBuilder();
            String hex = m_form.GetConvertedText();
            if ((hex.Length % 2) != 0) {
                return;
            }
            string tmp = "";
            String tmp2 = "";

            //for (int i = 0; i < hex.Length / 2; ++i)
            for (int i = hex.Length / 2; i > 0; --i)
            {
                tmp2 = hex.Substring((i * 2) - 2, 2);
                Char c = CFarsiCharacters.GetUnicodeChar(tmp2);
                sb.Append(c);
            }
            m_form.SetPersianText(sb.ToString());
        }

        private byte[] ConvertToHex(String str)
        {
            byte[] bytes;
            bytes = Encoding.Unicode.GetBytes(str);
            StringBuilder sb = new StringBuilder();
            string[] vals = null;
            char[] delim = " ".ToCharArray();

            int j = 0;
            bool basta = false;
            bool sonda = false;

            for (int i = 0; i < bytes.Length; ++i)
            {
                sb.Append(String.Format("{0} ", bytes[i]));
                if ((i % 2) != 0)
                {
                    if (i < bytes.Length - 2)
                    {
                        if (bytes[i + 1] == 32 && bytes[i + 2] == 0)
                        {
                            // bosluk
                            sonda = true;
                        }
                        else
                        {
                            sonda = false;
                        }
                    }
                    if (i > 2)
                    {
                        if (bytes[i - 1] == 32 && bytes[i - 2] == 0)
                        {
                            basta = true;
                        }
                        else
                        {
                            basta = false;
                        }
                    }
                    if (i == 0 || i == 1)
                    {
                        if (bytes[0] != 32 && bytes[1] != 0)
                        {
                            basta = true;
                        }
                    }
                    /*sb.Append(editFarisi.Text[j]);
                    if (basta)
                    {
                        sb.Append(" -Basta");
                    }
                    if (sonda)
                    {
                        sb.Append(" -Sonda");
                    }
                    if (!basta || !sonda)
                    {
                        sb.Append(" -Ortada");
                    }
                     * */
                    sb.Append("\r\n");
                    ++j;
                }
            }

            return bytes;
        }
    }
}
