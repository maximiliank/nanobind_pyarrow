#include <nanobind/nanobind.h>

#ifdef NANOBIND_PYARROW_USE_C_API
#define EXTENSION_NAME test_table_ext_capi
#else
#include <nanobind_pyarrow/pyarrow_import.h>
#define EXTENSION_NAME test_table_ext
#endif
#include <nanobind_pyarrow/table.h>

namespace nb = nanobind;


NB_MODULE(EXTENSION_NAME, m)
{
    using namespace nb::literals;

#ifndef NANOBIND_PYARROW_USE_C_API
    static nb::detail::pyarrow::ImportPyarrow module;
#endif
    m.def("test_table", [](std::shared_ptr<arrow::Table> table) {
        return arrow::Table::Make(table->schema(), table->columns(), table->num_rows());
    });
}