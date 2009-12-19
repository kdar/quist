#pragma once

namespace Quist
{
  #define CMD_SENDPAGE 0x8030

  #define OBJ_Ventrilo (*((int *)0x005311D4)) //0x001288A4;
  /*Variable offsets for OBJ_Ventrilo*/
  #define VO_ClientId ((int *)(OBJ_Ventrilo+0x71B0))
  /*Function offsets for OBJ_Ventrilo*/
  #define FO_SendCommandHandler 0x0044C090

  //--------------------------------
  ref class VentriloExecute
  {
    public:
      //================================
      property int ClientId
      {
        int get() { 
          return *VO_ClientId; 
        }

        void set(int value) { 
          (*VO_ClientId) = value;
        }
      }

      VentriloExecute();

      bool SendCommandHandler(int nCmd);      
  };

  //--------------------------------
  class VentriloExecuteUnmanaged
  {
    public:
      static bool SendCommandHandler(int nCmd);
  };
}