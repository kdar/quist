#include <windows.h>
#include <commctrl.h>
#using <mscorlib.dll>

#using <System.Windows.Forms.dll>
using namespace System;
using namespace System::Reflection;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;
using namespace System::IO;

//FIXME: temp
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

#include "Events.h"
#include "resource.h"
#include "Ventrilo.h"

#ifndef GWL_WNDPROC
#define GWL_WNDPROC -4
#endif

namespace Quist
{
  //================================
  Ventrilo::Ventrilo()
  {
    //2: flags, 3: username, 5: guest tag, 6: url tag, 7: comment
    //^(\"(.+)\")?\s?(.*?)\s?(\((GUEST:)?\s?(U:)?\s?(.+)?\))?$
    m_entryReg = gcnew Regex("^(\\\"(.+)\\\")?\\s?(.*?)\\s?(\\((GUEST)?:?\\s?(U:)?\\s?(.+)?\\))?$");
    //Basic url.. Maybe extend to any protocol?
    m_urlReg = gcnew Regex("^\\s*(ftp|http|https):\\/\\/(www\\.)?|^(www\\.)", RegexOptions::IgnoreCase);
    //1: host, 4: port
    //((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})|([a-zA-Z][\w\.-]*[a-zA-Z0-9]\.[a-zA-Z][a-zA-Z\.]*[a-zA-Z]))\s*:?\s*(\d+)
    m_steamReg = gcnew Regex("((\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})|([a-zA-Z][\\w\\.-]*[a-zA-Z0-9]\\.[a-zA-Z][a-zA-Z\\.]*[a-zA-Z]))\\s*:?\\s*(\\d+)?", RegexOptions::IgnoreCase);
  }

  //================================
  void Ventrilo::Initialize()
  {
    //Get the hwnd of Ventrilo
    m_hWndPtr = Process::GetCurrentProcess()->MainWindowHandle;
    m_hWnd = (HWND)m_hWndPtr.ToPointer();

    //Change the title of Ventrilo
    SetWindowText(m_hWnd, "Ventrilo [Quist]");

    //Change the icon of Ventrilo
    //We use GetModuleHandle on our dll, since this is where the resource is located.
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle("QuistDll32.dll");
    if (!hInstance)
      hInstance = (HINSTANCE)GetModuleHandle("QuistDll64.dll");
    HICON hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);

    //Get the hwnd of the tree
    m_hWndTree = FindWindowEx(m_hWnd, 0, "SysTreeView32", "Tree2");

