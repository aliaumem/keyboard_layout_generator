#ifndef CATCH_STRING_HELPER_HPP
#define CATCH_STRING_HELPER_HPP

#include <catch2/catch_tostring.hpp>
#include <format>

template <typename T>
concept formattable = requires() { typename std::formatter<T>; };

template <formattable T>
struct Catch::StringMaker<T> {
    static std::string convert(T const& value) { return std::format("{}", value); }
};

#endif // CATCH_STRING_HELPER_HPP
