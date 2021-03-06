using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ParsianConverter
{
    public partial class Form1 : Form, IConverterForm   
    {
        CConvertEngine m_engine;
        public Form1()
        {
            InitializeComponent();
            m_engine = new CConvertEngine(this);
        }

        private void btnConvert_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Trim().Length == 0) {
                m_engine.ConvertHexToPersian();
            }
            else if (textBox2.Text.Trim().Length == 0)
            {
                m_engine.ConvertParsianToHex();
            }
            else {
            }
        }

        #region IConverterForm Members

        public string GetPersianText()
        {
            return textBox1.Text;
        }

        public string GetConvertedText()
        {
            //return Encoding.Unicode.GetBytes(textBox2.Text);
            return textBox2.Text;
        }
        public void SetPersianText(String str)
        {
            textBox1.Text = str;
        }
        public void SetConvertedText(String str)
        {
            textBox2.Text = str;
        }


        #endregion

        private void btnClear_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox1.Focus();
        }

        private void btnCopy_Click(object sender, EventArgs e)
        {
            Clipboard.SetDataObject(textBox2.Text);
        }
    }
}