# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found!")
endif()

pkg_check_modules(LIBPNG libpng16 REQUIRED)
if(NOT LIBPNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng16 development libraries installed!")
endif()


# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${myrtchallenge_SOURCE_DIR}/include/myrtchallenge/*.hpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(myrtchallenge
    canvas.cpp
    colors.cpp
    intersections.cpp
    matrices.cpp
    rays.cpp
    spheres.cpp
    transformations.cpp
    tuples.cpp
    ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(myrtchallenge PUBLIC ../include)
target_include_directories(myrtchallenge PRIVATE ${LIBPNG_INCLUDE_DIRS})

target_link_directories(myrtchallenge PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(myrtchallenge PRIVATE ${LIBPNG_LIBRARIES})

# All users of this library will need at least C++20
target_compile_features(myrtchallenge PUBLIC cxx_std_20)
target_compile_options(myrtchallenge PRIVATE -Wall -Wextra -pedantic)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
