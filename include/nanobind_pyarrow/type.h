/*
    nanobind_pyarrow/type.h: conversion between arrow and pyarrow

    Copyright (c) 2024 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <memory>
#include <nanobind_pyarrow/detail/caster.h>
#include <arrow/type.h>

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template<>                                                                                       
struct pyarrow::pyarrow_caster_name_trait<arrow::Schema> {                                         
    static constexpr auto Name = const_name("Schema");                                 
};                                                                                               
template<>                                                                                       
struct type_caster<std::shared_ptr<arrow::Schema>> : pyarrow::pyarrow_caster<arrow::Schema, arrow::py::is_schema, arrow::py::wrap_schema, arrow::py::unwrap_schema> {};

template<>                                                                                       
struct pyarrow::pyarrow_caster_name_trait<arrow::Field> {                                         
    static constexpr auto Name = const_name("Field");                                 
};                                                                                               
template<>                                                                                       
struct type_caster<std::shared_ptr<arrow::Field>> : pyarrow::pyarrow_caster<arrow::Field, arrow::py::is_field, arrow::py::wrap_field, arrow::py::unwrap_field> {};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)