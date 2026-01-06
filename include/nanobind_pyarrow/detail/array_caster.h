/*
    nanobind_pyarrow/detail/array_caster.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_array_caster.h>
#include <arrow/c/bridge.h>
#else
#include <nanobind_pyarrow/detail/caster.h>
#endif

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

NAMESPACE_BEGIN(pyarrow)

#if NANOBIND_PYARROW_USE_C_API
using ImportArrayFn = arrow::Result<std::shared_ptr<arrow::Array>> (*)(struct ArrowArray*, struct ArrowSchema*);
inline constexpr ImportArrayFn import_array_fn = static_cast<ImportArrayFn>(&arrow::ImportArray);
template<typename T>
using pyarrow_array_caster =
        nanobind::detail::pyarrow::pyarrow_c_api_array_caster<T, import_array_fn, arrow::ExportArray>;
#else
template<typename T>
using pyarrow_array_caster = pyarrow_caster<T, arrow::py::is_array, arrow::py::wrap_array, arrow::py::unwrap_array>;
#endif

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)