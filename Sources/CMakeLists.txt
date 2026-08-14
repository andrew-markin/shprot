cmake_minimum_required(VERSION 3.20)

project(Shprot VERSION 1.0 LANGUAGES CXX)

set(PROJECT_TITLE "Shprot")
set(PROJECT_VENDOR "Andrew N. Markin")

set(PROJECT_DESCRIPTION "A lightweight GUI wrapper to manage a resilient proxy over SSH tunnel.")
set(PROJECT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Shprot.ico")

set(ORIGINAL_YEAR "2026")
string(TIMESTAMP CURRENT_YEAR "%Y")

if(ORIGINAL_YEAR EQUAL CURRENT_YEAR)
    set(COPYRIGHT_YEARS "${ORIGINAL_YEAR}")
else()
    set(COPYRIGHT_YEARS "${ORIGINAL_YEAR}-${CURRENT_YEAR}")
endif()

set(PROJECT_COPYRIGHT "Copyright (C) ${COPYRIGHT_YEARS} ${PROJECT_VENDOR}. All rights reserved.")

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

# Prepare extended license file for packaging

set(PACKAGE_LICENSE_TXT_IN "${CMAKE_CURRENT_SOURCE_DIR}/PackageLicense.txt.in")
set(PACKAGE_LICENSE_TXT "${CMAKE_CURRENT_BINARY_DIR}/PackageLicense.txt")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE" PROJECT_LICENSE)
configure_file("${PACKAGE_LICENSE_TXT_IN}" "${PACKAGE_LICENSE_TXT}" @ONLY)

# Configure packaging

set(CPACK_GENERATOR "NSIS")

SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")

set(CPACK_PACKAGE_NAME "${PROJECT_TITLE}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_MONOLITHIC_INSTALL ON)

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

set(CPACK_RESOURCE_FILE_LICENSE "${PACKAGE_LICENSE_TXT}")

set(CPACK_NSIS_MUI_LANGUAGE "English")
set(CPACK_NSIS_LICENSE_PAGE_FORCE_SELECTION_CHECKBOX ON)
set(CPACK_NSIS_MENU_LINKS "Shprot.exe" "${PROJECT_TITLE}")
set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_TITLE} ${PROJECT_VERSION}")
set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_TITLE}")
set(CPACK_NSIS_MUI_ICON "${PROJECT_ICON}")
set(CPACK_NSIS_INSTALLED_ICON_NAME "Shprot.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MANIFEST_DPI_AWARE TRUE)

set(CPACK_NSIS_DEFINES "
  !define MUI_FINISHPAGE_RUN \\\"explorer.exe\\\"
  !define MUI_FINISHPAGE_RUN_PARAMETERS \\\"$INSTDIR\\\\Shprot.exe\\\"
")

set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  WriteRegStr HKCU \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\\" \\\"${PROJECT_NAME}\\\" '\\\"$INSTDIR\\\\Shprot.exe\\\" --auto'
")

set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
  DeleteRegValue HKCU \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\\" \\\"${PROJECT_NAME}\\\"
")

include(CPack)
