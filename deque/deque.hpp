#include <algorithm>
#include <initializer_list>
#include <vector>

#include "chunk.hpp"

template <typename T, std::size_t CHUNK_SIZE = 8> class deque {
    enum class Direction { LEFT, RIGHT };

    std::vector<chunk<T, CHUNK_SIZE>> chunks;
    std::size_t size_;

    std::size_t left_chunk{0};
    std::size_t left_idx{0};

    std::size_t right_chunk;
    std::size_t right_idx;

    bool need_allocation(Direction direction) {
        return (direction == Direction::LEFT ? left_idx == 0
                                             : right_idx == CHUNK_SIZE - 1);
    }
    void allocate(Direction direction) {
        if (direction == Direction::LEFT) {
            chunks.insert(chunks.begin(), chunk<T, CHUNK_SIZE>{});
        } else {
            chunks.push_back(chunk<T, CHUNK_SIZE>{});
        }
    }

    std::size_t get_chunk(std::size_t idx) const {
        return (left_idx + idx) / CHUNK_SIZE;
    }
    std::size_t get_idx(std::size_t idx) const {
        return (left_idx + idx) % CHUNK_SIZE;
    }

  public:
    deque(std::initializer_list<T> list) : size_{list.size()} {
        auto end = list.end();
        chunks.reserve(list.size() / CHUNK_SIZE + 1);

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
        }
        left_idx = (left_idx == 0 ? CHUNK_SIZE - 1 : left_idx - 1);
        chunks[left_chunk][left_idx] = value;
        size_ += 1;
    }
    void push_back(const T &value) {
        if (need_allocation(Direction::RIGHT)) {
            allocate(Direction::RIGHT);
            right_chunk += 1;
        }
        right_idx = (right_idx == CHUNK_SIZE - 1 ? 0 : right_idx + 1);
        chunks[right_chunk][right_idx] = value;
        size_ += 1;
    }

    const T &operator[](std::size_t idx) const {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }
    T &operator[](std::size_t idx) {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }

    std::size_t size() const { return size_; }
};