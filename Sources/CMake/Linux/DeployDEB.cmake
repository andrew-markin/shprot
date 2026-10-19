# Build DEB for Ubuntu/Debian

set(DEB_PACKAGE_NAME ${PACKAGE_NAME})
set(DEB_PACKAGE_VERSION "${PROJECT_VERSION}")
set(DEB_PACKAGE_SECTION "misc")
set(DEB_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(DEB_PACKAGE_MAINTAINER ${PROJECT_DEVELOPER})
set(DEB_PACKAGE_URL ${PROJECT_HOME_URL})
set(DEB_PACKAGE_ARCH "amd64")

# Shared library dependencies are detected by dpkg-shlibdeps when the package is built,
# only dependencies that can't be detected from binaries are listed here
set(DEB_PACKAGE_DEPENDENCIES
    # SSH tools - tunnel is run by ssh, keys are handled by ssh-keygen
    "openssh-client"       # OpenSSH client (ssh, ssh-keygen)
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
