INTRODUCTION

First off, I wanted to do most of this code in .NET. This posses some problems with getting this whole system to work.
In .NET, a dll has no DllMain. So I am unable to execute code when the dll first gets injected into Ventrilo. But the
dll I made is in C++/CLI, so I am able to have a DllMain which will run natively. The problem is you are not allowed
to execute or call any managed code from within the DllMain. So this wasn't a good option for me. One of the ways I thought
about doing it was to first load the dll in Ventrilo, and then call an unmanaged function in the dll that would then call
a managed function to do all the initialization.

Of course I used the loader/dll approach. How much work the loader does and how much work the dll does is up to the
programmer. You can have the dll do all the memory modifications and execution, or have the loader do it. It really
doesn't matter. In my final product, I try to have the loader do as much of the work as possible, but I still rely
on the unmanaged part of the dll for producing the loader code itself.

THINGS TO NOTE

1. Why do I use LoadLibraryA instead of LoadLibrary?
  There are sometimes two different versions of functions in the win32 library: the ansi and unicode version.
  LoadLibrary doesn't exist implicitly, it is a define in the windows.h header file. It either gets compiled
  as LoadLibraryA or LoadLibraryW based on the compiler flags. So since there is no LoadLibrary in kernel32.dll, we
  must use either LoadLibraryA or LoadLibraryW.
  
2. Where does the InjectCodeStart go?
  This MUST be put as the end of your source code. The reason is we can't find out the size of this function at
  compile or runtime (there actually are a couple of ways I can think of... like searching for 0xC3(RET) or
  some other end of function marker we can think of). So we just start copying from the start of the function and onwards
  until 4096 bytes. So as long as the loader is less than this, we're fine.
  
RESOURCES

http://win32.mvps.org/processes/remthread/remthread.cpp
This helped me in figuring out the final method used by the loader/dll.

http://dotnetjunkies.com/WebLog/chris.taylor/archive/2004/05/31/14828.aspx
Helped with C# p/invoke and marshalling

http://www.codeproject.com/KB/threads/winspy.aspx
Helped with one of the methods I used by the loader/dll (but I changed it in favor of another).

LOADING DLL AND CALLING INITIALIZATION

Now comes the issue of how to get this dll loaded and the intialization function called. There are several ways to do this,
I will list a few that I came up with.

1) The assembly in array way.
  The steps are as follows:
  1. Put the assembly loader code into a byte array in C#.
  2. VirtualAlloc and Write the name of the dll into Ventrilo's memory. Save this address as dllname.
  3. VirtualAlloc and Write the name of the initialization function into Ventrilo's memory. Save this address as funcname.
  4. Modify the assembly code and insert the addresses addr1 and addr2 where appropriate.
  5. VirtualAlloc and Write the loader code into Ventrilo's memory.
  6. CreateRemoteThread the loader code.

  Here is an example of the assembly code:
  byte[] c = { 
    0x55,                           //PUSH    EBP
    0x8B,0xEC,                      //MOV     EBP, ESP
    0x83,0xEC,0x00,                 //SUB     ESP, 0
    0x68,0x00,0x00,0x00,0x33,       //PUSH    14002B    ; ASCII "CreVentDll.dll"
    0xFF,0x15,0xC4,0x72,0x4E,0x00,  //CALL    NEAR DWORD PTR DS:[<&KERNEL32.Lo>; kernel32.LoadLibraryA
    0x68,0x00,0x00,0x60,0x14,       //PUSH    14003A    ; ASCII "Initialize"
    0x50,                           //PUSH    EAX
    0xFF,0x15,0xC8,0x72,0x4E,0x00,  //CALL    NEAR DWORD PTR DS:[<&KERNEL32.Ge>; kernel32.GetProcAddress
    0xFF,0xD0,                      //CALL    NEAR EAX
    0x8B,0xE5,                      //MOV     ESP, EBP
    0x5D,                           //POP     EBP
    0xC3                            //RETN    
  };
  
  As you can see, where the ascii strings "CreVentDll.dll" and "Initialize" are, the addresses are not correct.
  So once you write to Ventrilo's memory those strings and get the addresses of those two strings, you just have
  to write the changes back to the array like this:
  
  int dllnamebin = dllname.ToInt32(); //dllname is an IntPtr to the location of "CreVentDll.dll" in Ventrilo's memory
  c[7] = (byte)(dllnamebin & 0x000000FF);
  c[8] = (byte)((dllnamebin & 0x0000FF00) >> 8);
  c[9] = (byte)((dllnamebin & 0x00FF0000) >> 16);
  c[10] = (byte)((dllnamebin & 0xFF000000) >> 24);

  int funcnamebin = funcname.ToInt32(); //funcname is an IntPtr to the location of "Initialize" in Ventrilo's memory
  c[18] = (byte)(funcnamebin & 0x000000FF);
  c[19] = (byte)((funcnamebin & 0x0000FF00) >> 8);
  c[20] = (byte)((funcnamebin & 0x00FF0000) >> 16);
  c[21] = (byte)((funcnamebin & 0xFF000000) >> 24);
  
  Remember that x86 is in little endian order, which is why we're writing the address backwards.
  After this, just write that byte array to Ventrilo's memory and CreateRemoteThread execute the code.
  
