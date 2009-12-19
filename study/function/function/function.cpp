// function.cpp : Defines the exported functions for the DLL application.
//

#include <windows.h>
#include "stdafx.h"

__declspec(dllexport) int process() {
  // The hwnd. It seems to not matter if I set this.
  int hwnd = 0;

  // The sendpage code
  int code = 0x8030;

  // Location of the object we need to call the function
  // on.
  int obj = *((int *)0x005311D4);//0x001288A4;

  // The location of a function that handles several different
  // cases/commands.
  int func = 0x0044C090;

  // The offset within the object where the client
  // id is stored. The id of the client we're going
  // to send a command to.
  int clientIdOffset = 0x71B0;
  
  *((int *)(obj+clientIdOffset)) = 2;

  int retv;
  _asm {
    pusha
    // Call the function with the __thiscall convention
    mov ecx, obj
    push hwnd
    push code
    mov edx, func
    call edx
    mov retv, eax
    popa
  }

  return retv;
}


__declspec(dllexport) int callme() {
  typedef int (*FUNC)(int id);
  FUNC addr = (FUNC)0x0042EBA0;
  int nAddr = 0x0042EBA0;

 int cls = 0x001265FC;

  //cls->addr(2);

  _asm {
    push 7
    call nAddr
  }

  return 1;
}

__declspec(dllexport) int dosomething() {
  //typedef int (__thiscall *FUNC)(int a);
  //FUNC addr = (FUNC)0x00411A90;  

  int retv;
  _asm {
    pusha
    mov ecx, 0x004192D4
    push 50
    mov edx, 0x00411AA0
    call edx
    mov retv, eax
    popa
  }

  return retv;
}

