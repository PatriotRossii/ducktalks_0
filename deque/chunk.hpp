#pragma once

#include <vector>
#include <initializer_list>

template <typename T, std::size_t N> class chunk {
    std::vector<T> data;
    std::size_t size_{0};

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

    auto operator[](std::size_t idx) const -> const T & { return data[N - idx - 1]; }
    auto operator[](std::size_t idx) -> T & { return data[N - idx - 1]; }

    auto begin() const -> const T * { return data.begin(); }
    auto end() const -> const T * { return data.end(); }
};
