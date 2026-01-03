/*
    nanobind_pyarrow/array_primitive_c_api.h: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

/*
  Select whether to use the C API implementation (define NB_PYARROW_USE_C_API as 1) or the normal API (0).
  Define NB_PYARROW_USE_C_API to 1 for C API, then include the shared implementation.
*/
#define NB_PYARROW_USE_C_API 1
#include "impl/array_primitive_common.h" // IWYU pragma: keep
#undef NB_PYARROW_USE_C_API