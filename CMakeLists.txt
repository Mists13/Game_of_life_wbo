project(game-of-life)
cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.36.0 COMPONENTS program_options)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -O2")

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(game-of-life
  src/futureState.cpp
  src/formula.cpp
  src/main.cpp
  src/initialState.cpp
  src/satSolver.cpp
)

include(Mergesat)
include_directories(${Boost_INCLUDE_DIRS} ${Mergesat_INCLUDE_DIRS})
target_link_libraries(game-of-life ${Boost_LIBRARIES} ${Mergesat_LIBRARIES})
add_dependencies(game-of-life MergesatLib)
