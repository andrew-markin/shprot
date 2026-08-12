cmake_minimum_required(VERSION 3.16)

project(Shprot VERSION 0.1 LANGUAGES CXX)

set(PROJECT_TITLE "Shprot")
set(PROJECT_VENDOR "Andrew Markin")

set(PROJECT_DESCRIPTION "A lightweight GUI wrapper to manage a resilient proxy over SSH tunnel")
set(PROJECT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Shprot.ico")

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/nologo /Zc:wchar_t)
add_compile_definitions(UNICODE _UNICODE)

add_definitions(
  -DPROJECT_NAME="${PROJECT_NAME}"
  -DPROJECT_TITLE="${PROJECT_TITLE}"
  -DPROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
  -DPROJECT_VERSION="${PROJECT_VERSION}"
  -DPROJECT_VENDOR="${PROJECT_VENDOR}"
)

# Include local environment configuration

include("${PROJECT_SOURCE_DIR}/../Environment.txt" OPTIONAL)

# Find required Qt libraries

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

get_filename_component(QT_BINARY_DIR "${QT6_INSTALL_PREFIX}/bin" ABSOLUTE)
get_filename_component(QT_PLUGINS_DIR "${QT6_INSTALL_PREFIX}/plugins" ABSOLUTE)

# Configure executable resources

string(TIMESTAMP CURRENT_YEAR "%Y")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Shprot.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/Shprot.rc
  @ONLY
)

# Add the target

add_executable(
  Shprot WIN32
  Preferences.cpp
  Preferences.h
  PreferencesDialog.cpp
  PreferencesDialog.h
  PreferencesDialog.ui
  Resources.qrc
  Shprot.cpp
  Shprot.h
  Utilities.cpp
  Utilities.h
  Websites.cpp
  Websites.h
)

target_sources(
  Shprot PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/Shprot.rc
)

target_link_libraries(
  Shprot PRIVATE
  Qt6::Widgets
  Qt6::Network
)

target_compile_options(
  Shprot PRIVATE
  /utf-8
  /permissive-
)

# Configure installation
# Install the target

install(TARGETS Shprot DESTINATION . )

# Install required system libraries

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION . )
include(InstallRequiredSystemLibraries)

# Install required Qt plugins

set(
  REQUIRED_QT_PLUGINS
  "platforms/qwindows.dll"
  "styles/qmodernwindowsstyle.dll"
  "iconengines/qsvgicon.dll"
)

foreach(PLUGIN_FILE_NAME ${REQUIRED_QT_PLUGINS})
  get_filename_component(PLUGIN_DIR ${PLUGIN_FILE_NAME} PATH)
  install(
    FILES "${QT_PLUGINS_DIR}/${PLUGIN_FILE_NAME}"
    DESTINATION "plugins/${PLUGIN_DIR}"
  )
endforeach()

# Install bundle fixup code

set(DEPENDENCIES_SEARCH_PATHS "${QT_BINARY_DIR}")

install(CODE "
  include(BundleUtilities)
  file(
    GLOB_RECURSE INSTALLED_QT_PLUGINS
    \"\${CMAKE_INSTALL_PREFIX}/plugins/*.dll\"
  )
  set(BU_CHMOD_BUNDLE_ITEMS TRUE) # This ensures that any bundle items are made user writable
  fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/Shprot.exe\" \"\${INSTALLED_QT_PLUGINS}\" \"${DEPENDENCIES_SEARCH_PATHS}\")
")

# Configure packaging

SET(CPACK_GENERATOR "ZIP")
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")

set(CPACK_PACKAGE_NAME "${PROJECT_TITLE}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_MONOLITHIC_INSTALL ON)

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

include(CPack)
