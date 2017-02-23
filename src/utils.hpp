#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <array>
#include <sstream>
#include <cassert>
#include <iostream>

char generate_random_char();
std::string generate_random_string (size_t length);
std::vector<char> generate_random_char_vector (size_t length);

template<std::size_t SIZE>
std::array<char, SIZE> generate_random_char_array () {
	std::array<char, SIZE> ar;
	std::generate_n(ar.begin(), SIZE, generate_random_char);
	return ar;
}

template<std::size_t SIZE>
std::array<bool, SIZE> generate_false_array () {
	std::array<bool, SIZE> ar;
	std::generate_n(ar.begin(), SIZE, false);
	return ar;
}

std::vector<std::vector<char>> split_char_vector(std::vector<char> &vec, size_t parts_num);
std::string char_vector_tostring(const std::vector<char> &vec, bool new_line=true);
std::string print_splitted_char_vector(const std::vector<std::vector<char>> &vec, bool new_line=true);


#endif // UTILS_HPP
