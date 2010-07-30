# Copyright (C) 2005-2009 MaNGOS project <http://getmangos.com/>
#
# Sandshroud DBC extractor based off of Mangos Map & DBC extractor.
#

cmake_minimum_required (VERSION 1.0)
project (SS_DBC_EXTRACTOR)

add_subdirectory (src/libmpq)
add_subdirectory (src/loadlib)

include_directories (${SS_DBC_EXTRACTOR_SOURCE_DIR}/src/libmpq) 
include_directories (${SS_DBC_EXTRACTOR_SOURCE_DIR}/src/loadlib) 

link_directories (${SS_DBC_EXTRACTOR_SOURCE_DIR}/src/libmpq) 
link_directories (${SS_DBC_EXTRACTOR_SOURCE_DIR}/src/loadlib) 

add_executable (ad src/dbcfile.cpp src/mpq_libmpq.cpp src/System.cpp)

target_link_libraries (ad src/libmpq)
target_link_libraries (ad src/loadlib)
