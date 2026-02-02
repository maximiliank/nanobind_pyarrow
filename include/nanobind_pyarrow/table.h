/*
    nanobind_pyarrow/table.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/table.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_stream_caster.h>
namespace {
    using TableCaster = nanobind::detail::pyarrow::pyarrow_c_api_stream_caster<arrow::Table>;
}
#else
#include <nanobind_pyarrow/detail/caster.h>

namespace {
    using TableCaster = nanobind::detail::pyarrow::pyarrow_caster<arrow::Table, arrow::py::is_table,
            arrow::py::wrap_table, arrow::py::unwrap_table>;
}
#endif
NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template<>
struct pyarrow::pyarrow_caster_name_trait<arrow::Table> {
    static constexpr auto Name = const_name("Table");
};
template<>
struct type_caster<std::shared_ptr<arrow::Table>> : TableCaster {};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)