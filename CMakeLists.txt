# ngmt-core — C++ library (NextGenMediaTransport / OMT protocol core)
cmake_minimum_required(VERSION 3.15)

project(ngmt-core VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(
  ngmt_core
  src/ngmt_core.cpp
  src/protocol/object.cpp
  src/protocol/packetizer.cpp
  src/jitter_buffer.cpp
  src/discovery/mdns_impl.cpp)

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

# Optional: link-test against ngmt-transport C ABI (build cdylib with Cargo first).
option(NGMT_ENABLE_TRANSPORT_FFI_SMOKE "Build tools/ngmt_transport_ffi_smoke (requires ../ngmt-transport)" OFF)
if(NGMT_ENABLE_TRANSPORT_FFI_SMOKE)
  set(NGMT_TRANSPORT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../ngmt-transport" CACHE PATH "Path to ngmt-transport repository")
  find_program(CARGO_EXECUTABLE cargo REQUIRED DOC "Rust toolchain (cargo build for cdylib)")
  set(_ngmt_release "${NGMT_TRANSPORT_ROOT}/target/release")
  add_custom_target(
    ngmt_transport_cdylib
    COMMAND ${CARGO_EXECUTABLE} build --release
    WORKING_DIRECTORY "${NGMT_TRANSPORT_ROOT}"
    COMMENT "Building ngmt-transport cdylib via Cargo"
    VERBATIM)
  add_executable(ngmt_transport_ffi_smoke tools/ngmt_transport_ffi_smoke.cpp)
  target_include_directories(ngmt_transport_ffi_smoke PRIVATE "${NGMT_TRANSPORT_ROOT}/include")
  add_dependencies(ngmt_transport_ffi_smoke ngmt_transport_cdylib)
  if(WIN32)
    target_link_libraries(
      ngmt_transport_ffi_smoke PRIVATE "${_ngmt_release}/ngmt_transport.dll.lib")
  else()
    target_link_directories(ngmt_transport_ffi_smoke PRIVATE "${_ngmt_release}")
    target_link_libraries(ngmt_transport_ffi_smoke PRIVATE ngmt_transport)
  endif()
  set_target_properties(
    ngmt_transport_ffi_smoke
    PROPERTIES BUILD_RPATH "${_ngmt_release}" INSTALL_RPATH "${_ngmt_release}")
endif()
