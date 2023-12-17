file(GLOB_RECURSE SOURCE *.h *.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC sofa eigen GeographicLib_SHARED calceph)