#pragma once

#include "Defines.h"

extern "C" DLL_API void Initialize(void *pExtraData);

namespace Quist
{
  //--------------------------------
  public ref class QuistDll : public IEntryPoint
  {
    public:
      QuistDll(RemoteHooking::IContext ^InContext, String ^sPath);
      void Run(RemoteHooking::IContext ^InContext, String ^sPath);

      void Initialize();

    private:
      static QuistDll ^m_pInstance;
      Ventrilo ^m_ventrilo;
      String ^m_sPath;

      Assembly ^MyResolveEventHandler(Object ^sender, ResolveEventArgs ^args);   
  };
}
