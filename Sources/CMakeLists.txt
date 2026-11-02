cmake_minimum_required(VERSION 3.20)

project(Shprot VERSION 1.5 LANGUAGES CXX)

set(PROJECT_TITLE "Shprot")
set(PROJECT_DESCRIPTION "A lightweight GUI wrapper to manage a resilient proxy over SSH tunnel.")

set(PROJECT_DEVELOPER_TITLE "Andrew N. Markin")
set(PROJECT_DEVELOPER_EMAIL "andrew.n.markin@gmail.com")
set(PROJECT_DEVELOPER "${PROJECT_DEVELOPER_TITLE} <${PROJECT_DEVELOPER_EMAIL}>")

set(ORIGINAL_YEAR "2026")
string(TIMESTAMP CURRENT_YEAR "%Y")

if(ORIGINAL_YEAR EQUAL CURRENT_YEAR)
    set(COPYRIGHT_YEARS "${ORIGINAL_YEAR}")
else()
    set(COPYRIGHT_YEARS "${ORIGINAL_YEAR}-${CURRENT_YEAR}")
endif()

set(PROJECT_COPYRIGHT "Copyright (C) ${COPYRIGHT_YEARS} ${PROJECT_DEVELOPER_TITLE}")

set(PROJECT_GITHUB_REPOSITORY "https://github.com/andrew-markin/shprot")
set(PROJECT_LINKEDIN_PROFILE "https://www.linkedin.com/in/andrew-markin-08396831")
set(PROJECT_HOME_URL "${PROJECT_GITHUB_REPOSITORY}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
endif()

# Project information macros, shared by the application and unit tests

add_library(ShprotDefinitions INTERFACE)

target_compile_definitions(
    ShprotDefinitions INTERFACE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_TITLE="${PROJECT_TITLE}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_COPYRIGHT="${PROJECT_COPYRIGHT}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_DEVELOPER_TITLE="${PROJECT_DEVELOPER_TITLE}"
    PROJECT_GITHUB_REPOSITORY="${PROJECT_GITHUB_REPOSITORY}"
    PROJECT_LINKEDIN_PROFILE="${PROJECT_LINKEDIN_PROFILE}"
)

# Find required Qt libraries

find_package(Qt6 REQUIRED COMPONENTS Widgets Network LinguistTools)

if(LINUX)
    find_package(Qt6 REQUIRED COMPONENTS DBus)
endif()

get_filename_component(QT_BINARY_DIR "${QT6_INSTALL_PREFIX}/bin" ABSOLUTE)
get_filename_component(QT_LIBRARY_DIR "${QT6_INSTALL_PREFIX}/lib" ABSOLUTE)
get_filename_component(QT_INCLUDE_DIR "${QT6_INSTALL_PREFIX}/include" ABSOLUTE)
get_filename_component(QT_PLUGINS_DIR "${QT6_INSTALL_PREFIX}/plugins" ABSOLUTE)

# Add the target

add_executable(
    Shprot WIN32
    AboutDialog.cpp
    AboutDialog.h
    LogsHighlighter.cpp
    LogsHighlighter.h
    Logs.cpp
    Logs.h
    LogsDialog.cpp
    LogsDialog.h
    HttpProxyServer.cpp
    HttpProxyServer.h
    HttpProxyStream.cpp
    HttpProxyStream.h
    Metrics.cpp
    Metrics.h
    Preferences.cpp
    Preferences.h
    PreferencesDialog.cpp
    PreferencesDialog.h
    PrivateKeyHighlighter.cpp
    PrivateKeyHighlighter.h
    PrivateKeyTextEdit.cpp
    PrivateKeyTextEdit.h
    Resources.qrc
    Shprot.cpp
    Shprot.h
    Translations.cpp
    Translations.h
    TweakStyle.cpp
    TweakStyle.h
    Utilities.cpp
    Utilities.h
    Websites.cpp
    Websites.h
)

target_include_directories(Shprot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Build timestamp is taken at configure time (release builds are always configured from scratch)
# and passed to a single source file, so it does not trigger rebuilding of the whole project.
# SOURCE_DATE_EPOCH environment variable is respected for reproducible builds.

string(TIMESTAMP PROJECT_BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%SZ" UTC)

set_source_files_properties(
    AboutDialog.cpp PROPERTIES
    COMPILE_DEFINITIONS PROJECT_BUILD_TIMESTAMP="${PROJECT_BUILD_TIMESTAMP}"
)

if(WIN32)
    # Use https://redketchup.io/icon-converter to make icon
    set(PROJECT_ICON "Shprot.ico")

    # Configure executable resources

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Shprot.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/Shprot.rc
        @ONLY
    )

target_sources(
    Shprot PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/Shprot.rc
)
elseif(LINUX)
    set(PROJECT_ICON "Shprot.png")
endif()

target_link_libraries(
    Shprot PRIVATE
    ShprotDefinitions
    Qt6::Widgets
    Qt6::Network
)

if (WIN32)
    target_link_libraries(
        Shprot PRIVATE
        wtsapi32
    )
elseif(LINUX)
    target_link_libraries(
        Shprot PRIVATE
        Qt6::DBus
    )
endif()

if (WIN32)
    target_compile_options(
        Shprot PRIVATE
        /utf-8
        /nologo
    )
    target_compile_definitions(
        Shprot PRIVATE
        UNICODE
        _UNICODE
    )
    set_target_properties(
        Shprot PROPERTIES
        MSVC_WCHAR_T_AS_BUILTIN_TYPE ON
    )
elseif(LINUX)
    target_compile_options(
        Shprot PRIVATE
        -march=x86-64
        -mtune=generic
        -Wno-padded
    )
    target_link_options(
        Shprot PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

# Translations are compiled into resources, the language is chosen by the system UI language.
# Run "cmake --build . --target update_translations" to update .ts files after texts are changed.

set(
    TRANSLATION_LANGUAGES
    ar az be bg bn ca cs da de el es et fa fi fr he hi hr hu hy id it ja ka kk
    ko lt lv ms nb nl pl pt_BR pt_PT ro ru sk sl sq sr sv th tr uk ur uz vi zh_CN zh_TW
)

set(TRANSLATION_TS_FILES)

foreach(TRANSLATION_LANGUAGE ${TRANSLATION_LANGUAGES})
    list(APPEND TRANSLATION_TS_FILES "Translations/${PROJECT_NAME}_${TRANSLATION_LANGUAGE}.ts")
endforeach()

# Texts are written in English, so the English file has no translations. It makes English
# preferred over other languages that the user also has in the list of UI languages.
qt_add_translations(
    Shprot
    TS_FILES ${TRANSLATION_TS_FILES}
    PLURALS_TS_FILE "Translations/${PROJECT_NAME}_en.ts"
    QM_FILES_OUTPUT_VARIABLE TRANSLATION_QM_FILES
    LUPDATE_OPTIONS -no-obsolete -locations none
    LRELEASE_OPTIONS -nounfinished -removeidentical
)

# Compiled translations shrink about three times, but rcc compresses only files that shrink
# more than that by default, so the threshold is lowered
qt_add_resources(
    Shprot "Translations"
    PREFIX "/Translations"
    BASE "${CMAKE_CURRENT_BINARY_DIR}"
    FILES ${TRANSLATION_QM_FILES}
    OPTIONS -threshold 50
)

# Unit tests are optional and not a part of release packages

option(SHPROT_BUILD_TESTS "Build unit tests" OFF)

if(SHPROT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

include(Deploy)
