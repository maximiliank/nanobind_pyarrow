#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/array_primitive.h>

namespace nb = nanobind;


NB_MODULE(test_array_primitive_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;
    // BaseTypes
    m.def("test_array", func<arrow::Array>());
    m.def("test_null_array", func<arrow::NullArray>());
    m.def("test_boolean_array", func<arrow::BooleanArray>());

    // NumericArrays
    m.def("test_halffloat_array", funcCopy<arrow::HalfFloatArray>());
    m.def("test_float_array", funcCopy<arrow::FloatArray>());
    m.def("test_double_array", funcCopy<arrow::DoubleArray>());

    m.def("test_int8_array", funcCopy<arrow::Int8Array>());
    m.def("test_int16_array", funcCopy<arrow::Int16Array>());
    m.def("test_int32_array", funcCopy<arrow::Int32Array>());
    m.def("test_int64_array", funcCopy<arrow::Int64Array>());

    m.def("test_uint8_array", funcCopy<arrow::UInt8Array>());
    m.def("test_uint16_array", funcCopy<arrow::UInt16Array>());
    m.def("test_uint32_array", funcCopy<arrow::UInt32Array>());
    m.def("test_uint64_array", funcCopy<arrow::UInt64Array>());

    // Time & Timestamp
    m.def("test_time32_array", funcCopy<arrow::Time32Array>());
    m.def("test_time64_array", funcCopy<arrow::Time64Array>());
    m.def("test_timestamp_array", funcCopy<arrow::TimestampArray>());
}