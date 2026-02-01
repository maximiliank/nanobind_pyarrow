cmake_minimum_required(VERSION 3.14)

project(
  nanobind_pyarrow
  VERSION 0.1.0
  DESCRIPTION "nanobind bindings for PyArrow"
  HOMEPAGE_URL "https://github.com/maximiliank/nanobind_pyarrow"
  LANGUAGES CXX)

include(ProjectOptions.cmake)
nanobind_pyarrow_setup_options()

string(
  COMPARE EQUAL
          "${CMAKE_SOURCE_DIR}"
          "${PROJECT_SOURCE_DIR}"
          PROJECT_IS_TOP_LEVEL)

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(nanobind_pyarrow_INCLUDES_WITH_SYSTEM
         "Use SYSTEM modifier for nanobind_pyarrow's includes, disabling warnings" ON)
  mark_as_advanced(nanobind_pyarrow_INCLUDES_WITH_SYSTEM)
  if(nanobind_pyarrow_INCLUDES_WITH_SYSTEM)
    set(warning_guard SYSTEM)
  endif()
endif()


nanobind_pyarrow_local_options()
# ---- Declare library ----

add_library(nanobind_pyarrow_nanobind_pyarrow INTERFACE)
add_library(nanobind_pyarrow::nanobind_pyarrow ALIAS nanobind_pyarrow_nanobind_pyarrow)

set_property(TARGET nanobind_pyarrow_nanobind_pyarrow PROPERTY EXPORT_NAME nanobind_pyarrow)

target_include_directories(nanobind_pyarrow_nanobind_pyarrow ${warning_guard}
                           INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>")

target_compile_features(nanobind_pyarrow_nanobind_pyarrow INTERFACE cxx_std_17)

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(tests)
endif()
