#ifndef CATCH_STRING_HELPER_HPP
#define CATCH_STRING_HELPER_HPP

#include <catch2/catch_tostring.hpp>
#include <fmt/format.h>

template <fmt::formattable T>
struct Catch::StringMaker<T> {
    static std::string convert(T const& value) { return fmt::format("{}", value); }
};

#endif // CATCH_STRING_HELPER_HPP
