#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/tensor.h>

namespace nb = nanobind;


NB_MODULE(test_tensor_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    m.def("test_tensor", func<arrow::Tensor>());
    m.def("test_int32_tensor", func<arrow::NumericTensor<arrow::Int32Type>>());
}