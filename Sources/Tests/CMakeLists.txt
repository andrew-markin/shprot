find_package(Qt6 REQUIRED COMPONENTS Test Network)

# Tests compile the application sources they cover directly, so the application target stays untouched
function(add_shprot_test TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp ${ARGN})

    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries(${TEST_NAME} PRIVATE Qt6::Test Qt6::Network)

    if(WIN32)
        target_compile_options(${TEST_NAME} PRIVATE /utf-8 /nologo)
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_shprot_test(
    TestUtilities
    ${PROJECT_SOURCE_DIR}/Utilities.cpp
    ${PROJECT_SOURCE_DIR}/Utilities.h
)

add_shprot_test(
    TestHttpProxy
    ${PROJECT_SOURCE_DIR}/HttpProxyServer.cpp
    ${PROJECT_SOURCE_DIR}/HttpProxyServer.h
    ${PROJECT_SOURCE_DIR}/HttpProxyStream.cpp
    ${PROJECT_SOURCE_DIR}/HttpProxyStream.h
    ${PROJECT_SOURCE_DIR}/Utilities.cpp
    ${PROJECT_SOURCE_DIR}/Utilities.h
)
