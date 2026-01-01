/*
    nanobind_pyarrow/c-api/double_caster.hpp: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <nanobind/nanobind.h>
#include <arrow/api.h>
#include <arrow/c/abi.h>
#include <arrow/c/bridge.h>

namespace nb = nanobind;

namespace nanobind::detail {

    template<>
    struct type_caster<std::shared_ptr<arrow::DoubleArray>> {
        NB_TYPE_CASTER(std::shared_ptr<arrow::DoubleArray>, const_name("pyarrow.DoubleArray"))

        bool from_python(handle src, uint8_t /*flags*/, cleanup_list* /*cleanup*/)
        {
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

            auto result = arrow::ImportArray(array_ptr, schema_ptr);
            if (!result.ok())
            {
                return false;
            }
            value = std::dynamic_pointer_cast<arrow::DoubleArray>(result.ValueOrDie());
            return static_cast<bool>(value);
        }

        static handle from_cpp(std::shared_ptr<arrow::DoubleArray> src, rv_policy /*policy*/, cleanup_list* /*cleanup*/)
        {
            if (!src)
            {
                return nb::none().release();
            }

            // Standard Arrow C structs
            auto* c_array = new ArrowArray();
            auto* c_schema = new ArrowSchema();

            // Export the C++ array to the C structs
            auto status = arrow::ExportArray(*src, c_array, c_schema);
            if (!status.ok())
            {
                return nb::none().release();
            }
            // Wrap structs in capsules with deleters to prevent leaks
            nb::capsule py_schema(c_schema, "arrow_schema", [](void* ptr) noexcept {
                auto* s = static_cast<ArrowSchema*>(ptr);
                if (s->release)
                {
                    s->release(s);
                }
                delete s;
            });

            nb::capsule py_array(c_array, "arrow_array", [](void* ptr) noexcept {
                auto* a = static_cast<ArrowArray*>(ptr);
                if (a->release)
                {
                    a->release(a);
                }
                delete a;
            });

            // Use pyarrow's constructor to turn the capsules back into an Array object
            // This requires 'pyarrow' to be imported in the Python environment
            object pyarrow = module_::import_("pyarrow");
            return pyarrow.attr("Array").attr("_import_from_c_capsule")(py_schema, py_array).release();
        }
    };

} // namespace nanobind::detail