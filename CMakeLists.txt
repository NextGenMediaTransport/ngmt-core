# ngmt-core — C++ library (NextGenMediaTransport / OMT protocol core)
cmake_minimum_required(VERSION 3.15)

project(ngmt-core VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(ngmt_core src/ngmt_core.cpp)
set_target_properties(ngmt_core PROPERTIES OUTPUT_NAME ngmt-core)

target_compile_features(ngmt_core PUBLIC cxx_std_17)

target_include_directories(
  ngmt_core
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
  TARGETS ngmt_core
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
