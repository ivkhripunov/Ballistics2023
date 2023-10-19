include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE tests_cpp *.cpp)
foreach(test ${tests_cpp})
    get_filename_component(name ${test} NAME_WLE)
    add_executable("${name}" ${test})
    add_test(NAME "${name}" COMMAND ${test})
    target_link_libraries("${name}" ${PROJECT_NAME} gtest gtest_main)
endforeach()