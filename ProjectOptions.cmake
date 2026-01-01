include(cmake/SystemLink.cmake)

macro(nanobind_pyarrow_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(nanobind_pyarrow_setup_options)

  nanobind_pyarrow_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL)
    option(Nanobind_Pyarrow_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_ADDRESS "Enable Address Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_LEAK "Enable Leak Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable Undefined Behavior Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_THREAD "Enable Thread Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_MEMORY "Enable Memory Sanitizer" OFF)
  else()
    option(Nanobind_Pyarrow_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_ADDRESS "Enable Address Sanitizer" ${SUPPORTS_ASAN})
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_LEAK "Enable Leak Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable Undefined Behavior Sanitizer" ${SUPPORTS_UBSAN})
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_THREAD "Enable Thread Sanitizer" OFF)
    option(Nanobind_Pyarrow_ENABLE_SANITIZER_MEMORY "Enable Memory Sanitizer" OFF)
  endif()
  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(Nanobind_Pyarrow_ENABLE_SANITIZER_ADDRESS 
                     Nanobind_Pyarrow_ENABLE_SANITIZER_LEAK
                     Nanobind_Pyarrow_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
                     Nanobind_Pyarrow_ENABLE_SANITIZER_THREAD
                     Nanobind_Pyarrow_ENABLE_SANITIZER_MEMORY)
  endif()
endmacro()


macro(nanobind_pyarrow_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(Nanobind_Pyarrow_warnings INTERFACE)
  add_library(Nanobind_Pyarrow_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  nanobind_pyarrow_set_project_warnings(
    Nanobind_Pyarrow_warnings
    ${Nanobind_Pyarrow_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  include(cmake/Sanitizers.cmake)
  nanobind_pyarrow_enable_sanitizers(
    Nanobind_Pyarrow_options
    ${Nanobind_Pyarrow_ENABLE_SANITIZER_ADDRESS}
    ${Nanobind_Pyarrow_ENABLE_SANITIZER_LEAK}
    ${Nanobind_Pyarrow_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
    ${Nanobind_Pyarrow_ENABLE_SANITIZER_THREAD}
    ${Nanobind_Pyarrow_ENABLE_SANITIZER_MEMORY})

  configure_file(${CMAKE_SOURCE_DIR}/.env.in ${CMAKE_SOURCE_DIR}/.env @ONLY)
endmacro()