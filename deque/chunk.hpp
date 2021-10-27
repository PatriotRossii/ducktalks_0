#pragma once

#include <vector>
#include <initializer_list>

enum ChunkType {
	reversed,
	direct
};

template <typename T, std::size_t N> class chunk {
    std::vector<T> data;
    std::size_t size_{0};
    ChunkType type_;
  public:
	chunk(): chunk(ChunkType::direct) { }
    chunk(ChunkType t): type_{t} { }
    chunk(std::initializer_list<T> list) : data{list}, size_{list.size()} {}
    template <typename Iterator> chunk(Iterator from, Iterator to) {
        std::size_t idx = 0;
        for (; from != to; ++from, ++idx) {
            data[idx] = *from;
        }
        size_ = idx;
    }

    auto operator[](std::size_t idx) const -> const T & { return (type_ == ChunkType::reversed ? data[N - idx - 1] : data[idx]); }
    auto operator[](std::size_t idx) -> T & { return (type_ == ChunkType::reversed ? data[N - idx - 1] : data[idx]); }

    auto begin() const -> const T * { return (type_ == ChunkType::reversed ? data.rbegin() : data.begin()); }
    auto end() const -> const T * { return (type_ == ChunkType::reversed ? data.rend() : data.end()); }

	auto size() const -> std::size_t {
		return size_;
	}
};
