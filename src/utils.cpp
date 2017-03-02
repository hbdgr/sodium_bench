#include <random>
#include "utils.hpp"
#include <fenv.h>

char generate_random_char() {
	static const char Charset[] = "0123456789"
								  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
								  "abcdefghijklmnopqrstuvwxyz";
	const size_t max_index = (sizeof(Charset) - 1);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, max_index);

	return Charset[dis(gen)];
}

std::string generate_random_string(size_t length) {
	std::string str(length, 0);
	std::generate_n(str.begin(), length, generate_random_char);
	return str;
}

std::vector<char> generate_random_char_vector(size_t length) {
	std::vector<char> vec(length);
	std::generate_n(vec.begin(), length, generate_random_char);
	return vec;
}


std::vector<std::vector<char> > split_char_vector(std::vector<char> &vec, size_t parts_num) {
	assert(parts_num > 0  && "Can not part to zero parts");
	assert(vec.size() > 0 && "Can not part zero size vector");
	if(parts_num > vec.size()) {
		std::cerr << "WARNING, parts number is greater than vector size : decreasing parts to max ["
				  << vec.size() << "] parts\n";
		parts_num = vec.size();
	}

	std::vector<std::vector<char>> vec_parts;

	size_t actual_pos = 0;
	size_t main_len = vec.size();
	for (size_t i = 0;i < parts_num; ++i) {
		double end_dist = (i+1)*(main_len/static_cast<double>(parts_num));

		auto begin_pos = vec.begin() + actual_pos;
		actual_pos = std::ceil(end_dist);
		auto end_pos = vec.end() - ((actual_pos >= main_len) ? 0 : (main_len - actual_pos));

		vec_parts.push_back(std::vector<char>(begin_pos, end_pos));
	}
	return vec_parts;
}

std::string char_vector_tostring(const std::vector<char> &vec, bool new_line) {
	std::stringstream ss;
	for (auto &i : vec) {
		ss << i;
	}
	if (new_line) ss << std::endl;
	return ss.str();
}

std::string print_splitted_char_vector(const std::vector<std::vector<char> > &vec, bool new_line) {
	std::stringstream ss;
	int nr = 0;
	for (auto &i : vec) {
		ss << nr << ":[";
		ss << char_vector_tostring(i,false); ss << "]";
		nr++;
	}
	if (new_line) ss << std::endl;
	return ss.str();
}
