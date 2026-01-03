/*
    nanobind_pyarrow/array_nested.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/array/array_nested.h>

#if NANOBIND_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_array_caster.h>
namespace {
    template<typename T>
    using ArrayCaster = nanobind::detail::pyarrow::pyarrow_c_api_array_caster<T>;
}
#else
#include <nanobind_pyarrow/detail/array_caster.h>
namespace {
    template<typename T>
    using ArrayCaster = nanobind::detail::pyarrow::pyarrow_array_caster<T>;
}
#endif

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

#define NB_REGISTER_PYARROW_NESTED_ARRAY(name)                                                                         \
    template<>                                                                                                         \
    struct pyarrow::pyarrow_caster_name_trait<arrow::name> {                                                           \
        static constexpr auto Name = const_name(NB_STRINGIFY(name));                                                   \
    };                                                                                                                 \
    template<>                                                                                                         \
    struct type_caster<std::shared_ptr<arrow::name>> : ArrayCaster<arrow::name> {};

// array_nested classes
NB_REGISTER_PYARROW_NESTED_ARRAY(ListArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(LargeListArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(MapArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(FixedSizeListArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(StructArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(UnionArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(SparseUnionArray)
NB_REGISTER_PYARROW_NESTED_ARRAY(DenseUnionArray)

#undef NB_REGISTER_PYARROW_NESTED_ARRAY
#undef NB_PYARROW_ARRAY_CASTER

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)