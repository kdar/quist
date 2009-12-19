using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Quist
{
  static class Win32
  {
    public const uint PROCESS_ALL_ACCESS = (uint)(0x000F0000L | 0x00100000L | 0xFFF);
    public const uint MEM_COMMIT = 0x1000;
    public const uint MEM_RELEASE = 0x8000;
    public const uint PAGE_READWRITE = 0x04;

    //================================
    [DllImport("kernel32")]
    public static extern IntPtr OpenProcess(uint dwDesiredAccess, bool bInheritHandle, 
      int dwProcessId);

    //================================
    [DllImport("kernel32")]
    public static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress,
      int dwSize, uint flAllocationType, uint flProtect);

    //================================
    [DllImport("kernel32")]
    public static extern bool VirtualFreeEx(IntPtr hProcess, IntPtr lpAddress, int dwSize,
      uint dwFreeType);

    //================================
    [DllImport("kernel32")]
    public static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress,
      IntPtr lpBuffer, int dwSize, out int lpNumberOfBytesWritten);

    //================================
    [DllImport("kernel32")]
    public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress,
      IntPtr lpBuffer, int dwSize, IntPtr lpNumberOfBytesRead);

    //================================
    [DllImport("kernel32")]
    public static extern bool CloseHandle(IntPtr hObject);

    //================================
    [DllImport("kernel32")]
    public static extern IntPtr CreateRemoteThread(
      IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize,
      IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags,
      out uint lpThreadId
    );

    //================================
    [DllImport("kernel32", SetLastError = true)]
    public static extern Int32 WaitForSingleObject(IntPtr Handle, Int32 Wait);

    //================================
    [DllImport("kernel32.dll")]
    public static extern IntPtr LoadLibrary(string lpFileName);

    //================================
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool FreeLibrary(IntPtr hModule);

    //================================
    [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true)]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
  }
}

