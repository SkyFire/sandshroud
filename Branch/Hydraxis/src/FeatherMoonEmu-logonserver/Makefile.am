INCLUDES += -I$(srcdir) -I$(srcdir)/../../dep/include -I$(srcdir)/../FeatherMoonEmu-shared

bin_PROGRAMS = FeatherMoonEmu-logonserver

FeatherMoonEmu_logonserver_SOURCES = \
	AccountCache.cpp \
	AccountCache.h \
	AuthSocket.cpp \
	AuthSocket.h \
	AuthStructs.h \
	LogonCommServer.cpp \
	LogonCommServer.h \
	LogonConsole.cpp \
	LogonConsole.h \
	LogonOpcodes.cpp \
	LogonOpcodes.h \
	LogonStdAfx.cpp \
	LogonStdAfx.h \
	AutoPatcher.h \
	AutoPatcher.cpp \
	Main.cpp \
	Main.h \
	PeriodicFunctionCall_Thread.h

FeatherMoonEmu_logonserver_LDADD = -lshared -lz
FeatherMoonEmu_logonserver_LDFLAGS = -L$(srcdir)/../FeatherMoonEmu-shared
