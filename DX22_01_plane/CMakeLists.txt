cmake_minimum_required(VERSION 3.14)
project(DX11Tests)

set(CMAKE_CXX_STANDARD 17)

# GoogleTest 自動取得
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# テスト対象のライブラリ（DirectXに依存しない部分だけ）
add_library(mathlib
    Source/MathUtils.cpp
)
target_include_directories(mathlib PUBLIC Source)

add_executable(dx11_tests
    Tests/test_math.cpp
)
target_link_libraries(dx11_tests
    mathlib
    GTest::gtest_main
)

include(GoogleTest)
add_test(NAME dx11_tests COMMAND dx11_tests)