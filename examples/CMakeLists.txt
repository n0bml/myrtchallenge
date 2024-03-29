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

add_executable(casting casting.cpp)
target_include_directories(casting PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(casting PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(casting PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(cylinders cylinders.cpp)
target_include_directories(cylinders PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(cylinders PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(cylinders PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(glass_sphere glass_sphere.cpp)
target_include_directories(glass_sphere PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(glass_sphere PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(glass_sphere PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(patterns patterns.cpp)
target_include_directories(patterns PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(patterns PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(patterns PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(planes planes.cpp)
target_include_directories(planes PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(planes PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(planes PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(projectile projectile.cpp)
target_include_directories(projectile PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(projectile PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(projectile PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(puppets puppets.cpp)
target_include_directories(puppets PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(puppets PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(puppets PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(reflect_refract reflect_refract.cpp)
target_include_directories(reflect_refract PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(reflect_refract PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(reflect_refract PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(scene scene.cpp)
target_include_directories(scene PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(scene PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(scene PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(shading shading.cpp)
target_include_directories(shading PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(shading PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(shading PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(table table.cpp)
target_include_directories(table PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(table PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(table PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})

add_executable(ticker ticker.cpp)
target_include_directories(ticker PRIVATE ${LIBPNG_INCLUDE_DIRS})
target_link_directories(ticker PRIVATE ${LIBPNG_LIBRARY_DIRS})
target_link_libraries(ticker PRIVATE myrtchallenge ${LIBPNG_LIBRARIES})
