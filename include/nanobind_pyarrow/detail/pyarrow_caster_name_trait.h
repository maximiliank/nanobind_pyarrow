/*
    nanobind_pyarrow/detail/pyarrow_caster_name_trait.h: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)
NAMESPACE_BEGIN(pyarrow)

template<typename T>
struct pyarrow_caster_name_trait;

template<typename T>
using has_pyarrow_caster_name_trait = decltype(pyarrow_caster_name_trait<T>::Name);

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)