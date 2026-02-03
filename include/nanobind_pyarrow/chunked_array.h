/*
    nanobind_pyarrow/chunked_array.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/chunked_array.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_stream_caster.h>
namespace {
    using ChunkedArrayCaster = nanobind::detail::pyarrow::pyarrow_c_api_stream_caster<arrow::ChunkedArray>;
}
#else
#include <nanobind_pyarrow/detail/caster.h>

namespace {
    using ChunkedArrayCaster = nanobind::detail::pyarrow::pyarrow_caster<arrow::ChunkedArray,
            arrow::py::is_chunked_array, arrow::py::wrap_chunked_array, arrow::py::unwrap_chunked_array>;
}
#endif


NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template<>
struct pyarrow::pyarrow_caster_name_trait<arrow::ChunkedArray> {
    static constexpr auto Name = const_name("ChunkedArray");
};
template<>
struct type_caster<std::shared_ptr<arrow::ChunkedArray>> : ChunkedArrayCaster {};


NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)