In order for this dll to work on other systems, you must set the following propertise in the VC++ environment:
C/C++ > Code Generation > Runtime Library: Multi-threaded (/MT)
C/C++ > Advanced > Calling Convention: __cdecl (/Gd)


Explaination of Multi-threaded vs Multi-threaded dll here:
http://www.msdner.com/dev-archive/145/8-29-1458613.shtm
http://www.rtsoft.com/forums/archive/index.php/t-1607.html

Explaination of __cdecl here:
http://unixwiz.net/techtips/win32-callconv-asm.html