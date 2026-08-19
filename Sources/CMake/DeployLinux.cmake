list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Linux")

set_target_properties(
    Shprot PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    INSTALL_RPATH_USE_LINK_PATH FALSE
)

# Use legacy RPATH instead of RUNPATH. This ensures $ORIGIN expansion works
# for dynamically loaded libraries (dlopen), which is required for Qt plugins
# and modules like QtDBus. RUNPATH does not apply to dlopen() in some glibc
# versions, leading to "library not found" errors.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")

# Configure deployment variables

set(PROJECT_BINARIES_DEST_DIR "opt/${PROJECT_NAME}")
set(PROJECT_QT_PLUGINS_DEST_DIR "${PROJECT_BINARIES_DEST_DIR}/Plugins")
set(PROJECT_EXECUTABLE "${PROJECT_BINARIES_DEST_DIR}/Shprot")
set(PROJECT_SEARCH_PATHS "${QT_LIBRARY_DIR}")

# Install main executable

install(TARGETS Shprot DESTINATION "${PROJECT_BINARIES_DEST_DIR}")

# Install configuration files

install(
    FILES "${CMAKE_CURRENT_LIST_DIR}/qt.conf"
    DESTINATION "${PROJECT_BINARIES_DEST_DIR}"
)

# Install required Qt plugins

set(
    REQUIRED_QT_PLUGINS
    "iconengines/libqsvgicon.so"
    "networkinformation/libqglib.so"
    "networkinformation/libqnetworkmanager.so"
    "platforms/libqwayland-generic.so"
    "platforms/libqxcb.so"
    "platformthemes/libqgtk3.so"
)

foreach(REQUIRED_QT_PLUGIN ${REQUIRED_QT_PLUGINS})
    get_filename_component(REQUIRED_QT_PLUGIN_DIR ${REQUIRED_QT_PLUGIN} PATH)
    install(
        FILES "${QT_PLUGINS_DIR}/${REQUIRED_QT_PLUGIN}"
        DESTINATION "${PROJECT_QT_PLUGINS_DEST_DIR}/${REQUIRED_QT_PLUGIN_DIR}"
    )
endforeach()

# Configure and install bundle fixup script

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/FixupBundle.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FixupBundle.cmake
    @ONLY
)

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/FixupBundle.cmake)

# Configure and install RPATHs fixup script

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/Linux/FixupRPATHs.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FixupRPATHs.cmake
    @ONLY
)

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/FixupRPATHs.cmake)

# Install project icon

get_filename_component(PROJECT_ICON_EXT ${PROJECT_ICON} EXT)
set(DESKTOP_ICON_FILENAME "${PROJECT_NAME}${PROJECT_ICON_EXT}")

install(
    FILES "${PROJECT_SOURCE_DIR}/${PROJECT_ICON}"
    DESTINATION "${PROJECT_BINARIES_DEST_DIR}"
    RENAME "${DESKTOP_ICON_FILENAME}"
)

# Configure and install desktop files

set(DESKTOP_NAME "${PROJECT_TITLE}")
set(DESKTOP_ICON "/${PROJECT_BINARIES_DEST_DIR}/${DESKTOP_ICON_FILENAME}")
set(DESKTOP_APPLICATION_EXEC "/${PROJECT_EXECUTABLE} --preferences")
set(DESKTOP_AUTOSTART_EXEC "/${PROJECT_EXECUTABLE} --delay")
set(DESKTOP_COMMENT "${PROJECT_DESCRIPTION}")
set(DESKTOP_CATEGORIES "Utility")

set(DESKTOP_FILENAME "${PROJECT_NAME}.desktop")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/Application.desktop.in"
    "${PROJECT_BINARY_DIR}/Application.desktop"
)

install(
    FILES "${PROJECT_BINARY_DIR}/Application.desktop"
    DESTINATION "usr/share/applications"
    RENAME "${DESKTOP_FILENAME}"
)

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/Autostart.desktop.in"
    "${PROJECT_BINARY_DIR}/Autostart.desktop"
)

install(
    FILES "${PROJECT_BINARY_DIR}/Autostart.desktop"
    DESTINATION "etc/xdg/autostart"
    RENAME "${DESKTOP_FILENAME}"
)

