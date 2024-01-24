#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/table.h>

namespace nb = nanobind;


NB_MODULE(test_table_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    m.def("test_table", [](std::shared_ptr<arrow::Table> table) {
        return arrow::Table::Make(table->schema(), table->columns(), table->num_rows());
    });
}