/*Low level class for ventrilo taht executes functions within ventrilo*/

#include "VentriloExecute.h"

//--------------------------------
namespace Quist
{
  //================================
  VentriloExecute::VentriloExecute()
  {
  }

  //================================
  bool VentriloExecute::SendCommandHandler(int nCmd)
  {
    return VentriloExecuteUnmanaged::SendCommandHandler(nCmd);
  }

  #pragma unmanaged

  //================================
  bool VentriloExecuteUnmanaged::SendCommandHandler(int nCmd)
  {
    int retv;
    int obj = OBJ_Ventrilo;
    
    /*_asm {
      pusha
      // Call the function with the __thiscall convention
      mov ecx, obj
      push 0 //The hWnd, Seems to not matter what it is for this.
      push nCmd
      mov edx, FO_SendCommandHandler
      call edx
      mov retv, eax
      popa
    }*/

    return retv != 0 ? true : false;
  }
}