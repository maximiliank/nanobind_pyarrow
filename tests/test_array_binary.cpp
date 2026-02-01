#include "helpers.hpp"

#include <nanobind/nanobind.h>

#ifdef NANOBIND_PYARROW_USE_C_API
#define EXTENSION_NAME test_array_binary_ext_capi
#else
#include <nanobind_pyarrow/pyarrow_import.h>
#define EXTENSION_NAME test_array_binary_ext
#endif
#include <nanobind_pyarrow/array_binary.h>

namespace nb = nanobind;


NB_MODULE(EXTENSION_NAME, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

#ifndef NANOBIND_PYARROW_USE_C_API
    static nb::detail::pyarrow::ImportPyarrow module;
#endif
    // StringArrays
    m.def("test_string_array", funcCopy<arrow::StringArray>());
    m.def("test_large_string_array", funcCopy<arrow::LargeStringArray>());

    // BinaryViews
    m.def("test_binary_view_array", funcCopy<arrow::BinaryViewArray>());
    m.def("test_string_view_array", funcCopy<arrow::StringViewArray>());
}