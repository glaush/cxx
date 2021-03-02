#include <iostream>
#include <compare>

int main()
{
	bool a = 0;
	bool b = 0;

	//std::weak_ordering a;
	//std::partial_ordering f;
	std::strong_ordering c = a <=> b;

	if (c < 0)
	{
		std::cout << "<";
	}
	else if (c > 0) {
		std::cout << ">";
	}
	else if (c == 0) {
		std::cout << "==";
	}

	return 0;
}