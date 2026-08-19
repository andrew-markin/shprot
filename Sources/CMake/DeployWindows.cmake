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
    DESTINATION "Plugins/${PLUGIN_DIR}"
  )
endforeach()

# Install bundle fixup code

set(DEPENDENCIES_SEARCH_PATHS "${QT_BINARY_DIR}")

install(CODE "
  include(BundleUtilities)
  file(
    GLOB_RECURSE INSTALLED_QT_PLUGINS
    \"\${CMAKE_INSTALL_PREFIX}/Plugins/*.dll\"
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
set(CPACK_PACKAGE_VENDOR "${PROJECT_DEVELOPER_TITLE}")
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
set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/${PROJECT_ICON}")
set(CPACK_NSIS_INSTALLED_ICON_NAME "Shprot.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MANIFEST_DPI_AWARE TRUE)

set(CPACK_NSIS_DEFINES "
  !define MUI_FINISHPAGE_RUN \\\"explorer.exe\\\"
  !define MUI_FINISHPAGE_RUN_PARAMETERS \\\"$INSTDIR\\\\Shprot.exe\\\"
")

set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\\" \\\"${PROJECT_NAME}\\\" '\\\"$INSTDIR\\\\Shprot.exe\\\" --auto'
")

set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
  DeleteRegValue HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\\" \\\"${PROJECT_NAME}\\\"
  ExecWait '\\\"$INSTDIR\\\\Shprot.exe\\\" --shutdown'
  Sleep 2000
")

include(CPack)
