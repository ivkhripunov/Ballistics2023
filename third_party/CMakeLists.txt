add_subdirectory(Sofa)

add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE eigen)

add_subdirectory(GeographicLib)
add_subdirectory(calceph)
add_subdirectory(googletest-main)