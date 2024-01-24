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

function(add_extension extension_name)
  set(extension_target test_${extension_name}_ext)
  nanobind_add_module(${extension_target} test_${extension_name}.cpp)
  target_link_libraries(${extension_target} PRIVATE nanobind_pyarrow::pyarrow nanobind_pyarrow::nanobind_pyarrow)
endfunction()

add_extension(array_binary)
add_extension(array_nested)
add_extension(array_primitive)
add_extension(buffer)
add_extension(chunked_array)
add_extension(datatype)
add_extension(record_batch)
add_extension(scalar)
add_extension(table)
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

if(NOT (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR) OR MSVC)
  if(MSVC)
    set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
  else()
    set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
  endif()

  foreach(TEST_FILE IN LISTS TEST_FILES)
    set(IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE})
    set(OUT_FILE ${OUT_DIR}/${TEST_FILE})
    set(TEST_FILES_OUT ${TEST_FILES_OUT} ${OUT_FILE})
    add_custom_command(
      DEPENDS ${IN_FILE} TARGET
      OUTPUT ${OUT_FILE}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${IN_FILE} ${OUT_DIR})
  endforeach()

  add_custom_target(copy-tests ALL DEPENDS ${TEST_FILES_OUT})
endif()
