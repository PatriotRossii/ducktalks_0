#pragma once

#include <array>
#include <initializer_list>

template <typename T, std::size_t N> class chunk {
    std::array<T, N> data;
    std::size_t size_;

  public:
    chunk() = default;
    chunk(std::initializer_list<T> list) : data{list}, size_{list.size()} {}
    template <typename Iterator> chunk(Iterator from, Iterator to) {
        std::size_t idx = 0;
        for (; from != to; ++from, ++idx) {
            data[idx] = *from;
        }
        size_ = idx;
    }

    auto operator[](std::size_t idx) const -> const T & { return data[idx]; }
    auto operator[](std::size_t idx) -> T & { return data[idx]; }

    auto begin() const -> const T * { return data.begin(); }
    auto end() const -> const T * { return data.end(); }
};
