INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src -I../Common
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunEventScripts.la

libSunEventScripts_la_SOURCES = Setup.cpp \
../Common/Base.cpp \
Halloween.cpp \
WinterVeil.cpp \
Event_DarkPortal.cpp
