#include <nanobind/nanobind.h>

#ifdef NANOBIND_PYARROW_USE_C_API
#define EXTENSION_NAME test_record_batch_ext_capi
#else
#include <nanobind_pyarrow/pyarrow_import.h>
#define EXTENSION_NAME test_record_batch_ext
#endif
#include <nanobind_pyarrow/record_batch.h>

namespace nb = nanobind;


NB_MODULE(EXTENSION_NAME, m)
{
    using namespace nb::literals;

#ifndef NANOBIND_PYARROW_USE_C_API
    static nb::detail::pyarrow::ImportPyarrow module;
#endif
    m.def("test_record_batch", [](std::shared_ptr<arrow::RecordBatch> recordBatch) {
        return arrow::RecordBatch::Make(recordBatch->schema(), recordBatch->num_rows(), recordBatch->columns());
    });
}