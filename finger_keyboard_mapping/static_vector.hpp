#ifndef STATIC_VECTOR_HPP
#define STATIC_VECTOR_HPP

#include <array>

namespace finger_tracking {
template <typename T, size_t N>
class static_vector {
    std::array<T, N> m_data;
    size_t           m_size = 0;

public:
    using iterator = typename std::array<T, N>::const_iterator;

    template <size_t N2>
        requires(N2 <= N)
    explicit static_vector(std::array<T, N2> args)
        : m_size{N2} {
        std::copy_n(args.begin(), N, m_data.begin());
    }

    static_vector() = default;

    template <typename... Args>
    static_vector(Args&&... args)
        : m_data{std::forward<Args>(args)...}
        , m_size{sizeof...(Args)} {}

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    T& emplace_back(Args&&... args) {
        m_data[m_size] = T{std::forward<Args>(args)...};
        return m_data[m_size++];
    }

    iterator begin() const { return m_data.cbegin(); }
    iterator end() const { return m_data.cbegin() + m_size; }

    size_t size() const { return m_size; }

    [[nodiscard]] T const& operator[](size_t index) const { return m_data[index]; }
    [[nodiscard]] T const& back() const { return m_data.back(); }
    [[nodiscard]] T const& front() const { return m_data.front(); }
};
} // namespace finger_tracking

#endif // STATIC_VECTOR_HPP
