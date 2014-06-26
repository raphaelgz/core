#
# $Id$
#

all : first

RES_EXT := .res
BIN_EXT := .exe
DYN_EXT := .dll

HB_GT_LIBS += gtwvt gtgui gtwin

# kernel32: needed by some compilers (pocc/watcom)
# user32: *Clipboard*(), GetKeyState(), GetKeyboardState(), SetKeyboardState(), gtwvt stuff
# ws2_32/wsock32: hbsocket
# ws2_32: WSAIoctl()
# advapi32: GetUserName()
# gdi32: gtwvt

# must come after user libs and before Windows system libs
ifeq ($(__HB_BUILD_WINUNI),unicows)
   SYSLIBS += unicows
endif

SYSLIBS += kernel32 user32 ws2_32 advapi32 gdi32
