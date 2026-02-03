/*
    nanobind_pyarrow/detail/capi_stream_caster.hpp: conversion between arrow and pyarrow

    Copyright (c) 2026 Maximilian Kleinert <kleinert.max@gmail.com>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#include <arrow/chunked_array.h>
#include <arrow/record_batch.h>
#include <nanobind/nanobind.h>
#include <nanobind_pyarrow/detail/pyarrow_caster_name_trait.h>
#include <memory>
#include <arrow/api.h>
#include <arrow/c/abi.h>
#include <arrow/c/bridge.h>
#include <nanobind_pyarrow/detail/always_false.h>


NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

NAMESPACE_BEGIN(pyarrow)

template<typename T>
struct pyarrow_c_api_stream_caster {
    static_assert(is_detected_v<has_pyarrow_caster_name_trait, T>,
            "No Name member for NameType in pyarrow_c_api_stream_caster");
    static_assert(std::is_same_v<T, arrow::Table> || std::is_same_v<T, arrow::ChunkedArray>,
            "pyarrow_c_api_stream_caster only supports Table or ChunkedArray");
    NB_TYPE_CASTER(std::shared_ptr<T>, const_name("pyarrow.lib.") + pyarrow_caster_name_trait<T>::Name)

    bool from_python(handle src, uint8_t /*flags*/, cleanup_list* /*cleanup*/)
    {
        namespace nb = nanobind;

        if (!nb::hasattr(src, "__arrow_c_stream__"))
        {
            return false;
        }
        // Call the capsule export method
        auto res = src.attr("__arrow_c_stream__")();
        nb::capsule stream_cap = nb::cast<nb::capsule>(res);

        // Extract the ArrowArrayStream pointer
        auto stream_ptr = static_cast<ArrowArrayStream*>(stream_cap.data());

        // Import using the Arrow C Bridge
        if constexpr (std::is_same_v<T, arrow::Table>)
        {
            auto reader_result = arrow::ImportRecordBatchReader(stream_ptr);
            if (!reader_result.ok())
            {
                return false;
            }
            auto& reader_value = reader_result.ValueOrDie();
            auto table_result = arrow::Table::FromRecordBatchReader(reader_value.get());
            if (!table_result.ok())
            {
                return false;
            }
            value = table_result.ValueOrDie();
            return true;
        }
        else if constexpr (std::is_same_v<T, arrow::ChunkedArray>)
        {
            auto result = arrow::ImportChunkedArray(stream_ptr);
            if (!result.ok())
            {
                return false;
            }
            value = std::dynamic_pointer_cast<arrow::ChunkedArray>(result.ValueOrDie());
            return static_cast<bool>(value);
        }
        else
        {
            static_assert(always_false<T>, "Unsupported type for pyarrow_c_api_stream_caster");
        }
    }

    static handle from_cpp(std::shared_ptr<T> src, rv_policy /*policy*/, cleanup_list* /*cleanup*/)
    {
        namespace nb = nanobind;

        if (!src)
        {
            return nb::none().release();
        }

        auto c_stream = std::make_unique<ArrowArrayStream>();
        arrow::Status status;

        if constexpr (std::is_same_v<T, arrow::Table>)
        {
            // Create a RecordBatchReader from the Table
            auto reader_result = arrow::TableBatchReader(*src);
            status = arrow::ExportRecordBatchReader(
                    std::make_shared<arrow::TableBatchReader>(reader_result), c_stream.get());
        }
        else if constexpr (std::is_same_v<T, arrow::ChunkedArray>)
        {
            status = arrow::ExportChunkedArray(src, c_stream.get());
        }
        if (!status.ok())
        {
            return nb::none().release();
        }

        nb::capsule py_stream(c_stream.get(), "arrow_array_stream", [](void* ptr) noexcept {
            auto* s = static_cast<ArrowArrayStream*>(ptr);
            if (s->release)
            {
                s->release(s);
            }
            delete s;
        });

        // NOLINTNEXTLINE(bugprone-unused-return-value)
        c_stream.release();

        object pyarrow = module_::import_("pyarrow");
        if constexpr (std::is_same_v<T, arrow::Table>)
        {
            auto record_batch_reader = pyarrow.attr("RecordBatchReader").attr("_import_from_c_capsule")(py_stream);
            object table = record_batch_reader.attr("read_all")();
            return table.release();
        }
        else if constexpr (std::is_same_v<T, arrow::ChunkedArray>)
        {
            return pyarrow.attr("ChunkedArray").attr("_import_from_c_capsule")(py_stream).release();
        }
        else
        {
            static_assert(always_false<T>, "Unsupported type for pyarrow_c_api_stream_caster");
        }
    }
};

NAMESPACE_END(pyarrow)
NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)