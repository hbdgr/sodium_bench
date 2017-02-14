#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>

std::string generate_random_string (size_t length) {
	auto generate_random_char = [] () -> char {
		static const char Charset[] = "0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(Charset) - 1);
		return Charset[rand() % max_index];
	};
	std::string str(length, 0);
	generate_n(str.begin(), length, generate_random_char);
	return str;
}

#endif // UTILS_HPP
