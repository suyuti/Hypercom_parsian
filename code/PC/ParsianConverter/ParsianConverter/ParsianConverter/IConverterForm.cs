using System;
using System.Collections.Generic;
using System.Text;

namespace ParsianConverter
{
    interface IConverterForm
    {
        String GetPersianText();
        string GetConvertedText();
        void SetPersianText(String str);
        void SetConvertedText(String str);
    }
}
