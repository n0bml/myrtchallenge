include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found!")
endif()

pkg_check_modules(LIBPNG libpng16 REQUIRED)
if(NOT LIBPNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng16 development libraries installed!")
endif()

add_executable(analog analog.cpp)
target_include_directories(analog PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(analog PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(analog PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(projectile projectile.cpp)
target_include_directories(projectile PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(projectile PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(projectile PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(ticker ticker.cpp)
target_include_directories(ticker PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(ticker PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(ticker PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})
