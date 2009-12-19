using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Quist
{
  public partial class PageBombSettingsDlg : Form
  {
    private Decimal m_nPages;
    private Decimal m_nInterval;

    public delegate void SendEvent();
    public event SendEvent Send;

    public Decimal Pages
    {
      get
      {
        return m_nPages;
      }
      set
      {
        m_nPages = value;
      }
    }

    public Decimal Interval
    {
      get
      {
        return m_nInterval;
      }
      set
      {
        m_nInterval = value;
      }
    }

    public PageBombSettingsDlg()
    {
      Pages = 50;
      Interval = 0;
      InitializeComponent();
    }

    private void buttonCancel_Click(object sender, EventArgs e)
    {
      this.Hide();
    }

    private void buttonSave_Click(object sender, EventArgs e)
    {
      this.Pages = numericUpDownPages.Value;
      this.Interval = numericUpDownInterval.Value;
      this.Hide();
    }

    private void buttonSaveAndSend_Click(object sender, EventArgs e)
    {
      this.buttonSave_Click(sender, e);
      if (this.Send != null) {
        this.Send();
      }
      this.Hide();
    }

    private void PageBombSettingsDlg_FormClosing(object sender, FormClosingEventArgs e)
    {
      e.Cancel = true;
      this.Hide();
    }
  }
}
