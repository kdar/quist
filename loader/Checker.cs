using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace QuistLoader
{
  //--------------------------------
  public class Checker
  {
    private CustomUIControls.TaskbarNotifier m_taskbarNotifer;
    public CustomUIControls.TaskbarNotifier TaskbarNotifier
    {
      get
      {
        return m_taskbarNotifer;
      }
    }

    public EventHandler NewVersion;
    public EventHandler LatestVersion;
    public EventHandler Failed;
    private QuistLoader.Config m_config;

    //================================
    public Checker()
    {
      m_config = new QuistLoader.Config();

      Bitmap skin_bmp = new Bitmap(@"resources\notification.png");
      Bitmap close_bmp = new Bitmap(@"resources\close.png");

      m_taskbarNotifer = new CustomUIControls.TaskbarNotifier();
      m_taskbarNotifer.SetBackgroundBitmap(skin_bmp, Color.FromArgb(0, 0, 0));
      m_taskbarNotifer.SetCloseBitmap(close_bmp, Color.FromArgb(0, 0, 0), new Point(280, 8));
      m_taskbarNotifer.TitleRectangle = new Rectangle(5, 3, 170, 25);
      m_taskbarNotifer.ContentRectangle = new Rectangle(0, 30, 300, 70);
      m_taskbarNotifer.TitleClick += new EventHandler(this.TitleClick);
      m_taskbarNotifer.ContentClick += new EventHandler(this.ContentClick);
      m_taskbarNotifer.CloseClick += new EventHandler(this.CloseClick);

      m_taskbarNotifer.CloseClickable = true;
      //m_taskbarNotifer.TitleClickable = true;
      m_taskbarNotifer.ContentClickable = true;
      //m_taskbarNotifer.EnableSelectionRectangle = true;
      m_taskbarNotifer.KeepVisibleOnMousOver = true;
      m_taskbarNotifer.ReShowOnMouseOver = true;
    }

    //================================
    public void Check()
    {
      Version v = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;

      m_config.LoadConfig("http://outroot.com/quist/backend/UpdateVersion", null, null, "", false);

      // Getting the version didn't succeed. Silently fail.
      if (m_config.AvailableVersion == null) {
        if (this.Failed != null) {
          this.Failed(this, null);
        }
        return;
      }

      if (m_config.AvailableVersion.CompareTo(v.ToString()) > 0)
      {
        if (this.NewVersion != null) {
          this.NewVersion(this, null);
        }
        m_taskbarNotifer.Show(
          "New version of Quist",
          String.Format(
            "Your Quist is out of date ({0}). Please click here to download the new Quist ({1}) or go to the website.",
            v,
            m_config.AvailableVersion
          ), 500, 7000, 500
        );
      } else {
        if (this.LatestVersion != null) {
          this.LatestVersion(this, null);
        }
      }
    }

    //================================
    void CloseClick(object obj, EventArgs ea)
    {
      //MessageBox.Show("Closed was Clicked");
    }

    //================================
    void TitleClick(object obj, EventArgs ea)
    {
      //MessageBox.Show("Title was Clicked");
    }

    //================================
    void ContentClick(object obj, EventArgs ea)
    {
      System.Diagnostics.Process.Start(m_config.AppFileURL);
      ((Form)obj).Hide();
    }
  }
}
