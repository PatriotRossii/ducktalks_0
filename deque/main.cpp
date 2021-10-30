#include <iostream>

#include "deque.hpp"

template<typename T>
void dump(deque<T>& object) {
	std::cout << '{';
	for(std::size_t i = 0, size = object.size(); i != size; ++i) {
		std::cout << object[i];
		if(i != size - 1) std::cout << ", ";
	}
	std::cout << "}, size = " << object.size() << '\n';
}

int main() {
	deque<int> myDeque{1, 2, 3, 4, 5};
	dump(myDeque);

	myDeque.push_front(0);
	myDeque.push_front(-1);
	dump(myDeque);

	myDeque.push_back(6);
	myDeque.push_back(7);
	dump(myDeque);

	myDeque.erase(3, 6);
	dump(myDeque);

	std::cout << '\n';
	std::cout << "Front: " << myDeque.front() << ", back: " << myDeque.back() << "\n\n";

	myDeque.clear();
	std::cout << "Size: " << myDeque.size() << '\n';
}