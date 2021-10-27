#include "../chunk.hpp"

struct NonDefaultConstructible {
	NonDefaultConstructible() = delete;
};

int main() {
	chunk<NonDefaultConstructible, 8> chunk;
}
