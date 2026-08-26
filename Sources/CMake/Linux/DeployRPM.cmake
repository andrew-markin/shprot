# Build RPM for Fedora/RHEL/AlmaLinux

set(RPM_PACKAGE_NAME ${PACKAGE_NAME})
set(RPM_PACKAGE_VERSION "${PROJECT_VERSION}")
set(RPM_PACKAGE_RELEASE "1")
set(RPM_PACKAGE_LICENSE "GPL-3.0-only AND (LGPL-3.0-only OR GPL-3.0-only WITH Qt-GPL-exception-1.0)")
set(RPM_PACKAGE_GROUP "Productivity/Other")
set(RPM_PACKAGE_SUMMARY ${PROJECT_DESCRIPTION})
set(RPM_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(RPM_PACKAGER ${PROJECT_DEVELOPER})
set(RPM_PACKAGE_URL ${PROJECT_HOME_URL})
set(RPM_PACKAGE_ARCH "x86_64")

set(RPM_PACKAGE_DEPENDENCIES
    # Core system libraries
    "glibc"                  # GNU C Library - fundamental system library
    "libstdc++"              # GNU Standard C++ Library v3

    # X11 core libraries - required for QtGui platform plugin
    "libxcb"                 # X C Binding - base X11 protocol library
    "libX11"                 # X11 client-side library - core X11 functions

    # XCB utility libraries - combined into single package in Fedora
    # Includes: icccm, image, keysyms, randr, render-util, shape, xfixes, sync, xinerama, xkb
    "xcb-util"               # XCB utility libraries (ICCCM, image, keysyms, randr, render-util, shape, xfixes, sync, xinerama, xkb)

    "xcb-util-cursor"        # XCB cursor library (mouse pointer support)

    # Keyboard handling
    "libxkbcommon-x11"       # X11 support for libxkbcommon (keyboard layouts)

    # OpenGL dependencies - required even for 2D apps (QtGui uses GL)
    "mesa-libGL"             # OpenGL library - required for QtGui rendering

    # Font rendering - required for text display in Qt
    "fontconfig"             # Font configuration and customization library
    "freetype"               # FreeType font rendering engine

    # Wayland support
    "libwayland-client"      # Wayland client library
    "libwayland-cursor"      # Wayland cursor handling
    "libwayland-egl"         # EGL implementation for Wayland
    "libxkbcommon"           # Keyboard handling for Wayland
)

set(RPM_PACKAGE_REQUIRES "")
foreach(RPM_PACKAGE_DEPENDENCY ${RPM_PACKAGE_DEPENDENCIES})
    set(RPM_PACKAGE_REQUIRES "${RPM_PACKAGE_REQUIRES}Requires: ${RPM_PACKAGE_DEPENDENCY}\n")
endforeach()

# Configure and install package building script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/RPM/Build.in"
    "${PROJECT_BINARY_DIR}/RPMBuild"
    @ONLY
)

file(
    CHMOD "${PROJECT_BINARY_DIR}/RPMBuild"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
)

# Add custom target for DEB-packaging

add_custom_target(
    "deploy_rpm"
    COMMAND "${PROJECT_BINARY_DIR}/RPMBuild" "${DEPLOY_PACKAGE_DIR}"
    DEPENDS "deploy_package"
)
