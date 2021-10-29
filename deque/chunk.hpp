#pragma once

#include <algorithm>
#include <initializer_list>
#include <vector>

enum ChunkType { reversed, direct };

template <typename T, std::size_t N> class chunk {
    std::vector<T> data;

  public:
    chunk() { data.reserve(N); };
    chunk(std::size_t count, const T &value = T()) : data(count, value) {}
    chunk(std::initializer_list<T> list) : data{list} {}
    template <typename Iterator> chunk(Iterator from, Iterator to) : chunk() {
        std::copy(from, to, std::back_inserter(data));
    }

    void add(const T &element) { data.push_back(element); }

    auto operator[](std::size_t idx) const -> const T & { return data[idx]; }
    auto operator[](std::size_t idx) -> T & { return data[idx]; }
    
    auto size() const -> std::size_t { return data.size(); }
};
