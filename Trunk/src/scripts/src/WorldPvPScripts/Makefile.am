INCLUDES = -I$(top_srcdir)/src/hearthstone-logonserver \
   -I$(top_srcdir)/src/hearthstone-realmserver -I$(top_srcdir)/src/hearthstone-shared -I$(top_srcdir)/src/hearthstone-voicechat \
   -I$(top_srcdir)/src/hearthstone-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../hearthstone-shared/g3dlite -I$(srcdir)/../../../hearthstone-shared/vmap \
            -I../Common/

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libWorldPvPScripts.la

libWorldPvPScripts_la_SOURCES = \
ZoneHellfirePeninsula.cpp \
ZoneSilithus.cpp \
ZoneTerokkarForest.cpp \
ZoneZangarmarsh.cpp \
ScriptStdAfx.cpp \
Setup.cpp
