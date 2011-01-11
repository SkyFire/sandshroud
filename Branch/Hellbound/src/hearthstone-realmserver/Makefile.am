INCLUDES += -I$(srcdir) -I$(srcdir)/../hearthstone-shared

bin_PROGRAMS = hearthstone-realmserver

hearthstone_realmserver_SOURCES = \
ClientManager.h \
ClusterManager.h \
LogonCommClient.h \
LogonCommHandler.h \
Master.h \
ObjectStorage.h \
RStdAfx.h \
Session.h \
Structures.h \
WorkerOpcodes.h \
WorkerServer.h \
WorkerServerSocket.h \
ClientSocket.h \
CharacterHandler.cpp \
ClientManager.cpp \
ClusterManager.cpp \
LogonCommClient.cpp \
LogonCommHandler.cpp \
Main.cpp \
Master.cpp \
ObjectStorage.cpp \
Player.cpp \
Player.h \
QueryHandler.cpp \
RStdAfx.cpp \
Session.cpp \
WorkerServer.cpp \
WorkerServerSocket.cpp \
ClientSocket.cpp 
	
hearthstone_realmserver_LDADD = -lshared -lz

hearthstone_realmserver_LDFLAGS = -L$(srcdir)/../hearthstone-shared \
               -L/usr/lib
               
