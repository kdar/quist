#pragma once

namespace Quist
{
  //--------------------------------
  /*Native enum. Managed enums look like:
   * enum class VentriloEvents : char {
   *   MENU_MISC_COPYCOMMENT,
   *   MENU_MISC_COPYURL
   * };
   */
  enum VentriloEvents
  {
    MENU_MISC_COPYCOMMENT = 32856,
    MENU_MISC_COPYURL = 32857,
    MENU_USER_SENDPAGE = 32816,
    MENU_RENTASERVER = 32875,
    CMD_ABOUT = 1014,
    CMD_VENTRILOWEBSITE = 1124
  };

  //--------------------------------
  enum QuistEvents
  {
    QUIST_BROWSETOURL = 40000,
    QUIST_BROWSETOCOMMENT,
    QUIST_PAGEBOMB,
    QUIST_PAGEBOMBSETTINGS,
    QUIST_PREVIEW,
    QUIST_STEAM
  };
}