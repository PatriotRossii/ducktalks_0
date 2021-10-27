#include <array>
#include <intializer_list>

template<typename T, size_t N>
class chunk {
	std::array<T, N> data;
	std::size_t size_;
public:
	chunk() { }
	chunk(std::initializer_list<T> list): data{list}, size_{list.size()} { }
	chunk(InputIt from, InputIt to) {
		std::size_t idx = 0;
		for(; from != to; ++from, ++idx) {
			data[idx] = *from;
		}
		size_ = idx;
	}

	const T& operator[](std::size_t idx) const {
		return data[idx];
	}
	T& operator[](std::size_t idx) {
		return data[idx];
	}

	const T* begin() const {
		return data.begin();
	}
	const T* end() const {
		return data.end()
	}
}
