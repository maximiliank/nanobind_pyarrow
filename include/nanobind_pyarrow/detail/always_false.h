#pragma once


namespace nanobind::detail::pyarrow {
    template<typename... T>
    constexpr bool always_false = false;
}