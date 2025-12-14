cmake_minimum_required(VERSION 3.16)
project(AsciiWebcam LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

include(FetchContent)

# --- Google Test ---
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# --- OpenCV ---
find_package(OpenCV QUIET)

if(NOT OpenCV_FOUND)
  message(STATUS "OpenCV not found, fetching from source...")
  
  FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG 4.6.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    CMAKE_ARGS -DBUILD_JASPER=OFF -DWITH_OPENJPEG=OFF -DWITH_JPEG2000=OFF -DWITH_GTK=ON -DWITH_OPENMP=ON -DWITH_FFMPEG=ON
  )

  FetchContent_MakeAvailable(opencv)
else()
  message(STATUS "OpenCV found at ${OpenCV_INCLUDE_DIRS}")
endif()

# Define the ascii_webcam_lib library
add_library(ascii_webcam_lib STATIC
  src/ascii_image.cpp
  src/raw_image.cpp
)

target_include_directories(ascii_webcam_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party"
)

target_link_libraries(ascii_webcam_lib PUBLIC
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_videoio
)

# Define the main application executable
add_executable(ascii_webcam_app src/main.cpp)

target_link_libraries(ascii_webcam_app PRIVATE
  ascii_webcam_lib
)

# Define the test executable
add_executable(ascii_image_test tests/ascii_image_tests.cpp)

target_compile_definitions(ascii_image_test PRIVATE IMAGE_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/images/light.png)
target_compile_definitions(ascii_image_test PRIVATE OUTPUT_GRAY_TXT_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/output/output_gray.txt)
target_compile_definitions(ascii_image_test PRIVATE OUTPUT_COLORED_TXT_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/output/output_colored.txt)

# Update the ascii_art_test target
target_link_libraries(ascii_image_test
PRIVATE
GTest::gtest_main
ascii_webcam_lib
)

target_include_directories(ascii_image_test PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/include"
"${CMAKE_CURRENT_SOURCE_DIR}/third_party"
)


# Define the test executable
add_executable(raw_image_test tests/raw_image_tests.cpp)

target_compile_definitions(raw_image_test PRIVATE IMAGE_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/images/light.png)
target_compile_definitions(raw_image_test PRIVATE OUTPUT_GRAY_TXT_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/output/output_gray.txt)
target_compile_definitions(raw_image_test PRIVATE OUTPUT_COLORED_TXT_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/output/output_colored.txt)


# Update the ascii_art_test target
target_link_libraries(raw_image_test
PRIVATE
GTest::gtest_main
ascii_webcam_lib
)

target_include_directories(raw_image_test PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/include"
"${CMAKE_CURRENT_SOURCE_DIR}/third_party"
)

gtest_add_tests(TARGET ascii_image_test raw_image_test)