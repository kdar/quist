#pragma once

namespace Quist
{
  ref class AboutDlg;
  ref class PreviewDlg;
  ref class PageBombSettingsDlg;
  ref class VentriloExecute;

  //--------------------------------
  value struct EntryData
  {
    String ^sComment;
    String ^sUrl;
    String ^sUser;
    String ^sFlags;
    bool bHasUrl;
    bool bHasComment;
    bool bHasGuest;
    bool bHasFlags;

    bool bCommentHasUrl;
    bool bCommentHasSteam;
  };

  //--------------------------------
  ref class Ventrilo
  {
    public:
      property HWND Hwnd
      {
        HWND get()
        { return m_hWnd; }

        void set(HWND value)
        { m_hWnd = value; }
      }

      property IntPtr HwndPtr
      {
        IntPtr get()
        { return m_hWndPtr; }

        void set(IntPtr value)
        { m_hWndPtr = value; }
      }

      property HWND HwndTree
      {
        HWND get()
        { return m_hWndTree; }

        void set(HWND value)
        { m_hWndTree = value; }
      }

    public:
      Ventrilo();

      void Initialize();
      void DisplayMenu(WPARAM wParam, LPARAM lParam);
      void OnMenuSelect(WPARAM wParam, LPARAM lParam);
      EntryData GetEntryData();
      String ^GetComment();
      String ^GetUrl();
      void BrowseToUrl(String ^sUrl);      
      void Preview();
      void Steam();
      void PageBomb() {}
      void _PageBomb() {}

    protected:
      String ^GetSelectedText();

      LRESULT NewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
      bool BeforeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
      bool AfterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

      LRESULT NewTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
      bool BeforeTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
      bool AfterTreeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
      delegate LRESULT WndProcDelegate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

      HWND m_hWnd;
      IntPtr m_hWndPtr;
      HWND m_hWndTree;
      LONG_PTR m_OldWndProc;
      LONG_PTR m_OldTreeProc;
      HMENU m_mainPopupMenu;

      System::Text::RegularExpressions::Regex ^m_entryReg;
      System::Text::RegularExpressions::Regex ^m_urlReg;
      System::Text::RegularExpressions::Regex ^m_steamReg;

      AboutDlg ^m_about;
      PreviewDlg ^m_preview;
      PageBombSettingsDlg ^m_pagebomb;
      VentriloExecute ^m_execute;
      EntryData m_entryData;
  };

  //--------------------------------
  ref class WindowWrapper: public IWin32Window
  {
    public:
      WindowWrapper(IntPtr handle)
      { _hwnd = handle; }

      property IntPtr Handle
      {
        virtual IntPtr get() {
          return _hwnd;
        }
      }

    private:
      IntPtr _hwnd;
  };
}
