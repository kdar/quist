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
  public partial class PreviewDlg : Form
  {
    public delegate void BrowseEvent(String sUrl);
    public event BrowseEvent Browse;

    public String Comment
    {
      get
      {
        return commentTextBox.Text;
      }
      set
      {
        commentTextBox.Text = value;
      }
    }

    public String Url
    {
      get
      {
        return urlTextBox.Text;
      }
      set
      {
        urlTextBox.Text = value;
      }
    }

    private String m_sUser = "";
    public String User
    {
      get
      {
        return m_sUser;
      }
      set
      {
        m_sUser = value;
        this.Text = "Preview of " + m_sUser;
      }
    }

    public PreviewDlg()
    {
      InitializeComponent();
    }

    private void button1_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void button2_Click(object sender, EventArgs e)
    {
      if (this.Browse != null) {
        this.Browse(urlTextBox.Text);
      }

      this.Close();
    }

    private void button3_Click(object sender, EventArgs e)
    {
      if (this.Browse != null)
      {
        this.Browse(commentTextBox.Text);
      }

      this.Close();
    }
  }
}