string(TOLOWER "${PROJECT_NAME}" PACKAGE_NAME)

# Build DEB for Ubuntu/Debian

set(DEB_PACKAGE_NAME ${PACKAGE_NAME})
set(DEB_PACKAGE_VERSION "${PROJECT_VERSION}")
set(DEB_PACKAGE_SECTION "misc")
set(DEB_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(DEB_PACKAGE_MAINTAINER ${PROJECT_DEVELOPER})
set(DEB_PACKAGE_URL ${PROJECT_HOME_URL})
set(DEB_PACKAGE_ARCH "amd64")

set(DEB_PACKAGE_DEPENDENCIES
    # Core system libraries
    "libc6 (>= 2.31)"      # GNU C Library - fundamental system library
    "libstdc++6 (>= 11)"   # GNU Standard C++ Library v3

    # X11 core libraries - required for QtGui platform plugin
    "libxcb1"              # X C Binding - base X11 protocol library
    "libx11-6"             # X11 client-side library - core X11 functions
    "libxcb-icccm4"        # XCB ICCCM (Inter-Client Communication) utilities
    "libxcb-image0"        # XCB image handling utilities
    "libxcb-keysyms1"      # XCB keysyms handling (keyboard events)
    "libxcb-randr0"        # XCB RandR extension (screen resize/rotation)
    "libxcb-render-util0"  # XCB render utilities (drawing operations)
    "libxcb-shape0"        # XCB shape extension (non-rectangular windows)
    "libxcb-xfixes0"       # XCB XFixes extension (fixed protocol issues)
    "libxcb-sync1"         # XCB sync extension (operation synchronization)
    "libxcb-xinerama0"     # XCB Xinerama extension (multiple monitors)
    "libxcb-xkb1"          # XCB X Keyboard Extension (advanced keyboard)
    "libxkbcommon-x11-0"   # X11 support for libxkbcommon (keyboard layouts)
    "libxcb-cursor0"       # XCB cursor library (mouse pointer support)

    # OpenGL dependencies - required even for 2D apps (QtGui uses GL)
    "libgl1"               # OpenGL library - required for QtGui rendering

    # Font rendering - required for text display in Qt
    "libfontconfig1"       # Font configuration and customization library
    "libfreetype6"         # FreeType font rendering engine

    # Wayland support
    "libwayland-client0"   # Wayland client library
    "libwayland-cursor0"   # Wayland cursor handling
    "libwayland-egl1"      # EGL implementation for Wayland
    "libxkbcommon0"        # Keyboard handling for Wayland
)

string(
    REPLACE ";" ", " DEB_PACKAGE_DEPENDENCIES
    "${DEB_PACKAGE_DEPENDENCIES}"
)

# Configure and install post-install script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/DEB/PostInstall.in"
    "${PROJECT_BINARY_DIR}/DEBPostInstall"
    @ONLY
)

set(DEB_PACKAGE_POSTINSTALL "${PROJECT_BINARY_DIR}/DEBPostInstall")

# Configure and install pre-remove script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/DEB/PreRemove.in"
    "${PROJECT_BINARY_DIR}/DEBPreRemove"
    @ONLY
)

set(DEB_PACKAGE_PREREMOVE "${PROJECT_BINARY_DIR}/DEBPreRemove")

# Configure and install post-remove script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/DEB/PostRemove.in"
    "${PROJECT_BINARY_DIR}/DEBPostRemove"
    @ONLY
)

set(DEB_PACKAGE_POSTREMOVE "${PROJECT_BINARY_DIR}/DEBPostRemove")

# Configure and install package building script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Linux/DEB/Build.in"
    "${PROJECT_BINARY_DIR}/DEBBuild"
    @ONLY
)

file(
    CHMOD "${PROJECT_BINARY_DIR}/DEBBuild"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
)

# Add custom target for DEB-packaging

add_custom_target(
    "deploy_deb"
    COMMAND "${PROJECT_BINARY_DIR}/DEBBuild" "${DEPLOY_PACKAGE_DIR}"
    DEPENDS "deploy_package"
)
