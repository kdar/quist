using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace Quist
{
  public partial class LocateVentriloDialog : Form
  {
    private bool m_bStopSearch;
    private delegate void StringEvent(String sDir);
    private delegate void SimpleEvent();
    private event StringEvent NewDir;
    private event StringEvent FoundDir;
    private String m_sPath;
    private String m_sSearchPath;

    public String Path
    {
      get
      {
        return m_sPath;
      }
      set
      {
        m_sPath = value;
      }
    }

    public LocateVentriloDialog()
    {
      InitializeComponent();
      pathTextBox.Select();
      NewDir += OnNewDir;
      FoundDir += OnFoundDir;
    }

    private void OnNewDir(String sDir)
    {
      textBox2.Text = sDir;
    }

    private void OnFoundDir(String sDir)
    {
      if (sDir != null && sDir.Length > 0) {
        pathTextBox.Text = sDir;
        textBox2.Text = "Found the Ventrilo directory";
      }
    }

    private void button1_Click(object sender, EventArgs e)
    {
      using (FolderBrowserDialog dialog = new FolderBrowserDialog()) {
        dialog.SelectedPath = Environment.SpecialFolder.ProgramFiles.ToString();
        if (dialog.ShowDialog() == DialogResult.OK) {
          pathTextBox.Text = dialog.SelectedPath;
        }
      }
    }

    private void button2_Click(object sender, EventArgs e)
    {
      m_bStopSearch = false;
      m_sSearchPath = "";
      Thread t = new Thread(StartFindVentrilo);
      t.Start();
    }

    public void StartFindVentrilo()
    {      
      FindVentrilo(System.Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles));
      if (m_sSearchPath == null || m_sSearchPath.Length == 0)
        FindVentrilo(System.Environment.GetFolderPath(Environment.SpecialFolder.Programs));

      if (m_sSearchPath != null && m_sSearchPath.Length > 0) {
        if (this.FoundDir != null)
          this.FoundDir(m_sSearchPath);
      }
    }

    public void FindVentrilo(String sPath)
    {
      if (m_bStopSearch)
        return;

      if (this.NewDir != null)
        this.NewDir(sPath);

      String []dirs = Directory.GetDirectories(sPath);
      foreach (String sSubDir in dirs) {
        FindVentrilo(sSubDir);
      }

      string []fileNames = Directory.GetFiles(sPath);
      foreach (String sFileName in fileNames) {
        if (m_bStopSearch)
          return;

        if (sFileName.Equals(sPath + "\\Ventrilo.exe", StringComparison.InvariantCultureIgnoreCase)) {
          m_sSearchPath = sPath;
          m_bStopSearch = true;
        }
      }
    }

    private void okButton_Click(object sender, EventArgs e)
    {
      this.Path = pathTextBox.Text;
      this.DialogResult = DialogResult.OK;
      this.Close();
    }

    private void cancelButton_Click(object sender, EventArgs e)
    {
      this.DialogResult = DialogResult.Cancel;
      this.Close();
    }
  }
}
