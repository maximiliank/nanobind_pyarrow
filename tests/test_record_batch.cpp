#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/record_batch.h>

namespace nb = nanobind;


NB_MODULE(test_record_batch_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    m.def("test_record_batch", [](std::shared_ptr<arrow::RecordBatch> recordBatch) {
        return arrow::RecordBatch::Make(recordBatch->schema(), recordBatch->num_rows(), recordBatch->columns());
    });
}