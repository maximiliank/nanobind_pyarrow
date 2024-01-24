#pragma once
#include <memory>

namespace nanobind_pyarrow::Testing {
    template<typename T>
    auto func()
    {
        return [](std::shared_ptr<T> arr) { return arr; };
    }
    template<typename T>
    auto funcCopy()
    {
        return [](std::shared_ptr<T> arr) {
            auto data = arr->data()->Copy();
            return std::make_shared<T>(std::move(data));
        };
    }
}