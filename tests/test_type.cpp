#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#ifdef NANOBIND_PYARROW_USE_C_API
#define EXTENSION_NAME test_type_ext_capi
#else
#include <nanobind_pyarrow/pyarrow_import.h>
#define EXTENSION_NAME test_type_ext
#endif
#include <nanobind_pyarrow/type.h>

namespace nb = nanobind;


NB_MODULE(EXTENSION_NAME, m)
{
    using namespace nb::literals;

#ifndef NANOBIND_PYARROW_USE_C_API
    static nb::detail::pyarrow::ImportPyarrow module;
#endif

    m.def("test_field", [](std::shared_ptr<arrow::Field> field) { return field->WithName(field->name()); });
    m.def("test_schema", [](std::shared_ptr<arrow::Schema> schema) {
        return arrow::schema(schema->fields(), schema->endianness(), schema->metadata());
    });
}