# - Find MySQL++
# Find the MySQL++ includes and client library
# This module defines
#  MYSQLPP_INCLUDE_DIR, where to find mysql.h
#  MYSQLPP_LIBRARIES, the libraries needed to use MySQL.
#  MYSQLPP_FOUND, If false, do not try to use MySQL.

FIND_PATH(MYSQLPP_INCLUDE_DIR mysql++.h
   /usr/include/mysql++
   /usr/local/include/mysql++
   /opt/mysql++/include/mysql++
   /opt/mysqlpp/include/mysql++
   /opt/mysqlpp/include/
)

FIND_LIBRARY(MYSQLPP_LIBRARIES NAMES mysqlpp
   PATHS
   /usr/lib 
   /usr/local/lib 
   /opt/mysql++
   /opt/mysqlpp
   /opt/mysqlpp/lib
)

if(MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARIES)
   set(MYSQLPP_FOUND TRUE)
   message(STATUS "Found MySQL++: ${MYSQLPP_INCLUDE_DIR}, ${MYSQLPP_LIBRARIES}")
else(MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARIES)
   set(MYSQLPP_FOUND FALSE)
   message(STATUS "MySQL++ was not found.")
endif(MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARIES)

mark_as_advanced(MYSQLPP_INCLUDE_DIR MYSQLPP_LIBRARIES)

