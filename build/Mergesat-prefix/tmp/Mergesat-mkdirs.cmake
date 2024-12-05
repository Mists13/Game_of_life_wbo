# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat")
  file(MAKE_DIRECTORY "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat")
endif()
file(MAKE_DIRECTORY
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat-build"
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix"
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/tmp"
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat-stamp"
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src"
  "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/santiago/Desktop/original-gol-sat-master/gol-sat-master/build/Mergesat-prefix/src/Mergesat-stamp${cfgdir}") # cfgdir has leading slash
endif()
