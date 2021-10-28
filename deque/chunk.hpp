#pragma once

#include <initializer_list>
#include <vector>

enum ChunkType { reversed, direct };

template <typename T, std::size_t N> class chunk {
    std::vector<T> data;

  public:
    chunk() = default;
    chunk(std::initializer_list<T> list) : data{list} {}
    template <typename Iterator> chunk(Iterator from, Iterator to) {
        for (size_t idx = 0; from != to; ++from, ++idx) {
            data[idx] = *from;
        }
    }

    auto get(std::size_t idx, ChunkType type = ChunkType::direct) const
        -> const T & {
        return (type == ChunkType::reversed ? data[N - idx - 1] : data[idx]);
    }
    auto get(std::size_t idx, ChunkType type = ChunkType::direct) -> T & {
        return (type == ChunkType::reversed ? data[N - idx - 1] : data[idx]);
    }

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
