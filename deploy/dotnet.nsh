;Function CheckDotNETInstallation
;	
;FunctionEnd

;  ${VersionCompare} $0 "3.5" $1
;  ${If} $1 == 2
;    MessageBox MB_OK|MB_ICONSTOP ".NET runtime library v3.5 or newer is required. You have $0."
;    Abort
;  ${EndIf}

;IfFileExists "$WINDIR\Microsoft.NET\Framework\v2.0.50727\installUtil.exe" NextStep
;  MessageBox MB_OK|MB_ICONEXCLAMATION "You must have the Microsoft .NET Framework 2.0 Installed to use this application. $\n$\nClick 'Open' in the following file dialog to download and run the Microsoft .NET Framework Installer..."
;  ExecShell Open "http://www.microsoft.com/downloads/info.aspx?na=90&p=&SrcDisplayLang=en&SrcCategoryId=&SrcFamilyId=0856eacb-4362-4b0d-8edd-aab15c5e04f5&u=http%3a%2f%2fdownload.microsoft.com%2fdownload%2f5%2f6%2f7%2f567758a3-759e-473e-bf8f-52154438565a%2fdotnetfx.exe" SW_SHOWNORMAL
;  Quit
;NextStep:
 