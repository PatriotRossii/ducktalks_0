#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

#include "chunk.hpp"

template <class T, class Allocator = std::allocator<T>> class deque {
    static constexpr std::size_t CHUNK_SIZE = 8;

    enum class Direction { LEFT, RIGHT };

    std::vector<chunk<T, CHUNK_SIZE>, std::allocator<chunk<T, CHUNK_SIZE>>>
        chunks;
    std::size_t size_;

    std::size_t left_chunk{0};
    std::size_t left_idx{0};

    std::size_t original_chunk{0};

    std::size_t right_chunk;
    std::size_t right_idx;

    auto need_allocation(Direction direction) -> bool {
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

    auto get_chunk(std::size_t idx) const -> std::size_t {
        return (left_idx + idx) / CHUNK_SIZE;
    }
    auto get_idx(std::size_t idx, ChunkType type) const -> std::size_t {
        idx = (left_idx + idx) % CHUNK_SIZE;
        return (type == ChunkType::reversed ? CHUNK_SIZE - idx - 1 : idx);
    }

  public:
    using value_type = T;
    using allocator_type = Allocator;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type &;
    using const_reference = const value_type &;

    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer =
        typename std::allocator_traits<Allocator>::const_pointer;

    deque(): chunks(Allocator()) { };
    explicit deque(const Allocator &alloc): chunks(alloc) {}
    explicit deque(size_type count, const T &value = T(),
                   const Allocator &alloc = Allocator()): chunks(alloc) {
        for(std::size_t i = 0; i != count / CHUNK_SIZE; ++i) {
            chunks.emplace_back(count, value);
        }
        if(count % CHUNK_SIZE) chunks.emplace_back(count % CHUNK_SIZE, value);
    }
    explicit deque(size_type count): chunks(Allocator()) {
        for(std::size_t i = 0; i != count / CHUNK_SIZE; ++i) {
            chunks.emplace_back(count, T());
        }
        if(count % CHUNK_SIZE) chunks.emplace_back(count % CHUNK_SIZE, T());
    }
    explicit deque(size_t count, const Allocator &alloc = Allocator()): chunks(alloc) {
        for(std::size_t i = 0; i != count / CHUNK_SIZE; ++i) {
            chunks.emplace_back(count, T());
        }
        if(count % CHUNK_SIZE) chunks.emplace_back(count % CHUNK_SIZE, T());
    }
    template <class InputIt>
    deque(InputIt first, InputIt last,
          const Allocator &alloc =
              Allocator()) requires(std::same_as<std::input_iterator_tag,
                                                 typename std::iterator_traits<
                                                     InputIt>::
                                                     iterator_category()>): chunks(alloc) {
        std::ptrdiff_t length = last - first;
        chunks.reserve(length / CHUNK_SIZE + 1);

        std::size_t count = 0;
        for (auto to = std::min(first + CHUNK_SIZE, last); first < to;
             first += CHUNK_SIZE, to = std::min(to + CHUNK_SIZE, last),
            ++count) {
            chunks.emplace_back(first, to);
        }
    }
    deque(const deque &other)
        : chunks(other.chunks, std::allocator_traits<allocator_type>::
                                   select_on_container_copy_construction(
                                       other.get_allocator())),
          size_(other.size_), left_chunk(other.left_chunk),
          left_idx(other.left_idx), original_chunk(other.original_chunk),
          right_chunk(other.right_chunk), right_idx(other.right_idx) {}
    deque(const deque &other, const Allocator &alloc)
        : chunks(other.chunks, alloc), size_(other.size_),
          left_chunk(other.left_chunk), left_idx(other.left_idx),
          original_chunk(other.original_chunk), right_chunk(other.right_chunk),
          right_idx(other.right_idx) {}
    deque(deque &&other)
        : chunks(std::move(other.chunks), other.get_allocator()),
          size_(other.size_), left_chunk(other.left_chunk),
          left_idx(other.left_idx), original_chunk(other.original_chunk),
          right_chunk(other.right_chunk), right_idx(other.right_idx) {}
    deque(deque &&other, const Allocator &alloc)
        : chunks(std::move(other.chunks), alloc), size_(other.size_),
          left_chunk(other.left_chunk), left_idx(other.left_idx),
          original_chunk(other.original_chunk), right_chunk(other.right_chunk),
          right_idx(other.right_idx) {}
    deque(std::initializer_list<T> init, const Allocator &alloc = Allocator())
        : chunks{alloc}, size_{init.size()} {
        auto end = init.end();
        chunks.reserve(init.size() / CHUNK_SIZE + 1);

        std::size_t count = 0;
        for (auto from = init.begin(), to = std::min(from + CHUNK_SIZE, end);
             from < end;
             from += CHUNK_SIZE, to = std::min(to + CHUNK_SIZE, end), ++count) {
            chunks.emplace_back(from, to);
        }

        right_chunk = count - 1;
        right_idx = chunks[right_chunk].size() - 1;
    }
    auto get_allocator() const noexcept -> allocator_type {
        return Allocator();
    }
    void push_front(const T &value) {
        if (need_allocation(Direction::LEFT)) {
            allocate(Direction::LEFT);
            right_chunk += 1;
            original_chunk += 1;
        }

        left_idx = (left_idx == 0 ? CHUNK_SIZE - 1 : left_idx - 1);
        chunks[left_chunk].add(value);
        size_ += 1;
    }
    void push_back(const T &value) {
        if (need_allocation(Direction::RIGHT)) {
            allocate(Direction::RIGHT);
            right_chunk += 1;
        }
        right_idx = (right_idx == CHUNK_SIZE - 1 ? 0 : right_idx + 1);
        chunks[right_chunk].add(value);
        size_ += 1;
    }

    auto operator[](std::size_t idx) const -> const T & {
        std::size_t chunk_idx = get_chunk(idx);
        return chunks[chunk_idx][get_idx(idx, chunk_idx < original_chunk
                                                  ? ChunkType::reversed
                                                  : ChunkType::direct)];
    }
    auto operator[](std::size_t idx) -> T & {
        std::size_t chunk_idx = get_chunk(idx);
        return chunks[chunk_idx][get_idx(idx, chunk_idx < original_chunk
                                                  ? ChunkType::reversed
                                                  : ChunkType::direct)];
    }

    auto size() const -> std::size_t { return size_; }
};
