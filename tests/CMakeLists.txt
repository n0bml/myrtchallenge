enable_testing()

# Testing library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.1.0)
FetchContent_MakeAvailable(Catch2)
# Adds Catch2::Catch2

add_executable(canvas_feature canvas_feature.cpp)
target_compile_features(canvas_feature PRIVATE cxx_std_20)
target_link_libraries(canvas_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME canvas_feature COMMAND canvas_feature)

add_executable(matrices_feature matrices_feature.cpp)
target_compile_features(matrices_feature PRIVATE cxx_std_20)
target_link_libraries(matrices_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME matrices_feature COMMAND matrices_feature)

add_executable(transformations_feature transformations_feature.cpp)
target_compile_features(transformations_feature PRIVATE cxx_std_20)
target_link_libraries(transformations_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME transformations_feature COMMAND transformations_feature)

add_executable(tuples_feature tuples_feature.cpp)
target_compile_features(tuples_feature PRIVATE cxx_std_20)
target_link_libraries(tuples_feature PRIVATE myrtchallenge Catch2::Catch2WithMain)
add_test(NAME tuples_feature COMMAND tuples_feature)

