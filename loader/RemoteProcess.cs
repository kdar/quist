using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Quist
{
  //--------------------------------
  public class RemoteProcess
  {
    private IntPtr hProcess = IntPtr.Zero;

    //================================
    public RemoteProcess(int processId)
    {
      hProcess = Win32.OpenProcess(Win32.PROCESS_ALL_ACCESS, false, processId);
      if (hProcess == IntPtr.Zero)
        throw new ApplicationException("Failed to access process");
    }

    //================================
    public IntPtr AllocAndWrite(IntPtr data, int nSize)
    {
      IntPtr buffer = Win32.VirtualAllocEx(hProcess, IntPtr.Zero, nSize, Win32.MEM_COMMIT, Win32.PAGE_READWRITE);
      if (buffer == IntPtr.Zero)
        throw new SystemException("Failed to allocate memory in remote process");

      int bytesWritten;
      bool bSuccess = Win32.WriteProcessMemory(hProcess, buffer, data, nSize, out bytesWritten);
      if (!bSuccess)
        throw new SystemException("Failed to write to process memory");

      return buffer;
    }

    //================================
    public IntPtr AllocAndWrite(String data)
    {
      IntPtr buffer = Win32.VirtualAllocEx(hProcess, IntPtr.Zero, data.Length, Win32.MEM_COMMIT, Win32.PAGE_READWRITE);
      if (buffer == IntPtr.Zero)
        throw new SystemException("Failed to allocate memory in remote process");

      IntPtr dataPtr = Marshal.StringToHGlobalAnsi(data);

      int bytesWritten;
      bool bSuccess = Win32.WriteProcessMemory(hProcess, buffer, dataPtr, data.Length, out bytesWritten);

      Marshal.FreeHGlobal(dataPtr);

      if (!bSuccess)
        throw new SystemException("Failed to write to process memory");

      return buffer;
    }

    //================================
    public IntPtr CreateRemoteThreadAndWait(IntPtr function, IntPtr param)
    {
      uint threadId;

      IntPtr hThread = Win32.CreateRemoteThread(
        hProcess,
        IntPtr.Zero,
        0,
        function,
        param,
        0,
        out threadId);
      if (hThread == IntPtr.Zero)
        throw new SystemException("Failed to CreateRemotethread");

      Win32.WaitForSingleObject(hThread, unchecked((int)-1));

      return hThread;
    }
  }
}
