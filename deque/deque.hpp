#pragma once

#include <initializer_list>
#include <vector>

template <class T> class deque {
    static constexpr std::size_t CHUNK_SIZE = 8;

    enum class Direction { LEFT, RIGHT };

    std::vector<std::vector<T>> chunks;
    std::size_t size_;

    std::size_t left_chunk{0}, left_idx{0};
    std::size_t original_chunk{0};
    std::size_t right_chunk, right_idx;

    auto need_allocation(Direction direction) -> bool {
        return (direction == Direction::LEFT ? left_idx == 0
                                             : right_idx == CHUNK_SIZE - 1);
    }
    void allocate(Direction direction) {
        std::vector<T> new_chunk; new_chunk.reserve(CHUNK_SIZE);
        if (direction == Direction::LEFT) {
            chunks.insert(chunks.begin(), std::move(new_chunk));
        } else {
            chunks.push_back(std::move(new_chunk));
        }
    }
    auto get_chunk(std::size_t idx) const -> std::size_t {
        return (left_idx + idx) / CHUNK_SIZE;
    }

    auto get_idx(std::size_t idx, bool reverse) const -> std::size_t {
        idx = (left_idx + idx) % CHUNK_SIZE;
        return (reverse ? CHUNK_SIZE - idx - 1 : idx);
    }
    auto get_idx(std::size_t idx) -> std::size_t {
        return get_idx(idx, get_chunk(idx) < original_chunk ? true : false);
     }
  public:
    deque() = default;
    deque(std::initializer_list<T> list) : size_{list.size()} {
        auto end = list.end();
        chunks.reserve(size_ / CHUNK_SIZE + 1);

        std::size_t count = 0;
        for (auto from = list.begin(), to = std::min(from + CHUNK_SIZE, end);
             from < end;
             from += CHUNK_SIZE, to = std::min(to + CHUNK_SIZE, end), ++count) {
            chunks.emplace_back(from, to);
        }

        right_chunk = count - 1;
        right_idx = chunks[right_chunk].size() - 1;
    }

    void push_front(const T &value) {
        if (need_allocation(Direction::LEFT)) {
            allocate(Direction::LEFT);
            right_chunk += 1;
            original_chunk += 1;
        }
        left_idx = (left_idx == 0 ? CHUNK_SIZE - 1 : left_idx - 1);
        chunks[left_chunk].push_back(value);
        size_ += 1;
    }
    void pop_front() { chunks[left_chunk].pop_back(); }

    void push_back(const T &value) {
        if (need_allocation(Direction::RIGHT)) {
            allocate(Direction::RIGHT);
            right_chunk += 1;
        }
        right_idx = (right_idx == CHUNK_SIZE - 1 ? 0 : right_idx + 1);
        chunks[right_chunk].push_back(value);
        size_ += 1;
    }
    void pop_back() { chunks[right_chunk].pop_back(); }

    void erase(std::size_t pos) {
        std::vector<T>& chunk = chunks[get_chunk(pos)];
        chunk.erase(chunk.begin() + get_idx(pos));
    }
    void erase(std::size_t from, std::size_t to) {
        for(; from != to; ++from) {
            erase(from);
        }
    }

    auto operator[](std::size_t idx) const -> const T & {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }
    auto operator[](std::size_t idx) -> T & {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }

    auto size() const -> std::size_t { return size_; }
    void clear() { chunks.clear(); }
};
