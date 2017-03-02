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

template <size_t SIZE>
std::vector<std::tuple<const char *, size_t>> split_char_array(std::array<char, SIZE> &arr, size_t parts_num) {
	size_t main_len = arr.size();

	assert(parts_num > 0  && "Can not part to zero parts");
	assert(main_len > 0 && "Can not part zero size vector");
	if(parts_num > main_len) {
		std::cerr << "WARNING, parts number is greater than array size : decreasing parts to max ["
				  << main_len << "] parts\n";
		parts_num = main_len;
	}

	std::vector<std::tuple<const char *, size_t>> arr_parts;

	size_t actual_pos = 0;
	for (size_t i = 0;i < parts_num; ++i) {
		double end_dist = (i+1)*(main_len/static_cast<double>(parts_num));

		auto begin_pos = arr.begin() + actual_pos;
		actual_pos = std::ceil(end_dist);
		auto end_pos = arr.end() - ((actual_pos >= main_len) ? 0 : (main_len - actual_pos));

		size_t sub_len = std::distance(begin_pos, end_pos);
		arr_parts.push_back(std::make_pair(&(*begin_pos), sub_len));
	}
	return arr_parts;
}

template <size_t SIZE>
std::vector<std::tuple<const char *, size_t>> chunk_char_array(std::array<char, SIZE> &arr, size_t chunk_size) {
	size_t main_len = arr.size();

	assert(chunk_size > 0  && "Can not part to zero parts");
	assert(main_len > 0 && "Can not part zero size vector");
	if(chunk_size > main_len) {
		std::cerr << "WARNING, chunk size is greater than array size : decreasing one chunk to max ["
				  << main_len << "]\n";
		chunk_size = main_len;
	}

	std::vector<std::tuple<const char *, size_t>> arr_parts;

	size_t actual_pos = 0;
	size_t parts = std::ceil(main_len/static_cast<double>(chunk_size));
	for (size_t i = 0;i < parts; ++i) {
		double end_dist = (i+1)*(main_len/static_cast<double>(parts));

		auto begin_pos = arr.begin() + actual_pos;
		actual_pos = std::ceil(end_dist);
		auto end_pos = arr.end() - ((actual_pos >= main_len) ? 0 : (main_len - actual_pos));

		size_t sub_len = std::distance(begin_pos, end_pos);
		arr_parts.push_back(std::make_pair(&(*begin_pos), sub_len));
	}
	return arr_parts;
}

std::vector<std::vector<char>> split_char_vector(std::vector<char> &vec, size_t parts_num);
std::string char_vector_tostring(const std::vector<char> &vec, bool new_line=true);
std::string print_splitted_char_vector(const std::vector<std::vector<char>> &vec, bool new_line=true);


#endif // UTILS_HPP
