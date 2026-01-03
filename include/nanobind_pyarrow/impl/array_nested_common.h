/*
    nanobind_pyarrow/impl/array_nested_common.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#ifndef NB_PYARROW_USE_C_API
#define NB_PYARROW_PRESERVE_USE_C_API 0
#define NB_PYARROW_USE_C_API          0
#else
#define NB_PYARROW_PRESERVE_USE_C_API 1
#endif

#include <nanobind/nanobind.h>
#include <memory>
#include <arrow/array/array_nested.h>

#if NB_PYARROW_USE_C_API
#include <nanobind_pyarrow/detail/capi_array_caster.h>
#define NB_PYARROW_ARRAY_CASTER pyarrow::pyarrow_c_api_array_caster
#else
#include <nanobind_pyarrow/detail/array_caster.h>
#define NB_PYARROW_ARRAY_CASTER pyarrow::pyarrow_array_caster
#endif

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

#define NB_REGISTER_PYARROW_NESTED_ARRAY(name)                                                                         \
    template<>                                                                                                         \
    struct pyarrow::pyarrow_caster_name_trait<arrow::name> {                                                           \
        static constexpr auto Name = const_name(NB_STRINGIFY(name));                                                   \
    };                                                                                                                 \
    template<>                                                                                                         \
    struct type_caster<std::shared_ptr<arrow::name>> : NB_PYARROW_ARRAY_CASTER<arrow::name> {};

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
#if NB_PYARROW_PRESERVE_USE_C_API == 0
#undef NB_PYARROW_USE_C_API
#undef NB_PYARROW_PRESERVE_USE_C_API
#endif

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)