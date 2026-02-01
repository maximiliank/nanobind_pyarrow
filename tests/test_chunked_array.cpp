#include "helpers.hpp"

#include <nanobind/nanobind.h>


#ifdef NANOBIND_PYARROW_USE_C_API
#define EXTENSION_NAME test_chunked_array_ext_capi
#else
#include <nanobind_pyarrow/pyarrow_import.h>
#define EXTENSION_NAME test_chunked_array_ext
#endif
#include <nanobind_pyarrow/chunked_array.h>

namespace nb = nanobind;


NB_MODULE(EXTENSION_NAME, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

#ifndef NANOBIND_PYARROW_USE_C_API
    static nb::detail::pyarrow::ImportPyarrow module;
#endif
    m.def("test_chunked_array", func<arrow::ChunkedArray>());
}