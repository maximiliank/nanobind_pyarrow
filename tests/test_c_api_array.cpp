#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/array_primitive_c_api.h>


NB_MODULE(test_c_api_array_ext, m)
{
    namespace nb = nanobind;
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    m.def("test_double_array", funcCopy<arrow::DoubleArray>());
}