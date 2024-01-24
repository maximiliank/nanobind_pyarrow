#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/type.h>

namespace nb = nanobind;


NB_MODULE(test_type_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    m.def("test_field", [](std::shared_ptr<arrow::Field> field) { return field->WithName(field->name()); });
    m.def("test_schema", [](std::shared_ptr<arrow::Schema> schema) {
        return arrow::schema(schema->fields(), schema->endianness(), schema->metadata());
    });
}