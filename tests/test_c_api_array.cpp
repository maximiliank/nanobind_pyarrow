#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/c-api/double_caster.hpp>


namespace nb = nanobind;


NB_MODULE(test_c_api_array_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    m.def("test_double_array", funcCopy<arrow::DoubleArray>());
}