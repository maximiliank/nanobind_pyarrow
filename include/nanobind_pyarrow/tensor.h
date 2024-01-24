/*
    nanobind_pyarrow/tensor.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <nanobind_pyarrow/detail/caster.h>


NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)
NAMESPACE_BEGIN(pyarrow)

template<typename T>
struct pyarrow_tensor_caster
    : pyarrow_caster<T, arrow::py::is_tensor, arrow::py::wrap_tensor, arrow::py::unwrap_tensor> {};

NAMESPACE_END(pyarrow)

#define NB_REGISTER_PYARROW_TENSOR(name)                                                                               \
    template<>                                                                                                         \
    struct pyarrow::pyarrow_caster_name_trait<arrow::name> {                                                           \
        static constexpr auto Name = const_name(NB_STRINGIFY(name));                                                   \
    };                                                                                                                 \
    template<>                                                                                                         \
    struct type_caster<std::shared_ptr<arrow::name>> : pyarrow::pyarrow_tensor_caster<arrow::name> {};

NB_REGISTER_PYARROW_TENSOR(Tensor)

#undef NB_REGISTER_PYARROW_TENSOR

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)