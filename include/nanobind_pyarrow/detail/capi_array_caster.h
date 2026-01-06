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
#include <type_traits>


NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

NAMESPACE_BEGIN(pyarrow)

template<typename T, auto& Import, auto& Export>
struct pyarrow_c_api_array_caster {
    static_assert(is_detected_v<has_pyarrow_caster_name_trait, T>,
            "No Name member for NameType in pyarrow_c_api_array_caster");
    static_assert(std::is_base_of_v<arrow::Array, T> || std::is_same_v<T, arrow::RecordBatch>,
            "pyarrow_c_api_array_caster only supports Arrays or RecordBatch");
    NB_TYPE_CASTER(std::shared_ptr<T>, const_name("pyarrow.lib.") + pyarrow_caster_name_trait<T>::Name)

    bool from_python(handle src, uint8_t /*flags*/, cleanup_list* /*cleanup*/)
    {
        namespace nb = nanobind;

        if (!nb::hasattr(src, "__arrow_c_array__"))
        {
            return false;
        }
        // Call the capsule export method
        auto res = src.attr("__arrow_c_array__")();
        nb::tuple out = nb::cast<nb::tuple>(res);

        // Extract the capsules
        nb::capsule schema_cap = nb::cast<nb::capsule>(out[0]);
        nb::capsule array_cap = nb::cast<nb::capsule>(out[1]);

        // Import using the Arrow C Bridge
        auto array_ptr = static_cast<ArrowArray*>(array_cap.data());
        auto schema_ptr = static_cast<ArrowSchema*>(schema_cap.data());

        auto result = Import(array_ptr, schema_ptr);
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
        auto c_array = std::make_unique<ArrowArray>();
        auto c_schema = std::make_unique<ArrowSchema>();

        // Export the C++ array/RecordBatch to the C structs
        auto status = Export(*src, c_array.get(), c_schema.get());
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

        nb::capsule py_array(c_array.get(), "arrow_array", [](void* ptr) noexcept {
            auto* a = static_cast<ArrowArray*>(ptr);
            if (a->release)
            {
                a->release(a);
            }
            delete a;
        });

        // NOLINTBEGIN(bugprone-unused-return-value)
        c_schema.release();
        c_array.release();
        // NOLINTEND(bugprone-unused-return-value)

        // Use pyarrow's constructor to turn the capsules back into an Array object
        // This requires 'pyarrow' to be imported in the Python environment
        object pyarrow = module_::import_("pyarrow");
        constexpr const char* PyArrowClassName = pyarrow_caster_get_object_name<T>();
        return pyarrow.attr(PyArrowClassName).attr("_import_from_c_capsule")(py_schema, py_array).release();
    }
};

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)