    //http://msdn2.microsoft.com/en-us/library/367eeye0(VS.80).aspx
    WndProcDelegate ^wpd = gcnew WndProcDelegate(this, &Quist::Ventrilo::NewWndProc);
    GCHandle gch = GCHandle::Alloc(wpd);
    IntPtr ip = Marshal::GetFunctionPointerForDelegate(wpd);
    m_OldWndProc = SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, ip.ToInt64());

    wpd = gcnew WndProcDelegate(this, &Quist::Ventrilo::NewTreeProc);
    gch = GCHandle::Alloc(wpd);
    ip = Marshal::GetFunctionPointerForDelegate(wpd);    
    m_OldTreeProc = SetWindowLongPtr(m_hWndTree, GWLP_WNDPROC, ip.ToInt64());
    SendMessage(m_hWndTree, CCM_SETVERSION, 5, 0);

    m_about = gcnew AboutDlg(Process::GetCurrentProcess()->MainModule->FileVersionInfo->ProductVersion);
    m_preview = gcnew PreviewDlg();
    m_preview->Browse += gcnew PreviewDlg::BrowseEvent(this, &Quist::Ventrilo::BrowseToUrl);
  }

  //================================
  void Ventrilo::DisplayMenu(WPARAM wParam, LPARAM lParam)
  {
    HMENU hMenu = (HMENU)wParam;

    this->m_entryData = GetEntryData();

    //This value is 65536 if it's the system menu, and 0 for everything else it seems.
    //So this helps avoid putting these items into the system menu.
    if (lParam == 0) {
      //Delete Rent a server...
      //DeleteMenu(hMenu, MENU_RENTASERVER, MF_BYCOMMAND);
      DeleteMenu(hMenu, 10, MF_BYPOSITION); //Removes separator
      DeleteMenu(hMenu, 10, MF_BYPOSITION); //Removes About
      DeleteMenu(hMenu, 10, MF_BYPOSITION); //Removes Rent a server

      if (this->m_entryData.bHasComment || this->m_entryData.bHasUrl || this->m_entryData.bCommentHasUrl) {
        InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, "");
        InsertMenu(hMenu, 0, MF_BYPOSITION, QUIST_PREVIEW, "Preview");
      }
      
      if (this->m_entryData.bHasComment) {
        /*if (this->m_entryData.bCommentHasSteam) {
          InsertMenu(hMenu, 0, MF_BYPOSITION, QUIST_STEAM, "Steam"); 
        }*/
        InsertMenu(hMenu, 0, MF_BYPOSITION, MENU_MISC_COPYCOMMENT, "Copy comment"); 
      }

      if (this->m_entryData.bHasUrl) { 
        InsertMenu(hMenu, 0, MF_BYPOSITION, QUIST_BROWSETOURL, "Browse to url");        
      }

      if (this->m_entryData.bCommentHasUrl && !this->m_entryData.sComment->Equals(this->m_entryData.sUrl)) {
        InsertMenu(hMenu, 0, MF_BYPOSITION, QUIST_BROWSETOCOMMENT, "Browse to comment");
      }
    }
  }

  //================================
  void Ventrilo::OnMenuSelect(WPARAM wParam, LPARAM lParam)
  {
    /*HMENU hMenu = (HMENU)lParam;

    int nItem = LOWORD(wParam);
    int nFlags = HIWORD(wParam);

    if (!(nFlags & MF_MOUSESELECT)) {
      return;
    }

    HWND hWnd = FindWindow("#32768", NULL);    
    IntPtr hWndPtr = IntPtr::IntPtr(hWnd);
    //MessageBox::Show(String::Format("{0}", hWndPtr.ToPointer() == this->m_hWndPtr.ToPointer()));

    //WindowWrapper ^window = gcnew WindowWrapper(this->m_hWndPtr);
    WindowWrapper ^window = gcnew WindowWrapper(hWndPtr);
    //MessageBox::Show(window, "TEST");

    ToolTip ^tt = gcnew ToolTip();
    tt->AutoPopDelay = 5000;
    tt->InitialDelay = 1000;
    tt->ReshowDelay = 500;
    tt->ShowAlways = true;    
    tt->Show("Hey there guy", window, 100, 100);*/
  }

  //================================
  // Parses the currently selected item by a regular expression
  // and fills the EntryData structure accordingly. Also grabs the
  // url since this is not provided in the item text.
  EntryData Ventrilo::GetEntryData()
  {
    EntryData data;

    String ^sText = GetSelectedText();

    if (sText != nullptr && sText->Length > 0) {
      Match ^m = m_entryReg->Match(sText);
      if (m->Success) {
        data.bHasComment = m->Groups[7]->Success;
        data.bHasUrl = m->Groups[6]->Success;
        data.bHasGuest = m->Groups[5]->Success;
        data.bHasFlags = m->Groups[2]->Success;

        if (data.bHasComment) {
          data.sComment = m->Groups[7]->Value;
          data.bCommentHasUrl = m_urlReg->Match(data.sComment)->Success;
        }

        if (data.bHasUrl) {
          data.sUrl = this->GetUrl();
        }

        if (data.bHasFlags)
          data.sFlags = m->Groups[2]->Value;

        data.sUser = m->Groups[3]->Value;
      }

      data.bCommentHasSteam = m_steamReg->Match(sText)->Success;
    }

    return data;
  }

  //================================
  // Don't think this is used anymore
  String ^Ventrilo::GetComment()
  {
    String ^sComment = nullptr;

    //Have to use the full path of IDataObject because COM has the same interface.
    System::Windows::Forms::IDataObject ^iData;

    //Save old clipboard data
    iData = Clipboard::GetDataObject();
    String ^oldData;
    if (iData->GetDataPresent(DataFormats::Text))
      oldData = (String^)(iData->GetData(DataFormats::Text));

    //Clear the clipboard. We do this so we know if we got the comment.
    Clipboard::Clear();

    SendMessage(GetActiveWindow(), WM_COMMAND, MENU_MISC_COPYCOMMENT, 0); 
    iData = Clipboard::GetDataObject();

    if (iData->GetDataPresent(DataFormats::Text)) {
      sComment = (String^)(iData->GetData(DataFormats::Text));
    }

    //Restore old clipboard data
    if (oldData)
      Clipboard::SetDataObject(oldData, true);

    return sComment;
  }

  //================================
  // Don't think this is used anymore
  String ^Ventrilo::GetUrl()
  {
    String ^sComment = nullptr;

    //Have to use the full path of IDataObject because COM has the same interface.
    System::Windows::Forms::IDataObject ^iData;

    //Save old clipboard data
    iData = Clipboard::GetDataObject();
    String ^oldData;
    if (iData->GetDataPresent(DataFormats::Text))
      oldData = (String^)(iData->GetData(DataFormats::Text));

    // This is not needed anymore because we only call this function when
    // we know we have a url. This seems to cause some errors on
    // vista x64. Ventrilo says "Unable to open the clipboard."
    //Clear the clipboard. We do this so we know if we got the url.
    //Clipboard::Clear();

    SendMessage(GetActiveWindow(), WM_COMMAND, MENU_MISC_COPYURL, 0); 
    iData = Clipboard::GetDataObject();

    if (iData->GetDataPresent(DataFormats::Text)) {
      sComment = (String^)(iData->GetData(DataFormats::Text));
    }

    //Restore old clipboard data
    if (oldData)
      Clipboard::SetDataObject(oldData, true);

    return sComment;
  }

  //================================
  //Gets the url from ventrilo either from the url comment, or from the regular
  //comment (if it's a well formed url)
  /*String ^Ventrilo::GetUrl()
  {
    String ^sUrl = nullptr;

    //Have to use the full path of IDataObject because COM has the same interface.
    System::Windows::Forms::IDataObject ^iData;

    //Save old clipboard data
    iData = Clipboard::GetDataObject();
    String ^oldData;
    if (iData->GetDataPresent(DataFormats::Text))
      oldData = (String^)(iData->GetData(DataFormats::Text));

    //Clear the clipboard. We do this so we know if we got a url or not.
    Clipboard::Clear();

    //Attempt to get url from the url comment
    SendMessage(GetActiveWindow(), WM_COMMAND, MENU_MISC_COPYURL, 0);  
    iData = Clipboard::GetDataObject();

    if (iData->GetDataPresent(DataFormats::Text)) {
      sUrl = (String^)(iData->GetData(DataFormats::Text));
    } else {
      //Attempt to get url from comment
      SendMessage(GetActiveWindow(), WM_COMMAND, MENU_MISC_COPYCOMMENT, 0); 
      iData = Clipboard::GetDataObject();

      if (iData->GetDataPresent(DataFormats::Text)) {
        String ^tmpurl = (String^)(iData->GetData(DataFormats::Text));
        Match ^m = m_urlReg->Match(tmpurl);
        if (m->Success) {
          sUrl = tmpurl;
        }
      }
    }

    //Restore old clipboard data
    if (oldData)
      Clipboard::SetDataObject(oldData, true); 

    if (sUrl != nullptr) {
      Match ^m = m_urlReg->Match(sUrl);
      if (!m->Success) {
        sUrl = "http://" + sUrl;
      }
    }

    return sUrl;
  }*/

  //================================
  void Ventrilo::BrowseToUrl(String ^sUrl)
  {
    if (sUrl != nullptr && sUrl->Length != 0) {
      ProcessStartInfo ^p = gcnew ProcessStartInfo(sUrl);
      p->Verb = "open";
      p->UseShellExecute = true;
      Process::Start(p);
    }
  }

  //================================
  void Ventrilo::Preview()
  {
    m_preview->Comment = this->m_entryData.sComment;
    m_preview->Url = this->m_entryData.sUrl;
    m_preview->User = this->m_entryData.sUser;
    m_preview->ShowDialog();
  }

  //================================
  void Ventrilo::Steam()
  {
    String ^s = this->m_entryData.sComment;
    if (s != nullptr && s->Length != 0) {
      Match ^m = m_steamReg->Match(s);
      if (m->Success) {
        s = "steam://connect/" + m->Groups[1]->Value + ":" + m->Groups[4]->Value;
        this->BrowseToUrl(s);
      }
    }
  }

  //================================
  String ^Ventrilo::GetSelectedText()
  {
    //http://www.codeguru.com/cpp/controls/treeview/misc-advanced/article.php/c3993/
    HTREEITEM selected = 0;
    selected = (HTREEITEM)SendMessage(m_hWndTree, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)selected);
    TV_ITEM tvi;
    memset(&tvi, 0, sizeof(tvi));
    char szText[255] = "";

    tvi.mask=TVIF_TEXT;
    tvi.pszText=szText;
    tvi.cchTextMax=256;
    tvi.hItem=selected;
    SendMessage(m_hWndTree, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi);

    return gcnew String(szText);
  }

  //================================    
  LRESULT Ventrilo::NewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    LRESULT ret;

    //OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
    //http://msdn2.microsoft.com/en-us/library/bb761817(VS.85).aspx
    //if ((IntPtr::IntPtr(&wParam)).Equals((IntPtr::IntPtr(m_hWndTree))))
    /*switch (message) {
      case WM_NOTIFY:     
        LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;
        if (pnm->hdr.hwndFrom == m_hWndTree) {
          switch (pnm->hdr.code){
            case NM_CUSTOMDRAW:
              
              LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)lParam;
              switch (pCustomDraw->nmcd.dwDrawStage)
			        {
				        //case CDDS_PREPAINT:
					        // Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW,
					        // otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH)
					      //  return CDRF_NOTIFYITEMDRAW;
                //break;

				        //case CDDS_ITEMPREPAINT:
                //  return CDRF_NOTIFYPOSTPAINT;

					        switch (pCustomDraw->iLevel)
					        {
						        // painting all 0-level items blue,
						        // and all 1-level items red (GGH)
						        case 0:
							        if (pCustomDraw->nmcd.uItemState == (CDIS_FOCUS | CDIS_SELECTED)) // selected
								        pCustomDraw->clrText = RGB(255, 255, 255);
							        else
								        pCustomDraw->clrText = RGB(0, 0, 255);
							        break;
						        case 1:
							        if (pCustomDraw->nmcd.uItemState == (CDIS_FOCUS | CDIS_SELECTED)) // selected
								        pCustomDraw->clrText = RGB(255, 255, 255);
							        else
								        pCustomDraw->clrText = RGB(255, 0, 0);
							        break;
					        }

                  //return CDRF_NEWFONT;
					        //return CDRF_SKIPDEFAULT;
                //break;

                case CDDS_ITEMPOSTPAINT:
                  try {
                    Graphics ^g = Graphics::FromHdc((IntPtr)pCustomDraw->nmcd.hdc); //Graphics::FromHdc((IntPtr)GetDC(m_hWndTree))
                    //TreeNode ^node = TreeNode::FromHandle((TreeView ^)IntPtr::IntPtr(m_hWndTree), pCustomDraw->nmcd.dwItemSpec);
                    TV_ITEM item;
                    item.mask =TVIF_TEXT;
                    item.cchTextMax=299;
                    item.pszText=new char[300];
                    strcpy(item.pszText,"");
                    item.hItem = (HTREEITEM)pCustomDraw->nmcd.dwItemSpec; 
                    TreeView_GetItem(m_hWndTree, &item);
                    RECT rect;
                    TreeView_GetItemRect(m_hWndTree, item.hItem, &rect, true); 
                    g->FillRectangle(Brushes::White, rect.left, rect.top, rect.right, rect.bottom);
                    Font ^f = SystemFonts::DefaultFont;
                    String ^s = gcnew String(item.pszText);
                    g->DrawString(s, f, Brushes::Black, rect.left, rect.top);
                    delete s;
                    delete f;
                    
                    if (pCustomDraw->nmcd.uItemState == (CDIS_FOCUS | CDIS_SELECTED)) {
                      SolidBrush ^b = gcnew SolidBrush(Color::FromArgb(100, 0, 0, 255));
                      g->FillRectangle(b, rect.left, rect.top, rect.right, rect.bottom);
                      delete b;
                    }

                    delete g;
                  } catch (OverflowException ^e) {}
                  return CDRF_SKIPDEFAULT;
                break;
			        }

            break;
          }
        }

      break;
    }*/

    try
    {
      if (BeforeWndProc(hWnd, message, wParam, lParam))
        ret = CallWindowProc((WNDPROC)m_OldWndProc, hWnd, message, wParam, lParam);
      AfterWndProc(hWnd, message, wParam, lParam);
    }
    catch (Exception ^e) {
      MessageBox::Show(e->ToString() + "\n\n" + "Please notify the maker of this program of the problem.", "QuistDll Exception");
    }

    return ret;
  }

  //================================
  bool Ventrilo::BeforeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    switch (message) {
      case WM_COMMAND:
        switch(wParam) {
          case QUIST_BROWSETOURL:
            this->BrowseToUrl(this->m_entryData.sUrl);
          break;

          case QUIST_BROWSETOCOMMENT:
            this->BrowseToUrl(this->m_entryData.sComment);
          break;

          case QUIST_PREVIEW:
            this->Preview();
          break;

          case QUIST_STEAM:
            this->Steam();
          break;

          case QUIST_PAGEBOMB:
            //MessageBoxA(data.hWnd, "PAGE BOMB AWAY!!", "Yay!", MB_OK);
          break;

          case CMD_ABOUT:
            this->m_about->ShowDialog();
            return false;
          break;
        }
      break;

      case WM_INITMENUPOPUP:
        this->DisplayMenu(wParam, lParam);
      break;

      case WM_MENUSELECT:
        this->OnMenuSelect(wParam, lParam);
      break;

      //Normal win32 behavior is when you right click on an item, it "looks" like it highlights
      //the item that you right clicked on, but really the true selected item is the previous
      //item that was selected. What this piece of code does is actually select the item you
      //right clicked on.
      case WM_NOTIFY:
        //Right Click
        if (((LPNMHDR)lParam)->code == NM_RCLICK) {
          HTREEITEM selected = (HTREEITEM)SendMessage(m_hWndTree, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0);
          if (selected != NULL) {
            SendMessage(m_hWndTree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)selected);
          }
        }
      break;
    }
    
    return true;
  }

  //================================
  bool Ventrilo::AfterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    switch(message) {
      case WM_COMMAND:
        /*switch(wParam) {
        }*/
      break;
    }

    return true;
  }

   //================================    
  LRESULT Ventrilo::NewTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    LRESULT ret;

    try
    {
      if (BeforeTreeProc(hWnd, message, wParam, lParam))
        ret = CallWindowProc((WNDPROC)m_OldTreeProc, hWnd, message, wParam, lParam);
      AfterTreeProc(hWnd, message, wParam, lParam);
    }
    catch (Exception ^e) {
      MessageBox::Show(e->ToString() + "\n\n" + "Please notify the maker of this program of the problem.", "QuistDll Exception");
    }

    return ret;
  }

  //================================
  bool Ventrilo::BeforeTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    /*TVHITTESTINFO info;

    switch(message) {
      case WM_MOUSEMOVE:
        memset(&info, 0, sizeof info);
        // converts screen mouse coordinates to client coordinates
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);        
        info.pt.x = pt.x;
        info.pt.y = pt.y;

        //MessageBox::Show(String::Format("{0}, {1}", pt.x, pt.y));
                    
        // check if the mouse position is on a tree lable
        TreeView_HitTest(m_hWndTree, &info);
        if (info.hItem != NULL) {
          if (info.flags & TVHT_ONITEMLABEL)
          {
            //display tooltip
          }
        }
      break;
    }*/
    
    return true;
  }

  //================================
  bool Ventrilo::AfterTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    switch(message) {
      case WM_COMMAND:
        /*switch(wParam) {
        }*/
      break;
    }

    return true;
  }
}