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
    "${CMAKE_CURRENT_LIST_DIR}/DEB/PostInstall.in"
    "${PROJECT_BINARY_DIR}/DEBPostInstall"
    @ONLY
)

set(DEB_PACKAGE_POSTINSTALL "${PROJECT_BINARY_DIR}/DEBPostInstall")

# Configure and install pre-remove script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/DEB/PreRemove.in"
    "${PROJECT_BINARY_DIR}/DEBPreRemove"
    @ONLY
)

set(DEB_PACKAGE_PREREMOVE "${PROJECT_BINARY_DIR}/DEBPreRemove")

# Configure and install post-remove script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/DEB/PostRemove.in"
    "${PROJECT_BINARY_DIR}/DEBPostRemove"
    @ONLY
)

set(DEB_PACKAGE_POSTREMOVE "${PROJECT_BINARY_DIR}/DEBPostRemove")

# Configure and install package building script

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/DEB/Build.in"
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
