if(NOT TARGET Python::Module OR NOT TARGET Python::Interpreter)
  find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
endif()

if(NOT TARGET nanobind::nanobind)
  # Import nanobind through CMake's find_package mechanism
  set(nanobind_ROOT "${Python_SITELIB}/nanobind/cmake")
  find_package(nanobind CONFIG REQUIRED)
endif()

if(NOT TARGET nanobind_pyarrow::pyarrow)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
  find_package(PyArrow REQUIRED)
endif()

set(NANOBIND_TARGET nanobind-static)
# Build the core parts of nanobind once
nanobind_build_library(${NANOBIND_TARGET})

function(add_extension extension_name)
  set(options USE_C_API)
  set(oneValueArgs)
  set(multiValueArgs)
  cmake_parse_arguments(
    EXT
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN})

  set(extension_target test_${extension_name}_ext)
  if(EXT_USE_C_API)
    set(extension_target "${extension_target}_capi")
  endif()

  #nanobind_add_module(${extension_target} test_${extension_name}.cpp)

  add_library(${extension_target} MODULE test_${extension_name}.cpp)

  if(EXT_USE_C_API)
    target_compile_definitions(${extension_target} PRIVATE NANOBIND_PYARROW_USE_C_API)
  endif()

  # .. enable size optimizations
  nanobind_opt_size(${extension_target})

  # .. enable link time optimization
  nanobind_lto(${extension_target})

  # .. set the default symbol visibility to 'hidden'
  nanobind_set_visibility(${extension_target})

  # .. strip unneeded symbols and debug info from the binary (only active in release builds)
  nanobind_strip(${extension_target})

  # .. disable the stack protector
  nanobind_disable_stack_protector(${extension_target})

  # .. set the Python extension suffix
  nanobind_extension(${extension_target})

  # .. set important compilation flags
  nanobind_compile_options(${extension_target})

  # .. set important linker flags
  nanobind_link_options(${extension_target})

  target_link_libraries(${extension_target} PRIVATE Nanobind_Pyarrow_warnings Nanobind_Pyarrow_options
                                                    nanobind_pyarrow::nanobind_pyarrow)
  target_link_system_libraries(
    ${extension_target}
    PRIVATE
    ${NANOBIND_TARGET}
    nanobind_pyarrow::pyarrow)

  nanobind_add_stub(
    ${extension_target}_stub
    MODULE
    ${extension_target}
    OUTPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/${extension_target}.pyi"
    PYTHON_PATH
    $<TARGET_FILE_DIR:${extension_target}>
    DEPENDS
    ${extension_target})
endfunction()

add_extension(array_binary)
add_extension(array_binary USE_C_API)
add_extension(array_nested)
add_extension(array_nested USE_C_API)
add_extension(array_primitive)
add_extension(array_primitive USE_C_API)
add_extension(buffer)
add_extension(chunked_array)
add_extension(chunked_array USE_C_API)
add_extension(datatype)
add_extension(record_batch)
add_extension(record_batch USE_C_API)
add_extension(scalar)
add_extension(table)
add_extension(table USE_C_API)
add_extension(tensor)
add_extension(type)

set(TEST_FILES
    test_array_binary.py
    test_array_nested.py
    test_array_primitive.py
    test_buffer.py
    test_chunked_array.py
    test_datatype.py
    test_record_batch.py
    test_scalar.py
    test_table.py
    test_tensor.py
    test_type.py)
