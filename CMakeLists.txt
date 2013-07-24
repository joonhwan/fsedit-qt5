cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

cmake_policy(SET CMP0020 NEW)
project(FSEditor)

include(InstallRequiredSystemLibraries)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# Set up the environment
find_package(Qt5Widgets)
find_package(Qt5Test)
find_package(Qt5LinguistTools)
enable_testing()
include(CTest)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Handle all the code
add_subdirectory(source/libfstest)
add_subdirectory(source/libmodel)
add_subdirectory(source/libmodelcommands)
add_subdirectory(source/libui)
add_subdirectory(source/application)
add_subdirectory(translations)

set(CPACK_PACKAGE_NAME "FSEditor")
set(CPACK_PACKAGE_VENDOR "Fionia Software")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An example application showing Qt and CMake working together.")
set(CPACK_PACKAGE_CONTACT "bo@fioniasoftware.dk")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/license.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_GENERATOR NSIS)
set(CPACK_NSIS_CONTACT "bo@fioniasoftware.dk")

if(WIN32 AND NOT UNIX)
	set(CPACK_PACKAGE_EXECUTABLES "fseditor" "FSEditor")
endif()

# Install the Qt library files
# INSTALL(FILES ${QT_BINARY_DIR}/QtAssistantClient4.dll DESTINATION bin)
# install(FILES ${QT_BINARY_DIR}/QtNetwork4.dll DESTINATION bin)
# install(FILES ${QT_BINARY_DIR}/QtCore4.dll DESTINATION bin)
# install(FILES ${QT_BINARY_DIR}/QtGui4.dll DESTINATION bin)
# install(FILES ${QT_BINARY_DIR}/QtXml4.dll DESTINATION bin)

# This should always be the last line
include(CPack)
