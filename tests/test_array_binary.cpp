#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/array_binary.h>

namespace nb = nanobind;


NB_MODULE(test_array_binary_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    // StringArrays
    m.def("test_string_array", funcCopy<arrow::StringArray>());
    m.def("test_large_string_array", funcCopy<arrow::LargeStringArray>());

    // BinaryViews
    m.def("test_binary_view_array", funcCopy<arrow::BinaryViewArray>());
    m.def("test_string_view_array", funcCopy<arrow::StringViewArray>());
}