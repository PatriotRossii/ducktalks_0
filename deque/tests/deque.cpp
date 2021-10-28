#include <iostream>

#include "../deque.hpp"

template<typename T, std::size_t N>
void dump(deque<T, N>& object) {
	std::cout << '{';
	for(std::size_t i = 0, size = object.size(); i != size; ++i) {
		std::cout << object[i];
		if(i != size - 1) std::cout << ", ";
	}
	std::cout << '}' << '\n';
}

int main() {
	{
		deque<int, 8> myDeque{1, 2, 3, 4, 5};
		dump(myDeque);
	}
	{
		deque<int, 8> myDeque{1, 2, 3, 4, 5, 6, 7, 8};
		dump(myDeque);
	}
	{
		deque<int, 8> myDeque{1, 2, 3, 4, 5, 6, 7, 8};
		
		myDeque.push_back(9);
		myDeque.push_back(10);
		myDeque.push_back(11);

		dump(myDeque);
	}
	{
		deque<int, 4> myDeque{2, 3};

		myDeque.push_front(1);
		myDeque.push_front(0);

		myDeque.push_back(4);
		myDeque.push_back(5);

		dump(myDeque);
	}
}