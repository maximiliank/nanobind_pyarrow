#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/scalar.h>

namespace nb = nanobind;


NB_MODULE(test_scalar_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    // Scalars
    m.def("test_boolean_scalar", func<arrow::BooleanScalar>());
    m.def("test_int8_scalar", func<arrow::Int8Scalar>());
    m.def("test_int16_scalar", func<arrow::Int16Scalar>());
    m.def("test_int32_scalar", func<arrow::Int32Scalar>());
    m.def("test_int64_scalar", func<arrow::Int64Scalar>());
    m.def("test_uint8_scalar", func<arrow::UInt8Scalar>());
    m.def("test_uint16_scalar", func<arrow::UInt16Scalar>());
    m.def("test_uint32_scalar", func<arrow::UInt32Scalar>());
    m.def("test_uint64_scalar", func<arrow::UInt64Scalar>());
    m.def("test_halffloat_scalar", func<arrow::HalfFloatScalar>());
    m.def("test_float_scalar", func<arrow::FloatScalar>());
    m.def("test_double_scalar", func<arrow::DoubleScalar>());
    m.def("test_decimal128_scalar", func<arrow::Decimal128Scalar>());
    m.def("test_decimal256_scalar", func<arrow::Decimal256Scalar>());
    m.def("test_string_scalar", func<arrow::StringScalar>());
    m.def("test_binary_scalar", func<arrow::BinaryScalar>());
    m.def("test_largestring_scalar", func<arrow::LargeStringScalar>());
    m.def("test_largebinary_scalar", func<arrow::LargeBinaryScalar>());
    m.def("test_fixedsizebinary_scalar", func<arrow::FixedSizeBinaryScalar>());
    m.def("test_list_scalar", func<arrow::ListScalar>());
    m.def("test_largelist_scalar", func<arrow::LargeListScalar>());
    m.def("test_fixedsizelist_scalar", func<arrow::FixedSizeListScalar>());
    m.def("test_date32_scalar", func<arrow::Date32Scalar>());
    m.def("test_date64_scalar", func<arrow::Date64Scalar>());
    m.def("test_timestamp_scalar", func<arrow::TimestampScalar>());
    m.def("test_time32_scalar", func<arrow::Time32Scalar>());
    m.def("test_time64_scalar", func<arrow::Time64Scalar>());
    m.def("test_duration_scalar", func<arrow::DurationScalar>());
    m.def("test_monthdaynanointerval_scalar", func<arrow::MonthDayNanoIntervalScalar>());
    m.def("test_struct_scalar", func<arrow::StructScalar>());
    m.def("test_map_scalar", func<arrow::MapScalar>());
    m.def("test_binaryview_scalar", func<arrow::BinaryViewScalar>());
    m.def("test_stringview_scalar", func<arrow::StringViewScalar>());
}