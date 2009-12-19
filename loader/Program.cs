using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Diagnostics;
using System.ComponentModel;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;

namespace Quist
{
  [StructLayout(LayoutKind.Sequential)]
  public struct InjectData
  {
    public IntPtr pDllNameAddr;
    public IntPtr pFuncNameAddr;
    public IntPtr fnLoadLibrary;
    public IntPtr fnGetProcAddress;
    public IntPtr pExtraData;
  }

  public struct ExtraData
  {
    public int pPath;
  }

  static class Program
  {
    //[DllImport("QuistDll")]
    //static extern int GetInjectCode(IntPtr dst);

    static void OnTaskbarNotifierVisibleChanged(Object obj, EventArgs ea)
    {
      if (!((Form)obj).Visible) {
        DoExit(obj, ea);
      }
    }

    static void DoExit(Object obj, EventArgs ea)
    {
      Environment.Exit(0);
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);

      String sCommandLineArgs = "";
      String []asCommandLineArgs = Environment.GetCommandLineArgs();
      //MessageBox.Show(String.Format("{0}", asCommandLineArgs));
      if (asCommandLineArgs.Length >= 1) {
        asCommandLineArgs[0] = "";
      }
      foreach (String sArg in asCommandLineArgs) {
        if (!String.IsNullOrEmpty(sArg))
          sCommandLineArgs += sArg + " ";
      }
      sCommandLineArgs = sCommandLineArgs.Trim();

      //MessageBox.Show(String.Format("{0}", sCommandLineArgs));

      try
      {
        String sQuistPath = Environment.CurrentDirectory;
        String sVentPath = Properties.Settings.Default.VentriloPath;
        String sVentFile = sVentPath + "\\Ventrilo.exe";
        String sDllFile32 = sQuistPath + "\\QuistDll32.dll";
        String sDllFile64 = sQuistPath + "\\QuistDll64.dll";

        while (!File.Exists(sVentFile)) {
          using (LocateVentriloDialog dialog = new LocateVentriloDialog()) {
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK) {
              sVentPath = dialog.Path;
              sVentFile = sVentPath + "\\Ventrilo.exe";
            } else if (result == DialogResult.Cancel) {
              return;
            }
          }
        }

        //Update the setting
        Properties.Settings.Default.VentriloPath = sVentPath;
        Properties.Settings.Default.Save();

        // Put all the DLLs into the GAC
        try {
          EasyHook.Config.Register("A Ventrilo modification.", "Quist.exe", "QuistDllEx.dll", "QuistDll32.dll");
          EasyHook.Config.Register("A Ventrilo modification.", "QuistDll64.dll");
        } catch (ApplicationException) {
          
        }

        //Start the Ventrilo process.
        Process process = new Process();
        process.StartInfo.FileName = sVentFile;
        process.StartInfo.WorkingDirectory = sVentPath;//Environment.CurrentDirectory + "\\..";
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.Arguments = sCommandLineArgs;
        process.Start();

        //Wait for the process to load and accept input from the user. Then we
        //can do our injection magic.
        process.WaitForInputIdle();

        EasyHook.RemoteHooking.Inject(process.Id, sDllFile32, sDllFile64, sQuistPath);

        // NOTE: We do not let EasyHook run ventrilo and inject the dll into it.
        // If we do this, there are so many crashing problems that occur. It is
        // safer to inject the dll after the process is already running. This is why
        // we run the process ourselfs, then just call inject into its process id.
        /*int nProcessId;
        RemoteHooking.CreateAndInject(sVentFile,
          sCommandLineArgs,
          sDllFile32,
          sDllFile64,
          out nProcessId,
          sQuistPath);*/

        Form form = new Form();
        form.Visible = false;

        QuistLoader.Checker checker = new QuistLoader.Checker();
        checker.TaskbarNotifier.VisibleChanged += OnTaskbarNotifierVisibleChanged;
        checker.LatestVersion += DoExit;
        checker.Failed += DoExit;
        checker.Check();

        Application.Run();
      }
      /*catch (Win32Exception)
      {
        MessageBox.Show("Error finding Ventrilo.exe. Make sure you're running Quist in the same directory as Ventrilo.exe.");
      }*/
      catch (Exception e)
      {
        MessageBox.Show(e.ToString() + "\n\n" + "Please notify the maker of this program of the problem.", "Quist Exception");
      }
    }
  }
}
