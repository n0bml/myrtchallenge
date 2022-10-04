enable_testing()

# Testing library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.1.0)
FetchContent_MakeAvailable(Catch2)
# Adds Catch2::Catch2

add_executable(camera_feature camera_feature.cpp)
target_compile_features(camera_feature PRIVATE cxx_std_20)
target_link_libraries(camera_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME camera_feature COMMAND camera_feature)

add_executable(canvas_feature canvas_feature.cpp)
target_compile_features(canvas_feature PRIVATE cxx_std_20)
target_link_libraries(canvas_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME canvas_feature COMMAND canvas_feature)

add_executable(intersections_feature intersections_feature.cpp)
target_compile_features(intersections_feature PRIVATE cxx_std_20)
target_link_libraries(intersections_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME intersections_feature COMMAND intersections_feature)

add_executable(lights_feature lights_feature.cpp)
target_compile_features(lights_feature PRIVATE cxx_std_20)
target_link_libraries(lights_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME lights_feature COMMAND lights_feature)

add_executable(materials_feature materials_feature.cpp)
target_compile_features(materials_feature PRIVATE cxx_std_20)
target_link_libraries(materials_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME materials_feature COMMAND materials_feature)

add_executable(matrices_feature matrices_feature.cpp)
target_compile_features(matrices_feature PRIVATE cxx_std_20)
target_link_libraries(matrices_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME matrices_feature COMMAND matrices_feature)

add_executable(planes_feature planes_feature.cpp)
target_compile_features(planes_feature PRIVATE cxx_std_20)
target_link_libraries(planes_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME planes_feature COMMAND planes_feature)

add_executable(rays_feature rays_feature.cpp)
target_compile_features(rays_feature PRIVATE cxx_std_20)
target_link_libraries(rays_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME rays_feature COMMAND rays_feature)

add_executable(shapes_feature shapes_feature.cpp)
target_compile_features(shapes_feature PRIVATE cxx_std_20)
target_link_libraries(shapes_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME shapes_feature COMMAND shapes_feature)

add_executable(spheres_feature spheres_feature.cpp)
target_compile_features(spheres_feature PRIVATE cxx_std_20)
target_link_libraries(spheres_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME spheres_feature COMMAND spheres_feature)

add_executable(transformations_feature transformations_feature.cpp)
target_compile_features(transformations_feature PRIVATE cxx_std_20)
target_link_libraries(transformations_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME transformations_feature COMMAND transformations_feature)

add_executable(tuples_feature tuples_feature.cpp)
target_compile_features(tuples_feature PRIVATE cxx_std_20)
target_link_libraries(tuples_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME tuples_feature COMMAND tuples_feature)

add_executable(world_feature world_feature.cpp)
target_compile_features(world_feature PRIVATE cxx_std_20)
target_link_libraries(world_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME world_feature COMMAND world_feature)
