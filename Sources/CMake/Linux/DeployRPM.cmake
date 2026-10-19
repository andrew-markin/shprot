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

# Shared library dependencies are detected by rpmbuild automatically,
# only dependencies that can't be detected from binaries are listed here
set(RPM_PACKAGE_DEPENDENCIES
    # SSH tools - tunnel is run by ssh, keys are handled by ssh-keygen
    "openssh-clients"        # OpenSSH client (ssh; ssh-keygen comes with required openssh package)
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
