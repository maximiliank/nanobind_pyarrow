#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/datatype.h>

namespace nb = nanobind;


NB_MODULE(test_datatype_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    // DataTypes
    m.def("test_data_type", func<arrow::DataType>());
    m.def("test_null_type", func<arrow::NullType>());
    m.def("test_bool_type", func<arrow::BooleanType>());
    m.def("test_int8_type", func<arrow::Int8Type>());
    m.def("test_int16_type", func<arrow::Int16Type>());
    m.def("test_int32_type", func<arrow::Int32Type>());
    m.def("test_int64_type", func<arrow::Int64Type>());
    m.def("test_uint8_type", func<arrow::UInt8Type>());
    m.def("test_uint16_type", func<arrow::UInt16Type>());
    m.def("test_uint32_type", func<arrow::UInt32Type>());
    m.def("test_uint64_type", func<arrow::UInt64Type>());
    m.def("test_time32_type", func<arrow::Time32Type>());
    m.def("test_time64_type", func<arrow::Time64Type>());
    m.def("test_date32_type", func<arrow::Date32Type>());
    m.def("test_date64_type", func<arrow::Date64Type>());
    m.def("test_timestamp_type", func<arrow::TimestampType>());
    m.def("test_duration_type", func<arrow::DurationType>());
    m.def("test_month_day_nano_interval_type", func<arrow::MonthDayNanoIntervalType>());
    m.def("test_halffloat_type", func<arrow::HalfFloatType>());
    m.def("test_float_type", func<arrow::FloatType>());
    m.def("test_double_type", func<arrow::DoubleType>());
    m.def("test_decimal128_type", func<arrow::Decimal128Type>());
    m.def("test_decimal256_type", func<arrow::Decimal256Type>());
    m.def("test_string_type", func<arrow::StringType>());
    m.def("test_binary_type", func<arrow::BinaryType>());
    m.def("test_fixedsizebinary_type", func<arrow::FixedSizeBinaryType>());
    m.def("test_large_string_type", func<arrow::LargeStringType>());
    m.def("test_large_binary_type", func<arrow::LargeBinaryType>());
    m.def("test_list_type", func<arrow::ListType>());
    m.def("test_fixedsizelist_type", func<arrow::FixedSizeListType>());
    m.def("test_largelist_type", func<arrow::LargeListType>());
    m.def("test_map_type", func<arrow::MapType>());
    m.def("test_struct_type", func<arrow::StructType>());
    m.def("test_denseunion_type", func<arrow::DenseUnionType>());
    m.def("test_sparseunion_type", func<arrow::SparseUnionType>());
    m.def("test_dictionary_type", func<arrow::DictionaryType>());
    m.def("test_runendencoded_type", func<arrow::RunEndEncodedType>());
    m.def("test_string_view_type", func<arrow::StringViewType>());
    m.def("test_binary_view_type", func<arrow::BinaryViewType>());
}