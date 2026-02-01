/*
    nanobind_pyarrow/record_batch.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/record_batch.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_array_caster.h>
#else
#include <nanobind_pyarrow/detail/caster.h>
#endif

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template<>
struct pyarrow::pyarrow_caster_name_trait<arrow::RecordBatch> {
    static constexpr auto Name = const_name("RecordBatch");
};

#if NANOBIND_PYARROW_USE_C_API
using ImportRecordBatchFn = arrow::Result<std::shared_ptr<arrow::RecordBatch>> (*)(
        struct ArrowArray*, struct ArrowSchema*);
inline constexpr ImportRecordBatchFn import_record_batch_fn =
        static_cast<ImportRecordBatchFn>(&arrow::ImportRecordBatch);
template<>
struct type_caster<std::shared_ptr<arrow::RecordBatch>>
    : pyarrow::pyarrow_c_api_array_caster<arrow::RecordBatch, import_record_batch_fn, arrow::ExportRecordBatch> {};
#else
template<>
struct type_caster<std::shared_ptr<arrow::RecordBatch>>
    : pyarrow::pyarrow_caster<arrow::RecordBatch, arrow::py::is_batch, arrow::py::wrap_batch, arrow::py::unwrap_batch> {
};
#endif

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)