if (NOT lab)
    set(lab 0)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

include(GoogleTest)

file(GLOB test_sources CONFIGURE_DEPENDS lab${lab}/*_test.cpp)
foreach(test_source ${test_sources})
    get_filename_component(test ${test_source} NAME)
    add_executable(${test} ${test_source})
    target_link_libraries(${test} thdb gtest_main)
    gtest_discover_tests(${test} EXTRA_ARGS --gtest_color=yes --gtest_output=json)
endforeach()

unset(lab CACHE)
