# Try to find Libcurl functionality
# Once done this will define
#
#  LIBCURL_FOUND - system has Libcurl
#  LIBCURL_INCLUDE_DIR - Libcurl include directory
#  LIBCURL_LIBRARIES - Libraries needed to use Libcurl
#
# Copyright (c) 2006, Chusslove Illich, <caslav.ilic@gmx.net>
# Copyright (c) 2007, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2007, Jone Marius Vignes, <jonemarius@vign.es>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(LIBCURL_INCLUDE_DIR AND LIBCURL_LIB_FOUND)
  set(Libcurl_FIND_QUIETLY TRUE)
endif(LIBCURL_INCLUDE_DIR AND LIBCURL_LIB_FOUND)

find_path(LIBCURL_INCLUDE_DIR curl/curl.h)

set(LIBCURL_LIB_FOUND FALSE)

if(LIBCURL_INCLUDE_DIR)
  include(CheckFunctionExists)
  check_function_exists(curl_easy_setopt LIBCURL_HAS_EASY_INIT)

  if (LIBCURL_HAS_EASY_INIT)
    set(LIBCURL_LIBRARIES)
    set(LIBCURL_LIB_FOUND TRUE CACHE BOOL "Libcurl found" FORCE)
  else (LIBCURL_HAS_EASY_INIT)
    find_library(LIBCURL_LIBRARIES NAMES curl libcurl )
    if(LIBCURL_LIBRARIES)
      set(LIBCURL_LIB_FOUND TRUE CACHE BOOL "Libcurl found" FORCE)
    endif(LIBCURL_LIBRARIES)
  endif (LIBCURL_HAS_EASY_INIT)

endif(LIBCURL_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libcurl  DEFAULT_MSG  LIBCURL_INCLUDE_DIR  LIBCURL_LIB_FOUND)

mark_as_advanced(LIBCURL_INCLUDE_DIR LIBCURL_LIBRARIES LIBCURL_HAS_EASY_INIT)