2) Screw assembly and use compiled C++ code.

  There are several ways to do it this way. They all have the same basic idea, the difference lies in how
  you get the actual addresses of the LoadLibraryA, GetProcAddress, dll name, and func name to this code.
  This code is going to be compiled without the knowledge of these locations, so you somehow have to get the 
  addresses of the above 4 items to the code at runtime.
  
  1. Fixed addresses for everything and just use the compiled LoadLibraryA and GetProcAddress locations (THIS FAILS)
    The code would look something like this:
    void __stdcall InjectCodeStart() {
      typedef void (*InitializeFn)();
      HMODULE hModule = LoadLibraryA((char *)0x1234);
      InitializeFn fn = (InitializeFn)GetProcAddress(hModule, (char *)0x5678);
      (*fn)();
    }
    
    The reason this fails is that when this code is compiled the functions are compiled as jmp locations, which are
    addresses that will only be contained within this executeable and not the Ventrilo executeable. You would also
    need to make sure that you could always put "CreVentDll.dll" and "Initialize" in static locations so that this
    function would know how to access them.
    
  2. This is a modification of the above. This time we will have function pointers to the LoadLibraryA and GetProcAddress, and
    we will set these pointers to point to the correct location in memory where these functions are. It is important to note that
    in every application (at least at the time of this writing.. until operating systems make themselves more secure) the kernel32.dll
    gets loaded in the same location. So we can take advatange of this and get the addresses of these functions before we inject the code
    into the Ventrilo binary. Also, we no longer will have static locations of the strings "CreVentDll.dll" and "Initialize"... so we need
    a way to modify the addresses in this function.
    
    Here is the code:
    typedef HMODULE (WINAPI *LoadLibraryFn)(LPCSTR lp);
    typedef FARPROC (WINAPI *GetProcAddressFn)(HMODULE hModule, LPCSTR lpProcName);
    struct InjectData {
      LoadLibraryFn fnLoadLibrary;
      GetProcAddressFn fnGetProcAddress;
    } data;
    HMODULE hk32 = LoadLibrary("kernel32.dll");
    data.fnLoadLibrary = (LoadLibraryFn)GetProcAddress(hk32, "LoadLibraryA");
    data.fnGetProcAddress = (GetProcAddressFn)GetProcAddress(hk32, "GetProcAddress");
    void __stdcall InjectCodeStart() {  
      int nDllAddr = 0x1234; //change to the correct address
      int nFuncAddr = 0x5678; //change to the correct address
    
      HMODULE hModule = data.fnLoadLibrary((char *)nDllAddr);
      InitializeFn fn = (InitializeFn)data.fnGetProcAddress(hModule, (char *)nFuncAddr);
      (*fn)();
    }
    
    So we get the address of LoadLibraryA and GetProcAddress, and we save them into function pointers. Since we're exploiting the fact
    that kernel32.dll is loaded in the same location in every process, this code will work.
    Now comes the question of how we update the variables nDllAddr and nFuncAddr. We do this by knowing how the disassembly looks like
    and by knowing the opcodes. Here is the disassembly of this function:
    
    10002300   .  55                           PUSH    EBP
    10002301   .  8BEC                         MOV     EBP, ESP
    10002303   .  83EC 10                      SUB     ESP, 10
    10002306   .  C745 F0 34120000             MOV     DWORD PTR SS:[EBP-10], 1234
    1000230D   .  C745 FC 78560000             MOV     DWORD PTR SS:[EBP-4], 5678
    10002314   .  8B45 F0                      MOV     EAX, DWORD PTR SS:[EBP-10]
    10002317   .  50                           PUSH    EAX                              ; /FileName
    10002318   .  E8 93FEFFFF                  CALL    <JMP.&KERNEL32.LoadLibraryA>     ; \LoadLibraryA
    1000231D   .  8945 F4                      MOV     DWORD PTR SS:[EBP-C], EAX
    10002320   .  8B4D FC                      MOV     ECX, DWORD PTR SS:[EBP-4]
    10002323   .  51                           PUSH    ECX                              ; /ProcNameOrOrdinal
    10002324   .  8B55 F4                      MOV     EDX, DWORD PTR SS:[EBP-C]        ; |
    10002327   .  52                           PUSH    EDX                              ; |hModule
    10002328   .  E8 7DFEFFFF                  CALL    <JMP.&KERNEL32.GetProcAddress>   ; \GetProcAddress
    1000232D   .  8945 F8                      MOV     DWORD PTR SS:[EBP-8], EAX
    10002330   .  FF55 F8                      CALL    NEAR DWORD PTR SS:[EBP-8]
    10002333   .  8BE5                         MOV     ESP, EBP
    10002335   .  5D                           POP     EBP
    10002336   .  C3                           RETN
    
    This is how the function looks like when it's compiled. You see the:
      MOV     DWORD PTR SS:[EBP-10], 1234
    and
      MOV     DWORD PTR SS:[EBP-4], 5678
    assembly instructions. What these are doing is putting these values on the stack. If you have a look at their corresponding opcodes:
      C745 F0 34120000
    and
      C745 FC 78560000
    You notice that the value they are storing is in the last 32 bytes of this instruction, in little endian order. So imagine if this were
    an array or we had a pointer to this data. All we would have to do is something like:
      memcpy(pDst+9, (void *)&nDllNameAddr, 4);
      memcpy(pDst+16, (void *)&nFuncNameAddr, 4);
    Where pDst is a pointer to this data (Example: BYTE *pDst = (BYTE *)InjectCodeStart;) and nDllNameAddr and nFuncNameAddr are the addresses
    of "CreVentDll.dll" and "Initialize" in Ventrilo's memory. 
    So basically using the loader/dll idea, you would have the loader put "CreVentDll.dll" and "Initialize" into Ventrilo's memory, then call 
    a function in the dll to get this code while passing the addresses of "CreVentDll.dll" and "Initialize". This function would then return
    to the C# loader, the dll loader code with the modified addresses.
    
    Example:
    extern "C" DLL_API int GetInjectCode(BYTE *pDst, int nDllNameAddr, int nFuncNameAddr) {
      //We use the assumption that kernel32.dll is loaded at the same memory address
      //in every process.
      HMODULE hk32 = LoadLibrary("kernel32.dll");
      data.fnLoadLibrary = (LoadLibraryFn)GetProcAddress(hk32, "LoadLibrary");
      data.fnGetProcAddress = (GetProcAddressFn)GetProcAddress(hk32, "GetProcAddress");

      int nSize = 4096;
    
      memcpy(pDst, InjectCodeStart, nSize);
      memcpy(pDst+9, (void *)&nDllNameAddr, 4);
      memcpy(pDst+16, (void *)&nFuncNameAddr, 4);
    
      return nSize;
    }

    Then just call this function from the C# loader with the addresses, and we have the dll loader code we can then put into Ventrilo's memory
    and run.
    
  3. This is the method I ended up using. This method uses the same basic concepts as the above, but instead of having to modify code at runtime
    (and therefore know how it is compiled into assembly and know certain offsets) we will make code that requires data from runtime variables.
    The InjectCodeStart function will have a data structure passed to it that will have all the information it needs to load the dll and run 
    the initialization function.
    
    Example:
    typedef HMODULE (WINAPI *LoadLibraryFn)(LPCSTR lp);
    typedef FARPROC (WINAPI *GetProcAddressFn)(HMODULE hModule, LPCSTR lpProcName);
    struct InjectData {
      int nDllNameAddr;
      int nFuncNameAddr;
      LoadLibraryFn fnLoadLibrary;
      GetProcAddressFn fnGetProcAddress;
    } injectdata;
    
    extern "C" DLL_API int GetInjectCode(BYTE *pDst) {
      int nSize = 4096;
      memcpy(pDst, InjectCodeStart, nSize);
      return nSize;
    }
    
    void __stdcall InjectCodeStart(InjectData *pData) {
      HMODULE hModule = pData->fnLoadLibrary((char *)pData->nDllNameAddr);
      InitializeFn fn = (InitializeFn)pData->fnGetProcAddress(hModule, (char *)pData->nFuncNameAddr);
      (*fn)();
    }
    
    So we see we have a structure named InjectData which contains the addresses of the functions and data we need.
    Then in InjectCodeStart, we have a function parameter that is a pointer to this data, and we just use the data
    at runtime as shown in the code in the function.
    Now to get this to work, the InjectData HAS to be in Ventrilo's memory. We do this with our C# loader. Here
    is a list of steps that occurs in the loader:
    
    1. Allocate and Write "CreVentDll.dll" into Ventrilo's memory. Save address into InjectData.nDllNameAddr.
    2. Allocate and Write "Initialize" into Ventrilo's memory. Save address as into InjectData.nFuncNameAddr.
    3. LoadLibrary("kernel32.dll") and GetProcAddress(..., "LoadLibraryA") and GetProcAddress(..., "GetProcAddress").
      Save addresses in InjectData.fnLoadLibrary and InjectData.fnGetProcAddress respectively.
    4. Allocate and Write the InjectData data structure into Ventrilo's memory. Save this address in variable data.
    5. Call GetInjectcode from our dll and then Allocate and Write the code into Ventrilo's memory. Save this address in variable code.
    6. Now call CreateRemoteThread and pass on the variable code as the function, and the variable data as its parameter.
    
    You can see the benefits of this technique. First we take away most of the hassel of loading from our dll. Second, we don't
    need to rely on knowing any assembly and worrying about how different compilers compile to different assembly. We also can
    pass any amount or type of data we want to our initialization function. Finally, the code is more elegant and just feals right.
    