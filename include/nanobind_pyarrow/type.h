/*
    nanobind_pyarrow/type.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/type.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_schema_caster.h>
#include <arrow/c/bridge.h>
namespace {
    using SchemaCaster = nanobind::detail::pyarrow::pyarrow_c_api_schema_caster<arrow::Schema, arrow::ImportSchema,
            arrow::ExportSchema>;
    using FieldCaster = nanobind::detail::pyarrow::pyarrow_c_api_schema_caster<arrow::Field, arrow::ImportField,
            arrow::ExportField>;
}
#else
#include <nanobind_pyarrow/detail/caster.h>

namespace {
    using SchemaCaster = nanobind::detail::pyarrow::pyarrow_caster<arrow::Schema, arrow::py::is_schema,
            arrow::py::wrap_schema, arrow::py::unwrap_schema>;
    using FieldCaster = nanobind::detail::pyarrow::pyarrow_caster<arrow::Field, arrow::py::is_field,
            arrow::py::wrap_field, arrow::py::unwrap_field>;
}
#endif
NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template<>
struct pyarrow::pyarrow_caster_name_trait<arrow::Schema> {
    static constexpr auto Name = const_name("Schema");
};
template<>
struct type_caster<std::shared_ptr<arrow::Schema>> : SchemaCaster {};

template<>
struct pyarrow::pyarrow_caster_name_trait<arrow::Field> {
    static constexpr auto Name = const_name("Field");
};
template<>
struct type_caster<std::shared_ptr<arrow::Field>> : FieldCaster {};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)