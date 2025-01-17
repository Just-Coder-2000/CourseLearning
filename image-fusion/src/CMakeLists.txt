set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(BUILD_TIFF ON)

find_package(OpenCV REQUIRED)

find_package(fmt REQUIRED)

aux_source_directory(./src SRC_FILES)

add_executable(${CMAKE_PROJECT_NAME}_fusion fusion.cpp ${SRC_FILES})


target_include_directories(${CMAKE_PROJECT_NAME}_fusion PRIVATE
${OpenCV_INCLUDE_DIRS}
${FMT_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/thirdparty
./include
)
target_link_libraries(${CMAKE_PROJECT_NAME}_fusion PRIVATE
  ${OpenCV_LIBS}
  fmt::fmt
  pthread
)

add_executable(${CMAKE_PROJECT_NAME}_evaluate evaluate.cpp ${SRC_FILES})

target_include_directories(${CMAKE_PROJECT_NAME}_evaluate PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  ${FMT_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/thirdparty
  ./include
)

target_link_libraries(${CMAKE_PROJECT_NAME}_evaluate PRIVATE
  ${OpenCV_LIBS}
  fmt::fmt
  pthread
)