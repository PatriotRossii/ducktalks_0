#pragma once

#include <initializer_list>
#include <vector>

template <class T> class deque {
    static constexpr std::size_t CHUNK_SIZE = 8; // Максимальный размер чанка
    enum class Direction { LEFT, RIGHT };

    std::vector<std::vector<T>>
        chunks; // Динамический массив чанков (динамических массивов, содержащих
                // сами элементы)
    std::size_t size_{0};

    std::size_t left_idx{0}; // Индекс крайнего левого элемента
    std::size_t start_chunk{0}; // Индекс первого чанка

    auto need_allocation(Direction direction) -> bool {
        // Если мы хотим вставить элемент в начало, то создание нового чанка
        // необходимо тогда когда индекс крайнего левого элемента равен нулю
        // Если в конец - тогда, когда размер крайнего правого чанка равен
        // CHUNK_SIZE
        return (direction == Direction::LEFT
                    ? left_idx == 0
                    : chunks.back().size() == CHUNK_SIZE)
    }
    void allocate(Direction direction) {
        std::vector<T> new_chunk;
        new_chunk.reserve(CHUNK_SIZE);
        if (direction == Direction::LEFT) {
            chunks.insert(chunks.begin(), std::move(new_chunk));
        } else {
            chunks.push_back(std::move(new_chunk));
        }
    }
    auto get_chunk(std::size_t idx) const -> std::size_t {
        // Пример:
        //  { {0, 1} {2, 3, 4, 5} {6, 7} }

        // get_chunk(0) = 2 / 4 = 0
        // get_chunk(5) = (2 + 5) / 4 = 7 / 4 = 1
        // get_chunk(7) = (2 + 7) / 4 = 9 / 4 = 2

        return (left_idx + idx) / CHUNK_SIZE;
    }

    auto get_idx(std::size_t idx, bool reverse) const -> std::size_t {
        idx = (left_idx + idx) % CHUNK_SIZE;
        return (reverse ? CHUNK_SIZE - idx - 1 : idx);
    }

    auto get_idx(std::size_t idx) -> std::size_t {
        // Перевод виртуальных индексов в фактические
        // Пример:

        // deque<int> myDeque{2, 3, 4, 5, 6, 7}
        // myDeque.push_front(1);
        // myDeque.push_front(0);

        // Внешнее представление:
        //            { {0, 1} {2, 3, 4, 5} {6, 7}}

        // Внутреннее представление:
        //            { {1, 0} {2, 3, 4, 5} {6, 7} }

        // left_idx: 2, start_chunk = 1, CHUNK_SIZE = 4

        // get_idx(0) = get_idx(0, true)
        //      idx = (2 + 0) % 4 = 2 % 4 = 2
        //      return CHUNK_SIZE - 2 - 1 = 4 - 3 = 1

        // get_idx(3) = get_idx(3, false)
        //      idx = (2 + 3) % 4 = 5 % 4 = 1
        //      return 1

        return get_idx(idx, get_chunk(idx) < start_chunk);
    }

  public:
    deque() = default;
    deque(std::initializer_list<T> list) : size_{list.size()} {
        // Пример:

        // deque{1, 2, 3, 4, 5, 6}
        // chunks.reserve(2)

        // Внутреннее представление: { }

        // First iteration:
        // from = 0, to = 4
        // chunks.emplace_back(from, to)

        // Внутреннее представление: { {1, 2, 3, 4} }

        // Second iteration:
        // from = 4, to = 6
        // chunks.emplace_back(from, to)

        // Внутреннее представление: { {1, 2, 3, 4}, {5, 6}}

        auto end = list.end();
        chunks.reserve(size_ / CHUNK_SIZE + 1);

        for (auto from = list.begin(), to = std::min(from + CHUNK_SIZE, end);
             from < end;
             from += CHUNK_SIZE, to = std::min(to + CHUNK_SIZE, end)) {
            chunks.emplace_back(from, to);
        }
    }

    void push_front(const T &value) {
        if (need_allocation(Direction::LEFT)) {
            allocate(Direction::LEFT);
            start_chunk += 1;
        }
        left_idx = (left_idx == 0 ? CHUNK_SIZE - 1 : left_idx - 1);
        chunks.front().push_back(value);
        size_ += 1;
    }
    void pop_front() {
        chunks.front().pop_back();
        size_ -= 1;
    }

    void push_back(const T &value) {
        if (need_allocation(Direction::RIGHT)) {
            allocate(Direction::RIGHT);
        }
        chunks.back().push_back(value);
        size_ += 1;
    }
    void pop_back() {
        chunks.back().pop_back();
        size_ -= 1;
    }

    void erase(std::size_t pos) {
        std::vector<T> &chunk = chunks[get_chunk(pos)];
        chunk.erase(chunk.begin() + get_idx(pos));
        size_ -= 1;
    }
    void erase(std::size_t from, std::size_t to) {
        for (std::size_t counter = from; counter != to; ++counter) {
            erase(from);
        }
    }

    auto operator[](std::size_t idx) const -> const T & {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }
    auto operator[](std::size_t idx) -> T & {
        return chunks[get_chunk(idx)][get_idx(idx)];
    }

    auto back() const -> const T & { return chunks.back().back(); }
    auto back() -> T & { return chunks.back().back(); }

    auto front() const -> const T & { return chunks.front().back(); }
    auto front() -> T & { return chunks.front().back(); }

    auto size() const -> std::size_t { return size_; }
    void clear() {
        chunks.clear();
        size_ = 0;
    }
};
