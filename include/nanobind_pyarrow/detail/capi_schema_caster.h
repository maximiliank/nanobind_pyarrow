/*
    nanobind_pyarrow/detail/capi_array_caster.hpp: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <arrow/record_batch.h>
#include <nanobind/nanobind.h>
#include <nanobind_pyarrow/detail/pyarrow_caster_name_trait.h>
#include <memory>
#include <arrow/api.h>
#include <arrow/c/abi.h>
#include <arrow/c/bridge.h>


NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

NAMESPACE_BEGIN(pyarrow)

template<typename T, auto& Import, auto& Export>
struct pyarrow_c_api_schema_caster {
    static_assert(is_detected_v<has_pyarrow_caster_name_trait, T>,
            "No Name member for NameType in pyarrow_c_api_schema_caster");
    NB_TYPE_CASTER(std::shared_ptr<T>, const_name("pyarrow.lib.") + pyarrow_caster_name_trait<T>::Name)

    bool from_python(handle src, uint8_t /*flags*/, cleanup_list* /*cleanup*/)
    {
        namespace nb = nanobind;

        if (!nb::hasattr(src, "__arrow_c_schema__"))
        {
            return false;
        }
        // Call the capsule export method
        auto res = src.attr("__arrow_c_schema__")();
        nb::capsule schema_cap = nb::cast<nb::capsule>(res);

        // Import using the Arrow C Bridge;
        auto schema_ptr = static_cast<ArrowSchema*>(schema_cap.data());

        auto result = Import(schema_ptr);
        if (!result.ok())
        {
            return false;
        }
        value = std::dynamic_pointer_cast<T>(result.ValueOrDie());
        return static_cast<bool>(value);
    }

    static handle from_cpp(std::shared_ptr<T> src, rv_policy /*policy*/, cleanup_list* /*cleanup*/)
    {
        namespace nb = nanobind;

        if (!src)
        {
            return nb::none().release();
        }

        // Standard Arrow C structs
        auto c_schema = std::make_unique<ArrowSchema>();

        // Export the C++ Schema/Field/DataType to the C structs
        auto status = Export(*src, c_schema.get());
        if (!status.ok())
        {
            return nb::none().release();
        }
        // Wrap structs in capsules with deleters to prevent leaks
        nb::capsule py_schema(c_schema.get(), "arrow_schema", [](void* ptr) noexcept {
            auto* s = static_cast<ArrowSchema*>(ptr);
            if (s->release)
            {
                s->release(s);
            }
            delete s;
        });

        // NOLINTNEXTLINE(bugprone-unused-return-value)
        c_schema.release();

        // Use pyarrow's constructor to turn the capsules back into an Schema/Field/DataType object
        nb::module_ pyarrow = nb::module_::import_("pyarrow");
        constexpr const char* PyArrowClassName = pyarrow_caster_get_object_name<T>();
        return pyarrow.attr(PyArrowClassName).attr("_import_from_c_capsule")(py_schema).release();
    }
};

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)