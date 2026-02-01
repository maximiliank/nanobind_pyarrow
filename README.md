# nanobind_pyarrow

## PyArrow bindings for nanobind

### 🎉 C API support 🎉

The library now supports the pyarrow C API for exchanging arrow objects. This is implemented via the [Arrow C Data Interface](https://arrow.apache.org/docs/format/CDataInterface.html) and its [PyCapsule Interface](https://arrow.apache.org/docs/format/CDataInterface/PyCapsuleInterface.html). This has the big advantage that it does not require the same compatible pyarrow version on the python side which was used to compile the extensions, e.g. the wheel. This makes it preferable because the pyarrow version is not fixed. The C API per se does not support all arrow types, but it supports the relevant types.

This is an extension library to [nanobind](https://github.com/wjakob/nanobind) to support exchange of ``pyarrow`` objects via a ``std::shared_ptr<..>``. The type caster headers are structured in a similar form than the headers in ``pyarrow`` (``array_primitive.h``, ``array_binary.h``, etc) itself:

| Types | Type caster header | Supports C API |
| ----- | ------------------ | :--------------: |
| ``Array``, ``DoubleArray``, ``Int64Array``, ... | ``#include <nanobind_pyarrow/array_primitive.h>`` | :white_check_mark: |
| ``BinaryArray``, ``LargeBinaryArray``, ``StringArray``, ``LargeStringArray``, ``FixedSizeBinaryArray``, ``BinaryViewArray``, ``StringViewArray`` | ``#include <nanobind_pyarrow/array_binary.h>`` | :white_check_mark: |
| ``ListArray``, ``LargeListArray``, ``MapArray``, ``FixedSizeListArray``, ``StructArray``, ``UnionArray``, ``SparseUnionArray``, ``DenseUnionArray``| ``#include <nanobind_pyarrow/array_nested.h>`` | :white_check_mark: |
| ``ChunkedArray`` | ``#include <nanobind_pyarrow/chunked_array.h>`` | :white_check_mark: |
| ``Table`` | ``#include <nanobind_pyarrow/table.h>`` | :white_check_mark: |
| ``RecordBatch``| ``#include <nanobind_pyarrow/record_batch.h>`` | :white_check_mark: |
| ``Field``, ``Schema``| ``#include <nanobind_pyarrow/type.h>`` | :white_check_mark: |
| ``Scalars`` | ``#include <nanobind_pyarrow/scalar.h>`` | |
| ``DataTypes`` | ``#include <nanobind_pyarrow/datatype.h>`` | :white_check_mark: |
| ``Buffer``, ``ResizableBuffer``, ``MutableBuffer`` | ``#include <nanobind_pyarrow/buffer.h>`` | |
| ``Tensor``, ``NumericTensor<..>`` | ``#include <nanobind_pyarrow/tensor.h>`` | |
| ``SparseCOOTensor``, ``SparseCSCMatrix``, ``SparseCSFTensor``, ``SparseCSRMatrix`` | ``#include <nanobind_pyarrow/sparse_tensor.h>`` | |

## Usage

### Usage via FetchContent

The recommended way to use `nanobind_pyarrow` is via `FetchContent`.

**Example CMake:**
```cmake
include(FetchContent)
FetchContent_Declare(
  nanobind_pyarrow
  GIT_REPOSITORY https://github.com/maximiliank/nanobind_pyarrow.git
  GIT_TAG origin/main # Or a specific commit/tag
  UPDATE_DISCONNECTED ON
)
FetchContent_MakeAvailable(nanobind_pyarrow)

# nanobind_pyarrow requires nanobind and arrow to be found
if(NOT TARGET Python::Module OR NOT TARGET Python::Interpreter)
  find_package(Python 3.9 REQUIRED COMPONENTS Interpreter Development.Module)
endif()

if(NOT TARGET nanobind::nanobind)
  # Import nanobind through CMake's find_package mechanism
  set(nanobind_ROOT "${Python_SITELIB}/nanobind/cmake")
  find_package(nanobind CONFIG REQUIRED)
endif()

# For the C API, we need to find the Arrow C++ library.
# This can be done in various ways, e.g., by finding the pyarrow package
# and using its included Arrow C++ libraries or via a package manager, e.g. conan
if(NOT TARGET arrow::arrow_shared)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${nanobind_pyarrow_SOURCE_DIR}/cmake")
  find_package(PyArrow REQUIRED)
endif()

nanobind_add_module(pyarrow_extension bindings.cpp)

# For C API (recommended):
# This avoids a hard dependency on a specific pyarrow version.
target_link_libraries(pyarrow_extension PRIVATE nanobind_pyarrow::nanobind_pyarrow arrow::arrow_shared)
target_compile_definitions(pyarrow_extension PRIVATE NANOBIND_PYARROW_USE_C_API)

# For legacy wrap/unwrap bindings:
# This requires a matching pyarrow version at compile and run time.
# if(NOT TARGET nanobind_pyarrow::pyarrow)
#   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${nanobind_pyarrow_SOURCE_DIR}/cmake")
#   find_package(PyArrow REQUIRED)
# endif()
# target_link_libraries(pyarrow_extension PRIVATE nanobind_pyarrow::nanobind_pyarrow nanobind_pyarrow::pyarrow)
```

There are two ways to use `nanobind_pyarrow`: via the new C API bindings (recommended) or the legacy wrap/unwrap bindings.

### C API (recommended)

This approach uses the Arrow C Data Interface, which avoids a hard dependency on a specific `pyarrow` version in your compiled binaries.

**Example C++:**
```cpp
#include <memory>
#include <nanobind/nanobind.h>
#include <nanobind_pyarrow/array_primitive.h>

namespace nb = nanobind;

NB_MODULE(test_pyarrow_ext, m) {
    m.def("my_pyarrow_function", [](std::shared_ptr<arrow::DoubleArray> arr) {
            auto data = arr->data()->Copy();
            return std::make_shared<arrow::DoubleArray>(std::move(data));
        }
    );
}
```

With the C API, you no longer need the `GET_RUNTIME_DEPENDENCIES` logic in your `CMakeLists.txt` for bundling `pyarrow` libraries into wheels, as the dependency is resolved at runtime.

### Wrap/Unwrap Bindings (Legacy)

This approach wraps and unwraps the underlying `pyarrow` objects. It requires a matching `pyarrow` version between your compile environment and the user's runtime environment.

To get started you have to

```cpp
   #include <nanobind_pyarrow/pyarrow_import.h>
```

and make sure to call the following [pyarrow initialization](https://arrow.apache.org/docs/python/integration/extending.html#_CPPv4N5arrow14import_pyarrowEv) on top of your module definition


```cpp
    NB_MODULE(test_pyarrow_ext, m) {
        static nanobind::detail::pyarrow::ImportPyarrow module;
        // ...
    }
```

**Example**: The following code snippet shows how to create bindings for a ``pyarrow.DoubleArray``:

```cpp
    #include <memory>
    #include <nanobind/nanobind.h>

    #include <nanobind_pyarrow/pyarrow_import.h>
    #include <nanobind_pyarrow/array_primitive.h>

    namespace nb = nanobind;

    NB_MODULE(test_pyarrow_ext, m) {
        static nb::detail::pyarrow::ImportPyarrow module;
        m.def("my_pyarrow_function", [](std::shared_ptr<arrow::DoubleArray> arr) {
                auto data = arr->data()->Copy();
                return std::make_shared<arrow::DoubleArray>(std::move(data));
            }
        );
    }
```

If you want to consume the ``C++`` artifacts as distributed by the ``PyPi`` ``pyarrow`` package in your own ``CMake`` 
project, please have a look at [FindPyArrow.cmake](cmake/FindPyArrow.cmake). It requires ``Python``, ``nanobind`` and ``pyarrow`` as dependencies.

In case you want to create ``wheel`` files via ``auditwheel`` it is important that the dependent ``pyarrow`` libraries are installed with the libraries, e.g. by adding the following to your ``CMakeLists.txt``:
```cmake
set_property(
  TARGET pyarrow_extension
  APPEND
  PROPERTY INSTALL_RPATH "$ORIGIN/lib")
  
install(CODE [[
  file(GET_RUNTIME_DEPENDENCIES
    LIBRARIES $<TARGET_FILE:pyarrow_extension>
    RESOLVED_DEPENDENCIES_VAR _r_deps
    UNRESOLVED_DEPENDENCIES_VAR _u_deps
  )
  foreach(_file ${_r_deps})
    if(_file MATCHES ".*lib(arrow|parquet).*\\.so")
      file(INSTALL
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        TYPE SHARED_LIBRARY
        FOLLOW_SYMLINK_CHAIN
        FILES "${_file}"
      )
    endif()
  endforeach()
  list(LENGTH _u_deps _u_length)
  if("${_u_length}" GREATER 0)
    message(WARNING "Unresolved dependencies detected!")
  endif()
]])

install(TARGETS pyarrow_extension LIBRARY DESTINATION .)
```