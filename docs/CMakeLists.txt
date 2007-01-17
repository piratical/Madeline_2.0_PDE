# Configure the script and doxyfile
#
#SUBDIRS(html)
INSTALL_FILES(${CMAKE_INSTALL_PREFIX}/docs "\\.html$")
#INSTALL(FILES  "\\.html$" DESTINATION ${CMAKE_INSTALL_PREFIX}/docs)
INSTALL_FILES(/docs AUTHORS)
