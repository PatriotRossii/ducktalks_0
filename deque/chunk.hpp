#pragma once

#include <initializer_list>
#include <vector>

enum ChunkType { reversed, direct };

template <typename T, std::size_t N> class chunk {
    std::vector<T> data;

  public:
    chunk() { data.reserve(N); };
    chunk(std::initializer_list<T> list) : data{list} {}
    template <typename Iterator> chunk(Iterator from, Iterator to) : chunk() {
        for (; from != to; ++from) {
            data.push_back(*from);
        }
    }

    auto operator[](std::size_t idx) const -> const T & { return data[idx]; }
    auto operator[](std::size_t idx) -> T & { return data[idx]; }

    auto begin(ChunkType type = ChunkType::direct) const -> const T * {
        return (type == ChunkType::reversed ? data.rbegin() : data.begin());
    }
    auto begin(ChunkType type = ChunkType::direct) -> T * {
        return (type == ChunkType::reversed ? data.rbegin() : data.begin());
    }

    auto end(ChunkType type = ChunkType::direct) const -> const T * {
        return (type == ChunkType::reversed ? data.rend() : data.end());
    }
    auto end(ChunkType type = ChunkType::direct) -> T * {
        return (type == ChunkType::reversed ? data.rend() : data.end());
    }

    auto size() const -> std::size_t { return data.size(); }
};
