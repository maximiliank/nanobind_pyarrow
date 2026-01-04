/*
    nanobind_pyarrow/detail/pyarrow_caster_name_trait.h: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <nanobind_pyarrow/detail/always_false.h>

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)
NAMESPACE_BEGIN(pyarrow)

template<typename T>
struct pyarrow_caster_name_trait;

template<typename T>
using has_pyarrow_caster_name_trait = decltype(pyarrow_caster_name_trait<T>::Name);

template<typename T>
using has_pyarrow_caster_object_name_trait = decltype(pyarrow_caster_name_trait<T>::ObjectName);

template<typename T>
static constexpr const char* pyarrow_caster_get_object_name()
{
    if constexpr (is_detected_v<has_pyarrow_caster_object_name_trait, T>)
    {
        return pyarrow_caster_name_trait<T>::ObjectName;
    }
    else if constexpr (is_detected_v<has_pyarrow_caster_name_trait, T>)
    {
        return pyarrow_caster_name_trait<T>::Name.text;
    }
    else
    {
        static_assert(always_false<T>, "No Name or ObjectName member in pyarrow_caster_name_trait");
    }
}

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)