#include "helpers.hpp"

#include <nanobind/nanobind.h>

#include <nanobind_pyarrow/pyarrow_import.h>
#include <nanobind_pyarrow/buffer.h>

namespace nb = nanobind;


NB_MODULE(test_buffer_ext, m)
{
    using namespace nb::literals;
    using namespace nanobind_pyarrow::Testing;

    static nb::detail::pyarrow::ImportPyarrow module;

    // Buffer
    m.def("test_buffer",
            [](std::shared_ptr<arrow::Buffer> buffer) { return arrow::Buffer::FromString(buffer->ToString()); });
    m.def("test_mutable_buffer", [](std::shared_ptr<arrow::MutableBuffer> buffer) {
        return arrow::MutableBuffer::FromString(buffer->ToString());
    });
    m.def("test_resizable_buffer", [](std::shared_ptr<arrow::ResizableBuffer> buffer) {
        return arrow::ResizableBuffer::FromString(buffer->ToString());
    });
}