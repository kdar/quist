//Send page switch case location: 0044CCA3

//This windows.h must come right before mscorlib.dll in order for you to get
//no compile errors.
#include <windows.h>
#include <commctrl.h>
#using <mscorlib.dll>

#using <System.Windows.Forms.dll>
using namespace System;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;
using namespace EasyHook;

#include "Ventrilo.h"
#include "QuistDll.h"

namespace Quist
{
  //================================
  // Called by EasyHook
  QuistDll::QuistDll(RemoteHooking::IContext ^InContext, String ^sPath)
  {
    m_ventrilo = gcnew Ventrilo();
  }

  //================================
  // Called by EasyHook
  void QuistDll::Run(RemoteHooking::IContext ^InContext, String ^sPath)
  {
    m_sPath = sPath;
    Initialize();
    
    // Keep this function from returning so the application
    // can continue to run.
    while (true) {
      Sleep(5000);
    }
  }

  //================================
  void QuistDll::Initialize()
  {
    try 
    {
      //Deprecated... WHY!?!? :(
      //AppDomain::CurrentDomain->AppendPrivatePath("Application Files");
      //AppDomain::CurrentDomain->Load("QuistDllEx");

      //Help us in resolving assemblies at runtime.
      //We could probably do this easier with a config. 
      AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(this, &Quist::QuistDll::MyResolveEventHandler);

      m_ventrilo->Initialize();
    } 
    catch (Exception ^e) 
    {
      MessageBox::Show(e->ToString(), "QuistDll");
    }
  }

  //================================   
  //http://support.microsoft.com/kb/837908
  Assembly ^QuistDll::MyResolveEventHandler(Object ^sender, ResolveEventArgs ^args)
  {
    //This handler is called only when the common language runtime tries to bind to the assembly and fails.

    //Retrieve the list of referenced assemblies in an array of AssemblyName.
    Assembly ^MyAssembly, ^objExecutingAssemblies;
    String ^strTempAssmbPath="";

    objExecutingAssemblies=Assembly::GetExecutingAssembly();
    array<AssemblyName^> ^arrReferencedAssmbNames = objExecutingAssemblies->GetReferencedAssemblies();
  			
    //Loop through the array of referenced assembly names.
    for each (AssemblyName ^strAssmbName in arrReferencedAssmbNames) {
	    //Check for the assembly names that have raised the "AssemblyResolve" event.
	    if(strAssmbName->FullName->Substring(0, strAssmbName->FullName->IndexOf(","))==args->Name->Substring(0, args->Name->IndexOf(","))) {
		    //Build the path of the assembly from where it has to be loaded.        
        strTempAssmbPath = m_sPath + "\\" + args->Name->Substring(0,args->Name->IndexOf(","))+".dll";
		    break;
	    }
    }

    //Load the assembly from the specified path. 	
    MyAssembly = Assembly::LoadFrom(strTempAssmbPath);

    //Return the loaded assembly.
    return MyAssembly;			
  }
}